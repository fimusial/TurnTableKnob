#pragma once

#include "vstgui/vstgui.h"

using namespace VSTGUI;

namespace TTK
{
    class AutoPlayControl : public CControl
    {
    public:
        AutoPlayControl(const CRect& size, IControlListener* listener);
        ~AutoPlayControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        CBaseObject* newCopy() const override { return new AutoPlayControl(*this); }
    };
}
