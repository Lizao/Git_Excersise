#include "VR_iAutoAudioImp.h"
#include "CL_AutoSync.h"
#include "VR_Log.h"

using namespace nutshell;

void VR_iAutoAudioImp::Finalize()
{
    if (NULL != m_pAvSettings) {
        m_pAvSettings->unRegisterCallback();
        delete m_pAvSettings;
        m_pAvSettings = NULL;
    }
}

VR_iAutoAudioImp::VR_iAutoAudioImp()
    : m_vrVolume(-1)
    , m_mediaVolume(-1)
    , m_naviVolume(-1)
    , m_syncVol()
    , m_pAvSettings(NULL)
{

}

VR_iAutoAudioImp::~VR_iAutoAudioImp()
{

}

void VR_iAutoAudioImp::Initialize()
{
    VR_LOGD_FUNC();
    m_pAvSettings = GetNMAVSettingProxy();
    if(NULL != m_pAvSettings){
        m_pAvSettings->registerCallback(this);
        m_pAvSettings->sync(NMCapability_VolNavi | NMCapability_VolMedia | NMCapability_VolVR);
    }

    m_volTypeMap.insert(std::make_pair<VRSDS_VOLUME_TYPE, NMVOLUMETYPE>(
                            VRSDS_VOLUME_MEDIA, NMVOLTYPE_MEDIAVOL));
    m_volTypeMap.insert(std::make_pair<VRSDS_VOLUME_TYPE, NMVOLUMETYPE>(
                            VRSDS_VOLUME_VR, NMVOLTYPE_VR));
    m_volTypeMap.insert(std::make_pair<VRSDS_VOLUME_TYPE, NMVOLUMETYPE>(
                            VRSDS_VOLUME_NAVI, NMVOLTYPE_NAVIVOL));
}

int VR_iAutoAudioImp::GetCurVolume(VRSDS_VOLUME_TYPE volType)
{
    CL_AutoSync lock(m_syncVol);
    NMVOLUMETYPE volTypeName = m_volTypeMap[volType];

    switch(volTypeName) {
    case NMVOLTYPE_NAVIVOL:
        return m_naviVolume;
    case NMVOLTYPE_VR:
        return m_vrVolume;
    case NMVOLTYPE_MEDIAVOL:
        return m_mediaVolume;
    default:
        return -1;
    }
    return -1;
}

int VR_iAutoAudioImp::UpdateVolume(int32_t volType, int value)
{
    CL_AutoSync lock(m_syncVol);

    // it means the system is just initialized when m_recoveryVol == -1
    // this time we need to remember the initialized volume to bootstrap
    // after the first time, the recovery volume should be the value of
    // last m_currentVol(only suit for mute)
    switch(volType) {
    case NMVOLTYPE_NAVIVOL:
        m_naviVolume = value;
        break;
    case NMVOLTYPE_VR:
        m_vrVolume = value;
        break;
    case NMVOLTYPE_MEDIAVOL:
        m_mediaVolume = value;
        break;
    default:
        return -1;
    }
    VR_LOGD("Current volume: Navi: [%d], Media: [%d], VR: [%d]", m_naviVolume, m_mediaVolume, m_vrVolume);

    return 0;
}

void VR_iAutoAudioImp::onVolumeChanged(int32_t spkChannel, int32_t volType, int32_t value)
{
    VR_LOGD_FUNC();

    VR_LOGD("Speaker channel: [%ld], volume type: [%ld], value: [%ld]\n", spkChannel, volType, value);
    if (NMSPEEKERCHANNEL_FRONT == spkChannel && NMVOLTYPE_NAVIVOL == volType) {
        UpdateVolume(volType, value);
    }
    else if (NMSPEEKERCHANNEL_FRONT == spkChannel && NMVOLTYPE_MEDIAVOL == volType) {
        UpdateVolume(volType, value);
    }
    else if (NMSPEEKERCHANNEL_FRONT == spkChannel && NMVOLTYPE_VR == volType) {
        UpdateVolume(volType, value);
    }
}

int32_t VR_iAutoAudioImp::getCurrentActiveChannelType()
{
    return m_pAvSettings->getCurrentActiveChannelTypeForVR();
}
