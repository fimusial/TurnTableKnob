#include "timelinecontrol.h"

namespace TTK
{
    TimelineControl::TimelineControl(const CRect& size, ITimelineControlListener* changeListener)
        : CControl(size), changeListener(changeListener) {}

    void TimelineControl::draw(CDrawContext* context)
    {
        context->setLineWidth(1);
        context->setFillColor(CColor(0, 255, 128, 255));
        context->setFrameColor(CColor(255, 0, 0, 255));
        context->drawRect(getViewSize(), kDrawFilledAndStroked);

        setDirty(false);
    }

    void TimelineControl::onMouseDownEvent(MouseDownEvent& event)
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

        selector->setTitle("Choose an audio file");
        selector->setAllowMultiFileSelection(false);
        selector->setDefaultExtension(CFileExtension("WAVE", "wav"));
        selector->run(this);

        const char* result = selector->getSelectedFile(0);
        if (result && filePath.compare(result) != 0)
        {
            filePath = result;
            if (changeListener)
            {
                changeListener->audioSegmentFilePathChanged(filePath);
            }
        }

        selector->forget();
        event.consumed = true;
    }
}
