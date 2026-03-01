#include "autoplaycontrol.h"

#include "../cids.h"
#include "../consts.h"

namespace TTK
{
    AutoPlayControl::AutoPlayControl(const CRect& size, IControlListener* listener)
        : CControl(size, listener, AutoPlay)
    {
    }

    AutoPlayControl::~AutoPlayControl()
    {
    }

    void AutoPlayControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();

        // background
        context->setFillColor(BackgroundColor);
        context->drawRect(viewSize, kDrawFilled);

        setDirty(false);
    }

    void AutoPlayControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        event.consumed = true;
    }
}
