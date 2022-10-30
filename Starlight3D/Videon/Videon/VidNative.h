// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the VIDEONATIVE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// VIDEONATIVE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#define VIDEONATIVE_API __declspec(dllexport)


// This class is exported from the dll
class VIDEONATIVE_API CVideoNative {
public:
	CVideoNative(void);
	// TODO: add your methods here.
};

extern VIDEONATIVE_API int nVideoNative;

VIDEONATIVE_API int fnVideoNative(void);



extern "C" {

	struct videoPB;

	videoPB* initVideoNative(const char* file);
	int initAudio();
	int audioHasBegun();
	int decodeNextFrame(videoPB* vid);
	int getFrameWidth(videoPB* vid);
	int getFrameHeight(videoPB* vid);
	void genFrameData(videoPB* vid);
	double getDPTS(videoPB* vid);
	char* getFrameData(videoPB* vid);
	double getDDelay(videoPB* vid);
	int64_t getPict(videoPB* vid);
}