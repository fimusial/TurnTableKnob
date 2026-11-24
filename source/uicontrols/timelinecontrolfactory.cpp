#include "timelinecontrolfactory.h"

#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "vstgui/plugin-bindings/vst3editor.h"

#include "timelinecontrol.h"
#include "../controller.h"

namespace TTK
{
    TimelineControlFactory::TimelineControlFactory()
    {
        UIViewFactory::registerViewCreator(*this);
    }

    TimelineControlFactory::~TimelineControlFactory()
    {
        UIViewFactory::unregisterViewCreator(*this);
    }

    IdStringPtr TimelineControlFactory::getViewName() const
    {
        return "TimelineControl";
    }

    IdStringPtr TimelineControlFactory::getBaseViewName() const
    {
        return UIViewCreator::kCControl;
    }

    CView* TimelineControlFactory::create(
        const UIAttributes& attributes, const IUIDescription* description) const
    {
        if (!description)
        {
            return nullptr;
        }

        CPoint origin;
        if (!attributes.getPointAttribute("origin", origin))
        {
            return nullptr;
        }

        CPoint size;
        if (!attributes.getPointAttribute("size", size))
        {
            return nullptr;
        }

        CRect timelineBox;
        if (!attributes.getRectAttribute("timelineBox", timelineBox))
        {
            return nullptr;
        }

        CRect filePathBox;
        if (!attributes.getRectAttribute("filePathBox", filePathBox))
        {
            return nullptr;
        }

        CRect holdIndicatorBox;
        if (!attributes.getRectAttribute("holdIndicatorBox", holdIndicatorBox))
        {
            return nullptr;
        }

        CRect deClickerBox;
        if (!attributes.getRectAttribute("deClickerBox", deClickerBox))
        {
            return nullptr;
        }

        IControlListener* listener = description->getControlListener("TurnTableKnobController");
        if (!listener)
        {
            return nullptr;
        }

        VSTGUI::VST3Editor* editor = (VSTGUI::VST3Editor*)description->getController();
        if (!editor)
        {
            return nullptr;
        }

        TurnTableKnobController* controller = (TurnTableKnobController*)editor->getController();
        if (!controller)
        {
            return nullptr;
        }

        ITimelineControlProcessor* processor = controller->getProcessor();
        if (!processor)
        {
            return nullptr;
        }

        return new TimelineControl(
            CRect(origin, size), timelineBox, filePathBox, holdIndicatorBox, deClickerBox,
            listener, *processor);
    }

    static TimelineControlFactory TimelineControlFactoryInstance;
}
