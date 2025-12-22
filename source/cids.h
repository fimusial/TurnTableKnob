#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

using namespace Steinberg;
using namespace Vst;

namespace TTK
{
    static const FUID kTurnTableKnobProcessorUID(0x7455BC4C, 0x8D295C82, 0x92081070, 0x6D8BB568);
    static const FUID kTurnTableKnobControllerUID(0xA9A0E3E4, 0xF7BF5017, 0xA8771A9B, 0x755A3BA0);

    #define TurnTableKnobVST3Category "Instrument"

    enum ParameterIds : ParamID
    {
        PlayForward = 256,
    };
}
