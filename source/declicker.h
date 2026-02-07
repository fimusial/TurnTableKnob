#pragma once

namespace TTK
{
    struct DeClicker
    {
    public:
        DeClicker(double step);
        void open();
        void close();
        double getGain();

    private:
        double step;
        double gain;
    };
}