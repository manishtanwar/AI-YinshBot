#include "miniMax.h"
#include "evalFunc.h"
#include<limits.h>
#include<fstream>
#include<algorithm>
// ofstream outFile1("output_eva.txt");

ll miniMax(board& B, int depth, int player, ll Min, ll Max, bool removeRing);

double timeGiven;
double time_taken_by_oppo;

int determineDepth(board& B){
	int xy = B.markerCnt[0] + B.markerCnt[1];
	if(xy < 6) return 2;
	int avgFac = 0;
	for(int p=0;p<2;p++){
		for(auto from : B.ringsPos[p]){
			vector<pii> possibleToPos = B.generateNeighbours(from);
			avgFac += possibleToPos.size();
		}
	}
	double elapsedTime = double(clock() - begin1) / CLOCKS_PER_SEC;
	double remTime = timeGiven - elapsedTime - time_taken_by_oppo - 2;

	// outFile1 << avgFac << endl;
	if(remTime < 2){
		return 0;
	}
	else if(remTime < 20){
		return 1;
	}
	else if(remTime < 30){
		if(avgFac < 50) return 5;
		return 4;
	}
	else if(remTime < 60){
		if(avgFac < 40) return 5;
		if(avgFac < 60) return 5;
		return 5;
	}
	else if(remTime < 119.8){
		if(avgFac < 40) return 5;
		if(avgFac < 60) return 5;
		return 5;
	}
	else{
		if(avgFac < 45) return 5;
		return 4;
	}
	// if(avgFac < 35) return 5;
	// if(avgFac < 50) return 5;
	// if(avgFac < 60) return 4;
	// if(avgFac < 70) return 4;
	// return 3;
}
// 1. moving the ring
pair<pii,pii> SelectMove1MoveRing(board& B){
	// Max Node
	int player = 0;
	ll Min = LLONG_MIN;
	ll Max = LLONG_MAX;
	ll ansScore = Min;
	int depth = determineDepth(B);
	pair<pii,pii> ans;

	for(auto from : B.ringsPos[player]){
		vector<pii> possibleToPos = B.generateNeighbours(from);
		for(auto to : possibleToPos){
			board currBoard(B);
			currBoard.moveRing(player,from,to);

			vector< pair<pii,pii> > markerSeqLoop;
			markerSeqLoop = currBoard.generateAllSequences(player);

			// checking if a marker seq can be removed
			if(markerSeqLoop.size() == 0){
				ll tmp = miniMax(currBoard, depth-1, 1-player, ansScore, Max, 0);
				if(ansScore < tmp){
					ansScore = tmp;
					ans = {from,to};
				}
			}
			else{
				ll tmp = miniMax(currBoard, depth, player, ansScore, Max, 0);
				if(ansScore < tmp){
					ansScore = tmp;
					ans = {from,to};
				}
			}
		}
	}
	return ans;
}

//2. removing a ring
pii SelectMove2RemoveRing(board& B){
	// Max Node
	int player = 0;
	ll Min = LLONG_MIN;
	ll Max = LLONG_MAX;
	ll ansScore = Min;
	int depth = determineDepth(B);
	pii ans;

	for(auto from : B.ringsPos[player]){
		board currBoard(B);
		currBoard.removeRing(player,from);
		vector< pair<pii,pii> > markerSeq;
		markerSeq = currBoard.generateAllSequences(player);
		// checking if more seq can be removed
		if(markerSeq.size() == 0){
			ll tmp = miniMax(currBoard, depth-1, 1-player, ansScore, Max, 0);
			if(ansScore < tmp){
				ansScore = tmp;
				ans = from;
			}
		}
		else{
			ll tmp = miniMax(currBoard, depth, player, ansScore, Max, 0);
			if(ansScore < tmp){
				ansScore = tmp;
				ans = from;
			}
		}
	}
	return ans;
}
//3. removing a seq
pair<pii,pii> SelectMove3RemoveSeq(board& B){
	// Max Node
	int player = 0;
	ll Min = LLONG_MIN;
	ll Max = LLONG_MAX;
	ll ansScore = Min;
	int depth = determineDepth(B);
	pair<pii,pii> ans;

	vector< pair<pii,pii> > markerSeq;
	markerSeq = B.generateAllSequences(player);
	for(auto z : markerSeq){
		board currBoard(B);
		currBoard.removeSequence(player,z.F,z.S);
		ll tmp = miniMax(currBoard, depth-1, player, ansScore, Max, 1);
		if(ansScore < tmp){
			ansScore = tmp;
			ans = {z.F,z.S};
		}
	}
	return ans;
}


bool GameOver(board& B){
	int a = B.ringsPos[0].size();
	int b = B.ringsPos[1].size();
	if(a <= (board::m - board::l) || b <= (board::m - board::l)) return true;
	return false;
}

pii whereToPlaceRing(board& B){
	int n = board :: n;
	for(int i = 0 ; i <= n ; i++){
		for(int j = 0; j < max(6*i,1); j++){
			pii at = board::convertCoordinatesForward({i,j});
			if(B.mat[at.F][at.S] == 0){
				return at;
			}
		}
	}
}


ll miniMax(board& B, int depth, int player, ll Min, ll Max, bool removeRing){
	if(depth == 0 || GameOver(B)) return evalFun(B);

	// *********************** Remove Ring Step **************************
	if(removeRing){
		if(player){
			// Min Node
			ll ansScore = Max;

			for(auto z : B.ringsPos[player]){
				board currBoard(B);
				currBoard.removeRing(player, z);
				vector< pair<pii,pii> > markerSeq;
				markerSeq = currBoard.generateAllSequences(player);
				// checking if more seq can be removed
				if(markerSeq.size() == 0)	ansScore = min(ansScore, miniMax(currBoard, depth-1, 1-player, Min, ansScore, 0));
				else	ansScore = min(ansScore, miniMax(currBoard, depth, player, Min, ansScore, 0));
				if(ansScore <= Min) return Min;
			}
			return ansScore;
		}
		else{
			// Max Node
			ll ansScore = Min;

			for(auto z : B.ringsPos[player]){
				board currBoard(B);
				currBoard.removeRing(player,z);
				vector< pair<pii,pii> > markerSeq;
				markerSeq = currBoard.generateAllSequences(player);
				// checking if more seq can be removed
				if(markerSeq.size() == 0)	ansScore = max(ansScore, miniMax(currBoard, depth-1, 1-player, ansScore, Max, 0));
				else ansScore = max(ansScore, miniMax(currBoard, depth, player, ansScore, Max, 0));
				if(ansScore >= Max) return Max;
			}
			return ansScore;
		}
	}

	vector< pair<pii,pii> > markerSeq;
	markerSeq = B.generateAllSequences(player);

	if(markerSeq.size() > 0){
		// *********************** Remove Marker Seq Step **************************
		if(player){
			// Min Node
			ll ansScore = Max;

			for(auto z : markerSeq){
				board currBoard(B);
				currBoard.removeSequence(player,z.F,z.S);
				ansScore = min(ansScore, miniMax(currBoard, depth, player, Min, ansScore, 1));
				if(ansScore <= Min) return Min;
			}
			return ansScore;
		}
		else{
			// Max Node
			ll ansScore = Min;

			for(auto z : markerSeq){
				board currBoard(B);
				currBoard.removeSequence(player,z.F,z.S);
				ansScore = max(ansScore, miniMax(currBoard, depth, player, ansScore, Max, 1));
				if(ansScore >= Max) return Max;
			}
			return ansScore;
		}
	}
	else{
	// *********************** Moving Ring Step **************************
		if(player){
			// Min Node
			ll ansScore = Max;

			for(auto z : B.ringsPos[player]){
				vector<pii> possibleToPos = B.generateNeighbours(z);
				for(auto itr : possibleToPos){
					board currBoard(B);
					currBoard.moveRing(player,z,itr);

					vector< pair<pii,pii> > markerSeqLoop;
					markerSeqLoop = currBoard.generateAllSequences(player);

					// checking if a marker seq can be removed
					if(markerSeqLoop.size() == 0)	ansScore = min(ansScore, miniMax(currBoard, depth-1, 1-player, Min, ansScore, 0));
					else ansScore = min(ansScore, miniMax(currBoard, depth, player, Min, ansScore, 0));
					if(ansScore <= Min) return Min;
				}
			}
			return ansScore;
		}
		else{
			// Max Node
			ll ansScore = Min;

			for(auto z : B.ringsPos[player]){
				vector<pii> possibleToPos = B.generateNeighbours(z);
				for(auto itr : possibleToPos){
					board currBoard(B);
					currBoard.moveRing(player,z,itr);

					vector< pair<pii,pii> > markerSeqLoop;
					markerSeqLoop = currBoard.generateAllSequences(player);

					// checking if a marker seq can be removed
					if(markerSeqLoop.size() == 0)	ansScore = max(ansScore, miniMax(currBoard, depth-1, 1-player, ansScore, Max, 0));
					else ansScore = max(ansScore, miniMax(currBoard, depth, player, ansScore, Max, 0));
					if(ansScore >= Max) return Max;
				}
			}
			return ansScore;
		}
	}
	
	return 0;
}	