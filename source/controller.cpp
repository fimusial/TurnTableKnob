#include "controller.h"

#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstreamer.h"
#include "vstgui/uidescription/uicontentprovider.h"
#include "cids.h"
#include "resourcemanager.h"

namespace TTK
{
    tresult PLUGIN_API TurnTableKnobController::initialize(FUnknown* context)
    {
        tresult result = EditControllerEx1::initialize(context);
        if (result != kResultOk)
        {
            return result;
        }

        // title, units, step count, default value normalized, flags, tag

        parameters.addParameter(
            STR16("Playhead"), nullptr, 0, 0.0, ParameterInfo::kCanAutomate, Playhead);

        parameters.addParameter(
            STR16("Hold"), nullptr, 1, 0.0, ParameterInfo::kCanAutomate, Hold);

        parameters.addParameter(
            STR16("AutoPlay"), nullptr, 0, 0.0, ParameterInfo::kCanAutomate, AutoPlay);

        parameters.addParameter(
            STR16("XFader"), nullptr, 0, 1.0, ParameterInfo::kCanAutomate, XFader);

        parameters.addParameter(
            STR16("XFaderCurve"), nullptr, 0, 0.0, ParameterInfo::kCanAutomate, XFaderCurve);

        return result;
    }

    tresult PLUGIN_API TurnTableKnobController::terminate()
    {
        return EditControllerEx1::terminate();
    }

    tresult PLUGIN_API TurnTableKnobController::setComponentState(IBStream* state)
    {
        if (!state)
        {
            return kInvalidArgument;
        }

        IBStreamer streamer(state, kLittleEndian);

        double autoPlay = 0.0;
        streamer.readDouble(autoPlay);
        if (setParamNormalized(AutoPlay, autoPlay) == kResultFalse)
        {
            return kResultFalse;
        }

        double xFaderValue = 1.0;
        streamer.readDouble(xFaderValue);
        if (setParamNormalized(XFader, xFaderValue) == kResultFalse)
        {
            return kResultFalse;
        }

        double xFaderCurveValue = 0.0;
        streamer.readDouble(xFaderCurveValue);
        if (setParamNormalized(XFaderCurve, xFaderCurveValue) == kResultFalse)
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

    tresult PLUGIN_API TurnTableKnobController::getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiCC, ParamID& id)
    {
        if (busIndex != 0)
        {
            return kResultFalse;
        }

        id = XFader;
        return kResultOk;
    }

    IPlugView* PLUGIN_API TurnTableKnobController::createView(FIDString name)
    {
        if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        {
            std::string uiDescription = ResourceManager::getFileContent("uidesc.json");

            return new VSTGUI::VST3Editor(
                new VSTGUI::UIDescription(
                    (VSTGUI::IContentProvider*)new VSTGUI::MemoryContentProvider(uiDescription.c_str(), (unsigned int)uiDescription.size()),
                    nullptr),
                this,
                "view");
        }

        return nullptr;
    }
}
