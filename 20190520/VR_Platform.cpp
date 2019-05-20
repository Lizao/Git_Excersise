#ifndef CXX_VR_PLATFORM_H
#   include "VR_Platform.h"
#endif

#include "BL_File.h"
#include "VC_CommonIF.h"
#include "VC_CommonDef.h"

#include "Configure/VR_ConfigIF.h"
#include "VR_PlatformConfig.h"
#include "Contact/VR_ContactAccessorBT.h"
#include "Music/VR_MusicAccessor.h"
#include "App/VR_AppNaturalAccessor.h"

#include "VR_MusicInfo.h"
#include "VR_POIInfo.h"
#include "VR_PhonenumInfo.h"
#include "VR_SearchAlongRouteInfo.h"
#include "VR_AppInfo.h"
#include "VR_VolumeInfo.h"
#include "VR_SwitchViewInfo.h"
#include "VR_ActionShowText.h"
#include "VR_ActionShowImage.h"
#include "VR_ActionCallPhone.h"
#include "VR_ActionShowView.h"
#include "VR_ActionShowList.h"
#include "VR_ActionShowPoiInfo.h"
#include "VR_ActionShowPOIList.h"
#include "VR_ActionShowWeatherInfo.h"

#include "VR_ActionStartApp.h"
#include "VR_ActionOperateApp.h"
#include "VR_ActionGetMissedCalls.h"
#include "VR_ActionGetRecentCalls.h"
#include "VR_ActionGetSourceStatus.h"
#include "VR_ActionModeNotify.h"
#include "VR_ActionChangeMICState.h"
#include "VR_ActionChangeSource.h"
#include "VR_ActionGetPlayControlStatus.h"
#include "VR_ActionPlayControl.h"
#include "VR_ActionNotifySessionClosed.h"
#include "VR_ActionReplyCreatSessionResult.h"
#include "VR_ActionReplyCloseSessionResult.h"
#include "VR_ActionReplyRecogWithCommandsResult.h"
#include "VR_ActionNotifyServiceStop.h"
#include "VR_ActionStopServiceProcess.h"
#include "VR_ActionReplyServiceStatus.h"
#include "VR_ActionGetMediaDeviceStatus.h"
#include "VR_ActionGetMediaPlayingStatus.h"
#include "VR_ActionGetLastAVSource.h"
#include "VR_ActionGetHFPConnectStatus.h"
#include "VR_ActionGetRedialCalls.h"
#include "VR_ActionGetDialBackCalls.h"
#include "VR_ActionOperateSetting.h"
#include "VR_ActionOperateVolume.h"
#include "VR_RequestAddCurrentPoint.h"
#include "VR_RequestCheckRegAddress.h"
#include "VR_RequestPlayTts.h"
#include "VR_RequestPlayTone.h"
#include "VR_RequestPlayFile.h"
#include "VR_RequestContact.h"
#include "VR_RequestGetPointList.h"
#include "VR_RequestPlayMusic.h"
#include "VR_RequestSearchKeyWord.h"
#include "VR_RequestStartVR.h"
// #include "VR_RequestStartSpeechRecogWithCommands.h"
#include "VR_RequestVoicePlay.h"
#include "VR_RequestGetPathStatus.h"
#include "VR_RequestFetchRemainInfo.h"
#include "VR_RequestSwitchLanguage.h"
#include "VR_RequestSearchNearbyFacility.h"
#include "VR_RequestSearchPOIList.h"
#include "VR_RequestSearchPostCode.h"
#include "VR_RequestGetTtsStatus.h"
#include "VR_RequestSwitchTrafficInfo.h"
#include "VR_RequestZoomMapView.h"
#include "VR_RequestSearchAlongRoute.h"
#include "VR_RequestAddWayPoint.h"
#include "VR_RequestCalculateRoute.h"
#include "VR_RequestSwitchViewMode.h"
#include "VR_RequestShowWholeRoute.h"
#include "VR_RequestCancelRoute.h"
#include "VR_RequestRegisterAddr.h"
#include "VR_RequestGetGuidanceStatus.h"
#include "VR_RequestGetDestinationInfo.h"
#include "VR_RequestShowPosition.h"
#include "VR_RequestGetRegisteredPoint.h"
#include "VR_RequestQueryWeather.h"
#include "VR_RequestQueryTrafficRestriction.h"
#include "VR_RequestShowMark.h"
#include "VR_ActionGetSongListByArtist.h"
#include "VR_ActionShowTelFavorites.h"
#include "VR_RequestStartWakeup.h"
#include "VR_RequestBargeInVR.h"
#include "VR_ActionOpenSetting.h"
#include "VR_ActionCommon.h"
#include "VrEngineNull/VRE_EngineNull.h"
#include "VREngine/VREngine_SpeechRecognition.h"
#include "VREngine/VREngine_SpeechWakeup.h"
#include "VR_iAutoAudioImp.h"
#include "VC_AudioSession.h"
#include "VREngine_AudioInFile.h"
#include "VR_SDSLocal_DEF.h"
#include "VR_ActionGetFavoritesNum.h"
#include "VR_SDSController.h"

static const NCHAR* const VR_BEEPWAV_PATH = NTEXT("/TTS/Beep/beep_vr.wav");

using namespace NVREngine;
using namespace nutshell;

VR_Platform::VR_Platform()
    : m_bTestMode(false)
    , m_CurrentPage(1)
    , m_bIsSwitchLang(false)
    , m_bIsSwitchCountry(false)
    , m_bNeedVrModeDelay(false)
    , m_pcUIListener(NULL)
    , m_pcEngine(new(MEM_Voice) VRE_EngineNull()) // LCOV_EXCL_LINE
    , m_pcEngineWakeup(new(MEM_Voice) VRE_EngineNull()) // LCOV_EXCL_LINE
    , m_pContactAccessor(new(MEM_Voice) VR_ContactAccessorBTNull(NULL)) // LCOV_EXCL_LINE
    , m_pWakeupSetting(new(MEM_Voice) VR_WakeupSetting()) // LCOV_EXCL_LINE
    , m_pListenTopApp(new(MEM_Voice) VR_ListenTopApp()) // LCOV_EXCL_LINE
    , m_pListenMultimediaStatus(new(MEM_Voice) VR_ListenMultimediaStatus()) // LCOV_EXCL_LINE
    , m_pAudioStream(new(MEM_Voice) VR_AudioStream()) // LCOV_EXCL_LINE
    , m_pRequestQueue(new(MEM_Voice) VR_RequestQueue()) // LCOV_EXCL_LINE
    , m_pStatusProvider(new(MEM_Voice) VR_StatusProvider()) // LCOV_EXCL_LINE
    , m_pIAutoAudio(new(MEM_Voice) VR_iAutoAudioImp()) // LCOV_EXCL_LINE
    , m_pSessionListener(new(MEM_Voice) VR_AudioSessionListener(this)) // LCOV_EXCL_LINE
    , m_pAudioInFile(new(MEM_Voice) VREngine_AudioInFile()) // LCOV_EXCL_LINE
    , m_vecPathRecognizing()
    , m_pNCNetworkManager(new(MEM_Voice) NCNetworkManager()) // LCOV_EXCL_LINE
{
}

VR_Platform::~VR_Platform()
{
    delete m_pSessionListener;
    m_pSessionListener = NULL;

    delete m_pAudioInFile;
    m_pAudioInFile = NULL;
}

bool VR_Platform::Initialize(const std::string &config)
{
    VR_LOGD_FUNC();

    m_pPlatformConfig.reset(new(MEM_Voice) VR_PlatformConfig()); // LCOV_EXCL_LINE
    if (! m_pPlatformConfig->Initialize(config))
    {
        VR_ERROR("platform config parse failed!");
        return false;
    }

    BL_String TestMode;
    if(m_pPlatformConfig->GetValue("testMode.enable", TestMode)
            && TestMode == "true")
    {
        VR_LOG("===VR_Platform test mode is enabled!===");
        m_bTestMode = true;
    }

    boost::shared_ptr<VRE_Engine> engine(new(MEM_Voice) VREngine_SpeechRecognition(VRE_ENGINE_IFLYTEK)); // LCOV_EXCL_LINE
    engine->SetRegion(m_pPlatformConfig->GetDataCountryLowCase().c_str());
    engine->SetLanguage(m_pPlatformConfig->GetEngineLanguage());
    if( ! engine->Initialize())
    {
        VR_ERROR("Engine initialize failed! Country=[%s], language=[%d]",
                 m_pPlatformConfig->GetDataCountryLowCase().c_str(), m_pPlatformConfig->GetLanguage());
        return false;
    }
    this->SetEngine(engine);

    m_pcEngineWakeup.reset(new(MEM_Voice) VREngine_SpeechWakeup(VRE_ENGINE_IFLYTEK)); // LCOV_EXCL_LINE
    if (!m_pcEngineWakeup->Initialize()) {
        VR_ERROR("Wakeup engine initialize failed!");
        return false;
    }
    m_pIAutoAudio->Initialize();
    VC_CommonIF::Instance()->GetAudioSession()->RegisterAudioListener(m_pSessionListener);

    return true;
}

void VR_Platform::Destroy()
{
    GetEngine()->Finalize();
    m_pIAutoAudio->Finalize();
    VC_CommonIF::Instance()->GetAudioSession()->UnregisterAudioListener(m_pSessionListener);
    delete m_pSessionListener;
    m_pSessionListener = NULL;
}

void VR_Platform::Start()
{
    VR_LOGD_FUNC();
    string startedSource = m_pListenMultimediaStatus->QueryStatusChange();
    if (!startedSource.empty()) {
        AddScene(startedSource);
    }
    string topApp = m_pListenTopApp->QueryTopApp();
    if (!topApp.empty()) {
        AddScene(topApp);
    }
    OnWakeup();
}

void VR_Platform::Stop()
{
    GetEngine()->Stop();
    StopWakeup();

    StopAudioStream();
}

void VR_Platform::StartWakeup(VRCP_ListenerBase *pcListener, int &reqID, const std::string &cSceneID)
{
    VR_LOGD_FUNC();

    VREngine_AudioIn * pHInstAudioIn = GetAudioIn();

    boost::shared_ptr<VR_RequestStartWakeup> req(new(MEM_Voice) VR_RequestStartWakeup(pcListener, // LCOV_EXCL_LINE
        m_pPlatformConfig, m_pcEngineWakeup, pHInstAudioIn, cSceneID));

    reqID = req->GetRequestID();

    m_pRequestQueue->Push(req);
}

void VR_Platform::OnWakeup()
{
    VR_LOGD_FUNC();

    if (NULL == m_pWakeupSetting) {
        VR_ERROR("m_pWakeupSetting is NULL");
        return;
    }

    if (!m_powerStatus.isPowerOff()) {
            VR_LOG("Screen has been turned on!");
            m_pWakeupSetting->OnWakeup();
        }
    else {
       VR_LOG("Screen has been turned off!");
    }
}

void VR_Platform::StopAudioStream()
{
    VR_LOGD_FUNC();

    if (NULL == m_pAudioStream) {
        VR_ERROR("m_pAudioStream is NULL");
        return;
    }

    m_pAudioStream->Stop();
}

void VR_Platform::StartAudioStream()
{
    VR_LOGD_FUNC();

    if (NULL == m_pAudioStream) {
        VR_ERROR("m_pAudioStream is NULL");
        return;
    }

    m_pAudioStream->Start();
}

void VR_Platform::StopWakeup()
{
    if (m_pcEngineWakeup) {
        m_pcEngineWakeup->Stop();
    }
//    m_pAudioInMic->Stop();
}

// LCOV_EXCL_START
void VR_Platform::StopServiceProcess()
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionStopServiceProcess>
            spcRequest(new(MEM_Voice) VR_ActionStopServiceProcess); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcRequest);
}

void VR_Platform::SwitchLanguage(VRCP_ListenerBase *pcListener, int &reqID, int iLang, const std::string &strCountry)
{
    VR_LOGD_FUNC();
    VR_LOG("iLang=[%d]", iLang);

    if(NULL != m_pcEngine_For_Switch_Langugae.get())
    {
        VR_ERROR("Langueage switch in process!");
        return;
    }
    m_pcEngine_For_Switch_Langugae = SetEngine(boost::shared_ptr<VRE_Engine>(new(MEM_Voice) VRE_EngineNull())); // LCOV_EXCL_LINE
    boost::shared_ptr<VR_RequestSwitchLanguage>
            spcRequest(new(MEM_Voice) VR_RequestSwitchLanguage(pcListener, m_pPlatformConfig, m_pcEngine_For_Switch_Langugae, iLang, strCountry)); // LCOV_EXCL_LINE
    reqID = spcRequest->GetRequestID();
    m_pRequestQueue->Push(spcRequest);
}
// LCOV_EXCL_STOP

void VR_Platform::SetWakeupSwitch(const std::string & status)
{
    VR_LOGD_FUNC();

    // if any, stop it first.
    StopWakeup();

    if (status.empty()) {
        VR_ERROR("wakeup status is empty!!");
        return;
    }

    if (NULL == m_pWakeupSetting) {
        VR_ERROR("NULL == m_pWakeupSetting!!");
        return;
    }

    m_pWakeupSetting->SetWakeupSetting(status);
}

// LCOV_EXCL_START
void VR_Platform::OnSwitchLanguageEnd(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();

    if(NULL == m_pcEngine_For_Switch_Langugae.get())
    {
        VR_ERROR("Langueage switch error!");
        return;
    }
    SetEngine(m_pcEngine_For_Switch_Langugae);
    m_pcEngine_For_Switch_Langugae.reset();
    m_bIsSwitchCountry = false;
    m_bIsSwitchLang = false;

    typedef typename VR_MusicAccessorMap::iterator musicItr;
    for(musicItr itr=m_MusicAccessorMap.begin(); itr!=m_MusicAccessorMap.end(); ++itr)
    {
        if(NULL != itr->second.get())
        {
            GetEngine()->UpdateData(itr->second);
        }
    }
    if(NULL != m_pContactAccessor.get())
    {
        GetEngine()->UpdateData(this->m_pContactAccessor);
    }

}

void VR_Platform::OnGetDataVersion(const VR_SenderInfo &sender, std::string &verStr)
{
    VR_LOGD_FUNC();

    verStr = GetEngine()->GetDataInformation();
}

void VR_Platform::OnGetServiceStatus(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();

    VR_LOG("sender=[%s]", sender.m_Sender.c_str());

    if(m_bIsSwitchCountry)
    {
        VR_LOG("service is switching country!");
        boost::shared_ptr<VR_ActionReplyServiceStatus> spcRequest(new(MEM_Voice) VR_ActionReplyServiceStatus(sender, VR_ActionReplyServiceStatus::VR_REPLY_SERVICE_STATUS_SWITCH_COUNTRY)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcRequest);
    }
    else if(m_bIsSwitchLang)
    {
        VR_LOG("service is switching language!");
        boost::shared_ptr<VR_ActionReplyServiceStatus> spcRequest(new(MEM_Voice) VR_ActionReplyServiceStatus(sender, VR_ActionReplyServiceStatus::VR_REPLY_SERVICE_STATUS_SWITCH_LANGUAGE)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcRequest);
    }
    else
    {
        boost::shared_ptr<VR_RequestGetTtsStatus> spcRequest(new(MEM_Voice) VR_RequestGetTtsStatus(sender.m_Sender)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcRequest);
    }
}
// LCOV_EXCL_STOP

void VR_Platform::OnGetAsrState(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    // get state from platform config
    if (!m_pPlatformConfig->IsUIReady()){
        m_pPlatformConfig->SetUIReady();
    }
    // push action
    boost::shared_ptr<VR_ActionChangeMICState> spcRequest(new(MEM_Voice) VR_ActionChangeMICState(m_pPlatformConfig->GetVrState())); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcRequest);
}

// LCOV_EXCL_START
void VR_Platform::OnServiceStarted()
{
    VR_LOGD_FUNC();

    // get tts service status
    boost::shared_ptr<VR_RequestGetTtsStatus> spcRequest(new(MEM_Voice) VR_RequestGetTtsStatus()); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcRequest);

    // get media status
    boost::shared_ptr<VRE_Engine> engine(this->GetEngine());
    VoiceVector<boost::shared_ptr<VR_MediaInfo> >::type mediaInfo;
    if(VR_MusicAccessor::GetProviderMediaInfo(mediaInfo) && ( NULL != engine.get()))
    {
        VoiceVector<boost::shared_ptr<VR_MediaInfo> >::const_iterator mediaInfoItr;
        for(mediaInfoItr = mediaInfo.begin(); mediaInfoItr != mediaInfo.end(); ++mediaInfoItr)
        {
            boost::shared_ptr<VR_MusicAccessor> spVR_MusicAccessor(new(MEM_Voice) VR_MusicAccessor((**mediaInfoItr), m_pcUIListener)); // LCOV_EXCL_LINE
            m_MusicAccessorMap[(*mediaInfoItr)->GetSourceType()] = spVR_MusicAccessor;
            engine->UpdateData(spVR_MusicAccessor);
        }
    }

    // get bt hfp status
    if(VR_ContactAccessorBT::GetBTConnectStatus() && (NULL != engine.get()))
    {
        m_pContactAccessor = boost::shared_ptr<VR_ContactAccessor>(new(MEM_Voice) VR_ContactAccessorBT(m_pcUIListener)); // LCOV_EXCL_LINE
        engine->UpdateData(m_pContactAccessor);
    }
}

void VR_Platform::OnClientDisconnected(const string &id)
{
    VR_LOGD_FUNC();
    VR_LOG("id=[%s]", id.c_str());
    if(NULL != m_pcUIListener)
    {
        m_pcUIListener->OnClientDisconnected(id);
    }
    else
    {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }

}

void VR_Platform::SetUIListener(VRCP_UIListener *pcListener)
{
    VR_LOG("pcListener=[%p]", pcListener);
    m_pcUIListener = pcListener;
}

void VR_Platform::OnRequestQueueNeed(const char *msg_name, void* msg)
{
    VR_LOG("");
    if (NULL != msg_name) {
        VR_LOG("receive message name [%s]",msg_name);
    }
    m_pRequestQueue->OnMessage(msg_name, msg);
}

void VR_Platform::OnReqCreateSession(const VR_SenderInfo &sender, int iLang, int max_time)
{
    VR_LOGD_FUNC();
    OnGetAsrState(sender);
    if(NULL != m_pcUIListener)
    {
        m_pcUIListener->OnCreateSession(sender.m_Id, sender.m_Sender, iLang, max_time);
    }
    else
    {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }
}

void VR_Platform::OnReqCloseSession(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_SDSIF> sds = VR_SDSController::s_SDSIF.lock();
    if (NULL != sds.get()) {
        sds->ClearFragmentedReqId();
    }

    if(NULL != m_pcUIListener)
    {
        m_pcUIListener->OnCloseSession(sender.m_Id, sender.m_Sender, sender.m_Session);
    }
    else
    {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }
}

void VR_Platform::OnReqStartVoiceRecog(const VR_SenderInfo &sender, const VR_RecogVoiceWithCommadsArgs &args)
{
    VR_LOGD_FUNC();
    if(NULL != m_pcUIListener)
    {
        m_pcUIListener->OnStartVoiceRecogWithCommads(sender.m_Id, sender.m_Sender, sender.m_Session,
                                                     args.m_iUserData, args.m_iLanguage, args.m_szPrompt, args.m_CommandsList, args.m_iBOF, args.m_iMaxTime);
    }
    else
    {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }

}

void VR_Platform::OnReqCancelVoiceRecog(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    if(NULL != m_pcUIListener)
    {
        m_pcUIListener->OnBack(sender.m_Id, sender.m_Sender, sender.m_Session);
    }
}

void VR_Platform::OnReqMic(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    if(! m_bTestMode)
    {
        if(NULL != m_pcUIListener)
        {
            m_pcUIListener->OnMic(sender.m_Id, sender.m_Sender, sender.m_Session);
        }
        else
        {
            VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
        }
    }
    else
    {
        const char* items[]=
        {
            "Call 10086",
            "Go Chiang Mai",
            "Play Album One",
            "Show Navi Help",
            "Show Prompt",
            "Search Street",
            "Get Home Address",
            "Get Favorite List",
            "Get Destination List",
            "Is Route Exist",
            "Show Whole Route",
            "Show Destination Map",
            "Show Regist Home Address",
            "Show Regist Corp Address",
            "Show Missed Calls",
            "Show Recent Calls",
            "Show Hints",
            "Get Source Status"
        };

        VoiceVector<std::string>::type listitems;
        int maxItems = (sizeof(items)/sizeof(items[0]));
        m_CurrentPage = m_CurrentPage<1 ? 1 : m_CurrentPage*6>maxItems ? (maxItems/6+1) : m_CurrentPage;
        for(int i=(m_CurrentPage - 1)*6; i< maxItems; i++)
        {
            listitems.push_back(items[i]);
            if(listitems.size() >= 6) break;
        }

        int pages = maxItems/7 + 1;
        int currentpage = m_CurrentPage;
        VR_LOG("pages=%d, currentpage=%d, ", pages, currentpage);
        boost::shared_ptr<VR_ActionShowList> spcAction(new(MEM_Voice) VR_ActionShowList(listitems, currentpage, pages)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcAction);
    }
}
// LCOV_EXCL_STOP

void VR_Platform::OnReqMic(const VR_SenderInfo &sender, const VoiceVector<std::string>::type &wavepaths)
{
    VR_LOGD_FUNC();

    if(!wavepaths.empty())
    {
        if (NULL == m_pAudioInFile) {
            VR_ERROR("VR_PlatformSA::SetListener m_pcEngine is NULL");
            return;
        }
        VR_LOG("wavepaths.size=[%d]",wavepaths.size());
        m_vecPathRecognizing = wavepaths;
    }

    this->OnReqMic(sender);

}

void
VR_Platform::OnCancelHTMLTTS(const VR_SenderInfo &sender)
{
    if(NULL != m_pcUIListener) {
        m_pcUIListener->OnCancelHTMLTTS(sender.m_Id, sender.m_Sender, sender.m_Session);
    }
    else {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }
}

void VR_Platform::OnReqStartOver(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    if(NULL != m_pcUIListener) {
        m_pcUIListener->OnStartOver(sender.m_Id, sender.m_Sender, sender.m_Session);
    }
    else {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }
}

// LCOV_EXCL_START
void VR_Platform::OnReqCancel(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    if(NULL != m_pcUIListener)
    {
        m_pcUIListener->OnBack();
        m_pcUIListener->OnBack(sender.m_Id, sender.m_Sender, sender.m_Session);
    }
    else
    {
        VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
    }
}
// LCOV_EXCL_STOP

void VR_Platform::OnSelect(const VR_SenderInfo &sender, int index)
{
    VR_LOGD_FUNC();
    VR_LOG("Index=[%d]", index);
    if( ! m_bTestMode)
    {
        if(NULL != m_pcUIListener)
        {
            m_pcUIListener->OnSelect(index);
            m_pcUIListener->OnSelect(sender.m_Id, sender.m_Sender, sender.m_Session, index);
        }
        else
        {
            VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
        }
    }
    else
    {
        index++;
        VR_MusicInfo info;
        VR_POIInfo poi;
        VR_PhonenumInfo phone;
        VR_SearchKeyWordInfo addr;
        VoiceVector<std::string>::type hints;
        int reqID;
        switch ((m_CurrentPage-1)*6 + index)
        {
        case 1:
            phone.m_cPhonenum = "10086";
            phone.m_cContact  = "name-10086";
            this->CallPhone(phone);
            break;
        case 2:
            poi.m_cLon = "184720";
            poi.m_cLat = "985900";
            poi.m_cTel = "987456321";
            poi.m_cAddr= "the largest and most culturally significant city in northern Thailand";
            poi.m_cName= "Chiang Mai";
            int req;
            this->CalculateRoute(NULL,req,VoiceVector<VR_POIInfo>::type(1,poi), 1); // normal route
            break;
        case 3:
            info.m_cAlbum = "One";
            this->PlayMedia(info);
            break;
        case 4:
            this->ShowView("NAVI_HELP");
            break;
        case 5:
            this->ShowPrompt("Show text test");
            break;
        case 6:
            //Lat Sawai
            addr.m_cCity = "Lat Sawai";
            this->SearchKeyWord(NULL, reqID, addr);
            break;
        case 7:
            this->IsHomeRegister(NULL, reqID);
            break;
        case 8:
            this->GetFavorite(NULL, reqID);
            break;
        case 9:
            this->GetDestHistory(NULL, reqID);
            break;
        case 10:
            this->IsRouteExist(NULL, reqID);
            break;
        case 11:
            // this->ShowWholeRoute();
            break;
        case 12:
            // this->ShowDestMap();
            break;
        case 13:
            // this->RegistHomeAddress();
            break;
        case 14:
            // this->RegistCorpAddress();
            break;
        case 15:
            m_pContactAccessor.reset(new(MEM_Voice) VR_ContactAccessorBT(m_pcUIListener)); // LCOV_EXCL_LINE
            this->GetRecentCall(NULL, reqID,2);
            break;
        case 16:
            m_pContactAccessor.reset(new(MEM_Voice) VR_ContactAccessorBT(m_pcUIListener)); // LCOV_EXCL_LINE
            this->GetRecentCall(NULL, reqID,1);
            break;
        case 17:
            hints.push_back("11111111");
            hints.push_back("22222222");
            hints.push_back("33333333");
            this->ShowHints(hints);
            break;
        case 18:
            this->GetSourceStatus(NULL, reqID);
            break;
        default:
            this->ShowPrompt("error!!!!!!");
        }
    }
}

// LCOV_EXCL_START
void VR_Platform::OnListPageUp(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    if( ! m_bTestMode )
    {
        if(NULL != m_pcUIListener)
        {
            m_pcUIListener->OnTurningPage(true);
            m_pcUIListener->OnTurningPage(sender.m_Id, sender.m_Sender, sender.m_Session, true);
        }
        else
        {
            VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
        }
    }
    else
    {
        m_CurrentPage--;
        this->OnReqMic(sender);
    }
}

void VR_Platform::OnListPageDown(const VR_SenderInfo &sender)
{
    VR_LOGD_FUNC();
    if( ! m_bTestMode )
    {
        if(NULL != m_pcUIListener)
        {
            m_pcUIListener->OnTurningPage(false);
            m_pcUIListener->OnTurningPage(sender.m_Id, sender.m_Sender, sender.m_Session, false);
        }
        else
        {
            VR_ERROR("m_pcUIListener=[%p]", m_pcUIListener);
        }
    }
    else
    {

        m_CurrentPage++;
        this->OnReqMic(sender);
    }
}

void VR_Platform::OnLanguageChanged(const VR_SenderInfo &sender, int iLang, const std::string &strCountry)
{
    VR_LOGD_FUNC();
    if (NULL == m_pcUIListener) {
        //Log
        return;
    }

    VR_ConfigIF conf;
    if(strCountry != conf.GetDataCountry())
    {
        m_bIsSwitchCountry = true;
        VR_LOG("m_bIsSwitchCountry=[%d], strCountry=[%s], conf.GetDataCountry()=[%s]", m_bIsSwitchCountry, strCountry.c_str(), conf.GetDataCountry().c_str());
    }
    else if(iLang != conf.GetVoiceLanguage())
    {
        m_bIsSwitchLang = true;
        VR_LOG("m_bIsSwitchCountry=[%d], iLang=[%d], conf.GetVoiceLanguage()=[%d]", m_bIsSwitchCountry, iLang, conf.GetVoiceLanguage());
    }
    else
    {
        VR_ERROR("Switch Action Undefine!");
    }

    m_pcUIListener->OnChangeLang(sender.m_Id, sender.m_Sender, sender.m_Session, iLang, strCountry);
}

void VR_Platform::OnLocationChange(long lon, long lat)
{
    //VR_LOG("lon=[%d],lat=[%d]", lon, lat);

    m_pPlatformConfig->SetLonLat(lon, lat);
}
// LCOV_EXCL_STOP

void VR_Platform::OnDistanceChange(const VR_RemainInfo &info)
{
    //VR_LOG("distance : %s", distance);
    //    m_pPlatformConfig->SetDistance(info);
    // not used.
}

// LCOV_EXCL_START
void VR_Platform::OnMediaScanUpdated(bool status, const char *dbPath, int64_t deviceID)
{
    ///< old interface for engine creat media info

//    VR_LOG("bStatus=%d, dbPath=%s, deviceID=%ul", status, dbPath, deviceID);

//    const std::string strStatus((status ? "valid" : "invalid"));
//    //for media old intent, it will be delete if new intent have been finished
//    const VR_MediaInfo info((u_int64_t)deviceID, "USB1", strStatus, "Media", dbPath);

//    boost::shared_ptr<VR_MusicAccessor> spVR_MusicAccessor(new VR_MusicAccessor(info));
//    m_MusicAccessorMap[info.GetSourceType()] = spVR_MusicAccessor;
//    GetEngine()->UpdateData(spVR_MusicAccessor);
}

void VR_Platform::OnMediaDatabaseUpdated(const VR_MediaInfo & info)
{
    boost::shared_ptr<VR_MusicAccessor> spVR_MusicAccessor(new(MEM_Voice) VR_MusicAccessor(info, m_pcUIListener)); // LCOV_EXCL_LINE
    m_MusicAccessorMap[info.GetSourceType()] = spVR_MusicAccessor;
    GetEngine()->UpdateData(spVR_MusicAccessor);
}

void VR_Platform::OnBTPhoneBookUpdated(bool status)
{
    VR_LOGD_FUNC();
    VR_LOG("bStatus=%d", status);

    boost::shared_ptr<VR_ContactAccessor> spVR_ContactAccessor
            (status ? new(MEM_Voice) VR_ContactAccessorBT(m_pcUIListener) : new(MEM_Voice) VR_ContactAccessorBTNull(m_pcUIListener)); // LCOV_EXCL_LINE
    m_pContactAccessor = spVR_ContactAccessor;
    GetEngine()->UpdateData(spVR_ContactAccessor);
}

void VR_Platform::OnDisplayOnOff(bool status)
{
    VR_LOGD_FUNC();
    if (NULL == m_pWakeupSetting) {
        VR_ERROR("m_pWakeupSetting is NULL");
        return;
    }

    if (status) {
        m_pWakeupSetting->OnWakeup();
    }
    else {
       m_pWakeupSetting->OnStopWakeup();
    }

}

void VR_Platform::OnCityChanged(const std::string &cityName, int cityId)
{
    VR_LOGD_FUNC();

    m_pPlatformConfig->SetCurrentCity(BL_String(cityName.c_str()), cityId);

    VR_LOG("cityName=[%s], cityId=[%d]", cityName.c_str(), cityId);
}
// LCOV_EXCL_STOP

int VR_Platform::GetWakeupType()
{
    VR_LOGD_FUNC();

    return m_pPlatformConfig->GetWakeupType();
}

void VR_Platform::SetWakeupType(int iType)
{
    VR_LOGD_FUNC();

    m_pPlatformConfig->SetWakeupType(iType);
}

void VR_Platform::AddScene(string scene)
{
    VR_LOGD_FUNC();
    m_pcEngineWakeup->AddScene(scene);

}

void VR_Platform::StopScene(string scene)
{
    VR_LOGD_FUNC();
    m_pcEngineWakeup->StopScene(scene);
}

void VR_Platform::CommonCommand(string command)
{
    VR_LOG("command is [%s]", command.c_str());

    boost::shared_ptr<VR_ActionCommon>
            spcAction(new(MEM_Voice) VR_ActionCommon(command)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

std::string VR_Platform::GetCurrentCity()
{
    VR_LOGD_FUNC();

    return m_pPlatformConfig->GetCityName().GetString();
}

bool VR_Platform::IsNetworkConnected()
{
    return true;
}

void VR_Platform::SetSearchKeyWordInfo(const VR_SearchKeyWordInfo &keywordInfo)
{
    VR_LOGD_FUNC();

    m_pPlatformConfig->SetSearchKeyWordInfo(keywordInfo);

}

void VR_Platform::SetFacilityInfo(const VR_SearchFacilityInfo &facilityInfo)
{
    VR_LOGD_FUNC();
    m_pPlatformConfig->SetFacilityInfo(facilityInfo);
}

void VR_Platform::GetSearchKeyWordInfo(VR_SearchKeyWordInfo &keywordInfo)
{
    VR_LOGD_FUNC();

    keywordInfo = m_pPlatformConfig->GetSearchKeyWordInfo();
}

void VR_Platform::GetFacilityInfo(VR_SearchFacilityInfo &facilityInfo)
{
    VR_LOGD_FUNC();
    facilityInfo = m_pPlatformConfig->GetFacilityInfo();
}

void VR_Platform::SetRoutePolicyStatus(const int &routePolicy)
{
    VR_LOGD_FUNC();
    m_pPlatformConfig->SetRoutePolicyStatus(routePolicy);
}

int VR_Platform::GetRoutePolicyStatus()
{
    return m_pPlatformConfig->GetRoutePolicyStatus();
}

bool VR_Platform::RequestMic(VR_AUDIO_TYPE ownerType)
{
    VR_LOGD_FUNC();
    VC_AudioSession* pAudioSession = VC_CommonIF::Instance()->GetAudioSession(VC_MODULE_VR);
    if (ownerType == VR_AUDIO_MICOWNER_WEAKUP) {
        return pAudioSession->ReqAudioInOn(VC_VR_WEAKUP);
    } else {
        return pAudioSession->ReqAudioInOn(VC_VR_SPEECHREC);
    }
}

void VR_Platform::ReleaseMic(VR_AUDIO_TYPE ownerType)
{
    VR_LOGD_FUNC();
    VC_AudioSession* pAudioSession = VC_CommonIF::Instance()->GetAudioSession(VC_MODULE_VR);
    if (ownerType == VR_AUDIO_MICOWNER_WEAKUP) {
        pAudioSession->ReqAudioInOff(VC_VR_WEAKUP);
    } else {
        pAudioSession->ReqAudioInOff(VC_VR_SPEECHREC);
    }
}

void VR_Platform::GetLocation(long &lLon, long &lLat)
{
    VR_LOGD_FUNC();

    lLon = m_pPlatformConfig->GetLon();
    lLat = m_pPlatformConfig->GetLat();

    VR_LOG("lon=[%d],lat=[%d]", lLon, lLat);
}

void VR_Platform::FindContact(VRCP_ListenerBase *pcListener, int &reqID, const std::string &name, int PhoneType)
{
    VR_LOGD_FUNC();
    VR_LOG("name=[%s], phoneType=[%d]", name.c_str(), PhoneType);

    boost::shared_ptr<VR_RequestContact>
            spcRequest(new(MEM_Voice) VR_RequestContact(pcListener, m_pContactAccessor, name, PhoneType)); // LCOV_EXCL_LINE
    reqID = spcRequest->GetRequestID();
    m_pRequestQueue->Push(spcRequest);
}

// LCOV_EXCL_START
void VR_Platform::SendSMS(const std::string &name, const std::string &phoneNum, const std::string &content)
{
}

void VR_Platform::StartApp(VRCP_ListenerBase *pcListener, int &reqID, const std::string &name, const std::string &param)
{
    VR_LOGD_FUNC();
    VR_LOG("name=[%s], param=[%s]\n", name.c_str(), param.c_str());

    std::string szname = name;
    std::string szparam = param;
    boost::shared_ptr<VR_ActionStartApp>
            spcAction(new(MEM_Voice) VR_ActionStartApp(pcListener, szname, szparam, m_pAppNaturalAccessor)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}
// LCOV_EXCL_STOP

void VR_Platform::StartInstalledApp(const VR_AppInfo& appInfo)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionOperateApp>
            spcAction(new(MEM_Voice) VR_ActionOperateApp(VR_OPERATE_APP_START, appInfo)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::CloseInstalledApp(const VR_AppInfo& appInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateApp>
                spcAction(new(MEM_Voice) VR_ActionOperateApp(VR_OPERATE_APP_CLOSE, appInfo)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::OperateSettingOn(const VR_SettingInfo& settingInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateSetting>
                spcAction(new(MEM_Voice) VR_ActionOperateSetting(settingInfo, true)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::OperateSettingOff(const VR_SettingInfo& settingInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateSetting>
                spcAction(new(MEM_Voice) VR_ActionOperateSetting(settingInfo, false)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::GetCurVolume(const VRSDS_VOLUME_TYPE type, int& volvalue)
{
    VR_LOGD_FUNC();
    volvalue = m_pIAutoAudio->GetCurVolume(type);
}

void VR_Platform::GetCurActiveChannelType(int& mediaActive)
{
    VR_LOGD_FUNC();
    mediaActive = m_pIAutoAudio->getCurrentActiveChannelType();
}

void VR_Platform::MuteVolume(const VR_VolumeInfo& volumeInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateVolume>
        spcAction(new(MEM_Voice) VR_ActionMuteVolume(volumeInfo, true)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::MuteOffVolume(const VR_VolumeInfo& volumeInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateVolume>
        spcAction(new(MEM_Voice) VR_ActionMuteVolume(volumeInfo, false)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::IncVolume(const VR_VolumeInfo& volumeInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateVolume>
        spcAction(new(MEM_Voice) VR_ActionIncVolume(volumeInfo)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::DecVolume(const VR_VolumeInfo& volumeInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionOperateVolume>
        spcAction(new(MEM_Voice) VR_ActionDecVolume(volumeInfo)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::TurnToVolume(const VR_VolumeInfo& volumeInfo)
{
    VR_LOGD_FUNC();
    int volvalue = m_pIAutoAudio->GetCurVolume(volumeInfo.m_VolType);
    boost::shared_ptr<VR_ActionOperateVolume>
        spcAction(new(MEM_Voice) VR_ActionTurnToVolume(volvalue, volumeInfo)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}


void VR_Platform::ChangeState(SESSION_STATE s, VRCP_ListenerBase *pListener)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(pListener);
    VC_AudioSession* pAudioSession = VC_CommonIF::Instance()->GetAudioSession(VC_MODULE_VR);
    boost::shared_ptr<VR_ActionModeNotify> req(new(MEM_Voice) VR_ActionModeNotify((s == SESSION_STATE_BEGIN), pAudioSession, m_bNeedVrModeDelay)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(req);

    if (SESSION_STATE_END == s) {
        VR_LOG("VR Session end , to start wake up ");
        OnWakeup();
    }
}

// LCOV_EXCL_START
void VR_Platform::SetVrMod(bool bOn)
{
    VR_LOGD_FUNC();
    VC_AudioSession* pAudioSession = VC_CommonIF::Instance()->GetAudioSession(VC_MODULE_VR);
    boost::shared_ptr<VR_ActionModeNotify> req(new(MEM_Voice) VR_ActionModeNotify(bOn, pAudioSession, false)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(req);
}
// LCOV_EXCL_STOP

void VR_Platform::ReplyCreatSessionResult(const string &id, const string &receiver, const string &session, int errorCode)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionReplyCreatSessionResult>
            spcAction(new(MEM_Voice) VR_ActionReplyCreatSessionResult(VR_SenderInfo(id, receiver, session), errorCode)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);

    // put this block after sending event to make sure the status go to UI firstly
    if(VR_PLATFORM_ERR_OK == errorCode)
    {
        m_bNeedVrModeDelay = false;
        m_pStatusProvider->UpdateStatus(VR_STATUS_TO_METER_GUIDANCE);
    }
}

void VR_Platform::ReplyCloseSessionResult(const string &id, const string &receiver,  const string &session, int errorCode)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionReplyCloseSessionResult>
            spcAction(new(MEM_Voice) VR_ActionReplyCloseSessionResult(VR_SenderInfo(id, receiver, session), errorCode)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);

    m_pStatusProvider->UpdateStatus(VR_STATUS_TO_METER_IDLE);
}

// LCOV_EXCL_START
void VR_Platform::ReplyRecogWithCommandsResult(const string &id, const std::string &session, int user_data, int error_code, const StdMap<float, string>::type &result)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionReplyRecogWithCommandsResult>
            spcAction(new(MEM_Voice) VR_ActionReplyRecogWithCommandsResult(VR_SenderInfo(id, "", session), user_data, error_code, result)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::NotifySessionClosed(const string &id, const string &session, int reason)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionNotifySessionClosed>
            spcAction(new(MEM_Voice) VR_ActionNotifySessionClosed(VR_SenderInfo(id, "", session), reason)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::IsHomeRegister(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestCheckAddress>
            req(new(MEM_Voice) VR_RequestCheckAddress(pcListener, VR_RequestCheckAddress::CHECK_HOME)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}
// LCOV_EXCL_STOP

void VR_Platform::RegistHomeAddress(VRCP_ListenerBase *pcListener, const VoiceVector<VR_POIInfo>::type& info,int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestRegisterAddr>
            req(new(MEM_Voice) VR_RequestRegisterAddr(pcListener, info, VR_RequestRegisterAddr::CHECK_HOME)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::RegistCorpAddress(VRCP_ListenerBase *pcListener, const VoiceVector<VR_POIInfo>::type &info, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestRegisterAddr>
            req(new(MEM_Voice) VR_RequestRegisterAddr(pcListener, info, VR_RequestRegisterAddr::CHECK_CORP)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}
void VR_Platform::IsCorpRegister(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestCheckAddress>
            req(new(MEM_Voice) VR_RequestCheckAddress(pcListener, VR_RequestCheckAddress::CHECK_CORP)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::SetActionIntentKey(std::string intent)
{
    m_pPlatformConfig->SetActionIntent(intent);
}

void VR_Platform::GetActionIntentKey(string &intent)
{
    intent = m_pPlatformConfig->GetActionIntent();
}

void VR_Platform::GetDestinationInfo(VRCP_ListenerBase* pcListener, int& reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetDestinationInfo>
            req(new(MEM_Voice) VR_RequestGetDestinationInfo(pcListener)); // LCOV_EXCL_LINE

    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::GetRegisteredPoint(VRCP_ListenerBase* pcListener, int& reqID, int iType)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetRegisteredPoint>
            req(new(MEM_Voice) VR_RequestGetRegisteredPoint(pcListener, iType)); // LCOV_EXCL_LINE

    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::GetDestHistory(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetDesHistory>
            req(new(MEM_Voice) VR_RequestGetDesHistory(pcListener)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);

}

// LCOV_EXCL_START
void VR_Platform::GetFavorite(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetFavoriteList> req(new(MEM_Voice) VR_RequestGetFavoriteList(pcListener)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);

}

void VR_Platform::IsDestExist(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetPathStatus> req(new(MEM_Voice) VR_RequestGetPathStatus(pcListener)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::IsRouteExist(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetPathStatus> req(new(MEM_Voice) VR_RequestGetPathStatus(pcListener)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}
// LCOV_EXCL_STOP

void VR_Platform::GetRemainDistance(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestFetchRemainInfo> req(new(MEM_Voice) // LCOV_EXCL_LINE
            VR_RequestFetchRemainInfo(pcListener, m_pPlatformConfig,
                                          VR_RequestFetchRemainInfo::VR_REMAIN_INFO_TYPE_DISTANCE));
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::GetRemainTime(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestFetchRemainInfo> req(new(MEM_Voice) // LCOV_EXCL_LINE
            VR_RequestFetchRemainInfo(pcListener, m_pPlatformConfig,
                                          VR_RequestFetchRemainInfo::VR_REMAIN_INFO_TYPE_TIME));
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::GetGuidanceStatus(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestGetGuidanceStatus> req(new(MEM_Voice) // LCOV_EXCL_LINE
            VR_RequestGetGuidanceStatus(pcListener));
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::GetDialBackCalls(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetDialBackCalls> spcAction(new(MEM_Voice) VR_ActionGetDialBackCalls(pcListener, m_pContactAccessor)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::GetRedialCalls(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetRedialCalls> spcAction(new(MEM_Voice) VR_ActionGetRedialCalls(pcListener, m_pContactAccessor)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::GetRecentCall(VRCP_ListenerBase *pcListener, int &reqID, int m_cCategory)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetRecentCalls> spcAction(new(MEM_Voice) VR_ActionGetRecentCalls(pcListener, m_pContactAccessor, m_cCategory)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::GetHFPConnectStatus(bool &bResult)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetHFPConnectStatus> spcAction(new(MEM_Voice) VR_ActionGetHFPConnectStatus(bResult)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);

}

void VR_Platform::SearchKeyWord(VRCP_ListenerBase *pcListener, int &reqID, const VR_SearchKeyWordInfo &info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSearchKeyWord> req(new(MEM_Voice) VR_RequestSearchKeyWord(pcListener, info)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::SearchPOI(VRCP_ListenerBase *pcListener, int &reqID, const VR_SearchKeyWordInfo &info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSearchPOIList> req(new(MEM_Voice) VR_RequestSearchPOIList(pcListener, info)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::SearchFacility(VRCP_ListenerBase *pcListener, int &reqID, const VR_SearchFacilityInfo & info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSearchNearbyFacility> req(new(MEM_Voice) VR_RequestSearchNearbyFacility(pcListener, info)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::SearchPostCode(VRCP_ListenerBase *pcListener, int &reqID, const string &postCode)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSearchPostCode> req(new(MEM_Voice) VR_RequestSearchPostCode(pcListener, postCode)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}
// LCOV_EXCL_STOP

void VR_Platform::SearchAlongRoute(VRCP_ListenerBase* pcListener, int& reqID, const VR_SearchAlongRouteInfo & info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSearchAlongRoute> req(new(MEM_Voice) VR_RequestSearchAlongRoute(pcListener, info)); // LCOV_EXCL_LINE

    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::AddWayPoint(VRCP_ListenerBase* pcListener, int& reqID, const VR_POIInfo & info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestAddWayPoint> req(new(MEM_Voice) VR_RequestAddWayPoint(pcListener, info)); // LCOV_EXCL_LINE

    reqID = req->GetRequestID();

    m_pRequestQueue->Push(req);
}

// LCOV_EXCL_START
void VR_Platform::PlayBeep(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

#ifdef _FOR_ANDROID_
    boost::shared_ptr<VR_RequestPlayTone> req(new(MEM_Voice) VR_RequestPlayTone(pcListener, 17)); // LCOV_EXCL_LINE
#else
    BL_String formatPath(VR_BEEPWAV_PATH);
    BL_String pyFilePath;
    BL_File dataFile(BL_FILE_PREFIX_VDATA);

    if (!dataFile.GetAbsolutePath(formatPath, pyFilePath))
    {
        VR_ERROR("Get beep path error!");
    }

    boost::shared_ptr<VR_RequestPlayFile> req(new(MEM_Voice) VR_RequestPlayFile(pyFilePath.GetString(), pcListener)); // LCOV_EXCL_LINE
#endif
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::PlayTTS(VRCP_ListenerBase *pcListener, int &reqID, const std::string &ttsContent)
{
    VR_LOGD_FUNC();
    VR_LOG("ttsContent=[%s]\n", ttsContent.c_str());

    int iLang = m_pPlatformConfig->GetLanguage();
#ifdef _FOR_ANDROID_
    VR_UNUSED_VAR(iLang);
    boost::shared_ptr<VR_RequestVoicePlay> req(new(MEM_Voice) VR_RequestVoicePlay(ttsContent, pcListener)); // LCOV_EXCL_LINE
#else
    boost::shared_ptr<VR_RequestPlayTTS> req(new(MEM_Voice) VR_RequestPlayTTS(iLang, ttsContent, pcListener)); // LCOV_EXCL_LINE
#endif

    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}
// LCOV_EXCL_STOP

void VR_Platform::StartSpeechRecog(VRCP_ListenerBase *pcListener, int &reqID, const std::string &szGrammerID)
{
    VR_LOGD_FUNC();
    VR_LOG("szGrammerID=[%s]\n", szGrammerID.c_str());

    VREngine_AudioIn * pHInstAudioIn = GetAudioIn();

    bool bNetworkConnected = false;
    isNetworkOn(bNetworkConnected);

    boost::shared_ptr<VR_RequestStartVR>
                         req(new(MEM_Voice) VR_RequestStartVR(pcListener, // LCOV_EXCL_LINE
                             m_pPlatformConfig, GetEngine(), pHInstAudioIn,
                            szGrammerID, VRE_MODE_FREEWORD, bNetworkConnected));
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

// LCOV_EXCL_START
void VR_Platform::StartSpeechRecogWithCommands(VRCP_ListenerBase *pcListener, int &reqID, const string &szGrammerID, const VoiceVector<string>::type &commands, int bof, int max_time)
{
    VR_LOGD_FUNC();
    VR_LOG("szGrammerID=[%s]\n", szGrammerID.c_str());
#if 0
    boost::shared_ptr<VR_RequestStartSpeechRecogWithCommands>
                         req(new(MEM_Voice) VR_RequestStartSpeechRecogWithCommands(pcListener, GetEngine(), szGrammerID, commands, bof, max_time)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
#endif
}
// LCOV_EXCL_STOP

void VR_Platform::StartBargeInVR(VRCP_ListenerBase *pcListener, int &reqID, const string &ttsContent, const string &szGrammerID)
{
    VR_LOGD_FUNC();
    VR_LOG("szGrammerID=[%s]\n", szGrammerID.c_str());

    int iLang = m_pPlatformConfig->GetLanguage();
    VREngine_AudioIn * pHInstAudioIn = GetAudioIn();

    boost::shared_ptr<VR_RequestBargeInVR>
                         req(new(MEM_Voice) VR_RequestBargeInVR(pcListener, iLang, ttsContent, m_pPlatformConfig, GetEngine(), pHInstAudioIn, szGrammerID, VRE_MODE_FREEWORD)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::CalculateRoute(VRCP_ListenerBase* pcListener, int& reqID,const VoiceVector<VR_POIInfo>::type& info, const int routeType)
{
    VR_LOGD_FUNC();
    VR_LOG("info.size=[%d]\n", info.size());

    boost::shared_ptr<VR_RequestCalculateRoute>
                        spcAction(new(MEM_Voice) VR_RequestCalculateRoute(pcListener, info, routeType)); // LCOV_EXCL_LINE

    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::CallPhone(const VR_PhonenumInfo & info)
{
    VR_LOGD_FUNC();
    VR_LOG("phone num=%s, phone name=%s", info.m_cPhonenum.GetString(), info.m_cContact.GetString());

    boost::shared_ptr<VR_ActionCallPhone>
            spcAction(new(MEM_Voice) VR_ActionCallPhone(info.m_cPhonenum.GetString(), info.m_cContact.GetString())); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}
// LCOV_EXCL_STOP

void VR_Platform::CancelRoute(VRCP_ListenerBase* pcListener, int& reqId)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestCancelRoute> spcAction(new(MEM_Voice) VR_RequestCancelRoute(pcListener)); // LCOV_EXCL_LINE
    reqId = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowCurrentPosition(VRCP_ListenerBase* pcListener, int& reqId)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestShowPosition> spcAction(
                new(MEM_Voice) VR_RequestShowPosition(pcListener)); // LCOV_EXCL_LINE
    reqId = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowPoiPosition(VRCP_ListenerBase* pcListener, int& reqId, const VR_POIInfo& poiInfo)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestShowPosition> spcAction(
                new(MEM_Voice) VR_RequestShowPosition(pcListener, poiInfo)); // LCOV_EXCL_LINE
    reqId = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowWholeRoute(VRCP_ListenerBase *pcListener, int& reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestShowWholeRoute> spcAction(new(MEM_Voice) VR_RequestShowWholeRoute(pcListener)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::ShowPrompt(const std::string & info, bool bFullScreen)
{
    VR_LOGD_FUNC();
    VR_LOG("text=[%s]", info.c_str());

    if(bFullScreen)
    {
        boost::shared_ptr<VR_ActionShowText>
                spcAction(new(MEM_Voice) VR_ActionShowText(info.c_str(), UITEXT_TYPE_PROMPT_FULLSCREEN)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcAction);
    }
    else
    {
        boost::shared_ptr<VR_ActionShowText>
                spcAction(new(MEM_Voice) VR_ActionShowText(info.c_str(), UITEXT_TYPE_PROMPT)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcAction);
    }
}
// LCOV_EXCL_STOP

void VR_Platform::ShowSpeechText(VRCP_ListenerBase* pListener, const std::string & info )
{
    VR_LOGD_FUNC();
    VR_LOG("text=[%s]", info.c_str());

    boost::shared_ptr<VR_ActionShowText>
            spcAction(new(MEM_Voice) VR_ActionShowText(info.c_str(), UITEXT_TYPE_SPEECH_TEXT, pListener)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::ShowHints(const VoiceVector<std::string>::type & hints)
{
    VR_LOGD_FUNC();
    VR_LOG("hints.size()=[%d]", hints.size());

    std::stringstream ss;
    for(size_t i=0; i<hints.size(); ++i)
    {
        ss<<hints.at(i)<<"\n";
    }
    std::string strJson = ss.str();
    strJson.resize(((strJson.empty()) ? 0 : (strJson.length()-1)));

    boost::shared_ptr<VR_ActionShowText>
            spcAction(new(MEM_Voice) VR_ActionShowText(strJson.c_str(), UITEXT_TYPE_HINTS)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowView(const std::string& viewName)
{
    VR_LOGD_FUNC();
    VR_LOG("viewName=[%s]\n", viewName.c_str());

    boost::shared_ptr<VR_ActionShowView> spcAction(new(MEM_Voice) VR_ActionShowView(viewName)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowList(const VoiceVector<std::string>::type &listItems, int iTotalPage, int iCurrPage)
{
    VR_LOGD_FUNC();
    VR_LOG("listItems=[%d], iTotalPage=[%d], iCurrPage=[%d]\n", listItems.size(), iTotalPage, iCurrPage);

    boost::shared_ptr<VR_ActionShowList>
            spcAction(new(MEM_Voice) VR_ActionShowList(listItems, iCurrPage, iTotalPage)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}
// LCOV_EXCL_STOP

void VR_Platform::ShowPoiInfo(const std::string &poiInfo)
{
    VR_LOGD_FUNC();
    boost::shared_ptr<VR_ActionShowPoiInfo>
            spcAction(new(MEM_Voice) VR_ActionShowPoiInfo(poiInfo)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::HideList()
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionShowList>
            spcAction(new(MEM_Voice) VR_ActionShowList()); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowImage(const std::string &szImagePath)
{
    VR_LOGD_FUNC();
    VR_LOG("szImagePath=[%s]\n", szImagePath.c_str());

    boost::shared_ptr<VR_ActionShowImage> spcAction(new(MEM_Voice) VR_ActionShowImage(szImagePath.c_str())); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowPOIList(const VoiceVector<VR_POIInfo>::type &info)
{
    VR_LOGD_FUNC();
    VR_LOG("info.size=[%d]\n", info.size());

    boost::shared_ptr<VR_ActionShowPOIList> spcAction(new(MEM_Voice) VR_ActionShowPOIList(info)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}
// LCOV_EXCL_STOP

void VR_Platform::ShowWeatherInfo(const VR_WeatherInfo& info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionShowWeatherInfo> spcAction(new(MEM_Voice) VR_ActionShowWeatherInfo(info)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ChangeVRState(VR_VRSTATE eState)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionChangeMICState> spcAction(new(MEM_Voice) VR_ActionChangeMICState(eState)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ChangeVRState(const std::string &id, const std::string &session, VR_VRSTATE eState)
{
    VR_LOGD_FUNC();
    m_pPlatformConfig->SetVrState(eState);

    if (m_pPlatformConfig->IsUIReady()){
        boost::shared_ptr<VR_ActionChangeMICState> spcAction(new(MEM_Voice) VR_ActionChangeMICState(VR_SenderInfo(id, "", session), eState)); // LCOV_EXCL_LINE
        m_pRequestQueue->Push(spcAction);
        if (!session.empty()) {
            NotifyMeterStatusChanged(eState);
        }
    }
    else{
        VR_ERROR("UI not ready for get VR state! ");
    }
}

void VR_Platform::NotifyMeterStatusChanged(VR_VRSTATE eState)
{
    VR_LOGD_FUNC();

    switch(eState)
    {
    case VRSTATE_SPEACH:
        m_pStatusProvider->UpdateStatus(VR_STATUS_TO_METER_LISTENING);
        break;
    case VRSTATE_BUSY:
        m_pStatusProvider->UpdateStatus(VR_STATUS_TO_METER_PROCESSING);
        break;
//    case VRSTATE_IDLE:
    case VRSTATE_PLAY_TTS:
        m_pStatusProvider->UpdateStatus(VR_STATUS_TO_METER_GUIDANCE);
        break;
    default:
        break;
    }
}

void VR_Platform::NotifyServiceStop(const string &id, const string &session)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionNotifyServiceStop> spcAction(new(MEM_Voice) VR_ActionNotifyServiceStop(id, session)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::CheckSupportStatus(bool &bSupport, int iControlType)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetPlayControlStatus> spcAction(new(MEM_Voice) VR_ActionGetPlayControlStatus((VR_MEDIA_CONTROL_TYPE)iControlType, bSupport)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::isCurrentlyMusicPlay(bool &bMusicPlay)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetPlayControlStatus> spcAction(new(MEM_Voice) VR_ActionGetPlayControlStatus(VR_MEDIA_CONTROL_MUSIC_CURRENTLY_PLAY, bMusicPlay)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::isCurrentlyMusicPause(bool &bMusicPause)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetPlayControlStatus> spcAction(new(MEM_Voice) VR_ActionGetPlayControlStatus(VR_MEDIA_CONTROL_MUSIC_CURRENTLY_PAUSE, bMusicPause)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::PlayControl(int iControlType)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionPlayControl> spcRequest(new(MEM_Voice) VR_ActionPlayControl((VR_MEDIA_CONTROL_TYPE)iControlType)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcRequest);
}

// LCOV_EXCL_START
void VR_Platform::PlayMedia(const VR_MusicInfo& info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_MusicAccessor> accessor;

    typedef typename VR_MusicAccessorMap::iterator musicItr;
    for(musicItr itr=m_MusicAccessorMap.begin(); itr!=m_MusicAccessorMap.end(); ++itr)
    {
        if(   (NULL != itr->second.get())
           && (itr->second->GetDeviceID() == info.m_iDeviceID))
        {
            accessor = itr->second;
            break;
        }
    }

    if(NULL == accessor.get())
    {
        VR_ERROR("info.m_iDeviceID find failed! ");
    }

    m_bNeedVrModeDelay = true;
    boost::shared_ptr<VR_RequestPlayMusic> spcRequest(new(MEM_Voice) VR_RequestPlayMusic(NULL, accessor, info)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcRequest);
}

void VR_Platform::MediaPlayingStatus(int &iPlayStatus)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetMediaPlayingStatus> spcAction(new(MEM_Voice) VR_ActionGetMediaPlayingStatus(iPlayStatus)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::MediaDeviceStatus(int &iDevicetatus)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetMediaDeviceStatus> spcAction(new(MEM_Voice) VR_ActionGetMediaDeviceStatus(iDevicetatus)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);

}
// LCOV_EXCL_STOP

void VR_Platform::isAllSourceOff(bool &isAllOff)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetPlayControlStatus> spcAction(new(MEM_Voice) VR_ActionGetPlayControlStatus(VR_MEDIA_CONTROL_SOURCE_ALL_OFF, isAllOff)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::SourceOff()
{
    VR_LOGD_FUNC();

    m_bNeedVrModeDelay = true;
    boost::shared_ptr<VR_ActionSourceOFF> spcAction(new(MEM_Voice) VR_ActionSourceOFF()); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ChangeSource(const VR_SourceInfo & info)
{
    VR_LOGD_FUNC();

    m_bNeedVrModeDelay = true;
    boost::shared_ptr<VR_ActionChangeSource> spcAction(new(MEM_Voice) VR_ActionChangeSource(info)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}
// LCOV_EXCL_STOP

void VR_Platform::GetSourceList(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetSourceStatus> spcAction(new(MEM_Voice) VR_ActionGetSourceStatus(pcListener, m_pPlatformConfig)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::GetSourceStatus(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetSourceStatus> spcAction(new(MEM_Voice) VR_ActionGetSourceStatus(pcListener, m_pPlatformConfig)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::GetSourceValib(VRCP_ListenerBase *pcListener, int &reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetSourceStatus> spcAction(new(MEM_Voice) VR_ActionGetSourceStatus(pcListener, m_pPlatformConfig)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

// LCOV_EXCL_START
void VR_Platform::GetLastAvSource(int &iSourceID, int &iSubSourceID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetLastAVSource> spcAction(new(MEM_Voice) VR_ActionGetLastAVSource(iSourceID, iSubSourceID)); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::StopRequest(int reqID)
{
    VR_LOGD_FUNC();
    VR_RequestHandle::SendMsg_Stop(reqID);
}
// LCOV_EXCL_STOP

void VR_Platform::CancelRequest(int reqID)
{
    VR_LOGD_FUNC();
    VR_RequestHandle::SendMsg_Cancel(reqID);
}

// LCOV_EXCL_START
void VR_Platform::QueryWeather(VRCP_ListenerBase *pcListener, int &reqID, const VR_QueryWeatherInfo &info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestQueryWeather> req(new(MEM_Voice) VR_RequestQueryWeather(pcListener, reqID, info)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}
// LCOV_EXCL_STOP
void VR_Platform::QueryTrafficRestriction(VRCP_ListenerBase *pcListener, int &reqID, const std::string &cityname, const std::string &iscurrentday)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestQueryTrafficRestriction> req(new(MEM_Voice) VR_RequestQueryTrafficRestriction(pcListener, reqID, cityname, iscurrentday)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

// LCOV_EXCL_START
boost::shared_ptr<VRE_Engine> VR_Platform::GetEngine()
{
    CL_AutoSync lock(this->m_MemberSyncObj);
    return m_pcEngine;
}

boost::shared_ptr<VRE_Engine> VR_Platform::SetEngine(boost::shared_ptr<VRE_Engine> engine)
{
    CL_AutoSync lock(this->m_MemberSyncObj);
    if(NULL == engine.get())
    {
        engine.reset(new(MEM_Voice) VRE_EngineNull()); // LCOV_EXCL_LINE
    }
    m_pcEngine.swap(engine);

    return engine;
}
// LCOV_EXCL_STOP

void VR_Platform::SwitchTrafficInfo(VRCP_ListenerBase* pcListener, int& reqID, bool bOpen)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSwitchTrafficInfo> req(
                new(MEM_Voice) VR_RequestSwitchTrafficInfo(pcListener, bOpen)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::SwitchViewMode(VRCP_ListenerBase* pcListener, int& reqID, const VR_SwitchViewInfo & switchInfo)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestSwitchViewMode> req(
                    new(MEM_Voice) VR_RequestSwitchViewMode(pcListener, switchInfo)); // LCOV_EXCL_LINE

    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

void VR_Platform::ZoomMapView(VRCP_ListenerBase* pcListener, int& reqID, const int zoomMapDirection)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestZoomMapView> spcAction(
                new(MEM_Voice) VR_RequestZoomMapView(pcListener, zoomMapDirection)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::AddCurrentPoint(VRCP_ListenerBase* pcListener, int& reqID,const int curPointType)
{
    boost::shared_ptr<VR_RequestAddCurrentPoint> spcAction(
                new(MEM_Voice) VR_RequestAddCurrentPoint(pcListener, curPointType)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

std::string VR_Platform::GetPathRecognizing()
{
    if (!m_vecPathRecognizing.empty()) {
        const std::string & curpath = m_vecPathRecognizing.front();

        VR_LOG("VR_Platform recoginize path= %s", curpath.c_str());

        m_vecPathRecognizing.erase(m_vecPathRecognizing.begin());

        return curpath;
    }
    else {
        VR_LOG("VR_Platform m_vecPathRecognizing.empty() = true");
    }

    return "";
}

VREngine_AudioIn * VR_Platform::GetAudioIn()
{
    const std::string & stringPathWav = GetPathRecognizing();
    VREngine_AudioIn * pHInstAudioIn = NULL;

    if (!stringPathWav.empty()) {
        m_pAudioInFile->SetFilePath(stringPathWav);
        pHInstAudioIn = m_pAudioInFile;
    }
    else {
        pHInstAudioIn = m_pAudioStream->CreatAudioReader(m_pPlatformConfig->GetOffset());
        m_pPlatformConfig->SetOffset(0);
    }

    return pHInstAudioIn;
}

//If status is false,stop audioin.If status is true ,start audio in
void VR_AudioSessionListener::OnMicChange(int vrType, bool status)
{
    VR_LOGD_FUNC();
    VR_LOG("vrType : %d, %d", vrType, VC_VR_WEAKUP);
    if(VC_VR_WEAKUP == vrType && status){
        m_platform->StartAudioStream();
        VR_LOG("Wakeup get the mic, start audio stream.");

    }
    else if (VC_VR_WEAKUP == vrType && !status) {
        VR_LOG("Wakeup loss the mic,but don't stop audio stream.");
    }
    else if (VC_VR_SPEECHREC == vrType && status) {
        m_platform->StartAudioStream();
        VR_LOG("SpeechRce get the Mic,start audio stream");
    }
    else if (VC_VR_SPEECHREC == vrType && !status) {
        m_platform->StopAudioStream();
        VR_LOG("SpeechRce loss the mic,stop audio stream.");
    }
    else{
        m_platform->StopAudioStream();
        VR_LOG("Other device get the mic,stop audio stream.");
    }
}


bool VR_Platform::IsSupportAm()
{
    return m_pPlatformConfig->IsSupportAm();
}

void VR_Platform::GetArtistAccessor(const VR_MusicInfo& info, boost::shared_ptr<VR_MusicAccessor>& accessor)
{
    VR_LOGD_FUNC();

    typedef typename VR_MusicAccessorMap::iterator musicItr;
    for(musicItr itr=m_MusicAccessorMap.begin(); itr!=m_MusicAccessorMap.end(); ++itr)
    {
        if ((NULL != itr->second.get())
           && (itr->second->GetDeviceID() == info.m_iDeviceID))
        {
            accessor = itr->second;
            break;
        }
    }

    if (NULL == accessor.get())
    {
        VR_ERROR("info.m_iDeviceID find failed! ");
    }
}


void VR_Platform::GetSongListByArtist(VRCP_ListenerBase* pcListener, int& reqID, VR_MusicInfo& info)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_MusicAccessor> accessor;
    typedef typename VR_MusicAccessorMap::iterator musicItr;
    for(musicItr itr=m_MusicAccessorMap.begin(); itr!=m_MusicAccessorMap.end(); ++itr)
    {
        if ((NULL != itr->second.get())
           && (itr->second->GetDeviceID() == info.m_iDeviceID))
        {
            accessor = itr->second;
            break;
        }
    }

    if (NULL == accessor.get())
    {
        VR_ERROR("info.m_iDeviceID find failed! ");
    }

    boost::shared_ptr<VR_ActionGetSongListByArtist> spcAction(new(MEM_Voice) VR_ActionGetSongListByArtist(pcListener, accessor, info.m_iArtistID, info.m_iDeviceID)); // LCOV_EXCL_LINE
    reqID = spcAction->GetRequestID();
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowTelFavorites(int& reqID)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionShowTelFavorites> spcAction(new(MEM_Voice) VR_ActionShowTelFavorites(reqID)); // LCOV_EXCL_LINE
        reqID = spcAction->GetRequestID();
        m_pRequestQueue->Push(spcAction);
}

void VR_Platform::GetFavoritesNum(VRCP_ListenerBase* pcListener, int& reqID, int contactNum)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionGetFavoritesNum> spcAction(new(MEM_Voice) VR_ActionGetFavoritesNum(pcListener, m_pContactAccessor, contactNum)); // LCOV_EXCL_LINE
        reqID = spcAction->GetRequestID();
        m_pRequestQueue->Push(spcAction);
}

void VR_Platform::isNetworkOn(bool & isNetworkOn)
{
    VR_LOGD_FUNC();
    isNetworkOn = m_pNCNetworkManager->getNetworkStatus();
}

void VR_Platform::OpenBTSetting()
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionOpenBTSetting> spcAction(new(MEM_Voice) VR_ActionOpenBTSetting()); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::OpenWiFiSetting()
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_ActionOpenWiFiSetting> spcAction(new(MEM_Voice) VR_ActionOpenWiFiSetting()); // LCOV_EXCL_LINE
    m_pRequestQueue->Push(spcAction);
}

void VR_Platform::ShowMark(VRCP_ListenerBase *pListener, int &reqID, bool bOpen, const std::string &markName)
{
    VR_LOGD_FUNC();

    boost::shared_ptr<VR_RequestShowMark> req(new(MEM_Voice) VR_RequestShowMark(pListener, bOpen, markName)); // LCOV_EXCL_LINE
    reqID = req->GetRequestID();
    m_pRequestQueue->Push(req);
}

bool VR_Platform::IsSupportChargerIcon()
{
    VR_LOGD_FUNC();

    return m_pPlatformConfig->IsSupportChargerIcon();
}
