#include "vstgui/lib/platform/iplatformgraphicsdevice.h"
#include "vstgui/lib/platform/iplatformgraphicspath.h"

namespace TTK
{
    void drawPolygon(CDrawContext* context, std::vector<CPoint> waveform, size_t start, size_t end)
    {
        PlatformGraphicsDeviceContextPtr deviceContext = context->getPlatformDeviceContext();
        if (!deviceContext)
        {
            return;
        }

        PlatformGraphicsPathFactoryPtr pathFactory = deviceContext->getGraphicsPathFactory();
        if (!pathFactory)
        {
            return;
        }

        PlatformGraphicsPathPtr path = pathFactory->createPath();
        if (!path)
        {
            return;
        }

        path->beginSubpath(waveform[start]);
        for (size_t i = start + 1; i < end; i++)
        {
            path->addLine(waveform[i]);
        }

        path->finishBuilding();
        deviceContext->drawGraphicsPath(*path, PlatformGraphicsPathDrawMode::Stroked, nullptr);
    }
}