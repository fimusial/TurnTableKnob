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
        virtual void audioSegmentChanged(AudioSegment32* newSegment) = 0;
    };

    class TimelineControl : public CControl
    {
    public:
        TimelineControl(const CRect& size, ITimelineControlProcessor& processor);
        ~TimelineControl();
        void draw(CDrawContext* context) override;
        void onMouseDownEvent(MouseDownEvent& event) override;
        CLASS_METHODS(TimelineControl, CControl);

    private:
        const CColor backgroundColor = CColor(0x07, 0x07, 0x07);
        const CColor waveformColor = CColor(0xcc, 0x00, 0xcc);
        const double SampleWaveformRatio = 64.0;

        ITimelineControlProcessor& processor;
        string filePath;
        vector<CPoint> waveform;

        void readWaveform(AudioSegment32* segment);
    };
}
