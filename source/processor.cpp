#include "processor.h"

#include "base/source/fstreamer.h"
#include "cids.h"
#include "cmath"

namespace TTK
{
    TurnTableKnobProcessor::TurnTableKnobProcessor()
        : timelineControlFactory(*this),
        segment(nullptr),
        filePath(""),
        segmentStart(0),
        segmentEnd(MIN_SEGMENT_WINDOW_SIZE),
        playhead(Playhead)
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

    AudioSegment32* TurnTableKnobProcessor::processNewFilePath(std::string newFilePath)
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
        segmentStart = 0;
        segmentEnd = MIN_SEGMENT_WINDOW_SIZE;
        playhead.reset();
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

    std::string TurnTableKnobProcessor::getFilePath()
    {
        return filePath;
    }

    size_t TurnTableKnobProcessor::getSegmentStart()
    {
        return segmentStart;
    }

    size_t TurnTableKnobProcessor::getSegmentEnd()
    {
        return segmentEnd;
    }

    void TurnTableKnobProcessor::scrollSegment(int by)
    {
        if (by < 0 && -by > segmentStart)
        {
            by = -segmentStart;
        }

        if (by > 0 && by + segmentEnd >= segment->sampleCount)
        {
            by = segment->sampleCount - segmentEnd - 1;
        }

        segmentStart += by;
        segmentEnd += by;
        playhead.reset();
    }

    void TurnTableKnobProcessor::zoomSegment(int by)
    {
        if (by < 0 && segmentEnd - segmentStart + by < MIN_SEGMENT_WINDOW_SIZE)
        {
            by = segmentStart + MIN_SEGMENT_WINDOW_SIZE - segmentEnd;
        }

        if (by > 0 && by + segmentEnd >= segment->sampleCount)
        {
            by = segment->sampleCount - segmentEnd - 1;
        }

        segmentEnd += by;
        playhead.reset();
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

            if (queue->getParameterId() == Playhead)
            {
               playhead.beginChanges(queue);
            }
        }
    }

    void TurnTableKnobProcessor::endParameterChanges()
    {
        playhead.endChanges();
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
            for (int c = 0; c < data.outputs->numChannels; c++)
            {
                for (int i = 0; i < data.numSamples && is32; i++)
                {
                    data.outputs[0].channelBuffers32[c][i] = 0.0f;
                }

                for (int i = 0; i < data.numSamples && is64; i++)
                {
                    data.outputs[0].channelBuffers64[c][i] = 0.0;
                }
            }

            data.outputs[0].silenceFlags = ((uint64)1 << data.outputs->numChannels) - 1;
            return;
        }

        int channelCount = min<int>(data.outputs->numChannels, (int)segment->channels.size());

        for (int i = 0; i < data.numSamples; i++)
        {
            for (int c = 0; c < channelCount; c++)
            {
                /*double volume = std::abs(std::tanh(playhead.getVelocity() * VOLUME_DAMPING_SCALE));
                size_t sampleIndex = segmentStart + playhead.getValue() * (segmentEnd - segmentStart);
                double sample = 0 <= sampleIndex && sampleIndex < segment->sampleCount
                    ? volume * segment->channels[c][sampleIndex]
                    : 0.0;*/

                double precise = (double)segmentStart + playhead.getValue() * (double)(segmentEnd - segmentStart);
                size_t indexA = segmentStart + playhead.getValue() * (segmentEnd - segmentStart);
                size_t indexB = indexA < segment->sampleCount - 1 ? indexA + 1 : indexA;
                double sampleA = segment->channels[c][indexA];
                double sampleB = segment->channels[c][indexB];
                double sample = (sampleB - sampleA) * (precise - indexA) + sampleA;

                if (is32)
                {
                    data.outputs[0].channelBuffers32[c][i] = sample;
                }

                if (is64)
                {
                    data.outputs[0].channelBuffers64[c][i] = sample;
                }
            }

            playhead.advance();
        }

        data.outputs[0].silenceFlags = 0;
    }
}
