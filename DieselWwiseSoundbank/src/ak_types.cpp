#include "ak_types.h"

namespace Wwise {
    // Type Conversion
    HIRCItemTypeNew ConvertType(HIRCItemTypeOld t) {
        switch (t)
        {
        case HIRCItemTypeOld::State:                    return HIRCItemTypeNew::State;
        case HIRCItemTypeOld::Sound:                    return HIRCItemTypeNew::Sound;
        case HIRCItemTypeOld::Action:                   return HIRCItemTypeNew::Action;
        case HIRCItemTypeOld::Event:                    return HIRCItemTypeNew::Event;
        case HIRCItemTypeOld::RandomOrSequenceContainer:return HIRCItemTypeNew::RandomOrSequenceContainer;
        case HIRCItemTypeOld::SwitchContainer:          return HIRCItemTypeNew::SwitchContainer;
        case HIRCItemTypeOld::ActorMixer:               return HIRCItemTypeNew::ActorMixer;
        case HIRCItemTypeOld::Bus:                      return HIRCItemTypeNew::AudioBus;
        case HIRCItemTypeOld::LayerContainer:           return HIRCItemTypeNew::LayerContainer;
        case HIRCItemTypeOld::MusicSegment:             return HIRCItemTypeNew::MusicSegment;
        case HIRCItemTypeOld::MusicTrack:               return HIRCItemTypeNew::MusicTrack;
        case HIRCItemTypeOld::MusicSwitch:              return HIRCItemTypeNew::MusicSwitch;
        case HIRCItemTypeOld::MusicRandomOrSequence:    return HIRCItemTypeNew::MusicRandomOrSequence;
        case HIRCItemTypeOld::Attenuation:              return HIRCItemTypeNew::Attenuation;
        case HIRCItemTypeOld::DialogueEvent:            return HIRCItemTypeNew::DialogueEvent;
        case HIRCItemTypeOld::FxShareSet:               return HIRCItemTypeNew::FxShareSet;
        case HIRCItemTypeOld::FxCustom:                 return HIRCItemTypeNew::FxCustom;
        case HIRCItemTypeOld::AuxiliaryBus:             return HIRCItemTypeNew::AuxiliaryBus;
        case HIRCItemTypeOld::LFO:                      return HIRCItemTypeNew::LFO;
        case HIRCItemTypeOld::Envelope:                 return HIRCItemTypeNew::Envelope;
        case HIRCItemTypeOld::AudioDevice:              return HIRCItemTypeNew::AudioDevice;
        }
    }

    RTPCType2022 ConvertType(RTPCType2015 t) {
        switch (t) {
        case RTPCType2015::GameParameter:
            return RTPCType2022::GameParameter;
        case RTPCType2015::MIDIParameter:
            return RTPCType2022::Modulator;
        case RTPCType2015::Modulator:
            return RTPCType2022::Modulator;
        }
    }

    RTPCAccum2022 ConvertType(RTPCAccum2015 t) {
        switch (t) {
        case RTPCAccum2015::Additive:
            return RTPCAccum2022::Additive;
        case RTPCAccum2015::Multiply:
            return RTPCAccum2022::Multiply;
        case RTPCAccum2015::Exclusive:
            return RTPCAccum2022::Exclusive;
        }
    }

    SourceTypeNew ConvertType(SourceTypeOld t) {
        switch (t) {
        case SourceTypeOld::Bank:
            return SourceTypeNew::Bank;
        case SourceTypeOld::Streaming:
            return SourceTypeNew::Streaming;
        case SourceTypeOld::PrefetchStreaming:
            return SourceTypeNew::PrefetchStreaming;
        }
    }

    AkPropID_2015 ConvertType(AkPropID_2013 t) {
        switch (t)
        {
        case AkPropID_2013::Volume:                  return AkPropID_2015::Volume;
        case AkPropID_2013::LFE:                     return AkPropID_2015::LFE;
        case AkPropID_2013::Pitch:                   return AkPropID_2015::Pitch;
        case AkPropID_2013::LPF:                     return AkPropID_2015::LPF;
        case AkPropID_2013::BusVolume:               return AkPropID_2015::BusVolume;
        case AkPropID_2013::Priority:                return AkPropID_2015::Priority;
        case AkPropID_2013::PriorityDistanceOffset:  return AkPropID_2015::PriorityDistanceOffset;
        case AkPropID_2013::Loop:                    return AkPropID_2015::Loop;
        case AkPropID_2013::FeedbackVolume:          return AkPropID_2015::FeedbackVolume;
        case AkPropID_2013::FeedbackLPF:             return AkPropID_2015::FeedbackLPF;
        case AkPropID_2013::MuteRatio:               return AkPropID_2015::MuteRatio;
        case AkPropID_2013::PAN_LR:                  return AkPropID_2015::PAN_LR;
        case AkPropID_2013::PAN_FR:                  return AkPropID_2015::PAN_FR;
        case AkPropID_2013::CenterPCT:               return AkPropID_2015::CenterPCT;
        case AkPropID_2013::DelayTime:               return AkPropID_2015::DelayTime;
        case AkPropID_2013::TransitionTime:          return AkPropID_2015::TransitionTime;
        case AkPropID_2013::Probability:             return AkPropID_2015::Probability;
        case AkPropID_2013::DialogueMode:            return AkPropID_2015::DialogueMode;
        case AkPropID_2013::UserAuxSendVolume0:      return AkPropID_2015::UserAuxSendVolume0;
        case AkPropID_2013::UserAuxSendVolume1:      return AkPropID_2015::UserAuxSendVolume1;
        case AkPropID_2013::UserAuxSendVolume2:      return AkPropID_2015::UserAuxSendVolume2;
        case AkPropID_2013::UserAuxSendVolume3:      return AkPropID_2015::UserAuxSendVolume3;
        case AkPropID_2013::GameAuxSendVolume:       return AkPropID_2015::GameAuxSendVolume;
        case AkPropID_2013::OutputBusVolume:         return AkPropID_2015::OutputBusVolume;
        case AkPropID_2013::OutputBusLPF:            return AkPropID_2015::OutputBusLPF;
        case AkPropID_2013::InitialDelay:            return AkPropID_2015::InitialDelay;
        case AkPropID_2013::HDRBusThreshold:         return AkPropID_2015::HDRBusThreshold;
        case AkPropID_2013::HDRBusRatio:             return AkPropID_2015::HDRBusRatio;
        case AkPropID_2013::HDRBusReleaseTime:       return AkPropID_2015::HDRBusReleaseTime;
        case AkPropID_2013::HDRBusGameParam:         return AkPropID_2015::HDRBusGameParam;
        case AkPropID_2013::HDRBusGameParamMin:      return AkPropID_2015::HDRBusGameParamMin;
        case AkPropID_2013::HDRBusGameParamMax:      return AkPropID_2015::HDRBusGameParamMax;
        case AkPropID_2013::HDRActiveRange:          return AkPropID_2015::HDRActiveRange;
        case AkPropID_2013::MakeUpGain:              return AkPropID_2015::MakeUpGain;
        case AkPropID_2013::LoopStart:               return AkPropID_2015::LoopStart;
        case AkPropID_2013::LoopEnd:                 return AkPropID_2015::LoopEnd;
        case AkPropID_2013::TrimInTime:              return AkPropID_2015::TrimInTime;
        case AkPropID_2013::TrimOutTime:             return AkPropID_2015::TrimOutTime;
        case AkPropID_2013::FadeInTime:              return AkPropID_2015::FadeInTime;
        case AkPropID_2013::FadeOutTime:             return AkPropID_2015::FadeOutTime;
        case AkPropID_2013::FadeInCurve:             return AkPropID_2015::FadeInCurve;
        case AkPropID_2013::FadeOutCurve:            return AkPropID_2015::FadeOutCurve;
        case AkPropID_2013::LoopCrossfadeDuration:   return AkPropID_2015::LoopCrossfadeDuration;
        case AkPropID_2013::CrossfadeUpCurve:        return AkPropID_2015::CrossfadeUpCurve;
        case AkPropID_2013::CrossfadeDownCurve:      return AkPropID_2015::CrossfadeDownCurve;
        }
    }

    AkPropID_2022 ConvertType_2013_to_2022(AkPropID_2013 t) {
        switch (t)
        {
        case AkPropID_2013::Volume:                  return AkPropID_2022::Volume;
        case AkPropID_2013::LFE:                     return AkPropID_2022::LFE;
        case AkPropID_2013::Pitch:                   return AkPropID_2022::Pitch;
        case AkPropID_2013::LPF:                     return AkPropID_2022::LPF;
        case AkPropID_2013::BusVolume:               return AkPropID_2022::BusVolume;
        case AkPropID_2013::Priority:                return AkPropID_2022::Priority;
        case AkPropID_2013::PriorityDistanceOffset:  return AkPropID_2022::PriorityDistanceOffset;
        case AkPropID_2013::Loop:                    return AkPropID_2022::Loop;
        case AkPropID_2013::FeedbackVolume:          return AkPropID_2022::_FeedbackVolume;
        case AkPropID_2013::FeedbackLPF:             return AkPropID_2022::_FeedbackLPF;
        case AkPropID_2013::MuteRatio:               return AkPropID_2022::MuteRatio;
        case AkPropID_2013::PAN_LR:                  return AkPropID_2022::PAN_LR;
        case AkPropID_2013::PAN_FR:                  return AkPropID_2022::PAN_FR;
        case AkPropID_2013::CenterPCT:               return AkPropID_2022::CenterPCT;
        case AkPropID_2013::DelayTime:               return AkPropID_2022::DelayTime;
        case AkPropID_2013::TransitionTime:          return AkPropID_2022::TransitionTime;
        case AkPropID_2013::Probability:             return AkPropID_2022::Probability;
        case AkPropID_2013::DialogueMode:            return AkPropID_2022::DialogueMode;
        case AkPropID_2013::UserAuxSendVolume0:      return AkPropID_2022::UserAuxSendVolume0;
        case AkPropID_2013::UserAuxSendVolume1:      return AkPropID_2022::UserAuxSendVolume1;
        case AkPropID_2013::UserAuxSendVolume2:      return AkPropID_2022::UserAuxSendVolume2;
        case AkPropID_2013::UserAuxSendVolume3:      return AkPropID_2022::UserAuxSendVolume3;
        case AkPropID_2013::GameAuxSendVolume:       return AkPropID_2022::GameAuxSendVolume;
        case AkPropID_2013::OutputBusVolume:         return AkPropID_2022::OutputBusVolume;
        case AkPropID_2013::OutputBusLPF:            return AkPropID_2022::OutputBusLPF;
        case AkPropID_2013::InitialDelay:            return AkPropID_2022::InitialDelay;
        case AkPropID_2013::HDRBusThreshold:         return AkPropID_2022::HDRBusThreshold;
        case AkPropID_2013::HDRBusRatio:             return AkPropID_2022::HDRBusRatio;
        case AkPropID_2013::HDRBusReleaseTime:       return AkPropID_2022::HDRBusReleaseTime;
        case AkPropID_2013::HDRBusGameParam:         return AkPropID_2022::HDRBusGameParam;
        case AkPropID_2013::HDRBusGameParamMin:      return AkPropID_2022::HDRBusGameParamMin;
        case AkPropID_2013::HDRBusGameParamMax:      return AkPropID_2022::HDRBusGameParamMax;
        case AkPropID_2013::HDRActiveRange:          return AkPropID_2022::HDRActiveRange;
        case AkPropID_2013::MakeUpGain:              return AkPropID_2022::MakeUpGain;
        case AkPropID_2013::LoopStart:               return AkPropID_2022::LoopStart;
        case AkPropID_2013::LoopEnd:                 return AkPropID_2022::LoopEnd;
        case AkPropID_2013::TrimInTime:              return AkPropID_2022::TrimInTime;
        case AkPropID_2013::TrimOutTime:             return AkPropID_2022::TrimOutTime;
        case AkPropID_2013::FadeInTime:              return AkPropID_2022::FadeInTime;
        case AkPropID_2013::FadeOutTime:             return AkPropID_2022::FadeOutTime;
        case AkPropID_2013::FadeInCurve:             return AkPropID_2022::FadeInCurve;
        case AkPropID_2013::FadeOutCurve:            return AkPropID_2022::FadeOutCurve;
        case AkPropID_2013::LoopCrossfadeDuration:   return AkPropID_2022::LoopCrossfadeDuration;
        case AkPropID_2013::CrossfadeUpCurve:        return AkPropID_2022::CrossfadeUpCurve;
        case AkPropID_2013::CrossfadeDownCurve:      return AkPropID_2022::CrossfadeDownCurve;
        }
    }

    AkPropID_2022 ConvertType(AkPropID_2015 t) {
        switch (t)
        {
        case AkPropID_2015::Volume:                  return AkPropID_2022::Volume;
        case AkPropID_2015::LFE:                     return AkPropID_2022::LFE;
        case AkPropID_2015::Pitch:                   return AkPropID_2022::Pitch;
        case AkPropID_2015::LPF:                     return AkPropID_2022::LPF;
        case AkPropID_2015::HPF:                     return AkPropID_2022::HPF;
        case AkPropID_2015::BusVolume:               return AkPropID_2022::BusVolume;
        case AkPropID_2015::Priority:                return AkPropID_2022::Priority;
        case AkPropID_2015::PriorityDistanceOffset:  return AkPropID_2022::PriorityDistanceOffset;
        case AkPropID_2015::FeedbackVolume:          return AkPropID_2022::_FeedbackVolume;
        case AkPropID_2015::FeedbackLPF:             return AkPropID_2022::_FeedbackLPF;
        case AkPropID_2015::MuteRatio:               return AkPropID_2022::MuteRatio;
        case AkPropID_2015::PAN_LR:                  return AkPropID_2022::PAN_LR;
        case AkPropID_2015::PAN_FR:                  return AkPropID_2022::PAN_FR;
        case AkPropID_2015::CenterPCT:               return AkPropID_2022::CenterPCT;
        case AkPropID_2015::DelayTime:               return AkPropID_2022::DelayTime;
        case AkPropID_2015::TransitionTime:          return AkPropID_2022::TransitionTime;
        case AkPropID_2015::Probability:             return AkPropID_2022::Probability;
        case AkPropID_2015::DialogueMode:            return AkPropID_2022::DialogueMode;
        case AkPropID_2015::UserAuxSendVolume0:      return AkPropID_2022::UserAuxSendVolume0;
        case AkPropID_2015::UserAuxSendVolume1:      return AkPropID_2022::UserAuxSendVolume1;
        case AkPropID_2015::UserAuxSendVolume2:      return AkPropID_2022::UserAuxSendVolume2;
        case AkPropID_2015::UserAuxSendVolume3:      return AkPropID_2022::UserAuxSendVolume3;
        case AkPropID_2015::GameAuxSendVolume:       return AkPropID_2022::GameAuxSendVolume;
        case AkPropID_2015::OutputBusVolume:         return AkPropID_2022::OutputBusVolume;
        case AkPropID_2015::OutputBusHPF:            return AkPropID_2022::OutputBusHPF;
        case AkPropID_2015::OutputBusLPF:            return AkPropID_2022::OutputBusLPF;
        case AkPropID_2015::HDRBusThreshold:         return AkPropID_2022::HDRBusThreshold;
        case AkPropID_2015::HDRBusRatio:             return AkPropID_2022::HDRBusRatio;
        case AkPropID_2015::HDRBusReleaseTime:       return AkPropID_2022::HDRBusReleaseTime;
        case AkPropID_2015::HDRBusGameParam:         return AkPropID_2022::HDRBusGameParam;
        case AkPropID_2015::HDRBusGameParamMin:      return AkPropID_2022::HDRBusGameParamMin;
        case AkPropID_2015::HDRBusGameParamMax:      return AkPropID_2022::HDRBusGameParamMax;
        case AkPropID_2015::HDRActiveRange:          return AkPropID_2022::HDRActiveRange;
        case AkPropID_2015::MakeUpGain:              return AkPropID_2022::MakeUpGain;
        case AkPropID_2015::LoopStart:               return AkPropID_2022::LoopStart;
        case AkPropID_2015::LoopEnd:                 return AkPropID_2022::LoopEnd;
        case AkPropID_2015::TrimInTime:              return AkPropID_2022::TrimInTime;
        case AkPropID_2015::TrimOutTime:             return AkPropID_2022::TrimOutTime;
        case AkPropID_2015::FadeInTime:              return AkPropID_2022::FadeInTime;
        case AkPropID_2015::FadeOutTime:             return AkPropID_2022::FadeOutTime;
        case AkPropID_2015::FadeInCurve:             return AkPropID_2022::FadeInCurve;
        case AkPropID_2015::FadeOutCurve:            return AkPropID_2022::FadeOutCurve;
        case AkPropID_2015::LoopCrossfadeDuration:   return AkPropID_2022::LoopCrossfadeDuration;
        case AkPropID_2015::CrossfadeUpCurve:        return AkPropID_2022::CrossfadeUpCurve;
        case AkPropID_2015::CrossfadeDownCurve:      return AkPropID_2022::CrossfadeDownCurve;
        case AkPropID_2015::MidiTrackingRootNote:    return AkPropID_2022::MidiTrackingRootNote;
        case AkPropID_2015::MidiPlayOnNoteType:      return AkPropID_2022::MidiPlayOnNoteType;
        case AkPropID_2015::MidiTransposition:       return AkPropID_2022::MidiTransposition;
        case AkPropID_2015::MidiVelocityOffset:      return AkPropID_2022::MidiVelocityOffset;
        case AkPropID_2015::MidiKeyRangeMin:         return AkPropID_2022::MidiKeyRangeMin;
        case AkPropID_2015::MidiKeyRangeMax:         return AkPropID_2022::MidiKeyRangeMax;
        case AkPropID_2015::MidiVelocityRangeMin:    return AkPropID_2022::MidiVelocityRangeMin;
        case AkPropID_2015::MidiVelocityRangeMax:    return AkPropID_2022::MidiVelocityRangeMax;
        case AkPropID_2015::MidiChannelMask:         return AkPropID_2022::MidiChannelMask;
        case AkPropID_2015::PlaybackSpeed:           return AkPropID_2022::PlaybackSpeed;
        case AkPropID_2015::MidiTempoSource:         return AkPropID_2022::MidiTempoSource;
        case AkPropID_2015::MidiTargetNode:          return AkPropID_2022::MidiTargetNode;
        case AkPropID_2015::AttachedPluginFXID:      return AkPropID_2022::AttachedPluginFXID;
        case AkPropID_2015::Loop:                    return AkPropID_2022::Loop;
        case AkPropID_2015::InitialDelay:            return AkPropID_2022::InitialDelay;
        }
    }

    RTPCParameterID_2015 ConvertType(RTPCParameterID_2013 t)
    {
        switch (t)
        {
        case RTPCParameterID_2013::Volume:                              return RTPCParameterID_2015::Volume;
        case RTPCParameterID_2013::LFE:                                 return RTPCParameterID_2015::LFE;
        case RTPCParameterID_2013::Pitch:                               return RTPCParameterID_2015::Pitch;
        case RTPCParameterID_2013::LPF:                                 return RTPCParameterID_2015::LPF;
        case RTPCParameterID_2013::BusVolume:                           return RTPCParameterID_2015::BusVolume;

        case RTPCParameterID_2013::InitialDelay:                        return RTPCParameterID_2015::InitialDelay;
        case RTPCParameterID_2013::PlayMechanismSpecialTransitionsValue:return RTPCParameterID_2015::PlayMechanismSpecialTransitionsValue;

        case RTPCParameterID_2013::Priority:                            return RTPCParameterID_2015::Priority;
        case RTPCParameterID_2013::MaxNumInstances:                    return RTPCParameterID_2015::MaxNumInstances;
        case RTPCParameterID_2013::PositioningType:                    return RTPCParameterID_2015::PositioningType;
        case RTPCParameterID_2013::Positioning_Divergence_Center_PCT:  return RTPCParameterID_2015::Positioning_Divergence_Center_PCT;
        case RTPCParameterID_2013::Positioning_Cone_Attenuation_ON_OFF:return RTPCParameterID_2015::Positioning_Cone_Attenuation_ON_OFF;
        case RTPCParameterID_2013::Positioning_Cone_Attenuation:       return RTPCParameterID_2015::Positioning_Cone_Attenuation;
        case RTPCParameterID_2013::Positioning_Cone_LPF:               return RTPCParameterID_2015::Positioning_Cone_LPF;

        case RTPCParameterID_2013::UserAuxSendVolume0:                 return RTPCParameterID_2015::UserAuxSendVolume0;
        case RTPCParameterID_2013::UserAuxSendVolume1:                 return RTPCParameterID_2015::UserAuxSendVolume1;
        case RTPCParameterID_2013::UserAuxSendVolume2:                 return RTPCParameterID_2015::UserAuxSendVolume2;
        case RTPCParameterID_2013::UserAuxSendVolume3:                 return RTPCParameterID_2015::UserAuxSendVolume3;
        case RTPCParameterID_2013::GameAuxSendVolume:                  return RTPCParameterID_2015::GameAuxSendVolume;

        case RTPCParameterID_2013::Position_PAN_X_2D:                  return RTPCParameterID_2015::Position_PAN_X_2D;
        case RTPCParameterID_2013::Position_PAN_Y_2D:                  return RTPCParameterID_2015::Position_PAN_Y_2D;

        case RTPCParameterID_2013::OutputBusVolume:                    return RTPCParameterID_2015::OutputBusVolume;
        case RTPCParameterID_2013::OutputBusLPF:                       return RTPCParameterID_2015::OutputBusLPF;

        case RTPCParameterID_2013::BypassFX0:                          return RTPCParameterID_2015::BypassFX0;
        case RTPCParameterID_2013::BypassFX1:                          return RTPCParameterID_2015::BypassFX1;
        case RTPCParameterID_2013::BypassFX2:                          return RTPCParameterID_2015::BypassFX2;
        case RTPCParameterID_2013::BypassFX3:                          return RTPCParameterID_2015::BypassFX3;
        case RTPCParameterID_2013::BypassAllFX:                        return RTPCParameterID_2015::BypassAllFX;

        case RTPCParameterID_2013::FeedbackVolume:                     return RTPCParameterID_2015::FeedbackVolume;
        case RTPCParameterID_2013::FeedbackLowpass:                    return RTPCParameterID_2015::FeedbackLowpass;
        case RTPCParameterID_2013::FeedbackPitch:                      return RTPCParameterID_2015::FeedbackPitch;

        case RTPCParameterID_2013::HDRBusThreshold:                    return RTPCParameterID_2015::HDRBusThreshold;
        case RTPCParameterID_2013::HDRBusReleaseTime:                  return RTPCParameterID_2015::HDRBusReleaseTime;
        case RTPCParameterID_2013::HDRBusRatio:                        return RTPCParameterID_2015::HDRBusRatio;
        case RTPCParameterID_2013::HDRActiveRange:                     return RTPCParameterID_2015::HDRActiveRange;
        case RTPCParameterID_2013::MakeUpGain:                         return RTPCParameterID_2015::MakeUpGain;

        case RTPCParameterID_2013::Position_PAN_X_3D:                  return RTPCParameterID_2015::Position_PAN_X_3D;
        case RTPCParameterID_2013::Position_PAN_Y_3D:                  return RTPCParameterID_2015::Position_PAN_Y_3D;
        }
    }

    RTPCParameterID_2022 ConvertType_2013_to_2022(RTPCParameterID_2013 t)
    {
        switch (t)
        {
        case RTPCParameterID_2013::Volume:                              return RTPCParameterID_2022::Volume;
        case RTPCParameterID_2013::LFE:                                 return RTPCParameterID_2022::LFE;
        case RTPCParameterID_2013::Pitch:                               return RTPCParameterID_2022::Pitch;
        case RTPCParameterID_2013::LPF:                                 return RTPCParameterID_2022::LPF;
        case RTPCParameterID_2013::BusVolume:                           return RTPCParameterID_2022::BusVolume;
        case RTPCParameterID_2013::InitialDelay:                        return RTPCParameterID_2022::InitialDelay;

        case RTPCParameterID_2013::PlayMechanismSpecialTransitionsValue:return RTPCParameterID_2022::PlayMechanismSpecialTransitionsValue;
        case RTPCParameterID_2013::MaxNumInstances:                    return RTPCParameterID_2022::MaxNumInstances;

        case RTPCParameterID_2013::Priority:                            return RTPCParameterID_2022::Priority;

        case RTPCParameterID_2013::PositioningType:                    return RTPCParameterID_2022::PositioningTypeBlend;
        case RTPCParameterID_2013::Positioning_Divergence_Center_PCT:  return RTPCParameterID_2022::Positioning_Divergence_Center_PCT;
        case RTPCParameterID_2013::Positioning_Cone_Attenuation_ON_OFF:return RTPCParameterID_2022::Positioning_Cone_Attenuation_ON_OFF;
        case RTPCParameterID_2013::Positioning_Cone_Attenuation:       return RTPCParameterID_2022::Positioning_Cone_Attenuation;
        case RTPCParameterID_2013::Positioning_Cone_LPF:               return RTPCParameterID_2022::Positioning_Cone_LPF;

        case RTPCParameterID_2013::UserAuxSendVolume0:                 return RTPCParameterID_2022::UserAuxSendVolume0;
        case RTPCParameterID_2013::UserAuxSendVolume1:                 return RTPCParameterID_2022::UserAuxSendVolume1;
        case RTPCParameterID_2013::UserAuxSendVolume2:                 return RTPCParameterID_2022::UserAuxSendVolume2;
        case RTPCParameterID_2013::UserAuxSendVolume3:                 return RTPCParameterID_2022::UserAuxSendVolume3;
        case RTPCParameterID_2013::GameAuxSendVolume:                  return RTPCParameterID_2022::GameAuxSendVolume;

        case RTPCParameterID_2013::Position_PAN_X_2D:                  return RTPCParameterID_2022::Position_PAN_X_2D;
        case RTPCParameterID_2013::Position_PAN_Y_2D:                  return RTPCParameterID_2022::Position_PAN_Y_2D;

        case RTPCParameterID_2013::Position_PAN_X_3D:                  return RTPCParameterID_2022::Position_PAN_X_3D;
        case RTPCParameterID_2013::Position_PAN_Y_3D:                  return RTPCParameterID_2022::Position_PAN_Y_3D;

        case RTPCParameterID_2013::OutputBusVolume:                    return RTPCParameterID_2022::OutputBusVolume;
        case RTPCParameterID_2013::OutputBusLPF:                       return RTPCParameterID_2022::OutputBusLPF;

        case RTPCParameterID_2013::BypassFX0:                          return RTPCParameterID_2022::BypassFX0;
        case RTPCParameterID_2013::BypassFX1:                          return RTPCParameterID_2022::BypassFX1;
        case RTPCParameterID_2013::BypassFX2:                          return RTPCParameterID_2022::BypassFX2;
        case RTPCParameterID_2013::BypassFX3:                          return RTPCParameterID_2022::BypassFX3;
        case RTPCParameterID_2013::BypassAllFX:                        return RTPCParameterID_2022::BypassAllFX;

        case RTPCParameterID_2013::HDRBusThreshold:                    return RTPCParameterID_2022::HDRBusThreshold;
        case RTPCParameterID_2013::HDRBusReleaseTime:                  return RTPCParameterID_2022::HDRBusReleaseTime;
        case RTPCParameterID_2013::HDRBusRatio:                        return RTPCParameterID_2022::HDRBusRatio;
        case RTPCParameterID_2013::HDRActiveRange:                     return RTPCParameterID_2022::HDRActiveRange;
        case RTPCParameterID_2013::MakeUpGain:                         return RTPCParameterID_2022::MakeUpGain;
        }
    }

    RTPCParameterID_2022 ConvertType(RTPCParameterID_2015 t)
    {
        switch (t)
        {
        case RTPCParameterID_2015::Volume:                              return RTPCParameterID_2022::Volume;
        case RTPCParameterID_2015::LFE:                                 return RTPCParameterID_2022::LFE;
        case RTPCParameterID_2015::Pitch:                               return RTPCParameterID_2022::Pitch;
        case RTPCParameterID_2015::LPF:                                 return RTPCParameterID_2022::LPF;
        case RTPCParameterID_2015::HPF:                                 return RTPCParameterID_2022::HPF;
        case RTPCParameterID_2015::BusVolume:                           return RTPCParameterID_2022::BusVolume;
        case RTPCParameterID_2015::InitialDelay:                        return RTPCParameterID_2022::InitialDelay;

        case RTPCParameterID_2015::PlayMechanismSpecialTransitionsValue:return RTPCParameterID_2022::PlayMechanismSpecialTransitionsValue;
        case RTPCParameterID_2015::MaxNumInstances:                    return RTPCParameterID_2022::MaxNumInstances;

        case RTPCParameterID_2015::Priority:                            return RTPCParameterID_2022::Priority;

        case RTPCParameterID_2015::PositioningType:                    return RTPCParameterID_2022::PositioningTypeBlend;
        case RTPCParameterID_2015::Positioning_Divergence_Center_PCT:  return RTPCParameterID_2022::Positioning_Divergence_Center_PCT;
        case RTPCParameterID_2015::Positioning_Cone_Attenuation_ON_OFF:return RTPCParameterID_2022::Positioning_Cone_Attenuation_ON_OFF;
        case RTPCParameterID_2015::Positioning_Cone_Attenuation:       return RTPCParameterID_2022::Positioning_Cone_Attenuation;
        case RTPCParameterID_2015::Positioning_Cone_LPF:               return RTPCParameterID_2022::Positioning_Cone_LPF;
        case RTPCParameterID_2015::Positioning_Cone_HPF:               return RTPCParameterID_2022::Positioning_Cone_HPF;

        case RTPCParameterID_2015::UserAuxSendVolume0:                 return RTPCParameterID_2022::UserAuxSendVolume0;
        case RTPCParameterID_2015::UserAuxSendVolume1:                 return RTPCParameterID_2022::UserAuxSendVolume1;
        case RTPCParameterID_2015::UserAuxSendVolume2:                 return RTPCParameterID_2022::UserAuxSendVolume2;
        case RTPCParameterID_2015::UserAuxSendVolume3:                 return RTPCParameterID_2022::UserAuxSendVolume3;
        case RTPCParameterID_2015::GameAuxSendVolume:                  return RTPCParameterID_2022::GameAuxSendVolume;

        case RTPCParameterID_2015::Position_PAN_X_2D:                  return RTPCParameterID_2022::Position_PAN_X_2D;
        case RTPCParameterID_2015::Position_PAN_Y_2D:                  return RTPCParameterID_2022::Position_PAN_Y_2D;

        case RTPCParameterID_2015::Position_PAN_X_3D:                  return RTPCParameterID_2022::Position_PAN_X_3D;
        case RTPCParameterID_2015::Position_PAN_Y_3D:                  return RTPCParameterID_2022::Position_PAN_Y_3D;

        case RTPCParameterID_2015::OutputBusVolume:                    return RTPCParameterID_2022::OutputBusVolume;
        case RTPCParameterID_2015::OutputBusHPF:                       return RTPCParameterID_2022::OutputBusHPF;
        case RTPCParameterID_2015::OutputBusLPF:                       return RTPCParameterID_2022::OutputBusLPF;

        case RTPCParameterID_2015::HDRBusThreshold:                    return RTPCParameterID_2022::HDRBusThreshold;
        case RTPCParameterID_2015::HDRBusReleaseTime:                  return RTPCParameterID_2022::HDRBusReleaseTime;
        case RTPCParameterID_2015::HDRBusRatio:                        return RTPCParameterID_2022::HDRBusRatio;
        case RTPCParameterID_2015::HDRActiveRange:                     return RTPCParameterID_2022::HDRActiveRange;

        case RTPCParameterID_2015::MakeUpGain:                         return RTPCParameterID_2022::MakeUpGain;

        case RTPCParameterID_2015::MidiTransposition:                  return RTPCParameterID_2022::MidiTransposition;
        case RTPCParameterID_2015::MidiVelocityOffset:                 return RTPCParameterID_2022::MidiVelocityOffset;
        case RTPCParameterID_2015::PlaybackSpeed:                      return RTPCParameterID_2022::PlaybackSpeed;

        case RTPCParameterID_2015::MuteRatio:                          return RTPCParameterID_2022::MuteRatio;
        }
    }
}