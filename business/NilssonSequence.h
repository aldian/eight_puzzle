#ifndef NILSSONSEQUENCE_H
#define NILSSONSEQUENCE_H

#include "BoardHeuristic.h"

class NilssonSequence: public BoardHeuristic
{
    public:
        static NilssonSequence &getInstance()
        {
            static NilssonSequence instance;
            return instance;
        }
        virtual ~NilssonSequence();

        virtual float calculate(const Board &board, const Board &boardGoal) const;
    protected:
    private:
        NilssonSequence();
        NilssonSequence(NilssonSequence const&);
        void operator=(NilssonSequence const&);
};

#endif // NILSSONSEQUENCE_H
