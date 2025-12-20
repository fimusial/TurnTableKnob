#include "timelinecontrol.h"

namespace TTK
{
    TimelineControl::TimelineControl(const CRect& size, CRect textBox, ITimelineControlProcessor& processor)
        : CControl(size),
        textBox(textBox),
        processor(processor),
        uiFilePath(DefaultUiFilePath),
        waveform(0)
    {
        readWaveform();
        readUiFilePath();
    }

    TimelineControl::~TimelineControl()
    {
    }

    void TimelineControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();

        // background
        context->setFillColor(BackgroundColor);
        context->drawRect(viewSize, kDrawFilled);

        // waveform
        context->setFrameColor(WaveformColor);
        if (waveform.size() > 1)
        {
            context->drawPolygon(waveform, kDrawStroked);
        }

        // text box
        CRect stringBox = textBox;
        stringBox.inset(4, 4);
        stringBox.offset(0, -1);
        context->setFontColor(TextColor);
        context->setFont(kSystemFont, stringBox.getHeight());
        context->drawString(uiFilePath, stringBox, kRightText);
        context->setLineWidth(1);
        context->setFrameColor(TextBoxColor);
        context->drawRect(textBox, kDrawStroked);
    }

    void TimelineControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        if (!textBox.pointInside(event.mousePosition))
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

        if (processor.processNewFilePath(selectorResult))
        {
            readWaveform();
            readUiFilePath();
            invalid();
        }

        selector->forget();
    }

    void TimelineControl::readWaveform()
    {
        waveform.clear();

        AudioSegment32* segment = processor.getSegment();
        if (!segment)
        {
            return;
        }

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

    void TimelineControl::readUiFilePath()
    {
        string filePath = processor.getFilePath();
        uiFilePath = filePath.empty() ? DefaultUiFilePath : filePath;
    }
}
