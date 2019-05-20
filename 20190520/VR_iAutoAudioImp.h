#ifndef VR_IAUTOAUDIOIMP_H
#define VR_IAUTOAUDIOIMP_H

#include "CL_SyncObj.h"
#include "NMAVSettingProxy.h"
#include "NMAVSettingCallback.h"
#include "VR_PlatformDEF.h"
#include "VR_VolumeInfo.h"
#include <map>

#include "common/NMAVSettingCommonDef.h"


#define     VOLUME_ADJUST_STEP          1
#define     MUTE_VOLUME                 0
#define     MAX_VOLUME_ADJUST_VALUE     7
#define     MIN_VOLUME_ADJUST_VALUE     0


using namespace nutshell;
class NMAVSettingProxyImp;

class VR_iAutoAudioImp : public nutshell::NMAVSetCallback
{
public:
    VR_iAutoAudioImp();
    ~VR_iAutoAudioImp();

    void Initialize();
    void Finalize();
    int UpdateVolume(int32_t volType, int value);
    void onVolumeChanged(int32_t spkChannel, int32_t volType, int32_t value);
    int GetCurVolume(VRSDS_VOLUME_TYPE volType);
    int32_t getCurrentActiveChannelType();

protected:
    VR_iAutoAudioImp(const VR_iAutoAudioImp &);
    VR_iAutoAudioImp & operator= (const VR_iAutoAudioImp &);

private:
    int m_vrVolume;
    int m_mediaVolume;
    int m_naviVolume;
    std::map<VRSDS_VOLUME_TYPE, NMVOLUMETYPE> m_volTypeMap;

    CL_SyncObj m_syncVol;
    nutshell::NMAVSettingProxy* m_pAvSettings;
};

#endif // VR_iAutoAudioImp_H
