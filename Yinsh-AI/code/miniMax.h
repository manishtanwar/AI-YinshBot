#ifndef MINIMAX_H
#define MINIMAX_H
#include <ctime>

#include "board.h"

pii whereToPlaceRing(board& B);
pair<pii,pii> SelectMove1MoveRing(board& B);
pii SelectMove2RemoveRing(board& B);
pair<pii,pii> SelectMove3RemoveSeq(board& B);
static clock_t begin1;
extern int steps_taken;
extern double timeGiven;
extern double time_taken_by_oppo;

#endif