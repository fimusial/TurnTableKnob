#pragma once

#include "vstgui/vstgui.h"

using namespace VSTGUI;

namespace TTK
{
    class AutoPlayControl : public CControl
    {
    public:
        AutoPlayControl(const CRect& size, IControlListener* listener);
        ~AutoPlayControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        CBaseObject* newCopy() const override { return new AutoPlayControl(*this); }

    private:
        SharedPointer<CBitmap> reptBitmap;
        SharedPointer<CBitmap> backBitmap;
        SharedPointer<CBitmap> stopBitmap;
        SharedPointer<CBitmap> playBitmap;

        // TODO: move to uidesc json
        CRect reptRect = CRect(10, 210, 90, 290);
        CRect backRect = CRect(110, 210, 190, 290);
        CRect stopRect = CRect(210, 210, 290, 290);
        CRect playRect = CRect(310, 210, 390, 290);

        bool reptPressed();
        bool backPressed();
        bool stopPressed();
        bool playPressed();
    };
}
