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
    };
}
