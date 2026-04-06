#pragma once

#include "vstgui/vstgui.h"

using namespace VSTGUI;

namespace TTK
{
    class AutoPlayControl : public CControl
    {
    public:
        AutoPlayControl(
            const CRect& viewSize,
            const CRect& rept,
            const CRect& back,
            const CRect& stop,
            const CRect& play,
            IControlListener* listener);

        ~AutoPlayControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        CBaseObject* newCopy() const override { return new AutoPlayControl(*this); }

    private:
        CRect rept;
        CRect back;
        CRect stop;
        CRect play;

        SharedPointer<CBitmap> reptBitmap;
        SharedPointer<CBitmap> backBitmap;
        SharedPointer<CBitmap> stopBitmap;
        SharedPointer<CBitmap> playBitmap;

        bool reptPressed(double value);
        bool backPressed(double value);
        bool stopPressed(double value);
        bool playPressed(double value);
    };

    double snapAutoPlayValue(double value);
}
