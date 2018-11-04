#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include <iostream>
#include <cmath>
using namespace std;

typedef pair<int,int> pii;
#define F first
#define S second
#define mp make_pair
#define pb push_back

class board
{
	public:
	// 0 -> player
	// 1 -> pc
	// 0 -> empty
	// 1 -> player ring
	// 2 -> player marker
	// -1 -> pc ring
	// -2 -> pc marker
	// -3 -> out of the board position
	static int n,m,k,l;
	static map<pii,pii> forwardConversionTable;
	static map<pii,pii> backwardConversionTable;

	int **mat;
	vector<pii> ringsPos[2];
	int markerCnt[2];

	// constructor
	board();

	// constructor
	board(board const& original);

	~board();


	static pii convertCoordinatesForward(pii p);
	static pii convertCoordinatesBackward(pii p);

    void placeRing(int player, pii at);
    void moveRing(int player, pii from, pii to);
    void removeRing(int player, pii from);
    void removeSequence(int player, pii from, pii to);
    vector<pii> generateNeighbours(pii ringPos);
    vector< pair<pii,pii> > generateAllSequences(int player);
    void init();
    bool isEndOfSeq(pii p, pii dir, int reverse);
    static bool inTheBoard(pii p);
};
#endif