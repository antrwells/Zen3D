#pragma once
#include <thread>
#include <queue>

class Texture2D;
struct videoPB;

struct VideoFrame {

	Texture2D* Img;
	int PTS, PKTPTS, DTS;
	int W, H;
	char* data = nullptr;
	int Clock;
	double DPTS = 0.0;
	double DDELAY = 0.0;
	int PICT;
	int CacheID = -1;
	int madeTime = 0;
};

class VideoDecoder
{
public:
	VideoDecoder();

	bool OpenVideo(const char* path);
	bool IsPaused() {
		return mPaused;
	}
	bool GotAudioTime() {
		return mGotAudioTime;
	}
	void SetGotAudioTime() {
		mGotAudioTime = true;
	}
	bool AudioHasBegun();
	void SetAudioStartTime(int time) {
		mAudioStartTime = time;
	}
	int GetAudioStartTime() {
		return mAudioStartTime;
	}
	void SetAudioClock(double ac)
	{
		AUDIOCLOCK = ac;
	}
	double GetAudioClock() {
		return AUDIOCLOCK;
	}
	std::queue<VideoFrame*> GetFrames() {
		return Frames;
	}
	void SetFrames(std::queue<VideoFrame*> frames)
	{
		Frames = frames;
	}
	double GetTimeDelta() {
		return TimeDelta;
	}
	void SetTimeDelta(double td)
	{
		TimeDelta = td;
	}
	void DecodeNextFrame();
	Texture2D* GetCurrentImage();
	VideoFrame* GetCurrentFrame();
	void DecodeThreadBody();
private:
	int time_start = 0;
	videoPB* pVideo;
	std::thread thread_Decode;
	bool mPaused = false;
	bool mGotAudioTime = false;
	int mAudioStartTime = 0;
	double AUDIOCLOCK = 0.0;
	double TimeDelta = 0.0;
	double CLOCK = 0.0;
	int LastTick = 0;
	std::queue<VideoFrame*> Frames;
	VideoFrame* CurrentFrame = nullptr;
	VideoFrame* UpFrame = nullptr;
	Texture2D* UpTex = nullptr;
	std::vector<VideoFrame*> allFrames;
	int frames = 0;
};

