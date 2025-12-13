#include "audiosegment32.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

namespace TTK
{
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

        AudioSegment32* segment = new AudioSegment32();
        segment->channelCount = channelCount;
        segment->sampleRate = sampleRate;
        segment->sampleCount = sampleCount;

        segment->channels.resize(channelCount);
        for (int c = 0; c < channelCount; c++)
        {
            segment->channels[c].resize(sampleCount);
            for (int s = 0; s < sampleCount; s++)
            {
                segment->channels[c][s] = interleaved[s * channelCount + c];
            }
        }

        drwav_free(interleaved, NULL);
        return segment;
    }
}
