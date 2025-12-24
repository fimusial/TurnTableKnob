#pragma once

using namespace std;

namespace TTK
{
    struct LerpFollowParameter
    {
    public:
        LerpFollowParameter();
        void beginChanges(int numSamples, double nextValue);
        void endChanges();
        double at(int sample);

    private:
        int blockSize;
        double last;
        double next;
    };
}