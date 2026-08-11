#include "xfadercurvecontrolfactory.h"

#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "xfadercurvecontrol.h"

namespace TTK
{
    XFaderCurveControlFactory::XFaderCurveControlFactory()
    {
        UIViewFactory::registerViewCreator(*this);
    }

    XFaderCurveControlFactory::~XFaderCurveControlFactory()
    {
        UIViewFactory::unregisterViewCreator(*this);
    }

    IdStringPtr XFaderCurveControlFactory::getViewName() const
    {
        return "XFaderCurveControl";
    }

    IdStringPtr XFaderCurveControlFactory::getBaseViewName() const
    {
        return UIViewCreator::kCControl;
    }

    CView* XFaderCurveControlFactory::create(
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

        CRect curveBox;
        if (!attributes.getRectAttribute("curveBox", curveBox))
        {
            return nullptr;
        }

        return new XFaderCurveControl(
            CRect(origin, size), curveBox,
            description->getControlListener("TurnTableKnobController"));
    }
}
