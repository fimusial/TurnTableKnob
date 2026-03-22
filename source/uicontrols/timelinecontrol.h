#pragma once

#include "vstgui/vstgui.h"

#include "itimelinecontrolprocessor.h"
#include "holdcontrol.h"

using namespace VSTGUI;

namespace TTK
{
    class TimelineControl : public CControl
    {
    public:
        TimelineControl(const CRect& size, CRect textBox,
            IControlListener* listener, ITimelineControlProcessor& processor);
        ~TimelineControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        void onMouseMoveEvent(MouseMoveEvent& event) override;
        void onMouseUpEvent(MouseUpEvent& event) override;
        void onMouseWheelEvent(MouseWheelEvent& event) override;
        CBaseObject* newCopy() const override { return new TimelineControl(*this); }

    private:
        HoldControl* holdControl;
        SharedPointer<CVSTGUITimer> timer;
        CRect textBox;
        ITimelineControlProcessor& processor;
        UTF8String filePath;
        std::vector<CPoint> waveform;

        void selectWaveform();
        void readWaveform();
        void readFilePath();
    };
}
