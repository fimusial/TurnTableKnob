#pragma once

#include "vstgui/vstgui_uidescription.h"

#include "itimelinecontrolprocessor.h"

using namespace VSTGUI;

namespace TTK
{
    class TimelineControlFactory : public ViewCreatorAdapter
    {
    public:
        TimelineControlFactory(ITimelineControlProcessor& processor);
        ~TimelineControlFactory();
        IdStringPtr getViewName() const override;
        IdStringPtr getBaseViewName() const override;
        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override;

    private:
        ITimelineControlProcessor& processor;
    };
}