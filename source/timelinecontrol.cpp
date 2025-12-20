#include "timelinecontrol.h"

namespace TTK
{
    TimelineControl::TimelineControl(const CRect& size, ITimelineControlProcessor& processor)
        : CControl(size),
        processor(processor),
        waveform(0)
    {
        AudioSegment32* segment = processor.getSegment();
        string filePath = processor.getFilePath();

        if (segment && !filePath.empty())
        {
            //readFilePathText(filePath);
            readWaveform(segment);
        }
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

        const char* selectorResult = selector->getSelectedFile(0);
        if (!selectorResult)
        {
            selector->forget();
            return;
        }

        AudioSegment32* newSegment = processor.processNewFilePath(selectorResult);
        if (newSegment)
        {
            readWaveform(newSegment);
            invalid();
        }

        selector->forget();
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
