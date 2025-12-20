#pragma once

#include "vstgui/vstgui.h"

#include "audiosegment32.h"

using namespace std;
using namespace VSTGUI;

namespace TTK
{
    class ITimelineControlProcessor
    {
    public:
        virtual AudioSegment32* processNewFilePath(string newFilePath) = 0;
        virtual AudioSegment32* getSegment() = 0;
        virtual string getFilePath() = 0;
    };

    class TimelineControl : public CControl
    {
    public:
        TimelineControl(const CRect& size, CRect textBox, ITimelineControlProcessor& processor);
        ~TimelineControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        CLASS_METHODS(TimelineControl, CControl);

    private:
        const CColor BackgroundColor = CColor(0x07, 0x07, 0x07);
        const CColor WaveformColor = CColor(0xdd, 0xdd, 0xdd);
        const CColor TextBoxColor = CColor(0x71, 0x71, 0x71);
        const CColor TextColor = CColor(0xff, 0xff, 0xff);
        const char* DefaultUiFilePath = "select a .wav file...";
        const double SampleWaveformRatio = 64.0;

        CRect textBox;
        ITimelineControlProcessor& processor;
        UTF8String uiFilePath;
        vector<CPoint> waveform;

        void readWaveform();
        void readUiFilePath();
    };
}
