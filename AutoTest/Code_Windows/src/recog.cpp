#include "hash.h"
#include "fingerprint.h"
#include "recog.h"
#include <json/json.h>
#include <algorithm>
#include "log.h"
#include <fstream>
#include <iostream>
#include <direct.h> 
#include <io.h>
#include <sstream>
#include <atltime.h>
#include <thread>

using namespace std;
#define MATCH_PER 0.1

#define HASH_INDEX "hash\\index.txt"
//#define _CRT_NONSTDC_NO_WARNINGS

void test(const char *format, ...)
{
	std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	static FILE *file ;
	char buffer[100];
	_getcwd(buffer, 100);
	string pwdpath;
	pwdpath = buffer;
	string logpath = pwdpath + "\\" + "aclog.log";
	if (!file) {
		errno_t err = fopen_s(&file, logpath.c_str(), "a+");
		if (0 != err) {
			printf("File open WRONG. %d\n", err);
			return;
		}
	}
	Log::getLogInstance(file);
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	if (length > 1024 * 1024 * 20) {
		fclose(file);
		string delfile = " del  " + logpath + " "  ;
		system(delfile.c_str());
		fopen_s(&file, logpath.c_str(), "a+");
	}

	// 基于当前系统的当前日期/时间
	time_t now = time(0);


	//char time_str[26];
	//char* dt = ctime(time_str, 26, &now);
	// 把 now 转换为字符串形式
	//ctime_s(time_str, 26, &now);

	//fprintf(file, "%s", time_str);

	fprintf(file, "time %s : ", timestring().c_str());
	va_list args;
	va_start(args, format);
	vfprintf(file, format, args);
	va_end(args);
	fprintf(file, "\n");
}
 
int makedir(string dir)
{
	if (_access(dir.c_str(), 0) == -1)
	{
		int flag = _mkdir(dir.c_str());

		if (flag != 0)
		{
			LOG("Dir:%s make failed! \n", dir.c_str());
			return -1;
		}
	}
	return 0;
}

float getSourceFileTime(string szSourceFilePath) {
	ifstream myfile(HASH_INDEX);
	if (!myfile.is_open())
	{
		LOG("Hash index file open failed! \n");
		return -1;
	}
	string sCurFile = "";
	float fFileTime = -1;
	while (myfile.peek() != EOF) {
		float fTime = -1;
		myfile >> sCurFile >> fTime;
		if (0 == sCurFile.compare(szSourceFilePath)) {
			fFileTime = fTime;
			break;
		}
	}
	myfile.close();
	return fFileTime;
}

void addSourceFile(string szSourceFilePath, float fSourceFileTime) {
	ofstream outfile(HASH_INDEX, ios::app);
	outfile << szSourceFilePath << " " << fSourceFileTime << endl;
	outfile.close();
}

string ProcessSelelct(const char *szMusicPath, const char *szAudioPath) {
	Json::Value res;
	
	if ((NULL == szMusicPath) || (NULL == szAudioPath)) {

		res["result_type"] = AUDIORECOGNITION_ERROR_TYPE_NULLPOINT;
		return res.toStyledString();
	}

	THash myhash;
	TExtractFeature fingerprint;

	myhash.BuildInit();

	int frame_num = 0;
	ostringstream oss;
	string sSourceFile = szMusicPath;

	int iStart = sSourceFile.rfind("\\");
	int iEnd = sSourceFile.rfind(".");
	string sHashFile = sSourceFile.substr(iStart + 1, iEnd - iStart - 1);

	char buffer[100];
	_getcwd(buffer, 100);
	string pwdpath;
	pwdpath = buffer;
	string hashpath = pwdpath + "\\" + "hash";
	oss << hashpath << "\\" << sHashFile << ".txt";
	float fSourceFileTime = getSourceFileTime(oss.str());
	if (-1 == fSourceFileTime) {
		myhash.AddSongList(szMusicPath);
		frame_num = fingerprint.ExtractPair(szMusicPath, myhash, 0, (float)0.98, 0, 0);
		fingerprint.UnInit();

		makedir(hashpath);
		myhash.Hash2File(oss.str().c_str());
		fSourceFileTime = (float)(frame_num * (float)FRAME_HOP / float(fingerprint.samples));
		addSourceFile(oss.str(), fSourceFileTime);
	}

	myhash.File2Hash(oss.str().c_str());
	res["SourceFileTime"] = fSourceFileTime;

	myhash.ReBuildInit();
	myhash.VoteInit();

	frame_num = fingerprint.ExtractPair(szAudioPath, myhash, 0, (float)0.98, 0, 1);
	fingerprint.UnInit();
	frame_num = fingerprint.ExtractPair(szAudioPath, myhash, 64, (float)0.98, 0, 1);
	fingerprint.UnInit();
	frame_num = fingerprint.ExtractPair(szAudioPath, myhash, 128, (float)0.98, 0, 1);
	fingerprint.UnInit();
	frame_num = fingerprint.ExtractPair(szAudioPath, myhash, 192, (float)0.98, 0, 1);
	fingerprint.UnInit();
	float fSaveFileTime = (float)(frame_num * (float)FRAME_HOP / float(fingerprint.samples));
	res["SaveFileTime"] = fSaveFileTime;
	size_t offset;
	size_t max = myhash.VoteResult(offset);
	res["keyNum"] = Json::Value(static_cast<unsigned int>(myhash.data_num));
	res["match"] = Json::Value(static_cast<unsigned int>(max));
	if (0 == max) {
		res["matchPer"] = Json::Value(static_cast<unsigned int>(0));
		res["matchResult"] = false;
	}
	else {
		float fMatchPer = (float)max / (float)myhash.data_num;
		res["matchPer"] = fMatchPer;
		if ((MATCH_PER * (min(fSourceFileTime, fSaveFileTime) / fSourceFileTime)) <= fMatchPer) {
			res["matchResult"] = true;
		}
		else {
			res["matchResult"] = false;
		}
	}

	res["startTime"] = (float)(offset*(float)FRAME_HOP / float(fingerprint.samples));
	res["endTime"] = res["startTime"].asDouble() + res["SaveFileTime"].asDouble();
	myhash.BuildUnInit();
	res["result_type"] = AUDIORECOGNITION_ERROR_TYPE_NO_ERROR;

	return res.toStyledString();
}


string ProcessBeep(const char *szBeepFilePath, double starttime) {
	TExtractFeature fingerprint;
	string result = fingerprint.BeepJudge(szBeepFilePath, starttime);
	fingerprint.UnInit();
	return result;
}

//string ProcessMusic(const char *szBeepFilePath, double starttime) {
//	TExtractFeature fingerprint;
//	string result = fingerprint.MusicJudge(szBeepFilePath, starttime);
//	fingerprint.UnInit();
//	return result;
//}