#ifndef MANHATTANDISTANCE_H
#define MANHATTANDISTANCE_H

#include "BoardHeuristic.h"

class ManhattanDistance: public BoardHeuristic
{
    public:
        static ManhattanDistance &getInstance()
        {
            static ManhattanDistance instance;
            return instance;
        }
        virtual ~ManhattanDistance();

        virtual float calculate(const Board &board, const Board &boardGoal) const;
    protected:
    private:
        ManhattanDistance();
        ManhattanDistance(ManhattanDistance const&);
        void operator=(ManhattanDistance const&);
};

#endif // MANHATTANDISTANCE_H
