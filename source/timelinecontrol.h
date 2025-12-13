#pragma once

#include "vstgui/vstgui.h"

#include "audiosegment32.h"

using namespace std;
using namespace VSTGUI;

namespace TTK
{
    class ITimelineControlListener
    {
    public:
        virtual void audioSegmentFilePathChanged(string filePath) = 0;
    };

    class TimelineControl : public CControl
    {
    public:
        TimelineControl(const CRect& size, ITimelineControlListener* changeListener);
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        CLASS_METHODS(TimelineControl, CControl);

    private:
        ITimelineControlListener* changeListener = nullptr;
        string filePath = "";
    };
}
