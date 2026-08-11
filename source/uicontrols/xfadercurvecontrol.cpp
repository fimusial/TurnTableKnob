#include "xfadercurvecontrol.h"

#include "../cids.h"
#include "../consts.h"
#include "cdrawcontextextensions.h"

namespace TTK
{
    XFaderCurveControl::XFaderCurveControl(
        const CRect& viewSize,
        const CRect& curveBox,
        IControlListener* listener)
        : CControl(viewSize, listener, XFaderCurve),
        curveBox(curveBox)
    {
        insetCurveBox = CRect(curveBox).inset(5, 5);
    }

    XFaderCurveControl::~XFaderCurveControl()
    {
    }

    void XFaderCurveControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();

        // button
        context->setFillColor(ShadowColor);
        context->drawRect(CRect(curveBox).offset(5, 5), kDrawFilled);
        context->setFillColor(BackgroundColor);
        context->drawRect(CRect(curveBox), kDrawFilled);
        context->setFillColor(BackgroundColor);
        context->drawRect(insetCurveBox, kDrawFilled);

        // grid
        context->setLineWidth(1.0);
        context->setFrameColor(ThinBorderColor);
        context->setLineStyle(kLineSolid);
        context->drawRect(insetCurveBox, kDrawStroked);
        context->drawLine(insetCurveBox.getLeftCenter(), insetCurveBox.getRightCenter());
        context->drawLine(insetCurveBox.getTopCenter(), insetCurveBox.getBottomCenter());

        // curve
        context->setLineWidth(2.0);
        context->setFrameColor(MainColor);
        drawFunction(context, insetCurveBox, [this](double x) { return getXFaderGain(x, getValue()); });

        setDirty(false);
    }

    void XFaderCurveControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        if (!curveBox.pointInside(event.mousePosition))
        {
            return;
        }

        if (isEditing())
        {
            return;
        }

        beginEdit();
        lastMousePosition = event.mousePosition;
        event.consumed = true;
    }

    void XFaderCurveControl::onMouseMoveEvent(MouseMoveEvent& event)
    {
        if (!isEditing())
        {
            return;
        }

        double delta = lastMousePosition.y - event.mousePosition.y;
        setValue(getValue() + delta * 0.005);
        valueChanged();

        lastMousePosition = event.mousePosition;
        event.consumed = true;
    }

    void XFaderCurveControl::onMouseUpEvent(MouseUpEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        if (isEditing())
        {
            endEdit();
        }

        event.consumed = true;
    }
}
