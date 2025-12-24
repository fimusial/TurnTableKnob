#include "timelinecontrol.h"

namespace TTK
{
    TimelineControl::TimelineControl(const CRect& size, CRect textBox,
        IControlListener* listener, ITimelineControlProcessor& processor)
        : CControl(size, listener, Playhead),
        textBox(textBox),
        processor(processor),
        uiFilePath(DefaultUiFilePath),
        waveform(0)
    {
        processor.setTimelineRange(size.getWidth() * SampleWaveformRatio);
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
        if (textBox.pointInside(event.mousePosition))
        {
            onTextBoxMouseDownEvent(event);
        }
        else
        {
            onTimelineMouseDownEvent(event);
        }

        event.consumed = true;
    }

    void TimelineControl::onMouseMoveEvent(MouseMoveEvent& event)
    {
        onTimelineMouseMoveEvent(event);
        event.consumed = true;
    }

    void TimelineControl::onMouseUpEvent(MouseUpEvent& event)
    {
        onTimelineMouseUpEvent(event);
        event.consumed = true;
    }

    void TimelineControl::onMouseWheelEvent(MouseWheelEvent& event)
    {
        // TODO: move waveform window
        event.consumed = true;
    }

    void TimelineControl::onTextBoxMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        CNewFileSelector* selector = CNewFileSelector::create(getFrame(), CNewFileSelector::kSelectFile);
        if (!selector)
        {
            return;
        }

        selector->setTitle("Select an audio file");
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

    void TimelineControl::onTimelineMouseDownEvent(MouseDownEvent& event)
    {
        beginEdit();
    }

    void TimelineControl::onTimelineMouseMoveEvent(MouseMoveEvent& event)
    {
        if (!isEditing())
        {
            return;
        }

        // TODO: end edit on mouse leave
        // TODO: CKnob has better resolution somehow
        setValue(event.mousePosition.x / getViewSize().getWidth());
        valueChanged();
    }

    void TimelineControl::onTimelineMouseUpEvent(MouseUpEvent& event)
    {
        endEdit();
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
