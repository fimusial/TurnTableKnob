#pragma once

#include "vstgui/vstgui.h"

using namespace VSTGUI;

namespace TTK
{
    class XFaderCurveControl : public CControl
    {
    public:
        XFaderCurveControl(
            const CRect& viewSize,
            const CRect& curveBox,
            IControlListener* listener);

        ~XFaderCurveControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        void onMouseMoveEvent(MouseMoveEvent& event) override;
        void onMouseUpEvent(MouseUpEvent& event) override;
        CBaseObject* newCopy() const override { return new XFaderCurveControl(*this); }

    private:
        CRect curveBox;
        CRect insetCurveBox;
        CPoint lastMousePosition;
    };

    static double getXFaderGain(double xFader, double xFaderCurve)
    {
        return max(0.0, min(xFader * (16.0 * xFaderCurve + 1), 1.0));
    }
}
