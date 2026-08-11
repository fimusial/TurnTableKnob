#pragma once

#include "vstgui/vstgui_uidescription.h"

using namespace VSTGUI;

namespace TTK
{
    class XFaderCurveControlFactory : public ViewCreatorAdapter
    {
    public:
        XFaderCurveControlFactory();
        ~XFaderCurveControlFactory();
        IdStringPtr getViewName() const override;
        IdStringPtr getBaseViewName() const override;
        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override;
    };

    XFaderCurveControlFactory XFaderCurveControlFactoryInstance;
}
