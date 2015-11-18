#ifndef SIMPLECOSTCALCULATION_H
#define SIMPLECOSTCALCULATION_H

#include "BoardHeuristic.h"

class SimpleCostCalculation: public BoardHeuristic
{
    public:
        static SimpleCostCalculation &getInstance()
        {
            static SimpleCostCalculation instance;
            return instance;
        }
        virtual ~SimpleCostCalculation();

        virtual float calculate(const Board &board, const Board &boardGoal) const;
    protected:
    private:
        SimpleCostCalculation();
        SimpleCostCalculation(SimpleCostCalculation const&);
        void operator=(SimpleCostCalculation const&);


};

#endif // SIMPLECOSTCALCULATION_H
