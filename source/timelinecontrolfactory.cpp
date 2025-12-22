#include "timelinecontrolfactory.h"

namespace TTK
{
    TimelineControlFactory::TimelineControlFactory(ITimelineControlProcessor& processor)
        : processor(processor)
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

        CPoint textBoxSize;
        if (!attributes.getPointAttribute("textbox-size", textBoxSize))
        {
            return nullptr;
        }

        return new TimelineControl(
            CRect(origin, size),
            CRect(origin, textBoxSize),
            description->getControlListener("TurnTableKnobController"),
            processor);
    }
}
