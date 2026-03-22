#include "autoplaycontrolfactory.h"

#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "autoplaycontrol.h"

namespace TTK
{
    AutoPlayControlFactory::AutoPlayControlFactory()
    {
        UIViewFactory::registerViewCreator(*this);
    }

    AutoPlayControlFactory::~AutoPlayControlFactory()
    {
        UIViewFactory::unregisterViewCreator(*this);
    }

    IdStringPtr AutoPlayControlFactory::getViewName() const
    {
        return "AutoPlayControl";
    }

    IdStringPtr AutoPlayControlFactory::getBaseViewName() const
    {
        return UIViewCreator::kCControl;
    }

    CView* AutoPlayControlFactory::create(
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

        CRect rept;
        if (!attributes.getRectAttribute("rept", rept))
        {
            return nullptr;
        }

        CRect back;
        if (!attributes.getRectAttribute("back", back))
        {
            return nullptr;
        }

        CRect stop;
        if (!attributes.getRectAttribute("stop", stop))
        {
            return nullptr;
        }

        CRect play;
        if (!attributes.getRectAttribute("play", play))
        {
            return nullptr;
        }

        return new AutoPlayControl(
            CRect(origin, size), rept, back, stop, play,
            description->getControlListener("TurnTableKnobController"));
    }
}
