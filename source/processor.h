#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "uicontrols/timelinecontrolfactory.h"
#include "processing/smoothfollowparameter.h"
#include "processing/declicker.h"

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

        AudioSegment32* processNewFilePath(std::string newFilePath) override;
        AudioSegment32* getSegment() override;
        std::string getFilePath() override;
        size_t getWindowStart() override;
        size_t getWindowEnd() override;
        double getPlayheadValue() override;
        bool getHoldValue() override;
        void resetPlayhead(double newValue = 0.0) override;
        void scrollSegment(int samples) override;
        void zoomSegment(int samples) override;
        void resetHold(bool newValue = false) override;

    private:
        TimelineControlFactory timelineControlFactory;
        AudioSegment32* segment;

        // TODO: persist plugin state
        std::string filePath;
        size_t windowStart;
        size_t windowEnd;

        // TODO: RTTransferT<double> rtt;
        SmoothFollowParameter playhead;
        bool hold;
        double autoPlay;
        size_t sampleIndex;
        DeClicker deClicker; // TODO: UI display

        // TODO: show hold and autoplay in the ui based on processor values
        void beginParameterChanges(ProcessData& data);
        void endParameterChanges();
        void processSamples(ProcessData& data);
        void outputSilence(ProcessData& data);
        void outputAutoPlay(ProcessData& data);
        void outputPlayhead(ProcessData& data);

        double snapAutoPlayValue(double value);
    };
}
