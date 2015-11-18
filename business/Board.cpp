#include "Board.h"

#include <tchar.h>
#include <boost/format.hpp>
#include "ManhattanDistance.h"

const int Board::solvedPuzzle[] = {1, 2, 3, 8, 0, 4, 7, 6, 5};
const int Board::weightMap[] = {4, 0, 1, 2, 5, 8, 7, 6, 3};
const Board Board::solvedBoard;

BoardHeuristic *Board::heuristic = &ManhattanDistance::getInstance();

Board::Board()
{
    copyPuzzle(solvedPuzzle);
}

Board::Board(const int puzzle[9])
{
    copyPuzzle(puzzle);
}

Board::Board(const Board &board)
{
    copyPuzzle(board.puzzle);
}

Board::~Board()
{
    //dtor
}

Board & Board::operator=(const Board &board)
{
    copyPuzzle(board.puzzle);
    return *this;
}

bool Board::operator==(const Board &board) const
{
    for (int i = 0; i < 9; ++i)
    {
        if (this->puzzle[i] != board.puzzle[i])
        {
            return false;
        }
    }
    return true;
}

bool Board::operator!=(const Board &board) const
{
    for (int i = 0; i < 9; ++i)
    {
        if (this->puzzle[i] != board.puzzle[i])
        {
            return true;
        }
    }
    return false;
}

bool Board::operator<(const Board &board) const
{
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] < board.puzzle[i]) {
            return true;
        } else if (board.puzzle[i] < puzzle[i]) {
            return false;
        }
    }
    return false;
}

bool operator<(Board const &a, Board const &b)
{
    for (int i = 0; i < 9; ++i)
    {
        if (a.puzzle[i] < b.puzzle[i]) {
            return true;
        } else if (b.puzzle[i] < a.puzzle[i]) {
            return false;
        }
    }
    return false;
}

bool Board::operator()(Board const &a, Board const &b) const
{
    for (int i = 0; i < 9; ++i)
    {
        if (a.puzzle[i] < b.puzzle[i]) {
            return true;
        } else if (b.puzzle[i] < a.puzzle[i]) {
            return false;
        }
    }
    return false;
}

std::basic_string<TCHAR> Board::toString() const
{
    std::basic_string<TCHAR> stateString;
    std::basic_string<TCHAR> ch;
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] == 0)
        {
            ch = __TEXT(" ");
        }
        else
        {
            ch = str(boost::basic_format<TCHAR>(__TEXT("%1%")) % puzzle[i]);
        }
        if (((i + 1) % 3) == 0)
        {
            stateString += ch + __TEXT("\n");
        }
        else
        {
            stateString += ch;
        }

    }
    return stateString;
}

Board & Board::shuffle()
{
    srand(time(NULL));
    size_t n = 9;
    size_t i;
    do {
         for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = puzzle[j];
            puzzle[j] = puzzle[i];
            puzzle[i] = t;
        }
    } while (!isSolvable());
    return *this;
}

bool Board::isSolvable()
{
    //solvable puzzle: {1, 2, 3, 8, 0, 4, 7, 6, 5};
    //weight:          {0, 1, 2, 3, 4, 5, 6, 7, 8};
    //weight map:      {4, 0, 1, 2, 5, 8, 7, 6, 3};
    int nInversion = 0;
    for (int i = 0; i < 8; ++i) {
        if (puzzle[i] == 0) {
            continue;
        }
        for (int j = i + 1; j < 9; ++j) {
            if (puzzle[j] == 0) {
                continue;
            }
            if (weightMap[puzzle[i]] > weightMap[puzzle[j]]) {
                nInversion += 1;
            }
        }
    }
    return ((nInversion % 2) == 0);
}

bool Board::isSolved()
{
    return (*this) == solvedBoard;
}

void Board::copyPuzzle(const int puzzle[9])
{
    for (int i = 0; i < 9; ++i)
    {
        this->puzzle[i] = puzzle[i];
    }
}

int Board::getHeuristicValue() const
{
    int value = 0;
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] == solvedBoard.puzzle[i])
        {
            value += 1;
        }
    }
    return value;
}

int Board::getHeuristicCost() const
{
    int cost = 0;
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] != solvedBoard.puzzle[i])
        {
            cost += 1;
        }
    }
    return cost;
}

int Board::getBlankIndex() const
{
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] == 0)
            return i;
    }
    return -1;
}

bool Board::moveBlank(int unmovableIdxs[], int movableIdxs[], int newBlankIdxs[], bool checkMovable)
{
    if (checkMovable) {
        for (int i = 0; i < 3; ++i)
        {
            if (puzzle[unmovableIdxs[i]] == 0)
            {
                return false;
            }
        }
    }
    for (int i = 0; i < 6; ++i)
    {
        if (puzzle[movableIdxs[i]] == 0)
        {
            puzzle[movableIdxs[i]] = puzzle[newBlankIdxs[i]];
            puzzle[newBlankIdxs[i]] = 0;
            break;
        }
    }
    return true;
}

bool Board::moveBlankLeft(bool checkMovable)
{
    int unmovableIdxs[] = {0, 3, 6};
    int movableIdxs[] = {1, 4, 7, 2, 5, 8};
    int newBlankIdxs[] = {0, 3, 6, 1, 4, 7};
    return moveBlank(unmovableIdxs, movableIdxs, newBlankIdxs, checkMovable);
}

bool Board::moveBlankRight(bool checkMovable)
{
    int unmovableIdxs[] = {2, 5, 8};
    int movableIdxs[] = {1, 4, 7, 0, 3, 6};
    int newBlankIdxs[] = {2, 5, 8, 1, 4, 7};
    return moveBlank(unmovableIdxs, movableIdxs, newBlankIdxs, checkMovable);
}

bool Board::moveBlankUp(bool checkMovable)
{
    int unmovableIdxs[] = {0, 1, 2};
    int movableIdxs[] = {3, 4, 5, 6, 7, 8};
    int newBlankIdxs[] = {0, 1, 2, 3, 4, 5};
    return moveBlank(unmovableIdxs, movableIdxs, newBlankIdxs, checkMovable);
}

bool Board::moveBlankDown(bool checkMovable)
{
    int unmovableIdxs[] = {6, 7, 8};
    int movableIdxs[] = {3, 4, 5, 0, 1, 2};
    int newBlankIdxs[] = {6, 7, 8, 3, 4, 5};
    return moveBlank(unmovableIdxs, movableIdxs, newBlankIdxs, checkMovable);
}

bool Board::isBlankNeighbour(int index) const
{
    int blankIndex = getBlankIndex();
    if (blankIndex == 0) {
        if ((index == 1) || (index == 3)) {
            return true;
        }

    } else if (blankIndex == 1) {
        if ((index == 0) || (index == 2) || (index == 4)) {
            return true;
        }
    } else if (blankIndex == 2) {
        if ((index == 1) || (index == 5)) {
            return true;
        }
    } else if (blankIndex == 3) {
        if ((index == 0) || (index == 4) || (index == 6)) {
            return true;
        }
    } else if (blankIndex == 4) {
        if ((index == 1) || (index == 3) || (index == 5) || (index == 7)) {
            return true;
        }
    } else if (blankIndex == 5) {
        if ((index == 2) || (index == 4) || (index == 8)) {
            return true;
        }
    } else if (blankIndex == 6) {
        if ((index == 3) || (index == 7)) {
            return true;
        }
    } else if (blankIndex == 7) {
        if ((index == 4) || (index == 6) || (index == 8)) {
            return true;
        }
    } else if (blankIndex == 8) {
        if ((index == 5) || (index == 7)) {
            return true;
        }
    }
    return false;
}

int Board::moveToBlank(int index) {
    int blankIndex = getBlankIndex();
    if (blankIndex == 0) {
        if ((index == 1) || (index == 3)) {
            if (index == 1) {
                moveBlankRight(false);
                return 0;
            } else {
                moveBlankDown(false);
                return 2;
            }
        }

    } else if (blankIndex == 1) {
        if ((index == 0) || (index == 2) || (index == 4)) {
            if (index == 0) {
                moveBlankLeft(false);
                return 1;
            } else if (index == 2) {
                moveBlankRight(false);
                return 0;
            } else {
                moveBlankDown(false);
                return 2;
            }
        }
    } else if (blankIndex == 2) {
        if ((index == 1) || (index == 5)) {
            if (index == 1) {
                moveBlankLeft(false);
                return 1;
            } else {
                moveBlankDown(false);
                return 2;
            }
        }
    } else if (blankIndex == 3) {
        if ((index == 0) || (index == 4) || (index == 6)) {
            if (index == 0) {
                moveBlankUp(false);
                return 3;
            } else if (index == 4) {
                moveBlankRight(false);
                return 0;
            } else {
                moveBlankDown(false);
                return 2;
            }
        }
    } else if (blankIndex == 4) {
        if ((index == 1) || (index == 3) || (index == 5) || (index == 7)) {
            if (index == 1) {
                moveBlankUp(false);
                return 3;
            } else if (index == 3) {
                moveBlankLeft(false);
                return 1;
            } else if (index == 5) {
                moveBlankRight(false);
                return 0;
            } else {
                moveBlankDown(false);
                return 2;
            }
        }
    } else if (blankIndex == 5) {
        if ((index == 2) || (index == 4) || (index == 8)) {
            if (index == 2) {
                moveBlankUp(false);
                return 3;
            } else if (index == 4) {
                moveBlankLeft(false);
                return 1;
            } else {
                moveBlankDown(false);
                return 2;
            }
        }
    } else if (blankIndex == 6) {
        if ((index == 3) || (index == 7)) {
            if (index == 3) {
                moveBlankUp(false);
                return 3;
            } else {
                moveBlankRight(false);
                return 0;
            }
        }
    } else if (blankIndex == 7) {
        if ((index == 4) || (index == 6) || (index == 8)) {
            if (index == 4) {
                moveBlankUp(false);
                return 3;
            } else if (index == 6) {
                moveBlankLeft(false);
                return 1;
            } else {
                moveBlankRight(false);
                return 0;
            }
        }
    } else if (blankIndex == 8) {
        if ((index == 5) || (index == 7)) {
            if (index == 5) {
                moveBlankUp(false);
                return 3;
            } else {
                moveBlankLeft(false);
                return 1;
            }
        }
    }
    return -1;
}

float Board::GoalDistanceEstimate( Board &nodeGoal )
{
    return heuristic->calculate(*this, nodeGoal);
}

bool Board::IsGoal( Board &nodeGoal )
{
	return IsSameState( nodeGoal );
}

bool Board::IsSameState( Board &rhs )
{
    return ((*this) == rhs);
}

bool Board::GetSuccessors( Search<Board> *astarsearch, Board *parent_node  )
{
	Board NewNode;

	bool ret;

	NewNode = (*this);
	if (NewNode.moveBlankLeft()) {
        ret = astarsearch->AddSuccessor(NewNode);
        if( !ret ) return false;
	}
	NewNode = (*this);
	if (NewNode.moveBlankRight()) {
        ret = astarsearch->AddSuccessor(NewNode);
        if( !ret ) return false;
	}
	NewNode = (*this);
	if (NewNode.moveBlankUp()) {
        ret = astarsearch->AddSuccessor(NewNode);
        if( !ret ) return false;
	}
	NewNode = (*this);
	if (NewNode.moveBlankDown()) {
        ret = astarsearch->AddSuccessor(NewNode);
        if( !ret ) return false;
	}

	return true;
}

float Board::GetCost( Board &successor )
{
	return 1.0f;

}
