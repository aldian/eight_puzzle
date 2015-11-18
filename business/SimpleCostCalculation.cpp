#include "SimpleCostCalculation.h"

SimpleCostCalculation::SimpleCostCalculation()
{
    //ctor
}

SimpleCostCalculation::~SimpleCostCalculation()
{
    //dtor
}

float SimpleCostCalculation::calculate(const Board &board, const Board &boardGoal) const
{
	// Heuristic dengan perhitungan sederhana

	int i, h = 0;

	for( i=0; i < 9; i++ )
	{
		// space tak dihitung
		if( board.puzzle[i] != boardGoal.puzzle[i] )
		{
		    ++h;
		}
	}

	return (float) h;
}
