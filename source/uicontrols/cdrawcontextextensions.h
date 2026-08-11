#pragma once

#include "vstgui/lib/platform/iplatformgraphicsdevice.h"
#include "vstgui/lib/platform/iplatformgraphicspath.h"

namespace TTK
{
    inline void drawPolygon(CDrawContext* context, std::vector<CPoint> points, size_t start, size_t end)
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

        path->beginSubpath(points[start]);
        for (size_t i = start + 1; i < end; i++)
        {
            path->addLine(points[i]);
        }

        path->finishBuilding();
        deviceContext->drawGraphicsPath(*path, PlatformGraphicsPathDrawMode::Stroked, nullptr);
    }

    template <typename Function>
    inline void drawFunction(CDrawContext* context, CRect box, Function function, int resolution = 31)
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

        path->beginSubpath(CPoint(box.left, box.bottom));
        for (int i = 1; i <= resolution; i++)
        {
            path->addLine(CPoint(
                box.left + i * box.getWidth() / (double)resolution,
                box.bottom - box.getHeight() * function((double)i / (double)resolution)));
        }

        path->finishBuilding();
        deviceContext->drawGraphicsPath(*path, PlatformGraphicsPathDrawMode::Stroked, nullptr);
    }
}
