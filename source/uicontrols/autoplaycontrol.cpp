#include "autoplaycontrol.h"

#include "../cids.h"
#include "../consts.h"

namespace TTK
{
    AutoPlayControl::AutoPlayControl(const CRect& size, IControlListener* listener)
        : CControl(size, listener, AutoPlay)
    {
        // TODO: real bitmaps, move to consts together with uidesc
        reptBitmap = VSTGUI::owned(new CBitmap("rept-button.bmp"));
        backBitmap = VSTGUI::owned(new CBitmap("back-button.bmp"));
        stopBitmap = VSTGUI::owned(new CBitmap("stop-button.bmp"));
        playBitmap = VSTGUI::owned(new CBitmap("play-button.bmp"));
    }

    AutoPlayControl::~AutoPlayControl()
    {
    }

    void AutoPlayControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();

        context->setFillColor(ShadowColor);

        if (!reptPressed()) context->drawRect(CRect(reptRect).offset(5, 5), kDrawFilled);
        if (!backPressed()) context->drawRect(CRect(backRect).offset(5, 5), kDrawFilled);
        if (!stopPressed()) context->drawRect(CRect(stopRect).offset(5, 5), kDrawFilled);
        if (!playPressed()) context->drawRect(CRect(playRect).offset(5, 5), kDrawFilled);

        context->drawBitmap(reptBitmap, reptPressed() ? CRect(reptRect).offset(5, 5) : reptRect);
        context->drawBitmap(backBitmap, backPressed() ? CRect(backRect).offset(5, 5) : backRect);
        context->drawBitmap(stopBitmap, stopPressed() ? CRect(stopRect).offset(5, 5) : stopRect);
        context->drawBitmap(playBitmap, playPressed() ? CRect(playRect).offset(5, 5) : playRect);

        setDirty(false);
    }

    void AutoPlayControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        beginEdit();

        if (reptRect.pointInside(event.mousePosition))
        {
            if (value == AP_STOP) setValue(AP_STOP_REPT);
            else if (value == AP_BACK) setValue(AP_BACK_REPT);
            else if (value == AP_PLAY) setValue(AP_PLAY_REPT);
            else if (value == AP_STOP_REPT) setValue(AP_STOP);
            else if (value == AP_BACK_REPT) setValue(AP_BACK);
            else if (value == AP_PLAY_REPT) setValue(AP_PLAY);

            valueChanged();
            invalid();
        }

        if (backRect.pointInside(event.mousePosition))
        {
            setValue(reptPressed() ? AP_BACK_REPT : AP_BACK);

            valueChanged();
            invalid();
        }

        if (stopRect.pointInside(event.mousePosition))
        {
            setValue(reptPressed() ? AP_STOP_REPT : AP_STOP);

            valueChanged();
            invalid();
        }

        if (playRect.pointInside(event.mousePosition))
        {
            setValue(reptPressed() ? AP_PLAY_REPT : AP_PLAY);

            valueChanged();
            invalid();
        }

        endEdit();

        event.consumed = true;
    }

    bool AutoPlayControl::reptPressed()
    {
        return value == AP_STOP_REPT || value == AP_BACK_REPT || value == AP_PLAY_REPT;
    }

    bool AutoPlayControl::backPressed()
    {
        return value == AP_BACK || value == AP_BACK_REPT;
    }

    bool AutoPlayControl::stopPressed()
    {
        return value == AP_STOP || value == AP_STOP_REPT;
    }

    bool AutoPlayControl::playPressed()
    {
        return value == AP_PLAY || value == AP_PLAY_REPT;
    }
}
