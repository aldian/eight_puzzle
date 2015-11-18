#ifndef ASTARLOGINFO_H
#define ASTARLOGINFO_H

#include "Board.h"

class AStarLogInfo
{
    public:
        AStarLogInfo();
        virtual ~AStarLogInfo();

        Board board;
        float f, g, h;
    protected:
    private:
};

#endif // ASTARLOGINFO_H
