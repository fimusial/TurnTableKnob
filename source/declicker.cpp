#include "declicker.h"

#include "algorithm"

namespace TTK
{
    DeClicker::DeClicker(double step)
        : step(step),
        gain(0.0)
    {
    }

    void DeClicker::up()
    {
        gain = std::max(0.0, std::min(1.0, gain + step));
    }

    void DeClicker::down()
    {
        gain = std::max(0.0, std::min(1.0, gain - step));
    }

    double DeClicker::getGain()
    {
        return gain;
    }
}