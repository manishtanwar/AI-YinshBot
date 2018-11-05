#include<fstream>
#include "evalFunc.h"

map<pii,ll> scoreTable = { {{3,0},10},{{3,1},	9},{{3,2},	8},{{2,0},	7},{{2,1},	6},{{1,0},	6},{{2,2},	5},{{1,1},	5},{{0,0},	5},{{0,1},	4},{{1,2},	4},{{0,2},	3},{{2,3},	2},{{1,3},	1},{{0,3},	0},};
vector<pii> direc = {{1,0}, {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,1}};

vector<ll> markers_near_rings(board& B, int player){
	vector<ll> count(board::k + 1,0);

	for(auto r : B.ringsPos[player]){
		for(auto z : direc){
			pii curr = {r.F+z.F, r.S+z.S};
			if(!board::inTheBoard(curr)) break;
			ll curr_color = B.mat[curr.F][curr.S];
			if(abs(curr_color) != 2) break;
			ll seqLength = 0;
			while(board::inTheBoard(curr) && B.mat[curr.F][curr.S] == curr_color){
				seqLength++; curr.F += z.F; curr.S += z.S;
			}
			if(seqLength > (board::k)) seqLength = (board::k);
			count[seqLength]++;
		}
	}
	return count;
}

vector<ll> markers_seq_on_board(board& B, int player){
	vector<ll> count(board::k + 1,0);
	ll n = board::n;
	ll reqMarker = (player == 0) ? 2 : -2;

	for(ll i=0; i<2*n+1; i++){
		for(ll j=0; j<2*n+1; j++){
			if(B.mat[i][j] != reqMarker) continue;

			for(ll l=0; l<3;l++){

				auto z = direc[l];
				ll seqLength=1;
				pii curr = {i+z.F, j+z.S};
				
				while(board::inTheBoard(curr) && B.mat[curr.F][curr.S] == reqMarker){
					seqLength++; curr.F += z.F; curr.S += z.S;
				}

				if(seqLength > board::k){
					seqLength = board::k;
				}
				count[seqLength]++;
			}
		}
	}
	return count;
}

bool GameOver(board& B){
	int a = B.ringsPos[0].size();
	int b = B.ringsPos[1].size();
	if(a <= (board::m - board::l) || b <= (board::m - board::l)) return true;
	return false;
}

ll evalFun(board& B, int player){
	if(GameOver(B)) return Vfunc(B);

	ll rings_removed[2];
	rings_removed[0] = board::m - (ll)B.ringsPos[0].size();
	rings_removed[1] = board::m - (ll)B.ringsPos[1].size();
	ll rings_fact = scoreTable[{rings_removed[0], rings_removed[1]}];

	vector<ll> apna_near_rings, uska_near_rings;
	apna_near_rings = markers_near_rings(B,0);
	uska_near_rings = markers_near_rings(B,1);

	vector<ll> apna_on_board, uska_on_board;
	apna_on_board = markers_seq_on_board(B,0);
	uska_on_board = markers_seq_on_board(B,1);

		
}