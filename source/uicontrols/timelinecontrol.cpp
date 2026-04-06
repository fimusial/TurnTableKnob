#include "timelinecontrol.h"

#include "../cids.h"
#include "../consts.h"
#include "cdrawcontextextensions.h"

namespace TTK
{
    TimelineControl::TimelineControl(
        const CRect& viewSize,
        const CRect& filePathBox,
        const CRect& holdIndicatorBox,
        const CRect& deClickerBox,
        IControlListener* listener,
        ITimelineControlProcessor& processor)
        : CControl(viewSize, listener, Playhead),
        filePathBox(filePathBox),
        holdIndicatorBox(holdIndicatorBox),
        deClickerBox(deClickerBox),
        processor(processor),
        filePath(DEFAULT_FILE_PATH),
        waveform(0)
    {
        holdControl = new HoldControl(viewSize, listener, processor);
        listener->controlTagDidChange(holdControl);

        readWaveform();
        readFilePath();

        timer = makeOwned<CVSTGUITimer>(
            [this](auto*) { invalid(); },
            TIMER_DELAY_MS,
            true);
    }

    TimelineControl::~TimelineControl()
    {
        listener->controlTagWillChange(holdControl);
        delete holdControl;
    }

    void TimelineControl::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();

        // background
        context->setLineWidth(1.0);
        context->setFrameColor(BorderColor);
        context->setFillColor(BackgroundColor);
        context->drawRect(viewSize, kDrawFilledAndStroked);

        // waveform
        if (waveform.size() > 1)
        {
            double start = (double)processor.getWindowStart() / SAMPLE_WAVEFORM_RATIO;
            double end = (double)processor.getWindowEnd() / SAMPLE_WAVEFORM_RATIO;

            CDrawContext::Transform _(*context, CGraphicsTransform()
                .translate(-start, 0.0)
                .scale(viewSize.getWidth() / (end - start), 1.0));

            context->setLineWidth(2.0);
            context->setFrameColor(PrimaryColor);
            drawPolygon(context, waveform, (size_t)start, (size_t)end);
        }

        // playhead bar
        double playhead = processor.getPlayheadValue();
        if (0.0 < playhead && playhead < 1.0)
        {
            playhead *= viewSize.getWidth();
            CRect playheadBar(playhead - 1.0, 0.0, playhead + 1.0, viewSize.getHeight());
            context->setFillColor(SecondaryColor);
            context->drawRect(playheadBar, kDrawFilled);
        }

        // file path box
        CRect filePathStringBox = filePathBox;
        filePathStringBox.inset(4, 4);
        filePathStringBox.offset(0, -1);
        context->setFontColor(TextColor);
        context->setFont(kSystemFont, filePathStringBox.getHeight());
        context->drawString(filePath, filePathStringBox, kRightText);
        context->setLineWidth(1.0);
        context->setFrameColor(BorderColor);
        context->drawRect(filePathBox, kDrawStroked);

        // hold indicator
        if (processor.getHoldValue())
        {
            CRect holdIndicatorEllipseBox = holdIndicatorBox;
            holdIndicatorEllipseBox.setWidth(holdIndicatorEllipseBox.getHeight());
            holdIndicatorEllipseBox.inset(4, 4);
            CRect holdIndicatorStringBox = holdIndicatorBox;
            holdIndicatorStringBox.inset(4, 4);
            holdIndicatorStringBox.offset(0, -1);
            context->setFontColor(TextColor);
            context->setFont(kSystemFont, holdIndicatorStringBox.getHeight());
            context->drawString("HOLD", holdIndicatorStringBox, kRightText);
            context->setFillColor(SecondaryColor);
            context->drawEllipse(holdIndicatorEllipseBox, kDrawFilled);
            context->setLineWidth(1.0);
            context->setFrameColor(BorderColor);
            context->drawRect(holdIndicatorBox, kDrawStroked);
        }

        // de clicker
        CRect deClickerStringBox = deClickerBox;
        deClickerStringBox.inset(4, 4);
        deClickerStringBox.offset(0, -1);
        CRect deClickerBarBox = deClickerBox;
        deClickerBarBox.inset(4, 4);
        deClickerBarBox.right = deClickerBarBox.left + processor.getDeClickerValue() * deClickerBox.getWidth() * 0.5;
        context->setFontColor(TextColor);
        context->setFont(kSystemFont, deClickerStringBox.getHeight());
        context->drawString("DC", deClickerStringBox, kRightText);
        context->setLineWidth(1.0);
        context->setFrameColor(BorderColor);
        context->drawRect(deClickerBox, kDrawStroked);
        context->setFillColor(BackgroundColor);
        context->drawRect(deClickerBarBox, kDrawFilledAndStroked);

        setDirty(false);
    }

    void TimelineControl::onMouseDownEvent(MouseDownEvent& event)
    {
        if (!event.buttonState.isLeft())
        {
            return;
        }

        event.consumed = true;

        if (filePathBox.pointInside(event.mousePosition))
        {
            selectWaveform();
            return;
        }

        if (isEditing())
        {
            return;
        }

        beginEdit();
        holdControl->begin();
        setValue(event.mousePosition.x / getViewSize().getWidth());
        processor.resetPlayhead(getValue());
        valueChanged();
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
            holdControl->end();
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

    void TimelineControl::selectWaveform()
    {
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
