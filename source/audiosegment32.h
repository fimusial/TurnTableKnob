#pragma once

#include "string"
#include "vector"

using namespace std;

namespace TTK
{
    struct AudioSegment32
    {
        AudioSegment32(double sampleRate, size_t sampleCount);

        double sampleRate;
        size_t sampleCount;
        vector<vector<float>> channels;

        static AudioSegment32* fromFile(string path);
    };
}
