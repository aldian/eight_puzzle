#include "AlgoView.h"


AlgoView::AlgoView()
    : nListItemVisible(10)
{
}

AlgoView::~AlgoView()
{
    //dtor
}

void AlgoView::addToSearchLog(const vector<Board> &searchSteps)
{
    searchLog.push_back(searchSteps);
}

void AlgoView::addToClosedLog(const vector<Board> &closedSteps)
{
    closedLog.push_back(closedSteps);
}

void AlgoView::addToFLog(const vector<float> &fSteps)
{
    fLog.push_back(fSteps);
}

void AlgoView::addToGLog(const vector<float> &gSteps)
{
    gLog.push_back(gSteps);
}

void AlgoView::addToHLog(const vector<float> &hSteps)
{
    hLog.push_back(hSteps);
}

void AlgoView::addToCFLog(const vector<float> &cfSteps)
{
    cfLog.push_back(cfSteps);
}

void AlgoView::addToCGLog(const vector<float> &cgSteps)
{
    cgLog.push_back(cgSteps);
}

void AlgoView::addToCHLog(const vector<float> &chSteps)
{
    chLog.push_back(chSteps);
}

Board AlgoView::nextMove(const Board &board, int &direction, int &movingItemIndex)
{
    Board nextBoard = solutionMap[board];
    int oldBlankIndex = board.getBlankIndex();
    int newBlankIndex = nextBoard.getBlankIndex();
    movingItemIndex = oldBlankIndex;
    if (oldBlankIndex == 0) {
        if (newBlankIndex == 1) {
            direction = 0;
        } else { // 3
            direction = 2;
        }
    } else if (oldBlankIndex == 1) {
        if (newBlankIndex == 0) {
            direction = 1;
        } else if (newBlankIndex == 2) {
            direction = 0;
        } else { // 4
            direction = 2;
        }
    } else if (oldBlankIndex == 2) {
        if (newBlankIndex == 1) {
            direction = 1;
        } else { // 5
            direction = 2;
        }
    } else if (oldBlankIndex == 3) {
        if (newBlankIndex == 0) {
            direction = 3;
        } else if (newBlankIndex == 4) {
            direction = 0;
        } else { // 6
            direction = 2;
        }
    } else if (oldBlankIndex == 4) {
        if (newBlankIndex == 1) {
            direction = 3;
        } else if (newBlankIndex == 3) {
            direction = 1;
        } else if (newBlankIndex == 5) {
            direction = 0;
        } else { // 7
            direction = 2;
        }
    } else if (oldBlankIndex == 5) {
        if (newBlankIndex == 2) {
            direction = 3;
        } else if (newBlankIndex == 4) {
            direction = 1;
        } else { // 8
            direction = 2;
        }
    } else if (oldBlankIndex == 6) {
       if (newBlankIndex == 3) {
            direction = 3;
        } else { // 7
            direction = 0;
        }
    } else if (oldBlankIndex == 7) {
        if (newBlankIndex == 4) {
            direction = 3;
        } else if (newBlankIndex == 6) {
            direction = 1;
        } else { // 8
            direction = 0;
        }
    } else if (oldBlankIndex == 8) {
       if (newBlankIndex == 5) {
            direction = 3;
        } else { // 7
            direction = 1;
        }
    }
    return nextBoard;
}


