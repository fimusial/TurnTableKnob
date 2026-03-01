#pragma once

#include "vstgui/vstgui.h"

#include "itimelinecontrolprocessor.h"

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
        SharedPointer<CVSTGUITimer> timer;
        CRect textBox;
        ITimelineControlProcessor& processor;
        UTF8String filePath;
        std::vector<CPoint> waveform;

        void readWaveform();
        void readFilePath();
    };

    static const CColor BackgroundColor = CColor(0x17, 0x17, 0x17);
    static const CColor BorderColor = CColor(0x03, 0x2e, 0x03);
    static const CColor TextColor = CColor(0xff, 0xff, 0xff);
    static const CColor PrimaryColor = CColor(0xdd, 0xdd, 0xdd);
    static const CColor SecondaryColor = CColor(0x0b, 0x71, 0x0b);

    static const char* DEFAULT_FILE_PATH = "select a .wav file...";
    static const double SAMPLE_WAVEFORM_RATIO = 128.0;
    static const int SCROLL_SPEED = 4096;
    static const int ZOOM_SPEED = 4096;
    static const int TIMER_DELAY_MS = 16;
}
