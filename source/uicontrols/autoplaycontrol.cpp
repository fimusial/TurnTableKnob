#include "autoplaycontrol.h"

#include "vstgui/lib/platform/platformfactory.h"
#include "../cids.h"
#include "../consts.h"
#include "../resourcemanager.h"

namespace TTK
{
    AutoPlayControl::AutoPlayControl(
        const CRect& viewSize,
        const CRect& rept,
        const CRect& back,
        const CRect& stop,
        const CRect& play,
        IControlListener* listener)
        : CControl(viewSize, listener, AutoPlay),
        rept(rept),
        back(back),
        stop(stop),
        play(play)
    {
        std::string reptButtonContent = ResourceManager::getFileContent("rept-button.bmp");
        std::string backButtonContent = ResourceManager::getFileContent("back-button.bmp");
        std::string stopButtonContent = ResourceManager::getFileContent("stop-button.bmp");
        std::string playButtonContent = ResourceManager::getFileContent("play-button.bmp");

        const IPlatformFactory& factory = VSTGUI::getPlatformFactory();
        reptBitmap = VSTGUI::owned(new CBitmap(factory.createBitmapFromMemory(reptButtonContent.c_str(), (unsigned int)reptButtonContent.size())));
        backBitmap = VSTGUI::owned(new CBitmap(factory.createBitmapFromMemory(backButtonContent.c_str(), (unsigned int)backButtonContent.size())));
        stopBitmap = VSTGUI::owned(new CBitmap(factory.createBitmapFromMemory(stopButtonContent.c_str(), (unsigned int)stopButtonContent.size())));
        playBitmap = VSTGUI::owned(new CBitmap(factory.createBitmapFromMemory(playButtonContent.c_str(), (unsigned int)playButtonContent.size())));
    }

    AutoPlayControl::~AutoPlayControl()
    {
    }

    void AutoPlayControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();
        double currentValue = snapAutoPlayValue(getValue());

        CRect reptOffset = CRect(rept).offset(ShadowOffset, ShadowOffset);
        CRect backOffset = CRect(back).offset(ShadowOffset, ShadowOffset);
        CRect stopOffset = CRect(stop).offset(ShadowOffset, ShadowOffset);
        CRect playOffset = CRect(play).offset(ShadowOffset, ShadowOffset);

        context->setFillColor(ShadowColor);

        // shadows
        context->drawRect(reptOffset, kDrawFilled);
        context->drawRect(backOffset, kDrawFilled);
        context->drawRect(stopOffset, kDrawFilled);
        context->drawRect(playOffset, kDrawFilled);

        // buttons
        context->drawBitmap(reptBitmap, reptPressed(currentValue) ? reptOffset : rept);
        context->drawBitmap(backBitmap, backPressed(currentValue) ? backOffset : back);
        context->drawBitmap(stopBitmap, stopPressed(currentValue) ? stopOffset : stop);
        context->drawBitmap(playBitmap, playPressed(currentValue) ? playOffset : play);

        // outlines
        context->setLineWidth(ThickLine);
        context->setFrameColor(BorderColor);
        context->drawRect(reptPressed(currentValue) ? reptOffset : rept, kDrawStroked);
        context->drawRect(backPressed(currentValue) ? backOffset : back, kDrawStroked);
        context->drawRect(stopPressed(currentValue) ? stopOffset : stop, kDrawStroked);
        context->drawRect(playPressed(currentValue) ? playOffset : play, kDrawStroked);

        setDirty(false);
    }

    void AutoPlayControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        beginEdit();

        double currentValue = snapAutoPlayValue(getValue());

        if (rept.pointInside(event.mousePosition))
        {
            if (currentValue == AP_STOP) setValue(AP_STOP_REPT);
            else if (currentValue == AP_BACK) setValue(AP_BACK_REPT);
            else if (currentValue == AP_PLAY) setValue(AP_PLAY_REPT);
            else if (currentValue == AP_STOP_REPT) setValue(AP_STOP);
            else if (currentValue == AP_BACK_REPT) setValue(AP_BACK);
            else if (currentValue == AP_PLAY_REPT) setValue(AP_PLAY);

            valueChanged();
            invalid();
        }

        if (back.pointInside(event.mousePosition))
        {
            setValue(reptPressed(currentValue) ? AP_BACK_REPT : AP_BACK);

            valueChanged();
            invalid();
        }

        if (stop.pointInside(event.mousePosition))
        {
            setValue(reptPressed(currentValue) ? AP_STOP_REPT : AP_STOP);

            valueChanged();
            invalid();
        }

        if (play.pointInside(event.mousePosition))
        {
            setValue(reptPressed(currentValue) ? AP_PLAY_REPT : AP_PLAY);

            valueChanged();
            invalid();
        }

        endEdit();

        event.consumed = true;
    }

    bool AutoPlayControl::reptPressed(double value)
    {
        return value == AP_STOP_REPT || value == AP_BACK_REPT || value == AP_PLAY_REPT;
    }

    bool AutoPlayControl::backPressed(double value)
    {
        return value == AP_BACK || value == AP_BACK_REPT;
    }

    bool AutoPlayControl::stopPressed(double value)
    {
        return value == AP_STOP || value == AP_STOP_REPT;
    }

    bool AutoPlayControl::playPressed(double value)
    {
        return value == AP_PLAY || value == AP_PLAY_REPT;
    }

    double snapAutoPlayValue(double value)
    {
        // 0.0 - 0.125
        if (AP_STOP <= value && value < AP_BACK)
        {
            return AP_STOP;
        }

        // 0.125 - 0.25
        if (AP_BACK <= value && value < AP_PLAY)
        {
            return AP_BACK;
        }

        // 0.25 - 0.75
        if (AP_PLAY <= value && value < AP_STOP_REPT)
        {
            return AP_PLAY;
        }

        // 0.75 - 0.875
        if (AP_STOP_REPT <= value && value < AP_BACK_REPT)
        {
            return AP_STOP_REPT;
        }

        // 0.875 - 1.0
        if (AP_BACK_REPT <= value && value < AP_PLAY_REPT)
        {
            return AP_BACK_REPT;
        }

        // 1.0
        if (AP_PLAY_REPT <= value)
        {
            return AP_PLAY_REPT;
        }

        return 0.0;
    }
}
