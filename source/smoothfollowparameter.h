#pragma once

#include "public.sdk/source/vst/utility/sampleaccurate.h"

using namespace Steinberg;
using namespace Vst;

namespace TTK
{
    struct SmoothFollowParameter
    {
    public:
        SmoothFollowParameter(
            unsigned int pid, double value = 0.0, double stiffness = 0.000125, double damping = 0.4);
        void beginChanges(IParamValueQueue* queue);
        void endChanges();
        double getValue();
        double advance();
        void reset();

    private:
        SampleAccurate::Parameter parameter;
        double value;
        double stiffness;
        double damping;
        double velocity;
    };
}