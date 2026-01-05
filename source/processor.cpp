#include "processor.h"

namespace TTK
{
    TurnTableKnobProcessor::TurnTableKnobProcessor()
        : timelineControlFactory(*this),
        segment(nullptr),
        filePath(""),
        speed(Speed, 0.5)
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

        return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
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
        // TODO
        IBStreamer streamer(state, kLittleEndian);
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::getState(IBStream* state)
    {
        // TODO
        IBStreamer streamer(state, kLittleEndian);
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobProcessor::process(ProcessData& data)
    {
        beginParameterChanges(data);
        processSamples(data);
        endParameterChanges();
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

    void TurnTableKnobProcessor::beginParameterChanges(ProcessData& data)
    {
        if (!data.inputParameterChanges)
        {
            return;
        }

        int count = data.inputParameterChanges->getParameterCount();
        for (int i = 0; i < count; i++)
        {
            IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
            if (!queue)
            {
                continue;
            }

            if (queue->getParameterId() == Speed)
            {
                speed.beginChanges(queue);
            }
        }
    }

    void TurnTableKnobProcessor::endParameterChanges()
    {
        speed.endChanges();
    }

    void TurnTableKnobProcessor::processSamples(ProcessData& data)
    {
        if (data.numOutputs != 1 || data.numSamples <= 0)
        {
            return;
        }

        bool is32 = processSetup.symbolicSampleSize == kSample32;
        bool is64 = processSetup.symbolicSampleSize == kSample64;

        if (!segment)
        {
            for (int channel = 0; channel < data.outputs->numChannels; channel++)
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

            data.outputs[0].silenceFlags = ((uint64)1 << data.outputs->numChannels) - 1;
            return;
        }

        int minChannelCount = min<int>(data.outputs->numChannels, (int)segment->channels.size());

        for (int sample = 0; sample < data.numSamples; sample++)
        {
            double diff = speed.getValue() * 8 - 4;
            playhead += diff;

            if (playhead < 0)
            {
                playhead = diff < 0 ? segment->sampleCount - 1 : 0;
            }

            if (playhead >= segment->sampleCount)
            {
                playhead = diff > 0 ? 0 : segment->sampleCount;
            }

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

            speed.advance(1);
        }

        data.outputs[0].silenceFlags = 0;
    }
}
