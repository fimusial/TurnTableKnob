#pragma once

#include "vstgui/vstgui.h"

using namespace VSTGUI;

namespace TTK
{
    class XFaderControl : public CControl
    {
    public:
        XFaderControl(
            const CRect& viewSize,
            const CRect& rangeBox,
            IControlListener* listener);

        ~XFaderControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        void onMouseMoveEvent(MouseMoveEvent& event) override;
        void onMouseUpEvent(MouseUpEvent& event) override;
        CBaseObject* newCopy() const override { return new XFaderControl(*this); }

    private:
        CRect rangeBox;

        SharedPointer<CBitmap> rangeBitmap;
    };
}
