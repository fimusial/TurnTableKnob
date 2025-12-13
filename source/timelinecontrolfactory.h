#pragma once

#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

#include "timelinecontrol.h"

using namespace VSTGUI;

namespace TTK
{
    class TimelineControlFactory : public ViewCreatorAdapter
    {
    public:
        TimelineControlFactory(ITimelineControlListener* changeListener);
        ~TimelineControlFactory();
        IdStringPtr getViewName() const override;
        IdStringPtr getBaseViewName() const override;
        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override;

    private:
        ITimelineControlListener* changeListener = nullptr;
    };
}