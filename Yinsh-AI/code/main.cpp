#include "board.h"
#include "util.h"
#include "miniMax.h"
#include <ctime>

using namespace std;

bool TimeOut(){
	return false;
}

int rings_rem[2];

inline bool GameOver(){
	if(rings_rem[0] == 0 || rings_rem[1] == 0) return true;
	return false;
}

int main(){
	srand(time(NULL));
	int player_id, time_limit;
	int n,m,k,l;
	cin >> player_id >> n >> time_limit >> k;

	begin1 = clock();
	timeGiven = (double)time_limit;
	time_taken_by_oppo = 0;

	m = n; l = 3;
	board::n = n; // size of the board i.e the number of hexagons
	board::m = m; // number of rings for each player
	board::k = k; // the number of consecutive markers to remove
	board::l = l; // for winning remove l rings before the opponent does

	player_id--;

	rings_rem[0] = l;
	rings_rem[1] = l;
	
	board B;

	for(int i=0; i < 2*m; i++){
		placeTheRing(player_id, B);
		player_id = 1 - player_id;
	}

	while(!TimeOut() && !GameOver()){
		if(player_id) PCMove(B, rings_rem);
		else ApnaMove(B, rings_rem);
		player_id = 1 - player_id;
	}
	return 0;
}