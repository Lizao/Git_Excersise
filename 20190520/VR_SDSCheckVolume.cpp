#include "stdafx.h"
#include "VR_SDSCheckVolume.h"
#include "VR_SDSEvent.h"
#include "VR_SDSStateLocal.h"
#include "VR_SDSLocal_DEF.h"
#include "VR_SDS_DEF.h"
#include "VR_JsonTools.h"
#include "VR_SDSIntentOperateVolume.h"


VR_SDSCheckVolume::VR_SDSCheckVolume(const std::string& cCheckType, CONST BL_String& rcParam, CONST BL_String& rcInfo)
    :VR_SDSCheckBase(cCheckType, rcParam, rcInfo)
{}

bool VR_SDSCheckVolume::OnCheck(VR_SDSStateLocal* pcState, CONST VR_SDSEvent& rcEvent)
{
    if (NULL == pcState) {
        // inside error
        return true;
    }
    return VR_SDSCheckBase::OnCheckSwitch(pcState, rcEvent);
}


const std::string VR_SDSCheckVolume::ParserToCase(VR_SDSStateLocal* pcState, CONST VR_SDSEvent& rcEvent)
{
    if (VR_PLATFORM_ERR_REQUEST_TIMEOUT == rcEvent.GetParam()) {
        return VRSDS_REQUEST_TIMEOUT_DEF;
    }

    std::string cContentJson = pcState->GetEnterData(VRSDSSTATELOCAL_TEMAPKEY_SELECT_CONTENT_DEF);
    VR_VolumeInfo cVolumeInfo;

    BL_String cIntent, cProperty;
    if (!VR_JsonTools::GetIntentAndProperty(cIntent, cProperty, cContentJson.c_str())) {
        VR_ERROR("VR_SDSCheckVolume parser intent property error!\n");
    }

    if (!cVolumeInfo.PraseFromJsonStr(cProperty.GetString())) {
        VR_ERROR("ParserToCase parser VolumeInfo error!\n");
        return "";
    }

    boost::shared_ptr<VR_PlatformIF> spPlatform = pcState->GetPlatformIF();
    if (NULL == spPlatform) {
        VR_ERROR("VR_SDSCheckVolume Platform NULL error!\n");
        return "";
    }

    int curVol;
    if (cVolumeInfo.m_VolType == VRSDS_VOLUME_TYPE_NDEFINE
        || cVolumeInfo.m_VolType == VRSDS_VOLUME_MEDIA ) {
        cVolumeInfo.m_VolType = VRSDS_VOLUME_MEDIA;

        pcState->GetPlatformIF()->GetCurVolume(cVolumeInfo.m_VolType, curVol);
    }
    else {
        pcState->GetPlatformIF()->GetCurVolume(cVolumeInfo.m_VolType, curVol);
    }
    VR_LOGD("Current volume is: [%d]", curVol);

    int mediaActive;
    spPlatform->GetCurActiveChannelType(mediaActive);

    if ((cVolumeInfo.m_VolType == VRSDS_VOLUME_TYPE_NDEFINE
        || cVolumeInfo.m_VolType == VRSDS_VOLUME_MEDIA) && (mediaActive != 1)) {
        return VRSDS_SOURCE_OFF;
    }

    if (cVolumeInfo.m_ActionType == VRSDS_VOLUME_ACTION_INCREASE) {
        return ParseIncreaseVolume(cVolumeInfo.m_VolType, curVol);
    }
    else if (cVolumeInfo.m_ActionType == VRSDS_VOLUME_ACTION_DECREASE) {
        return curVol == MUTE_VOLUME ? VRSDS_VOLUME_ALREADY_MIN : VRSDS_VOLUME_DECREASE;
    }
    else if (cVolumeInfo.m_ActionType == VRSDS_VOLUME_ACTION_MUTE) {
        return curVol == MUTE_VOLUME ? VRSDS_VOLUME_ALREADY_MUTE : VRSDS_VOLUME_NOT_MUTE;
    }
    else if (cVolumeInfo.m_ActionType == VRSDS_VOLUME_ACTION_MUTE_OFF) {
        return curVol == MUTE_VOLUME ? VRSDS_VOLUME_MUTE_OFF: VRSDS_VOLUME_NOT_MUTE_OFF;
    }
    else if (cVolumeInfo.m_ActionType == VRSDS_VOLUME_ACTION_TURNTO) {
        return ParseTurnToVolume(cVolumeInfo.m_VolType, curVol, cVolumeInfo.m_VolVal);
    }

    return "";
}

const std::string VR_SDSCheckVolume::ParseIncreaseVolume(int volType, int volVal)
{
    switch (volType) {
    case VRSDS_VOLUME_MEDIA:
    case VRSDS_VOLUME_TYPE_NDEFINE:
        return volVal == MEDIA_MAX_VOLUME_VALUE ? VRSDS_VOLUME_ALREADY_MAX : VRSDS_VOLUME_INCREASE;
    case VRSDS_VOLUME_NAVI:
    case VRSDS_VOLUME_VR:
        return volVal == NON_MEDIA_MAX_VOLUME_VALUE ? VRSDS_VOLUME_ALREADY_MAX : VRSDS_VOLUME_INCREASE;
    default:
        break;
    }

    return "";
}

const std::string VR_SDSCheckVolume::ParseTurnToVolume(int volType, int volVal, int turntoVolume)
{
    std::string result;
    if (volType == VRSDS_VOLUME_TYPE_NDEFINE || volType == VRSDS_VOLUME_MEDIA) {
        result = CheckVolume(turntoVolume, volVal, MEDIA_MAX_VOLUME_VALUE, MUTE_VOLUME);
    }
    else {
        result = CheckVolume(turntoVolume, volVal, NON_MEDIA_MAX_VOLUME_VALUE, MUTE_VOLUME);
    }

    return result;
}

const std::string VR_SDSCheckVolume::CheckVolume(int turnToVolume, int curVolume, int upperBound, int lowerBound)
{
    VR_LOGD("wanted volume: [%d], current volume: [%d], upper bound: [%d], lower bound: [%d]",
            turnToVolume, curVolume, upperBound, lowerBound);

    if (turnToVolume == upperBound) {
        return curVolume == upperBound ? VRSDS_VOLUME_ALREADY_MAX : VRSDS_VOLUME_TURN_MAX;
    }
    else if (turnToVolume == lowerBound) {
        return curVolume == lowerBound ? VRSDS_VOLUME_ALREADY_MIN : VRSDS_VOLUME_TURN_MIN;
    }
    else if (turnToVolume < upperBound && turnToVolume > lowerBound) {
        return curVolume == turnToVolume ? VRSDS_VOLUME_TURN_EQUAL : VRSDS_VOLUME_TURNTO;
    }
    else if (turnToVolume > upperBound) {
        return VRSDS_VOLUME_VALUE_INVALID;
    }

    return "";
}
