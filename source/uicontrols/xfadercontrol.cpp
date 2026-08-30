#include "xfadercontrol.h"

#include "../cids.h"
#include "../consts.h"

namespace TTK
{
    XFaderControl::XFaderControl(
        const CRect& viewSize,
        const CRect& rangeBox,
        IControlListener* listener)
        : CControl(viewSize, listener, XFader),
        rangeBox(rangeBox)
    {
        // TODO: real bitmaps, move to consts together with uidesc
        rangeBitmap = VSTGUI::owned(new CBitmap("xfader-range.bmp"));
        handleBitmap = VSTGUI::owned(new CBitmap("xfader-handle.bmp"));

        handleBitmapSize = handleBitmap.get()->getSize();
        insetRangeBox = CRect(rangeBox).inset(0, (rangeBox.getHeight() - handleBitmapSize.y) / 2.0);
        insetRangeBox.setWidth(insetRangeBox.getWidth() - handleBitmapSize.x);
    }

    XFaderControl::~XFaderControl()
    {
    }

    void XFaderControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();

        // range
        context->setFillColor(ShadowColor);
        context->drawRect(CRect(rangeBox).offset(5, 5), kDrawFilled);
        context->drawBitmap(rangeBitmap, rangeBox);

        // handle
        CRect handleBox(insetRangeBox.getTopLeft().offset(getValue() * insetRangeBox.getWidth(), 0), handleBitmapSize);
        context->drawBitmap(handleBitmap, handleBox);

        setDirty(false);
    }

    void XFaderControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        if (!rangeBox.pointInside(event.mousePosition))
        {
            return;
        }

        if (isEditing())
        {
            return;
        }

        beginEdit();
        setValue((event.mousePosition.x - insetRangeBox.left - 0.5 * handleBitmapSize.x) / insetRangeBox.getWidth());
        valueChanged();
        event.consumed = true;
    }

    void XFaderControl::onMouseMoveEvent(MouseMoveEvent& event)
    {
        if (!isEditing())
        {
            return;
        }

        setValue((event.mousePosition.x - insetRangeBox.left - 0.5 * handleBitmapSize.x) / insetRangeBox.getWidth());
        valueChanged();
        event.consumed = true;
    }

    void XFaderControl::onMouseUpEvent(MouseUpEvent& event)
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
