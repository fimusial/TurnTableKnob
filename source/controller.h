#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;
using namespace Vst;

namespace TTK
{
    class TurnTableKnobController
        : public EditControllerEx1,
        public IMidiMapping
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

        tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiCC, ParamID& id) SMTG_OVERRIDE;

        DEFINE_INTERFACES
            DEF_INTERFACE(IMidiMapping)
        END_DEFINE_INTERFACES(EditController)
        DELEGATE_REFCOUNT(EditController)
    };
}
