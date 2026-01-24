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
        CLASS_METHODS(TimelineControl, CControl);

    private:
        const CColor BackgroundColor = CColor(0x07, 0x07, 0x07);
        const CColor WaveformColor = CColor(0xdd, 0xdd, 0xdd);
        const CColor TextBoxColor = CColor(0x71, 0x71, 0x71);
        const CColor TextColor = CColor(0xff, 0xff, 0xff);

        CRect textBox;
        ITimelineControlProcessor& processor;
        UTF8String uiFilePath;
        std::vector<CPoint> waveform;

        void readWaveform();
        void readUiFilePath();
    };

    static const char* DEFAULT_UI_FILE_PATH = "select a .wav file...";
    static const double SAMPLE_WAVEFORM_RATIO = 128.0;
    static const int SCROLL_SPEED = 10240.0;
    static const int ZOOM_SPEED = 10240.0;
}
