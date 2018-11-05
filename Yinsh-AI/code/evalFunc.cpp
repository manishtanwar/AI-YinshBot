// #include<fstream>
#include <cassert>
#include "evalFunc.h"

double coeff = 1; // coeff for calculating diffr in base case
double alpha = 0.04; // learning rate in approx q-learning
int eps = 80;   // eps for eps-greedy
vector<double> weight(10,0); //

map<pii,int> scoreTable = { {{3,0},10},{{3,1},9},{{3,2},8},{{2,0},7},{{2,1},6},{{1,0},6},{{2,2},5},{{1,1},5},{{0,0},5},{{0,1},4},{{1,2},4},{{0,2},3},{{2,3},2},{{1,3},1},{{0,3},0},};
vector<pii> direc = {{1,0}, {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,1}};

vector<int> markers_near_rings(board& B, int player){
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

vector<int> markers_seq_on_board(board& B, int player){
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
	return count;
}

inline bool GameOver(board& B){
	int a = B.ringsPos[0].size();
	int b = B.ringsPos[1].size();
	if(a <= (board::m - board::l) || b <= (board::m - board::l)) return true;
	return false;
}

double Rfunc(board& B){
	int rings_removed[2];
	rings_removed[0] = board::m - B.ringsPos[0].size();
	rings_removed[1] = board::m - B.ringsPos[1].size();
	int rings_fact = scoreTable[{rings_removed[0], rings_removed[1]}];
	return (double)rings_fact;
}

vector<int> generate_Fi(board& B, int player){
	vector<int> apna_near_rings, uska_near_rings;
	apna_near_rings = markers_near_rings(B,0);
	uska_near_rings = markers_near_rings(B,1);

	vector<int> apna_on_board, uska_on_board;
	apna_on_board = markers_seq_on_board(B,0);
	uska_on_board = markers_seq_on_board(B,1);

	vector<int> ans;
	for(int i=2; i < apna_near_rings.size(); i++) ans.pb(apna_near_rings[i]);
	for(int i=2; i < uska_near_rings.size(); i++) ans.pb(uska_near_rings[i]);

	for(int i=2; i < apna_on_board.size(); i++) ans.pb(apna_on_board[i]);
	for(int i=2; i < uska_on_board.size(); i++) ans.pb(uska_on_board[i]);

	ans.pb(player);
	ans.pb(B.markerCnt[0]);	ans.pb(B.markerCnt[1]);
	ans.pb(B.ringsPos[0].size()); ans.pb(B.ringsPos[1].size());
	return ans;
}

double Vfunc(board& B, int player){
	double ans = 0;
	vector<int> f = generate_Fi(B,player);
	for(int i=0;i<weight.size();i++){
		ans += weight[i] * (double) f[i];
	}
	return ans;
}

vector< pair<board, int> > generate_neigh(board B, int player, int removeRing, int flip){
	vector<pair<board,int>> ans;
	if(GameOver(B)){
		ans.pb({B,player}); return ans;
	}
	if(removeRing){
		for(auto z : B.ringsPos[player]){
			board currBoard(B);
			currBoard.removeRing(player, z);
			auto seq = generate_neigh(currBoard,player,0,flip);
			for(auto y : seq) ans.pb(y);
		}
	}
	else{
		vector< pair<pii,pii> > markerSeq;
		markerSeq = B.generateAllSequences(player);
		if(markerSeq.size() > 0)
		for(auto z : markerSeq){
			board currBoard(B); 
			currBoard.removeSequence(player,z.F,z.S);
			auto seq = generate_neigh(currBoard,player,1,flip);
			for(auto y : seq) ans.pb(y);
		}
		else{
			if(flip) return ans;
			for(auto z : B.ringsPos[player]){
				vector<pii> possibleToPos = B.generateNeighbours(z);
				for(auto itr : possibleToPos){
					board currBoard(B);
					currBoard.moveRing(player,z,itr);

					vector< pair<pii,pii> > markerSeqLoop;
					markerSeqLoop = currBoard.generateAllSequences(player);

					// checking if a marker seq can be removed
					if(markerSeqLoop.size() == 0){
						ans.pb({currBoard,player});
					}
					else{
						auto seq = generate_neigh(currBoard,player,0,1-flip);
						for(auto y : seq) ans.pb(y);
					}
				}
			}
		}
	}
	return ans;
}

inline bool greedy_eps_explore(){
	int random = rand() % 100;
	return random < eps;
}

inline void update_weights(double diffr,board& B, int player){
	vector<int> f = generate_Fi(B,player);

	for(int i=0;i<weight.size();i++){
		weight[i] += alpha * diffr * f[i];
	}
}

double evalFun(board& B, int player){
	if(GameOver(B)) return Vfunc(B,player);

	pair<board, int> s({B,player}),s1;
	// state represents B -> board, Player->chance, third->boolean(removeRing), flip->flip variable for generating neighbours correctly
	vector< pair<board,int> > neighbours_list = generate_neigh(B,player,0,0);
	assert(neighbours_list.size() > 0);

	// determing s1
	if(greedy_eps_explore()){
		int random = rand() % neighbours_list.size();
		s1 = neighbours_list[random];
	}
	else{
		vector<double> neighbours_VList;
		for(auto z : neighbours_list) neighbours_VList.pb(Vfunc(z.F,z.S));
		double optimal_val = (player) ? -INF : INF;
		for(int i=0;i < neighbours_VList.size(); i++){
			auto &nv = neighbours_VList[i];
			auto &ns = neighbours_list[i];

			if((player==1 && optimal_val < nv) || (player==0 && optimal_val > nv)){
				optimal_val = nv;
				s1 = ns;
			}
		}
	}

	// is s1 terminating
	if(GameOver(s1.F)){
		double diffr = Rfunc(s1.F) - Rfunc(s.F) + coeff * Rfunc(s1.F) - Vfunc(s1.F,s1.S);
		update_weights(diffr,s1.F,s1.S);
	}
	else{
		vector< pair<board,int> > neighbours_s1_list = generate_neigh(s1.F,s1.S,0,0);
		assert(neighbours_s1_list.size() > 0);

		vector<double> neighbours_VList;
		for(auto z : neighbours_s1_list) neighbours_VList.pb(Vfunc(z.F,z.S));
		double optimal_val = (s1.S) ? -INF : INF;
		for(int i=0;i < neighbours_VList.size(); i++){
			auto &nv = neighbours_VList[i];
			auto &ns = neighbours_s1_list[i];

			if((s1.S==1 && optimal_val < nv) || (s1.S==0 && optimal_val > nv)){
				optimal_val = nv;
			}
		}

		double diffr = Rfunc(s1.F) - Rfunc(s.F) + optimal_val - Vfunc(s1.F,s1.S);
		update_weights(diffr,s1.F,s1.S);
	}
	return Vfunc(B,player);
}