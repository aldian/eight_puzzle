#ifndef BOARDHEURISTIC_H
#define BOARDHEURISTIC_H

#include "Board.h"

class BoardHeuristic
{
    public:
        BoardHeuristic();
        virtual ~BoardHeuristic();

        virtual float calculate(const Board &board, const Board &boardGoal) const = 0;
    protected:
    private:
};

#endif // BOARDHEURISTIC_H
