#include "controller.h"

namespace TTK
{
    tresult PLUGIN_API TurnTableKnobController::initialize(FUnknown* context)
    {
        tresult result = EditControllerEx1::initialize(context);
        if (result != kResultOk)
        {
            return result;
        }

        parameters.addParameter(
            STR16("Playhead"), nullptr, 0, 0, ParameterInfo::kCanAutomate, Playhead);

        // TODO: implement MIDI learn
        return result;
    }

    tresult PLUGIN_API TurnTableKnobController::terminate()
    {
        return EditControllerEx1::terminate();
    }

    tresult PLUGIN_API TurnTableKnobController::setComponentState(IBStream* state)
    {
        // TODO
        if (!state)
        {
            return kResultFalse;
        }

        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobController::setState(IBStream* state)
    {
        return kResultOk;
    }

    tresult PLUGIN_API TurnTableKnobController::getState(IBStream* state)
    {
        return kResultOk;
    }

    IPlugView* PLUGIN_API TurnTableKnobController::createView(FIDString name)
    {
        if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        {
            return new VSTGUI::VST3Editor(this, "view", "editor.uidesc");
        }

        return nullptr;
    }

    tresult PLUGIN_API TurnTableKnobController::onLiveMIDIControllerInput(
        int32 busIndex, int16 channel, CtrlNumber midiCC)
    {
        return kResultOk;
    }
}
