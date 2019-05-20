#ifndef CXX_VR_PLATFORM_H
#define CXX_VR_PLATFORM_H

#ifndef CXX_VR_PLATFORMDEF_H
#   include "VR_PlatformDEF.h"
#endif

#include "boost/shared_ptr.hpp"
#include "VoiceRecog_DEF.h"
#include "VR_RequestQueue.h"
#include "BL_AutoSync.h"
#include "VoiceRecogListener.h"
#include "VR_PlatformIF.h"
#include "VR_ProtoMessageProcessIF.h"
#include "VR_StatusProvider.h"
#include "MEM_map.h"
#include "VR_ActionOperateApp.h"
#include "VR_ActionOperateSetting.h"
#include "VR_ActionOperateVolume.h"
#include "VC_AudioSession.h"
#include "VR_WakeupSetting.h"
#include "Music/VR_MusicAccessor.h"
#include "VREngine_AudioIn.h"
#include "VR_AudioStream.h"
#include "VR_ListenTopApp.h"
#include "VR_ListenMultimediaStatus.h"
#include "NCNetworkManager.h"
#include "VR_ActionGetFavoritesNum.h"
#include "NCPowerStatus.h"

class TiXmlElement;
class VR_PlatformConfig;
class VRE_Engine;
class VR_ContactAccessor;
class VR_MusicAccessor;
class VR_AppNaturalAccessor;
class VR_iAutoAudioImp;
class VR_Platform;
class VR_SearchFacilityInfo;

class VR_AudioSessionListener :public VC_AudioSession::VC_AudioSessionListener
{
public:
    VR_AudioSessionListener(VR_Platform * platform) { m_platform = platform; }
    virtual void OnMicChange(int vrType, bool status);
    virtual void OnChannelCloseByOS() {}

private:
    VR_Platform *m_platform;
    VR_AudioSessionListener(const VR_AudioSessionListener &);
    VR_AudioSessionListener& operator=(const VR_AudioSessionListener&);
};

class VR_Platform  : public VR_PlatformIF
                   , public VR_ProtoMessageProcessIF
{
public:
    VR_Platform();
    virtual ~VR_Platform();

    virtual bool Initialize(const std::string &config);
    virtual void Destroy();
    virtual void Start();
    virtual void Stop();

    // service control
    virtual void StopServiceProcess();

    // event VR_ProtoMessageProcessIF//
    virtual void OnRequestQueueNeed(const char* msg_name, void* msg);

    virtual void OnReqCreateSession(const VR_SenderInfo &sender, int iLang, int max_time);
    virtual void OnReqCloseSession(const VR_SenderInfo &sender);
    virtual void OnReqStartVoiceRecog(const VR_SenderInfo &sender, const VR_RecogVoiceWithCommadsArgs &args);
    virtual void OnReqCancelVoiceRecog(const VR_SenderInfo &sender);
    virtual void OnReqMic(const VR_SenderInfo &sender);
    virtual void OnReqMic(const VR_SenderInfo &sender, const VoiceVector<std::string>::type &wavepaths);
    virtual void OnCancelHTMLTTS(const VR_SenderInfo &sender);
    virtual void OnReqCancel(const VR_SenderInfo &sender);
    virtual void OnReqStartOver(const VR_SenderInfo &sender);
    virtual void OnSelect(const VR_SenderInfo &sender, int index);
    virtual void OnListPageUp(const VR_SenderInfo &sender);
    virtual void OnListPageDown(const VR_SenderInfo &sender);
    virtual void OnLanguageChanged(const VR_SenderInfo &sender, int iLang, const std::string &strCountry);
    virtual void OnSwitchLanguageEnd(const VR_SenderInfo &sender);
    virtual void OnGetDataVersion(const VR_SenderInfo &sender, std::string& verStr);
    virtual void OnGetServiceStatus(const VR_SenderInfo &sender);
    virtual void OnGetAsrState(const VR_SenderInfo &sender);

    virtual void OnServiceStarted();
    virtual void OnClientDisconnected(const std::string &id);
    virtual void OnLocationChange(long lon, long lat);
    virtual void OnDistanceChange(const VR_RemainInfo& info);
    virtual void OnMediaScanUpdated(bool status, const char* dbPath, int64_t deviceID);
    virtual void OnMediaDatabaseUpdated(const VR_MediaInfo &info);
    virtual void OnBTPhoneBookUpdated(bool status);
    virtual void OnCityChanged(const std::string &cityName, int cityId);
    virtual void OnDisplayOnOff(bool status);

    // listener
    virtual void SetUIListener(VRCP_UIListener* pcListener);

    // function VR_PlatformIF//
    virtual bool IsNetworkConnected();
    virtual void GetLocation(long& lLon, long& lLat);
    virtual void FindContact(VRCP_ListenerBase* pcListener, int& reqID,const std::string& name, int PhoneType = PHONE_NONE);
    virtual void SendSMS(const std::string& name, const std::string& phoneNum, const std::string& content);
    virtual void StartApp(VRCP_ListenerBase *pcListener, int& reqID, const std::string& name, const std::string& param);
    virtual void ChangeState(SESSION_STATE s, VRCP_ListenerBase *pListener = NULL);
    virtual void SetVrMod(bool bOn);
    virtual void ReplyCreatSessionResult(const std::string &id, const std::string &receiver, const std::string &session, int errorCode);
    virtual void ReplyCloseSessionResult(const std::string &id, const std::string &receiver, const std::string &session, int errorCode);
    virtual void ReplyRecogWithCommandsResult(const std::string &id, const std::string &session, int user_data, int error_code, const StdMap<float, std::string>::type &result);
    virtual void NotifySessionClosed(const std::string &id, const std::string &session, int reason);
    virtual void StartInstalledApp(const VR_AppInfo& appInfo);
    virtual void CloseInstalledApp(const VR_AppInfo& appInfo);
    virtual void OperateSettingOn(const VR_SettingInfo& settingInfo);
    virtual void OperateSettingOff(const VR_SettingInfo& settingInfo);
    virtual void SetSearchKeyWordInfo(const VR_SearchKeyWordInfo &keywordInfo);
    virtual void SetFacilityInfo(const VR_SearchFacilityInfo & facilityInfo);
    virtual void GetSearchKeyWordInfo(VR_SearchKeyWordInfo &keywordInfo);
    virtual void GetFacilityInfo(VR_SearchFacilityInfo & facilityInfo);
    virtual void SetRoutePolicyStatus(const int& routePolicy);
    virtual int  GetRoutePolicyStatus();
    virtual void MuteVolume(const VR_VolumeInfo& volumeInfo);
    virtual void MuteOffVolume(const VR_VolumeInfo& volumeInfo);
    virtual void IncVolume(const VR_VolumeInfo& volumeInfo);
    virtual void DecVolume(const VR_VolumeInfo& volumeInfo);
    virtual void TurnToVolume(const VR_VolumeInfo& volumeInfo);

    virtual void GetCurVolume(const VRSDS_VOLUME_TYPE type, int& volvalue);
    virtual void GetCurActiveChannelType(int& mediaActive);
    virtual bool RequestMic(VR_AUDIO_TYPE ownerType);
    virtual void ReleaseMic(VR_AUDIO_TYPE ownerType);

    // engine
    virtual void SwitchLanguage(VRCP_ListenerBase* pcListener, int& reqID, int iLang, const std::string &strCountry);

    // point
    virtual void IsHomeRegister(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void IsCorpRegister(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetDestHistory(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetFavorite(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void RegistHomeAddress(VRCP_ListenerBase* pcListener, const VoiceVector<VR_POIInfo>::type &info, int& reqID);
    virtual void RegistCorpAddress(VRCP_ListenerBase* pcListener, const VoiceVector<VR_POIInfo>::type& info,int& reqID);
    virtual void GetActionIntentKey(std::string& intent);
    virtual void SetActionIntentKey(std::string intent);
    virtual void GetDestinationInfo(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetRegisteredPoint(VRCP_ListenerBase* pcListener, int& reqID, int iType);

    // guid
    virtual void IsDestExist(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void IsRouteExist(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetRemainDistance(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetRemainTime(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetGuidanceStatus(VRCP_ListenerBase* pcListener, int& reqID);

    // BT
    virtual void GetRecentCall(VRCP_ListenerBase* pcListener, int& reqID, int m_cCategory);
    virtual void GetHFPConnectStatus(bool& bResult);
    virtual void GetDialBackCalls(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetRedialCalls(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void ShowTelFavorites(int& reqID);
    virtual void GetFavoritesNum(VRCP_ListenerBase* pcListener, int& reqID, int contactNum);

    // search
    virtual void SearchKeyWord(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchKeyWordInfo &info);
    virtual void SearchPOI(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchKeyWordInfo& info);
    virtual void SearchFacility(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchFacilityInfo& info);
    virtual void SearchPostCode(VRCP_ListenerBase* pcListener, int& reqID, const std::string& postCode);
    virtual void SearchAlongRoute(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchAlongRouteInfo & info);
    virtual void AddWayPoint(VRCP_ListenerBase* pcListener, int& reqID, const VR_POIInfo & info);

    // voice
    virtual void PlayBeep(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void PlayTTS(VRCP_ListenerBase* pcListener, int& reqID, const std::string& ttsContent);
    virtual void StartSpeechRecog(VRCP_ListenerBase* pcListener, int& reqID, const std::string& szGrammerID);
    virtual void StartSpeechRecogWithCommands(VRCP_ListenerBase* pcListener, int& reqID, const std::string& szGrammerID, const VoiceVector<std::string>::type &commands, int bof, int max_time);
    virtual void StartBargeInVR(VRCP_ListenerBase* pcListener, int& reqID, const std::string& ttsContent, const std::string& szGrammerID);

    // UI
    virtual void CalculateRoute(VRCP_ListenerBase* pcListener, int& reqID,const VoiceVector<VR_POIInfo>::type& info, const int routeType);
    virtual void CallPhone(const VR_PhonenumInfo& info);
    virtual void ShowPrompt(const std::string& info, bool bFullScreen = false);
    virtual void ShowHints(const VoiceVector<std::string>::type&hints);
    virtual void ShowView(const std::string& viewName);
    virtual void ShowList(const VoiceVector<std::string>::type& listItems, int iTotalPage, int iCurrPage);
    virtual void ShowPoiInfo(const std::string &poiInfo);
    virtual void HideList();
    virtual void ShowImage(const std::string& szImagePath);
    virtual void ShowPOIList(const VoiceVector<VR_POIInfo>::type& info);
    virtual void ShowWeatherInfo(const VR_WeatherInfo& info);
    virtual void ChangeVRState(VR_VRSTATE eState);
    virtual void ChangeVRState(const std::string &id, const std::string &session, VR_VRSTATE eState);
    virtual void NotifyServiceStop(const std::string &id, const std::string &session);
    virtual void ShowSpeechText(VRCP_ListenerBase* pListener, const std::string & info );//9527
    virtual void ShowMark(VRCP_ListenerBase *pListener, int &reqID, bool bOpen, const std::string &markName);
    virtual bool IsSupportChargerIcon();

    // music
    virtual void CheckSupportStatus(bool &bSupport, int iControlType);
    virtual void isCurrentlyMusicPlay(bool &bMusicPlay);
    virtual void isCurrentlyMusicPause(bool &bMusicPause);
    virtual void PlayControl(int iControlType);
    virtual void PlayMedia(const VR_MusicInfo& info);
    virtual void MediaPlayingStatus(int &iPlayStatus);
    virtual void MediaDeviceStatus(int &iDevicetatus);
    virtual void GetArtistAccessor(const VR_MusicInfo& info, boost::shared_ptr<VR_MusicAccessor>& accessor);
    virtual void GetSongListByArtist(VRCP_ListenerBase* pcListener, int& reqID, VR_MusicInfo &info);

    // source
    virtual void isAllSourceOff(bool & isAllOff);
    virtual void SourceOff();
    virtual void ChangeSource(const VR_SourceInfo&);
    virtual void GetSourceList(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetSourceStatus(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetSourceValib(VRCP_ListenerBase* pcListener, int& reqID);
    virtual void GetLastAvSource(int& iSourceID, int &iSubSourceID);
    virtual bool IsSupportAm();
    virtual void isNetworkOn(bool & isNetworkOn);
    virtual void OpenWiFiSetting();
    virtual void OpenBTSetting();

    // navi
    virtual std::string GetCurrentCity();
    virtual void SwitchTrafficInfo(VRCP_ListenerBase* pcListener, int& reqID, bool bOpen);
    virtual void SwitchViewMode(VRCP_ListenerBase *pcListener, int& reqID,const VR_SwitchViewInfo & switchInfo);
    virtual void ZoomMapView(VRCP_ListenerBase* pcListener, int& reqID,const int zoomMapDirection);
    virtual void AddCurrentPoint(VRCP_ListenerBase* pcListener, int& reqID,const int curPointType);
    virtual void ShowCurrentPosition(VRCP_ListenerBase* pcListener, int& reqId);
    virtual void ShowPoiPosition(VRCP_ListenerBase* pcListener, int& reqId, const VR_POIInfo& poiInfo);
    virtual void ShowWholeRoute(VRCP_ListenerBase *pcListener, int& reqID);
    virtual void CancelRoute(VRCP_ListenerBase* pcListener, int& reqId);

     // request control
    virtual void StopRequest(int reqID);
    virtual void CancelRequest(int reqID);

    // information
    virtual void QueryWeather(VRCP_ListenerBase* pcListener, int& reqID, const VR_QueryWeatherInfo& info);
    virtual void QueryTrafficRestriction(VRCP_ListenerBase* pcListener, int& reqID, const std::string& cityname, const std::string& iscurrentday);

    virtual void StartWakeup(VRCP_ListenerBase *pcListener, int &reqID, const std::string &cSceneID);
    virtual void StopWakeup();
    virtual void SetWakeupSwitch(const std::string & status);
    virtual void OnWakeup();
    virtual void StopAudioStream();
    virtual int GetWakeupType();
    virtual void SetWakeupType(int iType);
    virtual void AddScene(std::string scene);
    virtual void StopScene(std::string scene);
    virtual void CommonCommand(std::string command);
    virtual void StartAudioStream();

public:
    boost::shared_ptr<VRE_Engine> GetEngine();
    boost::shared_ptr<VRE_Engine> SetEngine(boost::shared_ptr<VRE_Engine> engine);
protected:
    bool                                        m_bTestMode;    ///< for test
    int                                         m_CurrentPage;  ///< for test
    bool                                        m_bIsSwitchLang;
    bool                                        m_bIsSwitchCountry;
    bool                                        m_bNeedVrModeDelay;
protected:
    typedef VoiceMap<int, boost::shared_ptr<VR_MusicAccessor> >::type VR_MusicAccessorMap;

    VRCP_UIListener*                            m_pcUIListener;
    CL_SyncObj                                  m_MemberSyncObj;
    boost::shared_ptr<VR_PlatformConfig>        m_pPlatformConfig;
    boost::shared_ptr<VRE_Engine>               m_pcEngine;
    boost::shared_ptr<VRE_Engine>               m_pcEngine_For_Switch_Langugae;
    boost::shared_ptr<VRE_Engine>               m_pcEngineWakeup;
    boost::shared_ptr<VR_ContactAccessor>       m_pContactAccessor;
    boost::shared_ptr<VR_WakeupSetting>         m_pWakeupSetting;
    boost::shared_ptr<VR_ListenTopApp>          m_pListenTopApp;
    boost::shared_ptr<VR_ListenMultimediaStatus>  m_pListenMultimediaStatus;
    boost::shared_ptr<VR_AudioStream>           m_pAudioStream;
    VR_MusicAccessorMap                         m_MusicAccessorMap;
    boost::shared_ptr<VR_AppNaturalAccessor>    m_pAppNaturalAccessor;
    boost::shared_ptr<VR_RequestQueue>          m_pRequestQueue;
    boost::shared_ptr<VR_StatusProvider>        m_pStatusProvider;
    boost::shared_ptr<VR_iAutoAudioImp>         m_pIAutoAudio;
    VR_AudioSessionListener                    *m_pSessionListener;
    NVREngine::VREngine_AudioIn                *m_pAudioInFile;
    VR_vecString                                m_vecPathRecognizing;
    boost::shared_ptr<NCNetworkManager>         m_pNCNetworkManager;
    nutshell::NCPowerStatus                     m_powerStatus;


private:
    VR_Platform(const VR_Platform &);
    VR_Platform& operator=(const VR_Platform&);

    void NotifyMeterStatusChanged(VR_VRSTATE eState);

    std::string GetPathRecognizing();
    NVREngine::VREngine_AudioIn * GetAudioIn();
    void ResetSearchPreferStatus();
};

#endif
