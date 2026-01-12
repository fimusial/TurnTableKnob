#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/vst/ivstmidilearn.h"

using namespace Steinberg;
using namespace Vst;

namespace TTK
{
    class TurnTableKnobController
        : public EditControllerEx1,
        public IMidiLearn
    {
    public:
        TurnTableKnobController() = default;
        ~TurnTableKnobController() SMTG_OVERRIDE = default;

        static FUnknown* createInstance(void* context)
        {
            return (IEditController*)new TurnTableKnobController;
        }

        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        tresult PLUGIN_API terminate() SMTG_OVERRIDE;
        tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
        IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;
        tresult PLUGIN_API onLiveMIDIControllerInput(
            int32 busIndex, int16 channel, CtrlNumber midiCC) SMTG_OVERRIDE;

        DEFINE_INTERFACES
        END_DEFINE_INTERFACES(EditController)
        DELEGATE_REFCOUNT(EditController)
    };
}
