#include<fstream>
#include <cassert>
#include "evalFunc.h"

// ofstream outFile2("output_eva1.txt");
vector<ll> weight;
// map<pii,int> scoreTable = { {{3,0},10},{{3,1},9},{{3,2},8},{{2,0},7},{{2,1},6},{{1,0},6},{{2,2},5},{{1,1},5},{{0,0},5},{{0,1},4},{{1,2},4},{{0,2},3},{{2,3},2},{{1,3},1},{{0,3},0}};
vector<pii> direc = {{1,0}, {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,1}};

inline vector<int> markers_near_rings(board& B, int player){
	vector<int> count(board::k + 1,0);

	for(auto r : B.ringsPos[player]){
		for(auto z : direc){
			pii curr = {r.F+z.F, r.S+z.S};
			if(!board::inTheBoard(curr)) break;
			int curr_color = B.mat[curr.F][curr.S];
			if(abs(curr_color) != 2) break;
			int seqLength = 0;
			while(board::inTheBoard(curr) && B.mat[curr.F][curr.S] == curr_color){
				seqLength++; curr.F += z.F; curr.S += z.S;
			}
			if(seqLength > (board::k)) seqLength = (board::k);
			count[seqLength]++;
		}
	}
	return count;
}

inline vector<int> markers_seq_on_board(board& B, int player){
	vector<int> count(board::k + 1,0);
	int n = board::n;
	int reqMarker = (player == 0) ? 2 : -2;

	for(int i=0; i<2*n+1; i++){
		for(int j=0; j<2*n+1; j++){
			if(B.mat[i][j] != reqMarker) continue;

			for(int l=0; l<3;l++){

				auto z = direc[l];
				int seqLength=1;
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
	// cummulative
	for(int i=0;i < (int)count.size() - 1;i++){
		count[i] -= count[i+1];
	}
	return count;
}

inline bool GameOver(board& B){
	int a = B.ringsPos[0].size();
	int b = B.ringsPos[1].size();
	if(a <= (board::m - board::l) || b <= (board::m - board::l)) return true;
	return false;
}

// inline ll Rfunc(board& B){
// 	int rings_removed[2];
// 	rings_removed[0] = board::m - B.ringsPos[0].size();
// 	rings_removed[1] = board::m - B.ringsPos[1].size();
// 	int rings_fact = scoreTable[{rings_removed[0], rings_removed[1]}];
// 	return (ll)rings_fact;
// }

inline vector<int> generate_Fi(board& B, int player){
	vector<int> apna_near_rings, uska_near_rings;
	apna_near_rings = markers_near_rings(B,0);
	uska_near_rings = markers_near_rings(B,1);

	vector<int> apna_on_board, uska_on_board;
	apna_on_board = markers_seq_on_board(B,0);
	uska_on_board = markers_seq_on_board(B,1);

	vector<int> ans;
	ans.pb(player);
	ans.pb(B.markerCnt[0]);	ans.pb(B.markerCnt[1]);
	ans.pb(B.ringsPos[0].size()); ans.pb(B.ringsPos[1].size());

	for(int i=2; i < apna_near_rings.size(); i++) ans.pb(apna_near_rings[i]);
	for(int i=2; i < uska_near_rings.size(); i++) ans.pb(uska_near_rings[i]);

	for(int i=2; i < apna_on_board.size(); i++) ans.pb(apna_on_board[i]);
	for(int i=2; i < uska_on_board.size(); i++) ans.pb(uska_on_board[i]);

	return ans;
}

int p1[] = {4,2000,4800,10000,20000};
int p2[] = {1,500,1200,2500,5000};

void load_weight(){
	for(int i=0;i< 4*(board::k) + 1 ;i++){
		weight.pb(0);
	}
	weight[0] = 0;
	weight[1] = 1; weight[2] = -1;
	// weight[3] = -244140625; weight[4] = 444140625;
	weight[3] = -250000; weight[4] = 250000;

	int curr = 5;
	int p = 1;
	for(int i=0;i<(board::k)-1;i++) weight[i+curr] = p1[i], curr++;
	p=1;
	for(int i=0;i<(board::k)-1;i++) weight[i+curr] = -p1[i], curr++;
	p=1;
	for(int i=0;i<(board::k)-1;i++) weight[i+curr] = p2[i], curr++;
	p=1;
	for(int i=0;i<(board::k)-1;i++) weight[i+curr] = -p2[i], curr++;
	// load
}

// void print_weights(){
// 	ofstream outFile1("output_eva.txt");
// 	outFile1 << weight.size() << endl;
// 	outFile1 << board::k << endl;
// 	for(int i=0;i<weight.size();i++)
// 		outFile1 << weight[i] << endl;
// 	outFile1.close();
// }

ll evalFun(board& B, int player){
	ll ans = 0;
	vector<int> f = generate_Fi(B,player);
	for(int i=0;i<weight.size();i++){
		ans += (ll)weight[i] * f[i];
	}
	return ans;
}