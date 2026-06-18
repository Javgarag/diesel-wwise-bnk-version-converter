#pragma once
#include <cstdint>

namespace Wwise {
    enum class Header : uint32_t {
        BKHD = 0x44484B42,
        DIDX = 0x58444944,
        DATA = 0x41544144,
        HIRC = 0x43524948,
        STID = 0x44495453
    };

    enum class BankVersion : uint32_t {
        V2013 = 0x58,
        V2015 = 0x71,
        V2022 = 0x91
    };

    enum class HIRCItemTypeOld : uint8_t {
        State = 0x01,
        Sound,
        Action,
        Event,
        RandomOrSequenceContainer,
        SwitchContainer,
        ActorMixer,
        Bus,
        LayerContainer,
        MusicSegment,
        MusicTrack,
        MusicSwitch,
        MusicRandomOrSequence,
        Attenuation,
        DialogueEvent,
        FeedbackBus,
        FeedbackNode,
        FxShareSet,
        FxCustom,
        AuxiliaryBus,
        LFO,
        Envelope,
        AudioDevice
    };

    enum class HIRCItemTypeNew : uint8_t {
        State = 0x01,
        Sound,
        Action,
        Event,
        RandomOrSequenceContainer,
        SwitchContainer,
        ActorMixer, // now called Containers
        AudioBus,
        LayerContainer,
        MusicSegment,
        MusicTrack,
        MusicSwitch,
        MusicRandomOrSequence,
        Attenuation,
        DialogueEvent,
        FxShareSet,
        FxCustom,
        AuxiliaryBus,
        LFO,
        Envelope,
        AudioDevice,
        TimeMod,
        SidechainMix
    };

    enum class ActionType : uint16_t {
        None = 0x0000,
        SetState = 0x1204,
        BypassFX_M = 0x1A02,
        BypassFX_O = 0x1A03,
        ResetBypassFX_M = 0x1B02,
        ResetBypassFX_O = 0x1B03,
        ResetBypassFX_ALL = 0x1B04,
        ResetBypassFX_ALL_O = 0x1B05,
        ResetBypassFX_AE = 0x1B08,
        ResetBypassFX_AE_O = 0x1B09,
        SetSwitch = 0x1901,
        UseState_E = 0x1002,
        UnuseState_E = 0x1102,
        Play = 0x0403,
        PlayAndContinue = 0x0503,
        Stop_E = 0x0102,
        Stop_E_O = 0x0103,
        Stop_ALL = 0x0104,
        Stop_ALL_O = 0x0105,
        Stop_AE = 0x0108,
        Stop_AE_O = 0x0109,
        Pause_E = 0x0202,
        Pause_E_O = 0x0203,
        Pause_ALL = 0x0204,
        Pause_ALL_O = 0x0205,
        Pause_AE = 0x0208,
        Pause_AE_O = 0x0209,
        Resume_E = 0x0302,
        Resume_E_O = 0x0303,
        Resume_ALL = 0x0304,
        Resume_ALL_O = 0x0305,
        Resume_AE = 0x0308,
        Resume_AE_O = 0x0309,
        Break_E = 0x1C02,
        Break_E_O = 0x1C03,
        Mute_M = 0x0602,
        Mute_O = 0x0603,
        Unmute_M = 0x0702,
        Unmute_O = 0x0703,
        Unmute_ALL = 0x0704,
        Unmute_ALL_O = 0x0705,
        Unmute_AE = 0x0708,
        Unmute_AE_O = 0x0709,
        SetVolume_M = 0x0A02,
        SetVolume_O = 0x0A03,
        ResetVolume_M = 0x0B02,
        ResetVolume_O = 0x0B03,
        ResetVolume_ALL = 0x0B04,
        ResetVolume_ALL_O = 0x0B05,
        ResetVolume_AE = 0x0B08,
        ResetVolume_AE_O = 0x0B09,
        SetPitch_M = 0x0802,
        SetPitch_O = 0x0803,
        ResetPitch_M = 0x0902,
        ResetPitch_O = 0x0903,
        ResetPitch_ALL = 0x0904,
        ResetPitch_ALL_O = 0x0905,
        ResetPitch_AE = 0x0908,
        ResetPitch_AE_O = 0x0909,
        SetLPF_M = 0x0E02,
        SetLPF_O = 0x0E03,
        ResetLPF_M = 0x0F02,
        ResetLPF_O = 0x0F03,
        ResetLPF_ALL = 0x0F04,
        ResetLPF_ALL_O = 0x0F05,
        ResetLPF_AE = 0x0F08,
        ResetLPF_AE_O = 0x0F09,
        SetHPF_M = 0x2002,
        SetHPF_O = 0x2003,
        ResetHPF_M = 0x3002,
        ResetHPF_O = 0x3003,
        ResetHPF_ALL = 0x3004,
        ResetHPF_ALL_O = 0x3005,
        ResetHPF_AE = 0x3008,
        ResetHPF_AE_O = 0x3009,
        SetBusVolume_M = 0x0C02,
        SetBusVolume_O = 0x0C03,
        ResetBusVolume_M = 0x0D02,
        ResetBusVolume_O = 0x0D03,
        ResetBusVolume_ALL = 0x0D04,
        ResetBusVolume_AE = 0x0D08,
        PlayEvent = 0x2103,
        Duck = 0x1820,
        Trigger = 0x1D00,
        Trigger_O = 0x1D01,
        Trigger_E = 0x1D02,
        Trigger_E_O = 0x1D03,
        Seek_E = 0x1E02,
        Seek_E_O = 0x1E03,
        Seek_ALL = 0x1E04,
        Seek_ALL_O = 0x1E05,
        Seek_AE = 0x1E08,
        Seek_AE_O = 0x1E09,
        ResetPlaylist_E = 0x2202,
        ResetPlaylist_E_O = 0x2203,
        SetGameParameter = 0x1302,
        SetGameParameter_O = 0x1303,
        ResetGameParameter = 0x1402,
        ResetGameParameter_O = 0x1403,
        Release = 0x1F02,
        Release_O = 0x1F03,
        PlayEventUnknown_O = 0x2303,
        SetFX_M = 0x3102,
        ResetSetFX_M = 0x3202,
        ResetSetFX_ALL = 0x3204,
        NoOp = 0x4000
    };

    // RTPCs

    enum class CurveScaling : uint8_t {
        None,
        Unsupported,
        dB, // default
        Log,
        dBToLin,
        MaxNum = 0x08
    };

    enum class CurveInterpolation : uint32_t {
        Log3,
        Sine,
        Log1,
        InvSCurve,
        Linear,
        SCurve,
        Exp1,
        SineRecip,
        Exp3,
        Constant
    };

    enum class RTPCType2015 : uint8_t {
        GameParameter,
        MIDIParameter,
        Modulator
    };

    enum class RTPCType2022 : uint8_t {
        GameParameter,
        MIDIParameter,
        Switch,
        State,
        Modulator
    };

    enum class RTPCAccum2015 : uint8_t {
        Exclusive,
        Additive,
        Multiply
    };

    enum class RTPCAccum2022 : uint8_t {
        None,
        Exclusive,
        Additive,
        Multiply,
        Boolean,
        Maximum,
        Filter
    };

    // Positioning Params

    // Older positioning type
    enum class PositioningType : uint32_t {
        Undefined,
        Positioning2D,
        UserDef3D,
        GameDef3D
    };

    enum class SpatializationMode : uint8_t {
        None, // default
        PositionOnly,
        PositionAndOrientation
    };

    enum class PathMode { // either uint32 or uint8
        StepSequence,
        StepRandom,
        ContinuousSequence,
        ContinuousRandom,
        StepSequencePickNewPath,
        StepRandomPickNewPath
    };

    // 2022
    enum class SpeakerPanningType : uint8_t {
        DirectSpeakerAssignment, // default
        BalanceFadeHeight,
        SteeringPanner
    };

    // 2022
    enum class ThreeDPositionType : uint8_t {
        Emitter, // default
        EmitterWithAutomation,
        ListenerWithAutomation
    };

    // Plugins

    enum class PluginType : uint8_t { // only 4 used
        None,
        Codec,
        Source,
        Effect,
        MotionDevice,
        MotionSource,
        Mixer,
        Sink,
        GlobalExtension,
        Metadata
    };

    enum class PluginCompany : uint16_t { // only 12 used
        Audiokinetic = 0,
        AudiokineticExternal,
        Plugin = 64,
        Plugin_ = 255,
        McDSP,
        WaveArts,
        PhoneticArts,
        iZotope,
        CrankcaseAudio = 261,
        IOSONO,
        AuroTechnologies,
        Dolby,
        TwoBigEars,
        Oculus,
        BlueRippleSound,
        EnzienAudio,
        KrotosDehumanizer,
        Nurulize,
        SuperPowered,
        Google,
        NVIDIA,
        Reserved,
        Microsoft,
        YAMAHA,
        VisiSonics
    };

    enum class FXPluginSpecialID : uint32_t {
        FxSrcSineParams = 0x00640002,
        FxSrcSilenceParams = 0x00650002,
        ToneGenParams = 0x00660002,
        ParameterEQFXParams = 0x00690003,
        DelayFXParams = 0x006A0003,
        PeakLimiterFXParams = 0x006E0003,
        FDNReverbFXParams = 0x00730003,
        RoomVerbFXParams = 0x00760003,
        FlangerFXParams = 0x007D0003,
        GuitarDistortionFXParams = 0x007E0003,
        ConvolutionReverbFXParams = 0x007F0003,
        MeterFXParams = 0x00810003,
        HarmonizerFXParams = 0x008A0003,
        GainFXParams = 0x008B0003,
        StereoDelayFXParams = 0x00870003,
        SynthOneParams = 0x00940002,
        FxSrcAudioInputParams = 0x00C80002,
        IZTrashDelayFXParams = 0x00041033,
    };

    // Sound

    enum class SourceTypeOld : uint32_t {
        Bank,
        Streaming,
        PrefetchStreaming
    };

    enum class SourceTypeNew : uint8_t {
        Bank,
        PrefetchStreaming,
        Streaming
    };

    // Property IDs
    enum class AkPropID_2013 : uint8_t
    {
        Volume,
        LFE,
        Pitch,
        LPF,
        BusVolume,
        Priority,
        PriorityDistanceOffset,
        Loop,
        FeedbackVolume,
        FeedbackLPF,
        MuteRatio,
        PAN_LR,
        PAN_FR,
        CenterPCT,
        DelayTime,
        TransitionTime,
        Probability,
        DialogueMode,
        UserAuxSendVolume0,
        UserAuxSendVolume1,
        UserAuxSendVolume2,
        UserAuxSendVolume3,
        GameAuxSendVolume,
        OutputBusVolume,
        OutputBusLPF,
        InitialDelay,
        HDRBusThreshold,
        HDRBusRatio,
        HDRBusReleaseTime,
        HDRBusGameParam,
        HDRBusGameParamMin,
        HDRBusGameParamMax,
        HDRActiveRange,
        MakeUpGain,
        LoopStart,
        LoopEnd,
        TrimInTime,
        TrimOutTime,
        FadeInTime,
        FadeOutTime,
        FadeInCurve,
        FadeOutCurve,
        LoopCrossfadeDuration,
        CrossfadeUpCurve,
        CrossfadeDownCurve
    };

    enum class AkPropID_2015 : uint8_t
    {
        Volume,
        LFE,
        Pitch,
        LPF,
        HPF,
        BusVolume,
        Priority,
        PriorityDistanceOffset,
        FeedbackVolume,
        FeedbackLPF,
        MuteRatio,
        PAN_LR,
        PAN_FR,
        CenterPCT,
        DelayTime,
        TransitionTime,
        Probability,
        DialogueMode,
        UserAuxSendVolume0,
        UserAuxSendVolume1,
        UserAuxSendVolume2,
        UserAuxSendVolume3,
        GameAuxSendVolume,
        OutputBusVolume,
        OutputBusHPF,
        OutputBusLPF,
        HDRBusThreshold,
        HDRBusRatio,
        HDRBusReleaseTime,
        HDRBusGameParam,
        HDRBusGameParamMin,
        HDRBusGameParamMax,
        HDRActiveRange,
        MakeUpGain,
        LoopStart,
        LoopEnd,
        TrimInTime,
        TrimOutTime,
        FadeInTime,
        FadeOutTime,
        FadeInCurve,
        FadeOutCurve,
        LoopCrossfadeDuration,
        CrossfadeUpCurve,
        CrossfadeDownCurve,
        MidiTrackingRootNote,
        MidiPlayOnNoteType,
        MidiTransposition,
        MidiVelocityOffset,
        MidiKeyRangeMin,
        MidiKeyRangeMax,
        MidiVelocityRangeMin,
        MidiVelocityRangeMax,
        MidiChannelMask,
        PlaybackSpeed,
        MidiTempoSource,
        MidiTargetNode,
        AttachedPluginFXID,
        Loop,
        InitialDelay
    };

    enum class AkPropID_2022 : uint8_t {
        Volume,
        LFE,
        Pitch,
        LPF,
        HPF,
        BusVolume,
        MakeUpGain,
        Priority,
        PriorityDistanceOffset,
        _FeedbackVolume, // removed
        _FeedbackLPF,    // removed
        MuteRatio,
        PAN_LR,
        PAN_FR,
        CenterPCT,
        DelayTime,
        TransitionTime,
        Probability,
        DialogueMode,
        UserAuxSendVolume0,
        UserAuxSendVolume1,
        UserAuxSendVolume2,
        UserAuxSendVolume3,
        GameAuxSendVolume,
        OutputBusVolume,
        OutputBusHPF,
        OutputBusLPF,
        HDRBusThreshold,
        HDRBusRatio,
        HDRBusReleaseTime,
        HDRBusGameParam,
        HDRBusGameParamMin,
        HDRBusGameParamMax,
        HDRActiveRange,
        LoopStart,
        LoopEnd,
        TrimInTime,
        TrimOutTime,
        FadeInTime,
        FadeOutTime,
        FadeInCurve,
        FadeOutCurve,
        LoopCrossfadeDuration,
        CrossfadeUpCurve,
        CrossfadeDownCurve,
        MidiTrackingRootNote,
        MidiPlayOnNoteType,
        MidiTransposition,
        MidiVelocityOffset,
        MidiKeyRangeMin,
        MidiKeyRangeMax,
        MidiVelocityRangeMin,
        MidiVelocityRangeMax,
        MidiChannelMask,
        PlaybackSpeed,
        MidiTempoSource,
        MidiTargetNode,
        AttachedPluginFXID,
        Loop,
        InitialDelay,
        UserAuxSendLPF0,
        UserAuxSendLPF1,
        UserAuxSendLPF2,
        UserAuxSendLPF3,
        UserAuxSendHPF0,
        UserAuxSendHPF1,
        UserAuxSendHPF2,
        UserAuxSendHPF3,
        GameAuxSendLPF,
        GameAuxSendHPF,
        AttenuationID,
        PositioningTypeBlend,
        ReflectionBusVolume,
        PAN_UD
    };

    enum class RTPCParameterID_2013 : uint8_t
    {
        Volume = 0x0,
        LFE,
        Pitch,
        LPF,
        BusVolume,
        PlayMechanismSpecialTransitionsValue,
        InitialDelay,

        Priority = 0x8,
        MaxNumInstances,
        PositioningType,
        Positioning_Divergence_Center_PCT,
        Positioning_Cone_Attenuation_ON_OFF,
        Positioning_Cone_Attenuation,
        Positioning_Cone_LPF,

        UserAuxSendVolume0 = 0xF,
        UserAuxSendVolume1,
        UserAuxSendVolume2,
        UserAuxSendVolume3,
        GameAuxSendVolume,

        Position_PAN_X_2D = 0x14,
        Position_PAN_Y_2D,
        OutputBusVolume,
        OutputBusLPF,

        BypassFX0 = 0x18,
        BypassFX1,
        BypassFX2,
        BypassFX3,
        BypassAllFX,

        FeedbackVolume = 0x1D,
        FeedbackLowpass,
        FeedbackPitch,

        HDRBusThreshold = 0x20,
        HDRBusReleaseTime,
        HDRBusRatio,
        HDRActiveRange,
        MakeUpGain,

        Position_PAN_X_3D = 0x25,
        Position_PAN_Y_3D
    };

    enum class RTPCParameterID_2015 : uint8_t
    {
        Volume = 0x0,
        LFE,
        Pitch,
        LPF,
        HPF,
        BusVolume,
        InitialDelay,
        PlayMechanismSpecialTransitionsValue,

        Priority,
        MaxNumInstances,
        PositioningType,
        Positioning_Divergence_Center_PCT,
        Positioning_Cone_Attenuation_ON_OFF,
        Positioning_Cone_Attenuation,
        Positioning_Cone_LPF,
        Positioning_Cone_HPF,

        GameAuxSendVolume = 0x13,
        Position_PAN_X_2D,
        Position_PAN_Y_2D,

        BypassFX0 = 0x18,
        BypassFX1,
        BypassFX2,
        BypassFX3,
        BypassAllFX,

        FeedbackVolume = 0x1D,
        FeedbackLowpass,
        FeedbackPitch,

        HDRBusThreshold = 0x20,
        HDRBusReleaseTime,
        HDRBusRatio,
        HDRActiveRange,
        MakeUpGain,

        Position_PAN_X_3D = 0x25,
        Position_PAN_Y_3D,

        MidiTransposition = 0x27,
        MidiVelocityOffset,
        PlaybackSpeed,

        ModulatorLfoDepth = 0x2A,
        ModulatorLfoAttack,
        ModulatorLfoFrequency,
        ModulatorLfoWaveform,
        ModulatorLfoSmoothing,
        ModulatorLfoPWM,
        ModulatorLfoInitialPhase,
        ModulatorLfoRetrigger,
        ModulatorEnvelopeAttackTime,
        ModulatorEnvelopeAttackCurve,
        ModulatorEnvelopeDecayTime,
        ModulatorEnvelopeSustainLevel,
        ModulatorEnvelopeSustainTime,
        ModulatorEnvelopeReleaseTime,

        UserAuxSendVolume0 = 0x38,
        UserAuxSendVolume1,
        UserAuxSendVolume2,
        UserAuxSendVolume3,

        OutputBusVolume = 0x3C,
        OutputBusHPF,
        OutputBusLPF,

        MuteRatio = 0x3F
    };

    enum class RTPCParameterID_2022 : uint8_t
    {
        Volume = 0x0,
        LFE,
        Pitch,
        LPF,
        HPF,
        BusVolume,
        InitialDelay,
        MakeUpGain,

        Deprecated_FeedbackVolume,
        Deprecated_FeedbackLowpass,
        Deprecated_FeedbackPitch,

        MidiTransposition,
        MidiVelocityOffset,
        PlaybackSpeed,
        MuteRatio,
        PlayMechanismSpecialTransitionsValue,
        MaxNumInstances,

        Priority = 0x11,
        Position_PAN_X_2D,
        Position_PAN_Y_2D,
        Position_PAN_X_3D,
        Position_PAN_Y_3D,
        Position_PAN_Z_3D,
        PositioningTypeBlend,
        Positioning_Divergence_Center_PCT,
        Positioning_Cone_Attenuation_ON_OFF,
        Positioning_Cone_Attenuation,
        Positioning_Cone_LPF,
        Positioning_Cone_HPF,

        BypassFX0,
        BypassFX1,
        BypassFX2,
        BypassFX3,
        BypassAllFX,

        HDRBusThreshold,
        HDRBusReleaseTime,
        HDRBusRatio,
        HDRActiveRange,

        GameAuxSendVolume,
        UserAuxSendVolume0,
        UserAuxSendVolume1,
        UserAuxSendVolume2,
        UserAuxSendVolume3,

        OutputBusVolume,
        OutputBusHPF,
        OutputBusLPF,

        Positioning_EnableAttenuation,
        ReflectionsVolume,

        UserAuxSendLPF0,
        UserAuxSendLPF1,
        UserAuxSendLPF2,
        UserAuxSendLPF3,

        UserAuxSendHPF0,
        UserAuxSendHPF1,
        UserAuxSendHPF2,
        UserAuxSendHPF3,

        GameAuxSendLPF,
        GameAuxSendHPF,

        Position_PAN_Z_2D,
        BypassAllMetadata,

        Unknown_Custom_3D = 0x3D,
        Unknown_Custom_3E,
        Unknown_Custom_3F
    };

    // Type converters
    HIRCItemTypeNew ConvertType(HIRCItemTypeOld t);

    RTPCType2022 ConvertType(RTPCType2015 t);
    RTPCAccum2022 ConvertType(RTPCAccum2015 t);
    SourceTypeNew ConvertType(SourceTypeOld t);

    AkPropID_2015 ConvertType(AkPropID_2013 t);
    AkPropID_2022 ConvertType_2013_to_2022(AkPropID_2013 t);
    AkPropID_2022 ConvertType(AkPropID_2015 t);

    RTPCParameterID_2015 ConvertType(RTPCParameterID_2013 t);
    RTPCParameterID_2022 ConvertType_2013_to_2022(RTPCParameterID_2013 t);
    RTPCParameterID_2022 ConvertType(RTPCParameterID_2015 t);
}