#include "holdcontrol.h"

#include "../cids.h"
#include "../consts.h"

namespace TTK
{
    HoldControl::HoldControl(const CRect& size, IControlListener* listener)
        : CControl(size, listener, Hold)
    {
    }

    HoldControl::~HoldControl()
    {
    }

    void HoldControl::draw(CDrawContext* context)
    {
        setDirty(false);
    }

    void HoldControl::begin()
    {
        if (isEditing())
        {
            return;
        }

        beginEdit();
        setValue(1.0);
        valueChanged();
    }

    void HoldControl::end()
    {
        if (!isEditing())
        {
            return;
        }

        setValue(0.0);
        valueChanged();
        endEdit();
    }
}
