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
        const CColor BackgroundColor = CColor(0x07, 0x07, 0x07);
        const CColor WaveformColor = CColor(0xdd, 0xdd, 0xdd);
        const CColor TextBoxColor = CColor(0x71, 0x71, 0x71);
        const CColor TextColor = CColor(0xff, 0xff, 0xff);
        const CColor PlayheadColor = CColor(0x0b, 0x71, 0x0b);

        SharedPointer<CVSTGUITimer> timer;
        CRect textBox;
        ITimelineControlProcessor& processor;
        UTF8String filePath;
        std::vector<CPoint> waveform;

        void readWaveform();
        void readFilePath();
    };

    static const char* DEFAULT_FILE_PATH = "select a .wav file...";
    static const double SAMPLE_WAVEFORM_RATIO = 32.0;
    static const int SCROLL_SPEED = 8192;
    static const int ZOOM_SPEED = 8192;
    static const int TIMER_DELAY_MS = 16;
}
