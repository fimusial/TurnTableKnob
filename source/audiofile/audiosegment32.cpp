#include "audiosegment32.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

namespace TTK
{
    AudioSegment32::AudioSegment32(double sampleRate, size_t sampleCount)
        : sampleRate(sampleRate),
        sampleCount(sampleCount)
    {
    }

    AudioSegment32* AudioSegment32::fromFile(std::string path)
    {
        auto hasExtension = [](std::string path, std::string ext)
        {
            return path.size() > ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0;
        };

        if (hasExtension(path, ".wav"))
        {
            return fromWavFile(path);
        }

        if (hasExtension(path, ".mp3"))
        {
            return fromMp3File(path);
        }

        if (hasExtension(path, ".flac"))
        {
            return fromFlacFile(path);
        }

        return nullptr;
    }

    AudioSegment32* AudioSegment32::fromWavFile(std::string path)
    {
        unsigned int channelCount;
        unsigned int sampleRate;
        size_t sampleCount;

        float* interleaved = drwav_open_file_and_read_pcm_frames_f32(
            path.c_str(), &channelCount, &sampleRate, &sampleCount, NULL);

        if (interleaved == NULL)
        {
            return nullptr;
        }

        AudioSegment32* segment = fromInterleaved(interleaved, channelCount, sampleRate, sampleCount);
        drwav_free(interleaved, NULL);
        return segment;
    }

    AudioSegment32* AudioSegment32::fromMp3File(std::string path)
    {
        drmp3_config config;
        size_t sampleCount;

        float* interleaved = drmp3_open_file_and_read_pcm_frames_f32(
            path.c_str(), &config, &sampleCount, NULL);

        if (interleaved == NULL)
        {
            return nullptr;
        }

        AudioSegment32* segment = fromInterleaved(interleaved, config.channels, config.sampleRate, sampleCount);
        drmp3_free(interleaved, NULL);
        return segment;
    }

    AudioSegment32* AudioSegment32::fromFlacFile(std::string path)
    {
        unsigned int channelCount;
        unsigned int sampleRate;
        size_t sampleCount;

        float* interleaved = drflac_open_file_and_read_pcm_frames_f32(
            path.c_str(), &channelCount, &sampleRate, &sampleCount, NULL);

        if (interleaved == NULL)
        {
            return nullptr;
        }

        AudioSegment32* segment = fromInterleaved(interleaved, channelCount, sampleRate, sampleCount);
        drflac_free(interleaved, NULL);
        return segment;
    }

    AudioSegment32* AudioSegment32::fromInterleaved(float* interleaved, unsigned int channelCount, unsigned int sampleRate, size_t sampleCount)
    {
        AudioSegment32* segment = new AudioSegment32(sampleRate, sampleCount);

        segment->channels.resize(channelCount);
        for (unsigned int channel = 0; channel < channelCount; channel++)
        {
            segment->channels[channel].resize(sampleCount);
            for (size_t sample = 0; sample < sampleCount; sample++)
            {
                segment->channels[channel][sample]
                    = interleaved[sample * channelCount + channel];
            }
        }

        return segment;
    }
}
