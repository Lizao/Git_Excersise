#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <mutex>
#include <Windows.h>
#include "mmsystem.h"
#include "WavFile.h"
#include <stdio.h>
#include <thread>

#define SAMPLES 16000
#define SAMPLEBIT 16
#define CHANNELNUM 1
#define CHUNCK_SIZE 10240
#define AR_LOG_FILE_MAX_SIZE 1024*1024*20

#define LOG(fmt, ...) test(fmt, ##__VA_ARGS__)

class Log
{
private:
	Log(FILE* file);
	~Log();
	FILE * m_file;
public:
	static Log *getLogInstance(FILE* file)
	{
		std::mutex mtx;
		std::lock_guard<std::mutex> lock(mtx);
		static Log loginstance(file);
		return &loginstance;
	}
	//void write(const char *format, ...);

};

extern std::string timestring();

extern void test(const char *format, ...);

typedef struct AUDIO_Recog {
	void *UsrData;
	char *SourceFilePath;
	char *saveFilePath;
	// int BEEP_I;
	WavFile * audioin;
	HWAVEIN phwi;
	std::thread* r_j_th;
	WAVEINCAPS waveIncaps;
	WAVEFORMATEX pwfx;
	WAVEHDR pwh1;
	WAVEHDR pwh2;
} AUDIO_Recog;



#endif // LOG_H
