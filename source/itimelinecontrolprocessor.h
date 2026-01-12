#pragma once

#include "audiosegment32.h"

namespace TTK
{
    class ITimelineControlProcessor
    {
    public:
        virtual AudioSegment32* processNewFilePath(std::string newFilePath) = 0;
        virtual AudioSegment32* getSegment() = 0;
        virtual std::string getFilePath() = 0;
    };
}