#pragma once

#include "vstgui/vstgui.h"

#include "itimelinecontrolprocessor.h"

using namespace VSTGUI;

namespace TTK
{
    class HoldControl : public CControl
    {
    public:
        HoldControl(
            const CRect& viewSize,
            IControlListener* listener,
            ITimelineControlProcessor& processor);

        ~HoldControl();
        void draw(CDrawContext* context) override;
        void begin();
        void end();
        CBaseObject* newCopy() const override { return new HoldControl(*this); }

    private:
        ITimelineControlProcessor& processor;
    };
}
