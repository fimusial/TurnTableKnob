#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/utility/sampleaccurate.h"

#include "cids.h"
#include "timelinecontrol.h"
#include "timelinecontrolfactory.h"
#include "lerpfollowparameter.h"

using namespace Steinberg;
using namespace Vst;

namespace TTK
{
    class TurnTableKnobProcessor
        : public AudioEffect,
        public ITimelineControlProcessor
    {
    public:
        TurnTableKnobProcessor();
        ~TurnTableKnobProcessor() SMTG_OVERRIDE;

        static FUnknown* createInstance(void* context) 
        {
            return (IAudioProcessor*)new TurnTableKnobProcessor; 
        }

        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        tresult PLUGIN_API terminate() SMTG_OVERRIDE;
        tresult PLUGIN_API setBusArrangements(
            SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;
        tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
        tresult PLUGIN_API setupProcessing(ProcessSetup& newSetup) SMTG_OVERRIDE;
        tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
        tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;

        AudioSegment32* processNewFilePath(string newFilePath) override;
        AudioSegment32* getSegment() override;
        string getFilePath() override;

    private:
        TimelineControlFactory timelineControlFactory;
        AudioSegment32* segment;

        // persistent plugin state
        string filePath;
        double playhead = 0;

        // TODO: RTTransferT<double> rtt;
        SampleAccurate::Parameter speed;

        void beginParameterChanges(ProcessData& data);
        void endParameterChanges();
        void processSamples(ProcessData& data);
    };
}