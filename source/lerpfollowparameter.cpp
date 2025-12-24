#include "lerpfollowparameter.h"

namespace TTK
{
    LerpFollowParameter::LerpFollowParameter()
        : blockSize(0),
        last(-1.0),
        next(-1.0)
    {
    }

    void LerpFollowParameter::beginChanges(int nextBlockSize, double nextValue)
    {
        blockSize = nextBlockSize;
        last = next;
        next = nextValue;
    }

    void LerpFollowParameter::endChanges()
    {
        last = -1.0;
    }

    double LerpFollowParameter::at(int sample)
    {
        if (last < 0.0 || next < 0.0 || blockSize <= 0)
        {
            return 0.0;
        }

        return last + ((double)sample / (double)blockSize) * (next - last);
    }
}