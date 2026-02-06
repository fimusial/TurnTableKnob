#pragma once

namespace TTK
{
    struct DeClicker
    {
    public:
        DeClicker(double step);
        void up();
        void down();
        double getGain();

    private:
        double step;
        double gain;
    };
}