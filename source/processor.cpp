#include "processor.h"

namespace TTK
{
    TurnTableKnobProcessor::TurnTableKnobProcessor()
        : playhead(0),
        speakerArrangement(0),
        timelineControlFactory(*this),
        filePath(""),
        segment(nullptr)
    {
        setControllerClass(kTurnTableKnobControllerUID);
    }

    TurnTableKnobProcessor::~TurnTableKnobProcessor()
    {
        if (segment)
        {
            delete segment;
        }
    }

    tresult PLUGIN_API TurnTableKnobProcessor::initialize(FUnknown* context)
    {
        tresult result = AudioEffect::initialize(context);
        if (result != kResultOk)
        {
            return result;
        }

        addAudioOutput(STR16("Stereo Out"), SpeakerArr::kStereo);
        addEventInput(STR16("Event In"), 1);
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::terminate()
    {
        return AudioEffect::terminate();
    }

    tresult PLUGIN_API TurnTableKnobProcessor::setBusArrangements(
        SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
    {
        if (numOuts != 1)
        {
            return kResultFalse;
        }

        tresult result = AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
        if (result != kResultOk)
        {
            return result;
        }

        speakerArrangement = outputs[0];
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::canProcessSampleSize(int32 symbolicSampleSize)
    {
        return symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64
            ? kResultTrue
            : kResultFalse;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::setupProcessing(ProcessSetup& newSetup)
    {
        return AudioEffect::setupProcessing(newSetup);
    }

    tresult PLUGIN_API TurnTableKnobProcessor::setActive(TBool state)
    {
        return AudioEffect::setActive(state);
    }

    tresult PLUGIN_API TurnTableKnobProcessor::setState(IBStream* state)
    {
        IBStreamer streamer(state, kLittleEndian);
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::getState(IBStream* state)
    {
        IBStreamer streamer(state, kLittleEndian);
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::process(ProcessData& data)
    {
        // TODO: receive DAW automation parameter changes; UI parameter changes must still take precedence
        if (data.numOutputs == 0 || data.numSamples == 0)
        {
            return kResultOk;
        }

        bool is32 = processSetup.symbolicSampleSize == kSample32;
        bool is64 = processSetup.symbolicSampleSize == kSample64;
        int outputChannelCount = SpeakerArr::getChannelCount(speakerArrangement);

        if (!segment)
        {
            for (int channel = 0; channel < outputChannelCount; channel++)
            {
                for (int sample = 0; sample < data.numSamples && is32; sample++)
                {
                    data.outputs[0].channelBuffers32[channel][sample] = 0.0f;
                }

                for (int sample = 0; sample < data.numSamples && is64; sample++)
                {
                    data.outputs[0].channelBuffers64[channel][sample] = 0.0;
                }
            }

            data.outputs[0].silenceFlags = ((uint64)1 << outputChannelCount) - 1;
            return kResultOk;
        }

        int minChannelCount = outputChannelCount < segment->channels.size()
            ? outputChannelCount
            : segment->channels.size();

        for (int sample = 0; sample < data.numSamples; sample++)
        {
            for (int channel = 0; channel < minChannelCount && is32; channel++)
            {
                data.outputs[0].channelBuffers32[channel][sample]
                    = segment->channels[channel][playhead];
            }

            for (int channel = 0; channel < minChannelCount && is64; channel++)
            {
                data.outputs[0].channelBuffers64[channel][sample]
                    = segment->channels[channel][playhead];
            }

            playhead = (playhead + 1) % segment->sampleCount;
        }

        data.outputs[0].silenceFlags = 0;
        return kResultOk;
    }

    AudioSegment32* TurnTableKnobProcessor::processNewFilePath(string newFilePath)
    {
        if (newFilePath.empty())
        {
            return nullptr;
        }

        AudioSegment32* newSegment = AudioSegment32::fromFile(newFilePath);
        if (!newSegment)
        {
            return nullptr;
        }

        AudioSegment32* oldSegment = segment;
        filePath = newFilePath;
        playhead = 0.0;
        segment = newSegment;

        if (oldSegment)
        {
            delete oldSegment;
        }

        return newSegment;
    }

    AudioSegment32* TurnTableKnobProcessor::getSegment()
    {
        return segment;
    }

    string TurnTableKnobProcessor::getFilePath()
    {
        return filePath;
    }
}
