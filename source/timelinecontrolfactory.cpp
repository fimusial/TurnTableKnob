#include "timelinecontrolfactory.h"

namespace TTK
{
    TimelineControlFactory::TimelineControlFactory(ITimelineControlListener* changeListener)
        : changeListener(changeListener)
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
        CRect size(CPoint(45, 45), CPoint(400, 150));
        return new TimelineControl(size, changeListener);
    }
}
