// VideoNative.cpp : Defines the exported functions for the DLL.
//

#pragma warning(disable : 4996)

#include "pch.h"

#include "VidNative.h"



// This is an example of an exported variable
VIDEONATIVE_API int nVideoNative = 0;

// This is an example of an exported function.
VIDEONATIVE_API int fnVideoNative(void)
{
	return 0;
}


#ifndef __cplusplus
typedef uint8_t bool;
#define true 1
#define false 0
#endif

#ifdef __cplusplus
#define REINTERPRET_CAST(type, variable) reinterpret_cast<type>(variable)
#define STATIC_CAST(type, variable) static_cast<type>(variable)
#else
#define C_CAST(type, variable) ((type)variable)
#define REINTERPRET_CAST(type, variable) C_CAST(type, variable)
#define STATIC_CAST(type, variable) C_CAST(type, variable)
#endif

int printError(const char* prefix, int errorCode) {
	if (errorCode == 0) {
		return 0;
	}
	else {
		const size_t bufsize = 64;
		char buf[bufsize];

		if (av_strerror(errorCode, buf, bufsize) != 0) {
			strcpy(buf, "UNKNOWN_ERROR");
		}
		fprintf(stderr, "%s (%d: %s)\n", prefix, errorCode, buf);

		return errorCode;
	}
}


extern "C" {

	static void logging(const char* fmt, ...)
	{
		va_list args;
		fprintf(stderr, "LOG: ");
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
		fprintf(stderr, "\n");
	}


	struct videoPB {

		AVFormatContext* pFormatContext;
		AVCodec* codec;
		AVCodecParameters* pars;
		AVCodec* acodec;
		AVCodecParameters* apars;
		AVCodecContext* context;
		AVCodecContext* acontext;
		AVFrame* frame;
		AVPacket* packet;
		AVFrame* aframe;
		AVPacket* apacket;
		AVPacket packet1;
		AVPacket apacket1;
		int hasAudio = 0;
		int num_packets;
		int packets_proc;
		int video_index;
		int audio_index;
		char* frameDataR;
		char* frameDataG;
		char* frameDataB;
		int frameWidth;
		int frameHeight;
		int frameLine;

		int channels;
		int samples;

		ALuint audioSource;

		int64_t last_pts;
		int64_t last_pkt_pts;
		int64_t last_dts;
		int64_t last_clock;
		int64_t time_base;
		int64_t delay;

		double dPts;
		double pktDpts;
		double dTime;
		double dDelay;
		double dClock;

		double repeat;

		uint8_t* newframe = NULL;


	};

	FILE* outFile;

#define RAW_OUT_ON_PLANAR true

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

	float getSample(const AVCodecContext* codecCtx, uint8_t* buffer, int sampleIndex) {
		int64_t val = 0;
		float ret = 0;
		int sampleSize = av_get_bytes_per_sample(codecCtx->sample_fmt);
		switch (sampleSize) {
		case 1:
			// 8bit samples are always unsigned
			val = REINTERPRET_CAST(uint8_t*, buffer)[sampleIndex];
			// make signed
			val -= 127;
			break;

		case 2:
			val = REINTERPRET_CAST(int16_t*, buffer)[sampleIndex];
			break;

		case 4:
			val = REINTERPRET_CAST(int32_t*, buffer)[sampleIndex];
			break;

		case 8:
			val = REINTERPRET_CAST(int64_t*, buffer)[sampleIndex];
			break;

		default:
			fprintf(stderr, "Invalid sample size %d.\n", sampleSize);
			return 0;
		}

		// Check which data type is in the sample.
		switch (codecCtx->sample_fmt) {
		case AV_SAMPLE_FMT_U8:
		case AV_SAMPLE_FMT_S16:
		case AV_SAMPLE_FMT_S32:
		case AV_SAMPLE_FMT_U8P:
		case AV_SAMPLE_FMT_S16P:
		case AV_SAMPLE_FMT_S32P:
			// integer => Scale to [-1, 1] and convert to float.
			ret = val / STATIC_CAST(float, ((1 << (sampleSize * 8 - 1)) - 1));
			break;

		case AV_SAMPLE_FMT_FLT:
		case AV_SAMPLE_FMT_FLTP:
			// float => reinterpret
			ret = *REINTERPRET_CAST(float*, &val);
			break;

		case AV_SAMPLE_FMT_DBL:
		case AV_SAMPLE_FMT_DBLP:
			// double => reinterpret and then static cast down
			ret = STATIC_CAST(float, *REINTERPRET_CAST(double*, &val));
			break;

		default:
			fprintf(stderr, "Invalid sample format %s.\n", av_get_sample_fmt_name(codecCtx->sample_fmt));
			return 0;
		}

		return ret;
	}

	int findVideoStream(const AVFormatContext* formatCtx)
	{
		int videoStreamIndex = -1;
		for (size_t i = 0; i < formatCtx->nb_streams; ++i) {
			// Use the first audio stream we can find.
			// NOTE: There may be more than one, depending on the file.
			if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
				videoStreamIndex = i;
				break;
			}
		}
		return videoStreamIndex;


	}

	int findAudioStream(const AVFormatContext* formatCtx) {
		int audioStreamIndex = -1;
		for (size_t i = 0; i < formatCtx->nb_streams; ++i) {
			// Use the first audio stream we can find.
			// NOTE: There may be more than one, depending on the file.
			if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
				audioStreamIndex = i;
				break;
			}
		}
		return audioStreamIndex;
	}

	void printStreamInformation(const AVCodec* codec, const AVCodecContext* codecCtx, int audioStreamIndex) {
		fprintf(stderr, "Codec: %s\n", codec->long_name);
		if (codec->sample_fmts != NULL) {
			fprintf(stderr, "Supported sample formats: ");
			for (int i = 0; codec->sample_fmts[i] != -1; ++i) {
				fprintf(stderr, "%s", av_get_sample_fmt_name(codec->sample_fmts[i]));
				if (codec->sample_fmts[i + 1] != -1) {
					fprintf(stderr, ", ");
				}
			}
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "---------\n");
		fprintf(stderr, "Stream:        %7d\n", audioStreamIndex);
		fprintf(stderr, "Sample Format: %7s\n", av_get_sample_fmt_name(codecCtx->sample_fmt));
		fprintf(stderr, "Sample Rate:   %7d\n", codecCtx->sample_rate);
		fprintf(stderr, "Sample Size:   %7d\n", av_get_bytes_per_sample(codecCtx->sample_fmt));
		fprintf(stderr, "Channels:      %7d\n", codecCtx->channels);
		fprintf(stderr, "Planar:        %7d\n", av_sample_fmt_is_planar(codecCtx->sample_fmt));
		fprintf(stderr, "Float Output:  %7s\n", !RAW_OUT_ON_PLANAR || av_sample_fmt_is_planar(codecCtx->sample_fmt) ? "yes" : "no");
	}

	void handleFrame(const AVCodecContext* codecCtx, const AVFrame* frame) {
		if (av_sample_fmt_is_planar(codecCtx->sample_fmt) == 1) {
			// This means that the data of each channel is in its own buffer.
			// => frame->extended_data[i] contains data for the i-th channel.
			for (int s = 0; s < frame->nb_samples; ++s) {
				for (int c = 0; c < codecCtx->channels; ++c) {
					float sample = getSample(codecCtx, frame->extended_data[c], s);
					fwrite(&sample, sizeof(float), 1, outFile);
				}
			}
		}
		else {
			// This means that the data of each channel is in the same buffer.
			// => frame->extended_data[0] contains data of all channels.
			if (RAW_OUT_ON_PLANAR) {
				fwrite(frame->extended_data[0], 1, frame->linesize[0], outFile);
			}
			else {
				for (int s = 0; s < frame->nb_samples; ++s) {
					for (int c = 0; c < codecCtx->channels; ++c) {
						float sample = getSample(codecCtx, frame->extended_data[0], s * codecCtx->channels + c);
						fwrite(&sample, sizeof(float), 1, outFile);
					}
				}
			}
		}
	}


	int receiveAndHandle(AVCodecContext* codecCtx, AVFrame* frame) {
		int err = 0;
		// Read the packets from the decoder.
		// NOTE: Each packet may generate more than one frame, depending on the codec.
		while ((err = avcodec_receive_frame(codecCtx, frame)) == 0) {
			// Let's handle the frame in a function.
			handleFrame(codecCtx, frame);
			// Free any buffers and reset the fields to default values.
			av_frame_unref(frame);
		}
		return err;
	}

	void drainDecoder(AVCodecContext* codecCtx, AVFrame* frame) {
		int err = 0;
		// Some codecs may buffer frames. Sending NULL activates drain-mode.
		if ((err = avcodec_send_packet(codecCtx, NULL)) == 0) {
			// Read the remaining packets from the decoder.
			err = receiveAndHandle(codecCtx, frame);
			if (err != AVERROR(EAGAIN) && err != AVERROR_EOF) {
				// Neither EAGAIN nor EOF => Something went wrong.
				printError("Receive error.", err);
			}
		}
		else {
			// Something went wrong.
			printError("Send error.", err);
		}
	}

	struct listNode
	{
		listNode* next = NULL;

		ALuint data = NULL;

	};


	listNode* node1 = NULL;
	ALuint* bufs = NULL;
	ALint bc = 0, bm = 0;
	int audioBegun = -1;


	static int decode_audio_packet(AVPacket* pPacket, AVCodecContext* pCodecContext, AVFrame* pFrame, videoPB* pb) {

		int response = avcodec_send_packet(pCodecContext, pPacket);

		if (response < 0) {

			return response;
		}

		while (response >= 0) {

			response = avcodec_receive_frame(pCodecContext, pFrame);
			if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
				break;
			}
			else if (response < 0) {
				//	logging("Error while receiving a frame from the decoder: %s", av_err2str(response));
				return response;
			}
			if (response >= 0) {

				ALuint newb = 0;
				alGenBuffers(1, &newb);

				ALvoid* data = malloc(pFrame->nb_samples);
				char* datac = (char*)data;

			

				if (av_sample_fmt_is_planar(pCodecContext->sample_fmt) == 1) {
					// This means that the data of each channel is in its own buffer.
					// => frame->extended_data[i] contains data for the i-th channel.
					for (int s = 0; s < pFrame->nb_samples; ++s) {
						for (int c = 0; c < pCodecContext->channels; ++c) {
							float sample = getSample(pCodecContext, pFrame->extended_data[c], s);
							sample = sample * 128;


							sample = 128.0f + sample;



							int sam = (int)(sample);
							datac[s] = (char)sam;


							//	datac[s] = (char)((sample / 1024) * 255);
								//fwrite(&sample, sizeof(float), 1, outFile);
						}
					}



				}
				else {
					// This means that the data of each channel is in the same buffer.
					// => frame->extended_data[0] contains data of all channels.
					if (RAW_OUT_ON_PLANAR) {
						for (int cc = 0; cc < pFrame->linesize[0]; cc++) {

						}
						//fwrite(frame->extended_data[0], 1, frame->linesize[0], outFile);
					}
					else {
						for (int s = 0; s < pFrame->nb_samples; ++s) {
							for (int c = 0; c < pCodecContext->channels; ++c) {
								float sample = getSample(pCodecContext, pFrame->extended_data[0], s * pCodecContext->channels + c);
								//datac[s] = (char)sample;
								//	fwrite(&sample, sizeof(float), 1, outFile);
							}
						}
					}
				}

				if (bufs == NULL) {
					bufs = new ALuint[1024];
					bc = 0;
				}

				ALint state = 0;

				alGetSourcei(pb->audioSource, AL_SOURCE_STATE, &state);


				ALint bp = 0;

				alGetSourcei(pb->audioSource, AL_BUFFERS_PROCESSED, &bp);

				if (bp > 0)
				{


					listNode* bn = node1;
					int cp = 0;



					while (true) {


						alSourceUnqueueBuffers(pb->audioSource, 1, &bn->data);
						cp++;
						if (cp == bp)
						{
							bn = bn->next;
							node1 = bn;
							if (node1 == NULL) {
								node1 = new listNode;
								node1->data = 0;
								node1->next = NULL;
							}
							break;
						}
						bn = bn->next;
						if (bn == NULL) {
							bn = new listNode;
							bn->next = NULL;
							bn->data = 0;
							node1 = bn;
							break;
						}
					}


					//for (int i = 0;i < bp;i++) {

						//alSourceUnqueueBuffers(pb->audioSource, 1, &bufs[bc]);



				}


				alBufferData(newb, AL_FORMAT_MONO8, data, pFrame->nb_samples, pb->acontext->sample_rate);

				alSourceQueueBuffers(pb->audioSource, 1, &newb);
				if (node1 == NULL) {

					node1 = new listNode;
					node1->data = 0;
					node1->next = NULL;
				}

				listNode* nextN = node1;

				free((void*)data);

				while (true)
				{

					if (nextN->next == NULL) {
						break;
					}
					nextN = nextN->next;

				}

				nextN->data = newb;
				nextN->next = new listNode;
				nextN->next->next = NULL;
				nextN->next->data = 0;

				//	bufs[bc] = newb;
			//	bc++;



				//printf("Play:%d \n", state);

				//	printf("Playing:");
				if (state != 4114) {
					alSourcePlay(pb->audioSource);
					//exit(0);
					audioBegun = 1;

				}//}



			}

		}

		return 0;
	}

	AVFrame* f2 = NULL;

	static int decode_packet(AVPacket* pPacket, AVCodecContext* pCodecContext, AVFrame* pFrame, videoPB* pb)
	{

		//eturn 0;
		int response = avcodec_send_packet(pCodecContext, pPacket);
		//return 0;


		if (response < 0) {
			//logging("Error while sending a packet to the decoder: %s", av_err2str(response));
			return response;
		}



		while (response >= 0)
		{
			// Return decoded output data (into a frame) from a decoder
			//// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
			response = avcodec_receive_frame(pCodecContext, pFrame);

			if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
				break;
			}
			else if (response < 0) {
				//	logging("Error while receiving a frame from the decoder: %s", av_err2str(response));
				return response;
			}

			if (response >= 0) {


				//printf("pts:%f\n", (float)av_frame_get_best_effort_timestamp(pFrame));
				pb->dPts = (double)av_frame_get_best_effort_timestamp(pFrame);

				pb->dDelay = av_q2d(pb->pFormatContext->streams[pb->video_index]->codec->time_base);
				pb->dDelay += (double)pFrame->repeat_pict * (pb->dDelay * 0.5);




				//	printf("P===:%f", (float)pb->dPts);
				pb->frame = pFrame;
				//char frame_filename[1024];
				//snprintf(frame_filename, sizeof(frame_filename), "%snnn%d.pgm", "frame", pCodecContext->frame_number);
				// save a grayscale frame into a .pgm file

				//return 0;

				struct SwsContext* resize;
				resize = sws_getContext(pFrame->width, pFrame->height, (AVPixelFormat)pFrame->format, pFrame->width, pFrame->height, AVPixelFormat::AV_PIX_FMT_RGB24, SWS_BITEXACT, NULL, NULL, NULL);
				//	av_free(resize);
				//	av_freep(resize);
				//	av_free_packet(pPacket);


				if (f2 == NULL) {
					f2 = av_frame_alloc();
				}

				AVFrame* frame2 = av_frame_alloc();




				if (pb->newframe == NULL) {
					uint8_t* f2b = (uint8_t*)av_malloc(pFrame->width * pFrame->height * 3);
					pb->newframe = f2b;
				}


				frame2->data[0] = pb->newframe;
				frame2->width = pFrame->width;
				frame2->height = pFrame->height;
				frame2->linesize[0] = pFrame->width * 3;
				//	frame2->format = AVPixelFormat::AV_PIX_FMT_RGB8;

					//avpicture_fill((AVPicture*)frame2, f2b, AVPixelFormat::AV_PIX_FMT_RGB8, pFrame->width, pFrame->height);

				sws_scale(resize, pFrame->data, pFrame->linesize, 0, pFrame->height, frame2->data, frame2->linesize);

				pb->frameDataR = (char*)frame2->data[0];
				//pb->frameDataG = (char*)frame2->data[1];
				//pb->frameDataB = (char*)frame2->data[2];
				pb->frameWidth = frame2->width;
				pb->frameHeight = frame2->height;
				pb->frameLine = frame2->linesize[0];


				//av_frame_free(&pFrame);
			///	save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);

			}
		}
		return 0;
	}

	void fastMemcpy(void* pvDest, void* pvSrc, size_t nBytes) {

		//assert(nBytes % 32 == 0);
		//assert((intptr_t(pvDest) & 31) == 0);
		//assert((intptr_t(pvSrc) & 31) == 0);
		const __m256i* pSrc = reinterpret_cast<const __m256i*>(pvSrc);
		__m256i* pDest = reinterpret_cast<__m256i*>(pvDest);
		int64_t nVects = nBytes / sizeof(*pSrc);
		for (; nVects > 0; nVects--, pSrc++, pDest++) {
			const __m256i loaded = _mm256_stream_load_si256(pSrc);
			_mm256_stream_si256(pDest, loaded);
		}
		_mm_sfence();
	}

	VIDEONATIVE_API int audioHasBegun() {

		return audioBegun;

	}

	VIDEONATIVE_API void genFrameData(videoPB* vid) {


		vid->dDelay = av_q2d(vid->pFormatContext->streams[vid->video_index]->codec->time_base);
		vid->delay = vid->frame->repeat_pict;





		//if ((pts = (double)av_frame_get_best_effort_timestamp(vid->frame)) == AV_NOPTS_VALUE)
		//	pts = 0;
		if (vid->dPts == AV_NOPTS_VALUE) {

			vid->dPts = 0;

		}



		vid->dPts *= av_q2d(vid->pFormatContext->streams[vid->video_index]->time_base);





	}

	VIDEONATIVE_API void getFrameData(videoPB* vid, char* buf) {



		char* datR = vid->frameDataR;


		memcpy(buf, datR, vid->frameWidth * vid->frameHeight * 3);



	}

	VIDEONATIVE_API double getDPTS(videoPB* vid) {

		return vid->dPts;
	}

	VIDEONATIVE_API double getDDelay(videoPB* vid) {

		return vid->dDelay;

	}

	VIDEONATIVE_API int64_t getPict(videoPB* vid) {

		return vid->delay;

	}

	VIDEONATIVE_API int64_t getLastDTS(videoPB* vid) {

		return vid->last_dts;

	}

	VIDEONATIVE_API int64_t getLastPTS(videoPB* vid) {

		return vid->last_pts;

	}

	VIDEONATIVE_API int64_t getLastPktPTS(videoPB* vid) {

		return vid->last_pkt_pts;

	}

	VIDEONATIVE_API int64_t getTimeBase(videoPB* vid) {

		return vid->time_base;

	}

	VIDEONATIVE_API int64_t getDelay(videoPB* vid) {

		return vid->delay;

	}

	VIDEONATIVE_API int getFrameLineSize(videoPB* vid) {

		return (int)vid->frameLine;

	}

	VIDEONATIVE_API int getFrameWidth(videoPB* vid) {

		return vid->frameWidth;

	}

	VIDEONATIVE_API int getFrameHeight(videoPB* vid) {

		return vid->frameHeight;

	}

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

	bool defSDL = false;

	VIDEONATIVE_API int stopAudio(videoPB * v) {

		alSourceStop(v->audioSource);
		alSourcePause(v->audioSource);
		
		node1 = NULL;
		
		//v->audioSource
		return 1;


	}

	VIDEONATIVE_API int decodeNextFrame(videoPB* vid) {

		int response = 0;




		if (av_read_frame(vid->pFormatContext, vid->packet) >= 0)
		{

			int vidi = vid->packet->stream_index;

			if (vid->packet->stream_index == vid->video_index) {



				response = decode_packet(vid->packet, vid->context, vid->frame, vid);

				av_packet_unref(vid->packet);
				av_free_packet(vid->packet);


			}
			else if (vid->packet->stream_index == vid->audio_index) {

				response = decode_audio_packet(vid->packet, vid->acontext, vid->aframe, vid);

				av_packet_unref(vid->packet);
				av_free_packet(vid->packet);

			}

			return 1;

		}
		else {
			return 3;
		}



		return 1;
	}

	AVCodecContext* initStreamCodec(AVFormatContext* formatCtx, int index) {

		AVCodec* codec = avcodec_find_decoder(formatCtx->streams[index]->codecpar->codec_id);
		if (codec == NULL) {

			logging("No decoder found. aborting");
			exit(1);

		}

		AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
		if (codecCtx == NULL) {

			avformat_close_input(&formatCtx);
			logging("Could not allocate decoding context.");
			exit(1);

		}

		int err = 0;

		if ((err = avcodec_parameters_to_context(codecCtx, formatCtx->streams[index]->codecpar)) != 0)
		{
			avcodec_close(codecCtx);
			avcodec_free_context(&codecCtx);
			avformat_close_input(&formatCtx);
			logging("Failed to set stream pars.");
			exit(1);
		}

		printStreamInformation(codec, codecCtx, index);

		return codecCtx;

	}

	void initDecoder(AVFormatContext* formatCtx, AVCodecContext* codecCtx, int index)
	{

		int err = 0;
		if ((err = avcodec_open2(codecCtx, codecCtx->codec, NULL)) != 0)
		{
			avcodec_close(codecCtx);
			avcodec_free_context(&codecCtx);
			avformat_close_input(&formatCtx);
			logging("Failed to init decoder");
			exit(1);
		}

	}

	AVFrame* createFrame() {

		AVFrame* frame = NULL;
		if ((frame = av_frame_alloc()) == NULL) {

			exit(1);
			return NULL;

		}

		return frame;

	}

	static int alError() {

		ALCenum error;
		error = alGetError();
		if (error != AL_NO_ERROR)
		{
			return 1;
		}
		return 0;
	}

	static void list_audio_devices(const ALCchar* devices)
	{
		const ALCchar* device = devices, * next = devices + 1;
		size_t len = 0;


		while (device && *device != '\0' && next && *next != '\0') {

			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}

	}


	ALCdevice* aDevice;
	ALCcontext* aContext;


	int init_audio = false;

	VIDEONATIVE_API int initAudio() {

		if (init_audio) return 1;

		init_audio = true;

		aDevice = alcOpenDevice(NULL);

		if (!aDevice) {

			return 0;

		}

		ALboolean enumeration;

		enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
		if (enumeration == AL_FALSE)
		{

			// enumeration not supported
		}
		else
		{	// enumeration supported
			list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
		}

		aContext = alcCreateContext(aDevice, NULL);
		if (!alcMakeContextCurrent(aContext)) {

			printf("Context not valid.\m");

		}
		else {

			printf("Context made current.\n");

		}

		return 1;
	}

	int init_done = 0;

	VIDEONATIVE_API void stopVideo(videoPB* v)
	{

		avcodec_free_context(&v->context);
		avcodec_free_context(&v->acontext);
		av_packet_unref(v->packet);
		av_packet_unref(v->apacket);
		avformat_flush(v->pFormatContext);
		avformat_close_input(&v->pFormatContext);
		avformat_free_context(v->pFormatContext);

		//sws_freeContext(v->)




	}

	VIDEONATIVE_API videoPB* initVideoNative(const char* file) {


		logging("initializing all the containers, codecs and protocols.");


		videoPB* pb = new videoPB;
		pb->num_packets = 0;
		pb->packets_proc = 0;
		pb->video_index = 0;
		pb->audio_index = 0;

		int err = 0;





		if (!init_done) {

			av_register_all();
			init_done = true;
		}



		pb->pFormatContext = NULL;

		if (avformat_open_input(&pb->pFormatContext, file, NULL, 0) != 0) {
			logging("ERROR could not open the file");
			return NULL;
		}



		if (avformat_find_stream_info(pb->pFormatContext, NULL) < 0) {
			logging("ERROR could not get the stream info");
			return NULL;
		}





		int video_stream_index = findVideoStream(pb->pFormatContext);
		int audio_stream_index = findAudioStream(pb->pFormatContext);

		if (audio_stream_index == -1) {

			logging("No audio streams found.");
			//return NULL;

		}

		if (video_stream_index == -1) {

			logging("No video streams found.");
			//return NULL;

		}

		pb->video_index = video_stream_index;
		pb->audio_index = audio_stream_index;
		AVCodecContext* videoCodec = NULL;
		AVCodecContext* audioCodec = NULL;

		if (video_stream_index != -1) {
			videoCodec = initStreamCodec(pb->pFormatContext, video_stream_index);
		}
		if (audio_stream_index != -1) {
			audioCodec = initStreamCodec(pb->pFormatContext, audio_stream_index);
		}


		pb->context = videoCodec;
		pb->acontext = audioCodec;


		if (audio_stream_index != -1) {
			pb->acontext->request_sample_fmt = av_get_alt_sample_fmt(pb->acontext->sample_fmt, 0);
		}

		if (video_stream_index != -1) {
			initDecoder(pb->pFormatContext, pb->context, pb->video_index);
		}
		if (audio_stream_index != -1) {
			initDecoder(pb->pFormatContext, pb->acontext, pb->audio_index);
		}



		pb->frame = createFrame();
		pb->aframe = createFrame();

		av_init_packet(&pb->packet1);
		av_init_packet(&pb->apacket1);
		pb->packet = &pb->packet1;
		pb->apacket = &pb->apacket1;

		pb->last_dts = 0.0;

		pb->last_dts = 0;
		pb->last_pts = 0;
		pb->last_pkt_pts = 0;
		pb->last_clock = 0;
		pb->time_base = 0;
		pb->delay = 0;
		pb->dPts = 0.0;
		pb->pktDpts = 0.0;
		pb->dDelay = 0.0;
		pb->dTime = 0.0;
		pb->dClock = 0.0;





		if (audio_stream_index != -1) {
			alGenSources(1, &pb->audioSource);

			alSourcef(pb->audioSource, AL_PITCH, 1);
			alSourcef(pb->audioSource, AL_GAIN, 1);
			alSource3f(pb->audioSource, AL_POSITION, 0, 0, 0);
			alSource3f(pb->audioSource, AL_VELOCITY, 0, 0, 0);
			alSourcei(pb->audioSource, AL_LOOPING, AL_FALSE);

		}







		return pb;

	}





}

// This is the constructor of a class that has been exported.
CVideoNative::CVideoNative()
{
	return;
}
