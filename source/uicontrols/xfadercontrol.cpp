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
        context->setFillColor(PrimaryColor);
        context->drawBitmap(rangeBitmap, rangeBox);

        setDirty(false);
    }

    void XFaderControl::onMouseDownEvent(MouseDownEvent& event)
    {
    }

    void XFaderControl::onMouseMoveEvent(MouseMoveEvent& event)
    {
    }

    void XFaderControl::onMouseUpEvent(MouseUpEvent& event)
    {
    }
}
