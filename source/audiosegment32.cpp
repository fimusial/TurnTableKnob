#include "audiosegment32.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

namespace TTK
{
    AudioSegment32::AudioSegment32(double sampleRate, size_t sampleCount)
        : sampleRate(sampleRate),
        sampleCount(sampleCount)
    {
    }

    AudioSegment32* AudioSegment32::fromFile(string path)
    {
        const string ext = ".wav";
        if (path.size() <= ext.size()
            || path.compare(path.size() - ext.size(), ext.size(), ext) != 0)
        {
            return nullptr;
        }

        unsigned int channelCount;
        unsigned int sampleRate;
        size_t sampleCount;

        float* interleaved = drwav_open_file_and_read_pcm_frames_f32(
            path.c_str(), &channelCount, &sampleRate, &sampleCount, NULL);

        if (interleaved == NULL)
        {
            return nullptr;
        }

        AudioSegment32* segment = new AudioSegment32(sampleRate, sampleCount);

        segment->channels.resize(channelCount);
        for (int channel = 0; channel < channelCount; channel++)
        {
            segment->channels[channel].resize(sampleCount);
            for (size_t sample = 0; sample < sampleCount; sample++)
            {
                segment->channels[channel][sample]
                    = interleaved[sample * channelCount + channel];
            }
        }

        drwav_free(interleaved, NULL);
        return segment;
    }
}
