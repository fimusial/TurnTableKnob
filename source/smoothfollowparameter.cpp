#include "smoothfollowparameter.h"

namespace TTK
{
    SmoothFollowParameter::SmoothFollowParameter(
        unsigned int pid, double value, double stiffness, double damping)
        : parameter(pid),
        value(value),
        stiffness(stiffness),
        damping(damping),
        velocity(0.0)
    {
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
        return value;
    }

    double SmoothFollowParameter::getVelocity()
    {
        return velocity;
    }

    double SmoothFollowParameter::advance()
    {
        double diff = parameter.getValue() - value;
        double acceleration = stiffness * diff - damping * velocity;

        velocity += acceleration;
        value += velocity;

        parameter.advance(1);
        return value;
    }

    void SmoothFollowParameter::reset()
    {
        parameter.setValue(0.0);
        velocity = 0.0;
        value = 0.0;
    }
}