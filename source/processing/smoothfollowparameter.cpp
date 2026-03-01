#include "smoothfollowparameter.h"

namespace TTK
{
    SmoothFollowParameter::SmoothFollowParameter(
        unsigned int pid, double value, double stiffness, double damping)
        : parameter(pid),
        value(value),
        stiffness(stiffness),
        damping(damping),
        velocity(0.0),
        acceleration(0.0)
    {
    }

    void SmoothFollowParameter::reset(double newValue)
    {
        parameter.setValue(newValue);
        value = newValue;
        velocity = 0.0;
        acceleration = 0.0;
    }

    void SmoothFollowParameter::beginChanges(IParamValueQueue* queue)
    {
        parameter.beginChanges(queue);
    }

    void SmoothFollowParameter::endChanges()
    {
        parameter.endChanges();
    }

    double SmoothFollowParameter::getValue()
    {
        if (value < 0.0)
        {
            return 0.0;
        }

        if (value > 1.0)
        {
            return 1.0;
        }

        return value;
    }

    double SmoothFollowParameter::getAcceleration()
    {
        return acceleration;
    }

    double SmoothFollowParameter::advance()
    {
        double diff = parameter.getValue() - value;
        acceleration = stiffness * diff - damping * velocity;

        velocity += acceleration;
        value += velocity;

        parameter.advance(1);
        return value;
    }
}
