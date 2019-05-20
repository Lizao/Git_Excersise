/*
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Created on: 12/12/2013
 * Author    : webcloud
 */

/**
 * @file VR_PlatformIF.h
 * @class VR_PlatformIF
 * @brief
 *
 */

#ifndef VR_PLATFORMIF_H
#define VR_PLATFORMIF_H
#include "stdafx.h"
#include "VR_PlatformDEF.h"
#include "VoiceRecog_DEF.h"
#include <string>
#include "MEM_vector.h"
#include "VR_SwitchViewInfo.h"
#include "VR_ActionOperateApp.h"
#include "VR_ActionOperateSetting.h"
#include "VR_ActionOperateVolume.h"
#include "Music/VR_MusicAccessor.h"
#include "NCNetworkManager.h"

class VR_JNI_Listener;

class VR_ContactInfo;
class VR_MusicInfo;
class VR_SourceInfo;
class VR_PhonenumInfo;
class VR_SearchFacilityInfo;
class VR_SearchKeyWordInfo;
class VR_SearchSpecAlongInfo;
class VR_SearchAlongRouteInfo;
class VR_AppInfo;
class VR_QueryWeatherInfo;
class VR_VolumeInfo;
class VR_SearchFacilityInfo;
class VR_SearchKeyWordInfo;

class VR_PlatformIF
{
public:
    virtual ~VR_PlatformIF(){}

public:
    virtual bool Initialize(const std::string &) = 0;
    virtual void Destroy() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    // service control
    virtual void StopServiceProcess() = 0;

public:
    virtual bool IsNetworkConnected() = 0;
    virtual void GetLocation(long& lLon, long& lLat) = 0;
    virtual void FindContact(VRCP_ListenerBase* pcListener, int& reqID, const std::string& name, int PhoneType = PHONE_NONE) = 0;
    virtual void SendSMS(const std::string& name, const std::string& phoneNum, const std::string& content) = 0;
    virtual void StartApp(VRCP_ListenerBase *pcListener, int& reqID, const std::string& name, const std::string& param) = 0;
    virtual void ChangeState(SESSION_STATE s, VRCP_ListenerBase *pListener = NULL) = 0;
    virtual void SetVrMod(bool bOn) = 0;
    virtual void ReplyCreatSessionResult(const std::string &id, const std::string &receiver, const std::string &session, int errorCode) = 0;
    virtual void ReplyCloseSessionResult(const std::string &id, const std::string &receiver, const std::string &session, int errorCode) = 0;
    virtual void ReplyRecogWithCommandsResult(const std::string &id, const std::string &session, int user_data, int error_code, const StdMap<float, std::string>::type &result) = 0;
    virtual void NotifySessionClosed(const std::string &id, const std::string &session, int reason) = 0;
    virtual void StartInstalledApp(const VR_AppInfo& appInfo) = 0;
    virtual void CloseInstalledApp(const VR_AppInfo& appInfo) = 0;
    virtual void OperateSettingOn(const VR_SettingInfo& settingInfo) = 0;
    virtual void OperateSettingOff(const VR_SettingInfo& settingInfo) = 0;
    virtual void MuteVolume(const VR_VolumeInfo& volumeInfo) = 0;
    virtual void MuteOffVolume(const VR_VolumeInfo& volumeInfo) = 0;
    virtual void IncVolume(const VR_VolumeInfo& volumeInfo) = 0;
    virtual void DecVolume(const VR_VolumeInfo& volumeInfo) = 0;
    virtual void TurnToVolume(const VR_VolumeInfo& volumeInfo) = 0;
    virtual void GetCurVolume(const VRSDS_VOLUME_TYPE type, int& volvalue) = 0;
    virtual void GetCurActiveChannelType(int& mediaActive) = 0;
    virtual void SetSearchKeyWordInfo(const VR_SearchKeyWordInfo &) = 0;
    virtual void GetSearchKeyWordInfo(VR_SearchKeyWordInfo &)       = 0;
    virtual bool RequestMic(VR_AUDIO_TYPE ownerType) = 0;
    virtual void SetFacilityInfo(const VR_SearchFacilityInfo & facilityInfo) = 0;
    virtual void GetFacilityInfo(VR_SearchFacilityInfo & facilityInfo) = 0;
    virtual void SetRoutePolicyStatus(const int& routePolicy) = 0;
    virtual int  GetRoutePolicyStatus() = 0;
    virtual void ReleaseMic(VR_AUDIO_TYPE ownerType) = 0;
     virtual void StopAudioStream() = 0;
    virtual void StartAudioStream() = 0;

    // listener
    virtual void SetUIListener(VRCP_UIListener* pcListener) = 0;

    // engine
    virtual void SwitchLanguage(VRCP_ListenerBase* pcListener, int& reqID, int iLang, const std::string &strCountry) = 0;

    // wakeup
    virtual void StartWakeup(VRCP_ListenerBase *pcListener, int &reqID, const std::string &cSceneID) = 0;
    virtual void StopWakeup() = 0;
    virtual void SetWakeupSwitch(const std::string & status) = 0;
    virtual int  GetWakeupType() = 0;
    virtual void SetWakeupType(int iType) = 0;
    virtual void AddScene(std::string scene) = 0;
    virtual void StopScene(std::string scene) = 0;
    virtual void CommonCommand(std::string command) = 0;

    // point
    virtual void IsHomeRegister(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void IsCorpRegister(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetDestHistory(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetFavorite(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void RegistHomeAddress(VRCP_ListenerBase* pcListener, const VoiceVector<VR_POIInfo>::type &info, int& reqID) = 0;
    virtual void RegistCorpAddress(VRCP_ListenerBase* pcListener, const VoiceVector<VR_POIInfo>::type &info, int& reqID) = 0;
    virtual void GetActionIntentKey(std::string& intent) = 0;
    virtual void SetActionIntentKey(std::string intent) = 0;
    virtual void GetDestinationInfo(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetRegisteredPoint(VRCP_ListenerBase* pcListener, int& reqID, int iType) = 0;

    // guid
    virtual void IsDestExist(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void IsRouteExist(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetRemainDistance(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetRemainTime(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetGuidanceStatus(VRCP_ListenerBase* pcListener, int& reqID) = 0;

    // BT
    virtual void GetRecentCall(VRCP_ListenerBase* pcListener, int& reqID, int m_cCategory) = 0;
    virtual void GetHFPConnectStatus(bool& bResult) = 0;
    virtual void GetDialBackCalls(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetRedialCalls(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void ShowTelFavorites(int& reqID) = 0;
    virtual void GetFavoritesNum(VRCP_ListenerBase* pcListener, int& reqID, int contactNum) = 0;

    // search
    virtual void SearchKeyWord(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchKeyWordInfo&) = 0;
    virtual void SearchPOI(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchKeyWordInfo&) = 0;
    virtual void SearchFacility(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchFacilityInfo&) = 0;
    virtual void SearchPostCode(VRCP_ListenerBase* pcListener, int& reqID, const std::string& postCode) = 0;
    virtual void SearchAlongRoute(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchAlongRouteInfo & info)  = 0;
    virtual void AddWayPoint(VRCP_ListenerBase* pcListener, int& reqID, const VR_POIInfo & info) = 0;

    // voice
    virtual void PlayBeep(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void PlayTTS(VRCP_ListenerBase* pcListener, int& reqID, const std::string& ttsContent) = 0;
    virtual void StartSpeechRecog(VRCP_ListenerBase* pcListener, int& reqID, const std::string& szGrammerID) = 0;
    virtual void StartSpeechRecogWithCommands(VRCP_ListenerBase* pcListener, int& reqID, const std::string& szGrammerID, const VoiceVector<std::string>::type &commands, int bof, int max_time) = 0;
    virtual void StartBargeInVR(VRCP_ListenerBase* pcListener, int& reqID, const std::string& ttsContent, const std::string& szGrammerID) = 0;



    // UI
    virtual void CalculateRoute(VRCP_ListenerBase* pcListener, int& reqID,const VoiceVector<VR_POIInfo>::type& info, const int routeType) = 0;
    virtual void CallPhone(const VR_PhonenumInfo&) = 0;
    virtual void ShowPrompt(const std::string&,  bool bFullScreen = false) = 0;
    virtual void ShowHints(const VoiceVector<std::string>::type&) = 0;
    virtual void ShowView(const std::string&) = 0;
    virtual void ShowList(const VoiceVector<std::string>::type&, int iTotalPage, int iCurrPage) = 0;
    virtual void ShowPoiInfo(const std::string &listItem) = 0;
    virtual void ShowImage(const std::string& szImagePath) = 0;
    virtual void ShowPOIList(const VoiceVector<VR_POIInfo>::type&) = 0;
    virtual void ShowWeatherInfo(const VR_WeatherInfo&) = 0;
    virtual void ChangeVRState(VR_VRSTATE eState) = 0;
    virtual void ChangeVRState(const std::string &id, const std::string &session, VR_VRSTATE eState) = 0;
    virtual void NotifyServiceStop(const std::string &id, const std::string &session) = 0;
    virtual void ShowSpeechText(VRCP_ListenerBase* pListener, const std::string & info ) = 0;
    virtual void ShowMark(VRCP_ListenerBase *pListener, int &reqID, bool bOpen, const std::string &markName) = 0;
    virtual bool IsSupportChargerIcon() = 0;

    // music
    virtual void CheckSupportStatus(bool &bSupport, int iControlType) = 0;
    virtual void isCurrentlyMusicPlay(bool &bMusicPlay) = 0;
    virtual void isCurrentlyMusicPause(bool &bMusicPause) = 0;
    virtual void PlayControl(int iControlType) = 0;
    virtual void PlayMedia(const VR_MusicInfo&) = 0;
    virtual void MediaPlayingStatus(int &iPlayStatus) = 0;
    virtual void MediaDeviceStatus(int &iDevicetatus) = 0;
    virtual void GetArtistAccessor(const VR_MusicInfo& info, boost::shared_ptr<VR_MusicAccessor>& accessor) = 0;
    virtual void GetSongListByArtist(VRCP_ListenerBase* pcListener, int& reqID, VR_MusicInfo &info) = 0;

    // source
    virtual void isAllSourceOff(bool & isAllOff) = 0;
    virtual void SourceOff() = 0;
    virtual void ChangeSource(const VR_SourceInfo&) = 0;
    virtual void GetSourceList(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetSourceStatus(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetSourceValib(VRCP_ListenerBase* pcListener, int& reqID) = 0;
    virtual void GetLastAvSource(int& iSourceID, int& iSubSourceID) = 0;
    virtual bool IsSupportAm() = 0;
    virtual void isNetworkOn(bool & isNetworkOn) = 0;
    virtual void OpenBTSetting() = 0;
    virtual void OpenWiFiSetting() = 0;

    // request control
    virtual void StopRequest(int) = 0;
    virtual void CancelRequest(int) = 0;

    // navi setting
    virtual void SwitchTrafficInfo(VRCP_ListenerBase* pcListener, int& reqID, bool bOpen) = 0;
    virtual void SwitchViewMode(VRCP_ListenerBase *pcListener,int& reqID, const VR_SwitchViewInfo & switchInfo) = 0;
    virtual void ZoomMapView(VRCP_ListenerBase* pcListener, int& reqID,const int zoomMapDirection) = 0;
    virtual void AddCurrentPoint(VRCP_ListenerBase* pcListener, int& reqID,const int curPointType) = 0;
    virtual void ShowWholeRoute(VRCP_ListenerBase *pcListener, int& reqID) = 0;
    virtual void CancelRoute(VRCP_ListenerBase* pcListener, int& reqId) = 0;
    virtual void ShowCurrentPosition(VRCP_ListenerBase* pcListener, int& reqId) = 0;
    virtual void ShowPoiPosition(VRCP_ListenerBase* pcListener, int& reqId, const VR_POIInfo& poiInfo) = 0;
    virtual void QueryWeather(VRCP_ListenerBase* pcListener, int& reqID, const VR_QueryWeatherInfo& info) = 0;
    virtual void QueryTrafficRestriction(VRCP_ListenerBase* pcListener, int& reqID, const std::string& cityname, const std::string& iscurrentday) = 0;

};

class VR_PlatformNull : public VR_PlatformIF
{
public:
    virtual bool Initialize(const std::string &) { return false;}
    virtual void Destroy() {}
    virtual void Start() {}
    virtual void Stop() {}

    // service control
    virtual void StopServiceProcess() {}

    //
    virtual bool IsNetworkConnected()
    {
        return true;
    }

    virtual void GetLocation(long& lLon, long& lLat) {}
    virtual void FindContact(VRCP_ListenerBase* pcListener, int& reqID,const std::string& name, int PhoneType = PHONE_NONE) {}
    virtual void TakePicture(VRCP_ListenerBase* pcListener, int& reqID, int nWidth = 0, int nHeight = 0) {}
    virtual void QueryPOI(VRCP_ListenerBase* pcListener, int& reqID, const std::string& poiName, const std::string& cat="") {}
    virtual void SendReport(VRCP_ListenerBase* pcListener, int& reqID, const std::string& content, const std::string& szImagePath = "") {}
    virtual void RequestDialog(VRCP_ListenerBase* pcListener, int& reqID, const std::string& url) {}
    virtual void SendConversation(const VoiceVector<VRCP_ConversationInfo>::type& vConversationInfo) {}
    virtual void UploadContacts() {}
    virtual void SendSMS(const std::string& name, const std::string& phoneNum, const std::string& content) {}
    virtual void StartApp(VRCP_ListenerBase *pcListener, int& reqID, const std::string& name, const std::string& param) {}
    virtual void ChangeState(SESSION_STATE s, VRCP_ListenerBase *pListener = NULL) {}
    virtual void SetVrMod(bool bOn) {}
    virtual void ReplyCreatSessionResult(const std::string &id, const std::string &receiver, const std::string &session, int errorCode) {}
    virtual void ReplyCloseSessionResult(const std::string &id, const std::string &receiver, const std::string &session, int errorCode) {}
    virtual void ReplyRecogWithCommandsResult(const std::string &id, const std::string &session, int user_data, int error_code, const StdMap<float, std::string>::type &result) {}
    virtual void NotifySessionClosed(const std::string &id, const std::string &session, int reason) {}
    virtual void StartInstalledApp(const VR_AppInfo& appInfo) {}
    virtual void CloseInstalledApp(const VR_AppInfo& appInfo) {}
    virtual void OperateSettingOn(const VR_SettingInfo& settingInfo) {}
    virtual void OperateSettingOff(const VR_SettingInfo& settingInfo) {}
    virtual void MuteVolume(const VR_VolumeInfo& volumeInfo) {}
    virtual void MuteOffVolume(const VR_VolumeInfo& volumeInfo) {}
    virtual void IncVolume(const VR_VolumeInfo& volumeInfo) {}
    virtual void DecVolume(const VR_VolumeInfo& volumeInfo) {}
    virtual void TurnToVolume(const VR_VolumeInfo& volumeInfo) {}
    virtual void GetCurVolume(const VRSDS_VOLUME_TYPE type, int& volvalue) {}
    virtual void GetCurActiveChannelType(int& mediaActive) {}
    virtual void SetSearchKeyWordInfo(const VR_SearchKeyWordInfo &) {}
    virtual void GetSearchKeyWordInfo(VR_SearchKeyWordInfo &) {}
    virtual bool RequestMic(VR_AUDIO_TYPE ownerType) {return false;}
    virtual void SetFacilityInfo(const VR_SearchFacilityInfo & facilityInfo) {}
    virtual void GetFacilityInfo(VR_SearchFacilityInfo & facilityInfo) {}
    virtual void SetRoutePolicyStatus(const int& routePolicy) {}
    virtual int  GetRoutePolicyStatus()
    {
        return 0;
    }

    virtual void ReleaseMic(VR_AUDIO_TYPE ownerType) {}
    virtual void StopAudioStream() {}
    virtual void StartAudioStream() {}

    // listener
    virtual void SetUIListener(VRCP_UIListener* pcListener) {}

    // engine
    virtual void SwitchLanguage(VRCP_ListenerBase* pcListener, int& reqID, int iLang, const std::string &strCountry) {}
    // wakeup
    virtual void StartWakeup(VRCP_ListenerBase *pcListener, int &reqID, const std::string &cSceneID) {}

    virtual void StopWakeup() {}

    virtual int GetWakeupType()
    {
        return 0;
    }

    virtual void SetWakeupType(int iType) {}
    virtual void SetWakeupSwitch(const std::string & status) {}
    virtual void AddScene(std::string scene) {}
    virtual void StopScene(std::string scene) {}
    virtual void CommonCommand(std::string command) {}


    // point
    virtual void IsHomeRegister(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void IsCorpRegister(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetDestHistory(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetFavorite(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void RegistHomeAddress(VRCP_ListenerBase* pcListener, const VoiceVector<VR_POIInfo>::type &info, int& reqID) {}
    virtual void RegistCorpAddress(VRCP_ListenerBase* pcListener, const VoiceVector<VR_POIInfo>::type &info, int& reqID) {}
    virtual void GetActionIntentKey(std::string& intent) {}
    virtual void SetActionIntentKey(std::string intent) {}
    virtual void GetDestinationInfo(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetRegisteredPoint(VRCP_ListenerBase* pcListener, int& reqID, int iType) {}

    // guid
    virtual void IsDestExist(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void IsRouteExist(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetRemainDistance(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetRemainTime(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetGuidanceStatus(VRCP_ListenerBase* pcListener, int& reqID) {}

    // BT
    virtual void GetRecentCall(VRCP_ListenerBase* pcListener, int& reqID, int m_cCategory) {}
    virtual void GetHFPConnectStatus(bool& bResult) {}
    virtual void GetDialBackCalls(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetRedialCalls(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void ShowTelFavorites(int& reqID)  {}
    virtual void GetFavoritesNum(VRCP_ListenerBase* pcListener, int& reqID, int contactNum)  {}

    // search
    virtual void SearchKeyWord(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchKeyWordInfo&) {}
    virtual void SearchPOI(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchKeyWordInfo&) {}
    virtual void SearchFacility(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchFacilityInfo&) {}
    virtual void SearchPostCode(VRCP_ListenerBase* pcListener, int& reqID, const std::string& postCode) {}
    virtual void SearchAlongRoute(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchAlongRouteInfo & info) {}
    virtual void AddWayPoint(VRCP_ListenerBase* pcListener, int& reqID, const VR_POIInfo & info) {}

    // voice
    virtual void PlayBeep(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void PlayTTS(VRCP_ListenerBase* pcListener, int& reqID, const std::string& ttsContent) {}
    virtual void StartSpeechRecog(VRCP_ListenerBase* pcListener, int& reqID, const std::string& szGrammerID) {}
    virtual void StartSpeechRecogWithCommands(VRCP_ListenerBase* pcListener, int& reqID, const std::string& szGrammerID, const VoiceVector<std::string>::type &commands, int bof, int max_time) {}
    virtual void StartBargeInVR(VRCP_ListenerBase* pcListener, int& reqID, const std::string& ttsContent, const std::string& szGrammerID) {}

    // UI
    virtual void CalculateRoute(VRCP_ListenerBase* pcListener, int& reqID,const VoiceVector<VR_POIInfo>::type& info, const int routeType) {}
    virtual void CallPhone(const VR_PhonenumInfo&) {}
    virtual void ShowPrompt(const std::string&, bool) {}
    virtual void ShowHints(const VoiceVector<std::string>::type&) {}
    virtual void ShowView(const std::string&) {}
    virtual void ShowList(const VoiceVector<std::string>::type&, int iTotalPage, int iCurrPage) {}
    virtual void ShowPoiInfo(const std::string &listItem) {}
    virtual void ShowImage(const std::string& szImagePath) {}
    virtual void ShowPOIList(const VoiceVector<VR_POIInfo>::type&) {}
    virtual void ShowWeatherInfo(const VR_WeatherInfo&) {}
    virtual void ChangeVRState(VR_VRSTATE eState) {}
    virtual void ChangeVRState(const std::string &id, const std::string &session, VR_VRSTATE eState) {}
    virtual void NotifyServiceStop(const std::string &id, const std::string &session) {}
    virtual void ShowSpeechText(VRCP_ListenerBase* pListener, const std::string & info ){}
    virtual void ShowMark(VRCP_ListenerBase *pListener, int &reqID, bool bOpen, const std::string &markName) {}
    virtual bool IsSupportChargerIcon()
    {
        return true;
    }

    // music
    virtual void CheckSupportStatus(bool &bSupport, int iControlType) {}
    virtual void isCurrentlyMusicPlay(bool &bMusicPlay) {}
    virtual void isCurrentlyMusicPause(bool &bMusicPause) {}
    virtual void PlayControl(int iControlType) {}
    virtual void PlayMedia(const VR_MusicInfo&) {}
    virtual void MediaPlayingStatus(int &iPlayStatus) {}
    virtual void MediaDeviceStatus(int &iDevicetatus) {}
    virtual void GetArtistAccessor(const VR_MusicInfo& info, boost::shared_ptr<VR_MusicAccessor>& accessor) {}
    virtual void GetSongListByArtist(VRCP_ListenerBase* pcListener, int& reqID, VR_MusicInfo &info) {}

    // source
    virtual void isAllSourceOff(bool & isAllOff) {}
    virtual void SourceOff() {}
    virtual void ChangeSource(const VR_SourceInfo&) {}
    virtual void GetSourceList(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetSourceStatus(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetSourceValib(VRCP_ListenerBase* pcListener, int& reqID) {}
    virtual void GetLastAvSource(int& iSourceID, int& iSubSourceID) {}
    virtual bool IsSupportAm() {return true;}
    virtual void isNetworkOn(bool & isNetworkOn) {}
    virtual void OpenBTSetting() {}
    virtual void OpenWiFiSetting() {}

    // request control
    virtual void StopRequest(int) {}
    virtual void CancelRequest(int) {}

    // navi setting
    virtual void SwitchTrafficInfo(VRCP_ListenerBase* pcListener, int& reqID, bool bOpen) {}
    virtual void SwitchViewMode(VRCP_ListenerBase *pcListener, int& reqID, const VR_SwitchViewInfo & switchInfo) {}
    virtual void ZoomMapView(VRCP_ListenerBase* pcListener, int& reqID,const int zoomMapDirection) {}
    virtual void AddCurrentPoint(VRCP_ListenerBase* pcListener, int& reqID,const int curPointType) {}
    virtual void ShowWholeRoute(VRCP_ListenerBase *pcListener, int& reqID) {}
    virtual void CancelRoute(VRCP_ListenerBase* pcListener, int& reqId) {}
    virtual void ShowCurrentPosition(VRCP_ListenerBase* pcListener, int& reqId) {}
    virtual void ShowPoiPosition(VRCP_ListenerBase* pcListener, int& reqId, const VR_POIInfo& poiInfo) {}
    virtual void QueryWeather(VRCP_ListenerBase* pcListener, int& reqID, const VR_QueryWeatherInfo& info) {}
    virtual void QueryTrafficRestriction(VRCP_ListenerBase* pcListener, int& reqID, const std::string& cityname, const std::string& iscurrentday) {}
};

#endif // VR_PLATFORMIF_H
