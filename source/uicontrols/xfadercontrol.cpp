#include "xfadercontrol.h"

#include "vstgui/lib/platform/platformfactory.h"
#include "../cids.h"
#include "../consts.h"
#include "../resourcemanager.h"

namespace TTK
{
    XFaderControl::XFaderControl(
        const CRect& viewSize,
        const CRect& rangeBox,
        IControlListener* listener)
        : CControl(viewSize, listener, XFader),
        rangeBox(rangeBox)
    {
        std::string rangeContent = ResourceManager::getFileContent("xfader-range.bmp");
        std::string handleContent = ResourceManager::getFileContent("xfader-handle.bmp");

        const IPlatformFactory& factory = VSTGUI::getPlatformFactory();
        rangeBitmap = VSTGUI::owned(new CBitmap(factory.createBitmapFromMemory(rangeContent.c_str(), (unsigned int)rangeContent.size())));
        handleBitmap = VSTGUI::owned(new CBitmap(factory.createBitmapFromMemory(handleContent.c_str(), (unsigned int)handleContent.size())));

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
