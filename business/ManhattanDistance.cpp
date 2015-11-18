#include <cmath>
#include "ManhattanDistance.h"

ManhattanDistance::ManhattanDistance()
{
    //ctor
}

ManhattanDistance::~ManhattanDistance()
{
    //dtor
}

float ManhattanDistance::calculate(const Board &board, const Board &boardGoal) const
{
	// Heuristic dengan manhattan distance

	int i, cx, cy, ax, ay, h = 0;

    // tile i ada di index x berapa?
    // tile 0/blank ada di index x 1, karena di tengah
    // tile 1 ada di index x 0, karena di paling kiri
    // tile 2 ada di index x 1, karena di tengah
    // tile 3 ada di index x 2, karena di paling kanan
    // tile 4 ada di index x 2, karena di paling kanan
    // tile 5 ada di index x 2, karena di paling kanan
    // tile 6 ada di index x 1, karena di tengah
    // tile 7 ada di index x 0, karena di paling kiri
    // tile 8 ada di index x 0, karena di paling kiri
	int tile_x[ 9 ] =
	{
		1, 0, 1, // 1 2 3
		2, 2, 2, // 8 0 4
		1, 0, 0, // 7 6 5
	};

    // tile i ada di index y berapa?
    // tile 0/blank ada di index y 1, karena di tengah
    // tile 1 ada di index y 0, karena di paling atas
    // tile 2 ada di index y 0, karena di paling atas
    // tile 3 ada di index y 0, karena di paling atas
    // tile 4 ada di index y 1, karena di tengah
    // tile 5 ada di index y 2, karena di paling bawah
    // tile 6 ada di index y 2, karena di paling bawah
    // tile 7 ada di index y 2, karena di paling bawah
    // tile 8 ada di index y 1, karena di tengah
	int tile_y[ 9 ] =
	{
		1, 0, 0,
		0, 1, 2,
		2, 2, 1,
	};

	for( i=0; i < 9; i++ )
	{
		// space tak dihitung
		if( board.puzzle[i] == 0 )
		{
			continue;
		}

		// ambil x dan y yang benar dari tile
		cx = tile_x[board.puzzle[i]];
		cy = tile_y[board.puzzle[i]];

		// ambil x dan y yang terjadi sekarang
		ax = i % 3;
		ay = i / 3;

		// tambahkan manhattan distance ke h
		h += abs( cx-ax );
		h += abs( cy-ay );
	}

	return (float) h;
}

