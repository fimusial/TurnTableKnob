#pragma once

#include "string"
#include "vector"

namespace TTK
{
    struct AudioSegment32
    {
        AudioSegment32(double sampleRate, size_t sampleCount);

        double sampleRate;
        size_t sampleCount;
        std::vector<std::vector<float>> channels;

        static AudioSegment32* fromFile(std::string path);
        static AudioSegment32* fromWavFile(std::string path);
        static AudioSegment32* fromMp3File(std::string path);
        static AudioSegment32* fromFlacFile(std::string path);
        static AudioSegment32* fromInterleaved(float* interleaved, unsigned int channelCount, unsigned int sampleRate, size_t sampleCount);
    };
}
