#include "processor.h"

namespace TTK
{
    TurnTableKnobProcessor::TurnTableKnobProcessor()
        : speakerArrangement(0),
        timelineControlFactory(*this),
        filePath(""),
        segment(nullptr),
        playForward(false),
        playhead(0)
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
        beginParameterChanges(data.inputParameterChanges);
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

    void TurnTableKnobProcessor::beginParameterChanges(IParameterChanges* changes)
    {
        if (!changes)
        {
            return;
        }

        int count = changes->getParameterCount();
        for (int i = 0; i < count; i++)
        {
            IParamValueQueue* queue = changes->getParameterData(i);
            if (!queue)
            {
                continue;
            }

            if (queue->getParameterId() == PlayForward)
            {
                int sampleOffset;
                ParamValue value;
                queue->getPoint(queue->getPointCount() - 1, sampleOffset, value);
                playForward = value > 0;
            }
        }
    }

    void TurnTableKnobProcessor::endParameterChanges()
    {
    }

    void TurnTableKnobProcessor::processSamples(ProcessData& data)
    {
        if (data.numOutputs == 0 || data.numSamples == 0)
        {
            return;
        }

        bool is32 = processSetup.symbolicSampleSize == kSample32;
        bool is64 = processSetup.symbolicSampleSize == kSample64;
        int outputChannelCount = SpeakerArr::getChannelCount(speakerArrangement);

        if (!segment || !playForward)
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
            return;
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
        return;
    }
}
