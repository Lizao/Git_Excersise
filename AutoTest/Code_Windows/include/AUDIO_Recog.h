#ifndef  AUDIO_RECOG_H
#define  AUDIO_RECOG_H
#include <string>


typedef enum AUDIO_RECOG_ERROR_TYPE {
	AUDIORECORD_ERROR_TYPE_NO_ERROR,
	AUDIORECORD_ERROR_TYPE_CREATE,
	AUDIORECORD_ERROR_TYPE_START,
	AUDIORECORD_ERROR_TYPE_END,
	AUDIORECORD_ERROR_TYPE_DELETE,
	AUDIORECORD_ERROR_TYPE_FILE_NOTEXIST,
	AUDIORECORD_ERROR_TYPE_OPEN,
	AUDIORECORD_ERROR_TYPE_PLAY,
	AUDIORECORD_ERROR_TYPE_PAUSE,
	AUDIORECORD_ERROR_TYPE_RESUME,
	AUDIORECORD_ERROR_TYPE_STOP,
	AUDIORECORD_ERROR_TYPE_NULLPOINTER,
} AUDIO_RECOG_ERROR_TYPE;


/*
创建对象

- 返回值：
0：创建成功
其他值：创建失败
- 参数：
hinst，void的指针的地址
usrdata，包含用户信息
- 返回类型：
同步返回
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE AudioRecogCreate(void **hinst, void * usrdata);
/*
删除对象

- 返回值：
0：删除成功
其他值：创建失败
- 参数：
hinst，传入创建时所用的指针
- 返回类型：
同步返回
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE AudioRecogDelete(void *hinst);

/*
检查当前系统是否支持录音

- 返回值：
True：支持录音
False：不支持录音
- 参数：
输入参数： string msg，主要是用于返回一些信息，例如如果是返回值是false，需要返回错误的原因等
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE checkRecordVoiceStatus(void * hinst, std::string& msg);


/*
录音开始

- 返回值：
0：开始录制成功
其他值：开始录制失败
- 参数：
hinst，在AudioRecogCreate中的hinst的指针
savefilepath，包含文件名成wave格式
- 音频格式：
wave
- 返回类型：
同步返回
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE AudioRecordStart(void * hinst, const char* savefilepath);

/*
录音结束并保存录音结果

- 返回值：
0：结束录制成功
- 参数：
hinst，在AudioRecogCreate中的hinst的指针
- 返回类型：
同步返回
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE AudioRecordEnd(void *hinst);

/*
录音同时并比较

- 返回值：
1：比较开始成功
2：file不存在
- 参数：
sourceFile:比较原始文件
saveFilePath：保存录音文件路径
writer参数：
data，会将json转换的值传入
类型：string
{
endTime，
match,
recordTime,
saveFilePath,
sourceFile,
startTime
}

- 返回类型：
异步返回，通过注册的writer返回比较结果
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE RecordAndJudge(void *hinst, const char * sourcefilepath, const char *savefilepath, bool(*write) (std::string data, void* userdata));

/*
比较两个存在的voice文件

- 返回值：
类型：int
内容：
{
endTime，
match,
recordTime,
saveFilePath,
sourceFile,
startTime
}
- 参数：
hinst：在AudioRecogCreate中的hinst的指针
sourcefilepath:原始声音
savefilepath:录制声音片段
类型：int

- 返回类型：
异步返回，通过注册的writer返回比较结果
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE JudgeOnly(void *hinst, const char * sourcefilepath, const char *savefilepath, std::string& result);

/*
开始播放声音

- 返回值：
类型：int
- 参数：
hinst：在AudioRecogCreate中的hinst的指针
filepath:待播放的声音
类型：int

- 返回类型：
同步返回
返回值：
0：成功
其他值：失败
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE PlayMusic(void *hinst, const char* filepath, double starttime, double endtime);

/*
暂停播放声音

- 返回值：
类型：int
- 参数：
hinst：在AudioRecogCreate中的hinst的指针
- 返回类型：
同步返回
- 返回值：
0：成功
其他值：失败
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE PauseMusic(void *hinst);

/*
恢复播放声音

- 返回值：
类型：int
- 参数：
hinst：在AudioRecogCreate中的hinst的指针
- 返回类型：
同步返回
- 返回值：
0：成功
其他值：失败
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE ResumeMusic(void *hinst);

/*
停止播放音乐

- 返回值：
类型：int
- 参数：
hinst：在AudioRecogCreate中的hinst的指针
- 返回类型：
同步返回
- 返回值：
0：成功
其他值：失败
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE StopMusic(void *hinst);

/*
比较两个存在的voice文件

- 返回值：
类型：int
- 参数：
file1:原始声音
file2:录制声音片段
starttime:录制声音的开始时间 （-1）
endtime:录制声音的结束时间（-1）
- 返回类型：
同步返回
*/
extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE JudgeOnly2(void *hinst, const char* sourcefilepath, const char *savefilepath, double startime, double endtime, std::string& result);


/*
比较Beep音

-返回值：
类型：int
- 参数：
file1 : 录制声音片段
starttime : 检查beep音的开始时间 （ - 1）
endtime : 检查beep音的结束时间（ - 1）
如果time为 - 1，检查整个片段
- 返回类型：
同步返回
*/

extern "C" __declspec(dllexport) AUDIO_RECOG_ERROR_TYPE JudgeBeep(void *hinst, const char * filepath, double startime, double endtime, std::string& result);


#endif // AUDIO_RECOG_H
