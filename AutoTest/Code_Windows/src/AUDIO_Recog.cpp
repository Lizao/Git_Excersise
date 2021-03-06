#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "AUDIO_Recog.h"
#include "AudioProcess.h"
#include "recog.h"
#include "WavFile.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include <string>
#include <algorithm>
#include  <direct.h>  
#include  <stdio.h> 
#include <sstream>
#include "log.h"
#include <stdarg.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define MAX_FILE_NAME 256
#define RECORDTIME 5



//wchar_t：在windows下是Unicode 16编码
//char：指一个字节，函数调用时需要char型，所以将w_char转成char型
string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)
{
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	string str = m_char;
	delete m_char;
	return str;
}

//将utf转成本代码需要使用的unicode型，但是转成Unicode必然将char型变成了w_char型
string Utf82Unicode(const char* utf)
{
	if (!utf || !strlen(utf))
	{
		return 0;
	}
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf, -1, NULL, 0);
	size_t num = dwUnicodeLen * sizeof(wchar_t);

	wchar_t* wstr = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, utf, -1, wstr, dwUnicodeLen);
	string str = WcharToChar(wstr);
	delete wstr;
	return str;
}

//====================判断文件是否存在

bool whether_exist(const char* filepath)
{
	fstream _file;

	if (NULL == filepath) {
		LOG("filepath is null!");
		return false;
	}
	_file.open(filepath, ios::in);
	if (!_file)
	{
		LOG("%s not exist!", filepath);
		_file.close();
		return false;
	}
	_file.close();
	return true;
}

//================================task 1=================================\\

AUDIO_RECOG_ERROR_TYPE AudioRecogCreate(void **hinst, void * usrdata) {
	LOG("AudioRecogCreate start!");
	if (NULL == hinst) {
		LOG("hinst is null!");
		return AUDIORECORD_ERROR_TYPE_CREATE;
	}

	AUDIO_Recog* audiorecord = (AUDIO_Recog*)malloc(sizeof(AUDIO_Recog));//创建了一个结构体指针，指向一块存了该结构体结构的数据
	if (NULL == audiorecord)
	{
		LOG("audiorecord is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	audiorecord->UsrData = usrdata;
	audiorecord->audioin = NULL; // new WavFile();
	audiorecord->saveFilePath = (char*)malloc(sizeof(char) * MAX_FILE_NAME);
	if (NULL == audiorecord->saveFilePath)
	{
		LOG("audiorecord->saveFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	audiorecord->SourceFilePath = (char*)malloc(sizeof(char) * MAX_FILE_NAME);
	if (NULL == audiorecord->SourceFilePath)
	{
		LOG("audiorecord->SourceFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	audiorecord->pwh1.lpData = NULL;
	audiorecord->pwh2.lpData = NULL;
	audiorecord->r_j_th = NULL;
	*hinst = audiorecord; //指向该结构体指针的指针

	return AUDIORECORD_ERROR_TYPE_NO_ERROR;
}

AUDIO_RECOG_ERROR_TYPE AudioRecogDelete(void *hinst) {
	LOG("AudioRecogDelete start!");
	if (NULL == hinst) {
		LOG("AudioRecogDelete:hinst is null!");
		return AUDIORECORD_ERROR_TYPE_DELETE;
	}
	AUDIO_Recog* audiorecord = (AUDIO_Recog*)hinst;
	if (NULL != audiorecord->r_j_th) {
		if (audiorecord->r_j_th->joinable()) {
			audiorecord->r_j_th->join();
		}
		delete audiorecord->r_j_th;
		audiorecord->r_j_th = NULL;
	}

	if (NULL != audiorecord->SourceFilePath) {
		free(audiorecord->SourceFilePath);
		audiorecord->SourceFilePath = NULL;
	}

	if (NULL != audiorecord->saveFilePath) {
		free(audiorecord->saveFilePath);
		audiorecord->saveFilePath = NULL;
	}
	free(hinst);
	LOG("--------------------------------------------------\n\n");
	return AUDIORECORD_ERROR_TYPE_NO_ERROR;
}

AUDIO_RECOG_ERROR_TYPE checkRecordVoiceStatus(void * hinst, std::string& msg) {
	return checkRecordVoice(hinst, msg);
}

// 返回0表示成功
AUDIO_RECOG_ERROR_TYPE AudioRecordStart(void * hinst, const char* savefilepath) {
	string save = Utf82Unicode(savefilepath);

	if (NULL == hinst) {
		LOG("hinst is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	if (NULL == save.c_str())
	{
		LOG("savefilepath is null!");
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	AUDIO_Recog *audiorecord = (AUDIO_Recog*)hinst;
	if (NULL == audiorecord->saveFilePath) {
		LOG("audiorecord->saveFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}

	strncpy_s(audiorecord->saveFilePath, MAX_FILE_NAME, save.c_str(), save.length() + 1);
	return startRecord(audiorecord);
}

AUDIO_RECOG_ERROR_TYPE AudioRecordEnd(void * hinst) {
	AUDIO_Recog * audiorecord = (AUDIO_Recog*)hinst;
	return endRecord(audiorecord);
}

void record_judge(void *hinst, bool(*write) (string, void*))
{
	if (NULL == hinst)
	{
		LOG("hinst is null!");
		return;
	}

	//录音部分
	AUDIO_Recog *judge_record = (AUDIO_Recog*)hinst;

	int res = startRecord(judge_record);
	if (MMSYSERR_NOERROR != res)
	{
		LOG("start record error \n");
		return;
	}
	Sleep(RECORDTIME * 1000);
	endRecord(judge_record);

	if (!whether_exist(judge_record->saveFilePath)) {
		LOG(" record error! record file produce failed");
		return;
	}
	//Judge 部分
	string result;

	result = ProcessSelelct(judge_record->SourceFilePath, judge_record->saveFilePath);

	write(result, judge_record->UsrData);
}

AUDIO_RECOG_ERROR_TYPE RecordAndJudge(void *hinst, const char * sourcefilepath, const char *savefilepath, bool(*write) (std::string, void*))
{
	LOG("RecordAndJudge start!");
	string source = Utf82Unicode(sourcefilepath);
	string save = Utf82Unicode(savefilepath);

	if (!whether_exist(source.c_str()))
	{
		LOG("%s not exist!", source.c_str());
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	if (NULL == hinst)
	{
		LOG("hinst is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}

	AUDIO_Recog *judge_two = (AUDIO_Recog*)hinst;

	strncpy_s(judge_two->saveFilePath, MAX_FILE_NAME, save.c_str(), save.length() + 1);
	strncpy_s(judge_two->SourceFilePath, MAX_FILE_NAME, source.c_str(), source.length() + 1);
	judge_two->r_j_th = new thread(record_judge, hinst, write);
	if (NULL == judge_two->r_j_th)
	{
		LOG("judge_two->r_j_th is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	return AUDIORECORD_ERROR_TYPE_NO_ERROR;
}

AUDIO_RECOG_ERROR_TYPE JudgeOnly(void *hinst, const char * sourcefilepath, const char *savefilepath, std::string& result) {
	LOG("JudgeOnly start!");
	string source = Utf82Unicode(sourcefilepath);
	string save = Utf82Unicode(savefilepath);

	if (!whether_exist(source.c_str()))
	{
		LOG("%s not exist!", source.c_str());
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	if (!whether_exist(save.c_str()))
	{
		LOG("%s not exist!", save.c_str());
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	if (NULL == hinst)
	{
		LOG("hinst is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	AUDIO_Recog *judge_two = (AUDIO_Recog*)hinst;

	if (NULL == judge_two->saveFilePath)
	{
		LOG("judge_two->saveFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	if (NULL == judge_two->SourceFilePath)
	{
		LOG("judge_two->SourceFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}


	strncpy_s(judge_two->saveFilePath, MAX_FILE_NAME, save.c_str(), save.length() + 1);
	strncpy_s(judge_two->SourceFilePath, MAX_FILE_NAME, source.c_str(), source.length() + 1);

	//============Judge============

	result = ProcessSelelct(source.c_str(), save.c_str());

	return AUDIORECORD_ERROR_TYPE_NO_ERROR;
}

//==============================task 2.2 v2.0===============================\\

AUDIO_RECOG_ERROR_TYPE JudgeOnly2(void *hinst, const char * sourcefilepath, const char *savefilepath, double starttime, double endtime, std::string &result) {

	LOG("JudgeOnly2 start!");
	string source = Utf82Unicode(sourcefilepath);
	string save = Utf82Unicode(savefilepath);

	if (!whether_exist(source.c_str()))
	{
		LOG("%s not exist!", source.c_str());
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}
	if (!whether_exist(save.c_str()))
	{
		LOG("%s not exist!", save.c_str());
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	if (NULL == hinst)
	{
		LOG("hinst is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	AUDIO_Recog *judge_two = (AUDIO_Recog*)hinst;

	if (NULL == judge_two->saveFilePath)
	{
		LOG("judge_two->saveFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	if (NULL == judge_two->SourceFilePath)
	{
		LOG("judge_two->SourceFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}

	strncpy_s(judge_two->saveFilePath, MAX_FILE_NAME, save.c_str(), save.length() + 1);
	strncpy_s(judge_two->SourceFilePath, MAX_FILE_NAME, source.c_str(), source.length() + 1);

	char buffer[100];
	_getcwd(buffer, 100);
	string pwdpath;
	pwdpath = buffer;
	string save_wav_piece = "saveFile_piece.wav";
	string path = pwdpath + "\\" + save_wav_piece;

	//===========截取音频===========
	char start_time[10];
	sprintf_s(start_time, 10, "%.3f", starttime);

	char end_time[10];
	sprintf_s(end_time, 10, "%.3f", endtime);

	string syscmd_piece = "ffmpeg.exe -i  \"" + save + "\" -ss  " + start_time + " -to " + end_time + "  " + path + " -y -loglevel quiet";
	system(syscmd_piece.c_str());

	if (!whether_exist(path.c_str()))
	{
		LOG("saveFile_piece.wav not exist!");
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	//=============Jugde=============
	result = ProcessSelelct(source.c_str(), path.c_str());

	//============删除临时文件============

	if (whether_exist(path.c_str()))
	{
		string save_wav_piece_str = "del " + path;
		system(save_wav_piece_str.c_str());

	}
	else
	{
		LOG("saveFile_piece.wav not exist!");
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	return  AUDIORECORD_ERROR_TYPE_NO_ERROR;
}

//==============================task 2.3===============================\\

AUDIO_RECOG_ERROR_TYPE JudgeBeep(void *hinst, const char * filepath, double starttime, double endtime, std::string &result) {

	LOG("JudgeBeep start!");
	string save = Utf82Unicode(filepath);
	if (!whether_exist(save.c_str()))
	{
		LOG("beep test file %s not exist!", save.c_str());
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	if (NULL == hinst)
	{
		LOG("hinst is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}
	AUDIO_Recog *judgebeep = (AUDIO_Recog*)hinst;

	if (NULL == judgebeep->SourceFilePath)
	{
		LOG("judgebeep->SourceFilePath is null!");
		return AUDIORECORD_ERROR_TYPE_NULLPOINTER;
	}

	strncpy_s(judgebeep->SourceFilePath, MAX_FILE_NAME, save.c_str(), save.length() + 1);

	//===========截取音频===========
	char start_time[10];
	sprintf_s(start_time, 10, "%.3f", starttime);

	char end_time[10];
	sprintf_s(end_time, 10, "%.3f", endtime);

	char buffer[100];
	_getcwd(buffer, 100);
	string pwdpath;
	pwdpath = buffer;
	string file_piece = "piece.wav";
	string path = pwdpath + "\\" + file_piece;

	string syscmd_piece = "ffmpeg.exe -i  \"" + save + "\" -ss  " + start_time + " -to " + end_time + "  " + file_piece + " -y -loglevel quiet";
	system(syscmd_piece.c_str());



	if (!whether_exist(path.c_str()))
	{
		LOG("piece.wav not exist!");
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	//============进行beep音判断========
	result = ProcessBeep(path.c_str(), starttime);

	//=========删除临时文件=========
	if (whether_exist(path.c_str()))
	{
		string save_wav_piece_str = "del  " + path;
		system(save_wav_piece_str.c_str());

	}
	else
	{
		LOG("piece.wav not exist!");
		return AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST;
	}

	return  AUDIORECORD_ERROR_TYPE_NO_ERROR;

}


AUDIO_RECOG_ERROR_TYPE PlayMusic(void *hinst, const char* filepath, double starttime, double endtime)
{
	string save = string(filepath);
	return playMusic(save.c_str(), starttime, endtime);
}

AUDIO_RECOG_ERROR_TYPE PauseMusic(void *hinst)
{
	return pauseMusic();
}

AUDIO_RECOG_ERROR_TYPE ResumeMusic(void *hinst)
{
	return resumeMusic();
}

AUDIO_RECOG_ERROR_TYPE StopMusic(void *hinst)
{
	return stopMusic();
}