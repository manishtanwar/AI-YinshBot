#include<fstream>
#include "evalFunc.h"


map<pii,ll> scoreTable = 
{ {{3,0},25},
{{3,1},	20},
{{3,2},	18},
{{2,0},	15},
{{2,1},	9},
{{1,0},	7},
{{2,2},	4},
{{1,1},	2},
{{0,0},	0},
{{0,1},	-5},
{{1,2},	-10},
{{0,2},	-14},
{{2,3},	-16},
{{1,3},	-18},
{{0,3},	-20},
};

vector<pii> direc = {{1,0}, {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,1}};

vector<ll> markers_near_rings(board& B, int player){
	vector<ll> count(board::k,0);

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
			if(seqLength >= (board::k)) seqLength = (board::k) - 1;
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

				ll cnt = 1;
				if(seqLength > board::k){
					cnt = 1 + seqLength - board::k;
					seqLength = board::k;
				}
				count[seqLength] += cnt;
			}
		}
	}
	return count;
}

ll evalFun(board& B){
	ll k = board::k;

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

	ll ans = 0;
	ll base = 3;
	vector<ll> powers(k+6,1);
	for(int i=1;i<k+6;i++)
		powers[i] = powers[i-1] * base;

	ans += apna_on_board[k] * powers[k];
	ans -= uska_on_board[k] * powers[k+1];

	ans += apna_on_board[k-1] * powers[k-2];
	ans -= uska_on_board[k-1] * powers[k-2];

	ans += apna_near_rings[k-1] * powers[k-1];
	ans -= uska_near_rings[k-1] * powers[k-1];

	ll coeff = 1;
	for(int i=0;i<k+3;i++)
		coeff *= 5;


	ans += rings_fact * coeff;
	for(int i=0;i<2;i++) ans *= base;
	ans += B.markerCnt[0] - B.markerCnt[1];

	return ans;
}