#ifndef BOARD_H
#define BOARD_H

#include <tchar.h>
#include <string>
#include "search.h"

class BoardHeuristic;

class Board
{
    public:
        Board();
        Board(const int puzzle[9]);
        Board(const Board &board);
        virtual ~Board();
        Board & operator=(const Board &board);
        bool operator==(const Board &board) const;
        bool operator!=(const Board &board) const;
        bool operator<(const Board &board) const;
        friend bool operator<(Board const &a, Board const &b);
        bool operator()(Board const &a, Board const &b) const;
        std::basic_string<TCHAR> toString() const;
        Board & shuffle();
        bool isSolved();
        bool isSolvable();
        int getHeuristicValue() const;
        int getHeuristicCost() const;

        int getBlankIndex() const;

        bool moveBlankLeft(bool checkMovable = true);
        bool moveBlankRight(bool checkMovable = true);
        bool moveBlankUp(bool checkMovable = true);
        bool moveBlankDown(bool checkMovable = true);

        int* getPuzzle()
        {
            return puzzle;
        }

        static void setHeuristic(BoardHeuristic *newHeuristic)
        {
            heuristic = newHeuristic;
        }

        bool isBlankNeighbour(int index) const;
        int moveToBlank(int index);

        float GoalDistanceEstimate( Board &nodeGoal );
        bool IsGoal( Board &nodeGoal );
        bool IsSameState( Board &rhs );
        bool GetSuccessors( Search<Board> *astarsearch, Board *parent_node );
        float GetCost( Board &successor );
        static const int solvedPuzzle[];

        int puzzle[9];
    protected:
    private:
        void copyPuzzle(const int puzzle[9]);
        bool moveBlank(int unmovableIdxs[], int movableIdxs[], int newBlankIdxs[], bool checkMovable = true);

        static const Board solvedBoard;
        static const int weightMap[];

        static BoardHeuristic *heuristic;
};

#endif // BOARD_H
