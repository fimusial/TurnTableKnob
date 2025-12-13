#pragma once

#include "string"
#include "vector"

using namespace std;

namespace TTK
{
    struct AudioSegment32
    {
        int channelCount = 0;
        double sampleRate = 0;
        size_t sampleCount = 0;
        vector<vector<float>> channels;

        static AudioSegment32* fromFile(string path);
    };
}
