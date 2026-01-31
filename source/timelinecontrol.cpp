#include "timelinecontrol.h"

#include "cids.h"

namespace TTK
{
    TimelineControl::TimelineControl(const CRect& size, CRect textBox,
        IControlListener* listener, ITimelineControlProcessor& processor)
        : CControl(size, listener, Playhead),
        textBox(textBox),
        processor(processor),
        filePath(DEFAULT_FILE_PATH),
        waveform(0)
    {
        readWaveform();
        readFilePath();

        timer = makeOwned<CVSTGUITimer>(
            [this](auto*) { invalid(); },
            TIMER_DELAY_MS,
            true);
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

        // TODO: high segmentStart causes lag in audio
        if (waveform.size() > 1)
        {
            double start = (double)processor.getSegmentStart() / SAMPLE_WAVEFORM_RATIO;
            double end = (double)processor.getSegmentEnd() / SAMPLE_WAVEFORM_RATIO;
            CDrawContext::Transform _(*context, CGraphicsTransform()
                .translate(-start, 0.0)
                .scale(viewSize.getWidth() / (end - start), 1.0)
                );

            context->drawPolygon(waveform, kDrawStroked);
        }

        // playhead
        double playheadX = processor.getPlayhead() * viewSize.getWidth();
        CRect playheadLine(playheadX - 1.0, 0.0, playheadX + 1.0, viewSize.getHeight());
        context->setFillColor(PlayheadColor);
        context->drawRect(playheadLine, kDrawFilled);

        // text box
        CRect stringBox = textBox;
        stringBox.inset(4, 4);
        stringBox.offset(0, -1);
        context->setFontColor(TextColor);
        context->setFont(kSystemFont, stringBox.getHeight());
        context->drawString(filePath, stringBox, kRightText);
        context->setLineWidth(1);
        context->setFrameColor(TextBoxColor);
        context->drawRect(textBox, kDrawStroked);

        setDirty(false);
    }

    void TimelineControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        event.consumed = true;

        if (!textBox.pointInside(event.mousePosition))
        {
            if (!isEditing())
            {
                beginEdit();
                setValue(event.mousePosition.x / getViewSize().getWidth());
                valueChanged();
            }

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
            readFilePath();
            invalid();
        }

        selector->forget();
    }

    void TimelineControl::onMouseMoveEvent(MouseMoveEvent& event)
    {
        if (!isEditing())
        {
            return;
        }

        setValue(event.mousePosition.x / getViewSize().getWidth());
        valueChanged();

        event.consumed = true;
    }

    void TimelineControl::onMouseUpEvent(MouseUpEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        if (isEditing())
        {
            endEdit();
        }

        event.consumed = true;
    }

    void TimelineControl::onMouseWheelEvent(MouseWheelEvent& event)
    {
        int direction = event.deltaY > 0.0 ? 1 : -1;

        if (event.modifiers.has(ModifierKey::Control))
        {
            processor.zoomSegment(ZOOM_SPEED * direction);
        }
        else
        {
            processor.scrollSegment(SCROLL_SPEED * direction);
        }

        invalid();
        event.consumed = true;
    }

    void TimelineControl::readWaveform()
    {
        waveform.clear();

        AudioSegment32* segment = processor.getSegment();
        if (!segment)
        {
            return;
        }

        waveform.resize(segment->sampleCount / SAMPLE_WAVEFORM_RATIO);

        int channelCount = (int)segment->channels.size();
        double height = getViewSize().getHeight();
        for (size_t waveformSample = 0; waveformSample < waveform.size(); waveformSample++)
        {
            double sum = 0.0;
            for (int channel = 0; channel < channelCount; channel++)
            {
                sum += segment->channels[channel][waveformSample * SAMPLE_WAVEFORM_RATIO];
            }

            double y = ((sum / channelCount) + 1) / 2 * height;
            waveform[waveformSample] = CPoint(waveformSample, y);
        }
    }

    void TimelineControl::readFilePath()
    {
        std::string newFilePath = processor.getFilePath();
        filePath = newFilePath.empty() ? DEFAULT_FILE_PATH : newFilePath;
    }
}
