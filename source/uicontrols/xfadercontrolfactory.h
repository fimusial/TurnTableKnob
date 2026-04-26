#pragma once

#include "vstgui/vstgui_uidescription.h"

using namespace VSTGUI;

namespace TTK
{
    class XFaderControlFactory : public ViewCreatorAdapter
    {
    public:
        XFaderControlFactory();
        ~XFaderControlFactory();
        IdStringPtr getViewName() const override;
        IdStringPtr getBaseViewName() const override;
        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override;
    };

    XFaderControlFactory XFaderControlFactoryInstance;
}
