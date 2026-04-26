#include "xfadercontrolfactory.h"

#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "xfadercontrol.h"

namespace TTK
{
    XFaderControlFactory::XFaderControlFactory()
    {
        UIViewFactory::registerViewCreator(*this);
    }

    XFaderControlFactory::~XFaderControlFactory()
    {
        UIViewFactory::unregisterViewCreator(*this);
    }

    IdStringPtr XFaderControlFactory::getViewName() const
    {
        return "XFaderControl";
    }

    IdStringPtr XFaderControlFactory::getBaseViewName() const
    {
        return UIViewCreator::kCControl;
    }

    CView* XFaderControlFactory::create(
        const UIAttributes& attributes, const IUIDescription* description) const
    {
        CPoint origin;
        if (!attributes.getPointAttribute("origin", origin))
        {
            return nullptr;
        }

        CPoint size;
        if (!attributes.getPointAttribute("size", size))
        {
            return nullptr;
        }

        CRect rangeBox;
        if (!attributes.getRectAttribute("rangeBox", rangeBox))
        {
            return nullptr;
        }

        return new XFaderControl(
            CRect(origin, size), rangeBox,
            description->getControlListener("TurnTableKnobController"));
    }
}
