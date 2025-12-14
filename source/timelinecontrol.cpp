#include "timelinecontrol.h"

namespace TTK
{
    TimelineControl::TimelineControl(const CRect& size, ITimelineControlProcessor& processor)
        : CControl(size),
        processor(processor),
        filePath(""),
        waveform(0)
    {
    }

    TimelineControl::~TimelineControl()
    {
    }

    void TimelineControl::draw(CDrawContext* context)
    {
        CRect sizeRect = getViewSize();

        // background
        context->setFillColor(backgroundColor);
        context->drawRect(sizeRect, kDrawFilled);

        // waveform
        context->setFrameColor(waveformColor);
        if (waveform.size() > 1)
        {
            context->drawPolygon(waveform, kDrawStroked);
        }
    }

    void TimelineControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        event.consumed = true;

        CNewFileSelector* selector = CNewFileSelector::create(getFrame(), CNewFileSelector::kSelectFile);
        if (!selector)
        {
            return;
        }

        selector->setTitle("Choose an audio file");
        selector->setAllowMultiFileSelection(false);
        selector->setDefaultExtension(CFileExtension("WAVE", "wav"));
        selector->run(this);

        string newFilePath = "";
        const char* selectorResult = selector->getSelectedFile(0);
        if (selectorResult)
        {
            newFilePath = selectorResult;
        }

        selector->forget();

        if (newFilePath.empty() || filePath.compare(newFilePath) == 0)
        {
            return;
        }

        AudioSegment32* newSegment = AudioSegment32::fromFile(newFilePath);
        if (!newSegment)
        {
            return;
        }

        // TODO: remember state when editor is reopened
        readWaveform(newSegment);
        filePath = newFilePath;
        processor.audioSegmentChanged(newSegment);
        invalid();
    }

    void TimelineControl::readWaveform(AudioSegment32* segment)
    {
        waveform.clear();
        waveform.resize(segment->sampleCount / SampleWaveformRatio);

        int channelCount = segment->channels.size();
        double height = getViewSize().getHeight();
        for (size_t waveformSample = 0; waveformSample < waveform.size(); waveformSample++)
        {
            double sum = 0.0;
            for (int channel = 0; channel < channelCount; channel++)
            {
                sum += segment->channels[channel][waveformSample * SampleWaveformRatio];
            }

            double y = ((sum / channelCount) + 1) / 2 * height;
            waveform[waveformSample] = CPoint(waveformSample, y);
        }
    }
}
