#include <cmath>
#include "NilssonSequence.h"

NilssonSequence::NilssonSequence()
{
    //ctor
}

NilssonSequence::~NilssonSequence()
{
    //dtor
}


float NilssonSequence::calculate(const Board &board, const Board &boardGoal) const
{
	// Heuristic dengan nilsson's sequence

	int i, cx, cy, ax, ay, h = 0, s, t;

	// clockwise-nya tile i adalah tile ...
	int correct_follower_to[9] =
	{
		0, 2, 3, // 1 2 3
		4, 5, 6, // 8 0 4
		7, 8, 1, // 7 5 4
	};

	// clockwise-nya index i ada di index nomor brp?
	int clockwise_tile_of[ 9 ] =
	{
		1, 2, 5, // 1 2 3 // 0 1 2
		0,-1, 8, // 8 0 4 // 3 4 5
		3, 6, 7  // 7 6 5 // 6 7 8
	};

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

	s=0;

	// score 1 jika tengah masih salah
	//if( tiles[(BOARD_HEIGHT*BOARD_WIDTH)/2] != nodeGoal.tiles[(BOARD_HEIGHT*BOARD_WIDTH)/2] )
	if (board.puzzle[5] != boardGoal.puzzle[5])
	{
 		s = 1;
	}

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

		// pada tile tengah tak ada nilai s
		if( (ax == (3/2)) && (ay == (3/2)) )
		{
			continue;
		}

		// tambahkankan cost 2 untuk posisi searah jarum jam yang salah
		if( correct_follower_to[ board.puzzle[i] ] != board.puzzle[ clockwise_tile_of[ i ] ] )
		{
			s += 2;
		}


	}

	t = h + (3*s);

	return (float) t;
}
