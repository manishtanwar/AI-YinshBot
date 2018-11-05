#include "util.h"
#include "miniMax.h"
#include<string>
#include<sstream>

void placeTheRing(int p, board& B){
	int h,pn;
	pii at;
	if(p){
		string strInput, buf;
		while( strInput.size() == 0 )
			getline(cin, strInput);
		stringstream ss(strInput);
		vector<string> tokens;

		while(ss >> buf)
			tokens.pb(buf);
		
		h = stoi(tokens[1]);
		pn = stoi(tokens[2]);
		at = board::convertCoordinatesForward({h,pn});
	}
	else{
		at = whereToPlaceRing(B);
		pii at1 = board::convertCoordinatesBackward(at);
		cout<<"P "<<at1.F<<' '<<at1.S<<endl;
		// degub output
		cerr<<"P "<<at1.F<<' '<<at1.S<<endl;
	}
	B.placeRing(p,at);
}

void PCMove(board& B, int rings_rem[]){
	string strInput, buf;
	clock_t time_st_local = clock();
	getline(cin, strInput);
	double time_taken_local = double(clock() - time_st_local) / CLOCKS_PER_SEC;

	time_taken_by_oppo += time_taken_local;

	stringstream ss(strInput);
	vector<string> tokens;

	while(ss >> buf)
		tokens.pb(buf);
	int ind=0;

	while(ind < tokens.size()){
		if(tokens[ind] == "S"){
			pii from = {stoi(tokens[++ind]), stoi(tokens[++ind])};
			++ind;
			pii to = {stoi(tokens[++ind]), stoi(tokens[++ind])};
			++ind;
			from = board::convertCoordinatesForward(from);
			to   = board::convertCoordinatesForward(to);
			B.moveRing(1,from,to);
		}
		if(ind >= tokens.size()) break;

		if(tokens[ind] == "RS"){
			pii from = {stoi(tokens[++ind]), stoi(tokens[++ind])};
			++ind;
			pii to = {stoi(tokens[++ind]), stoi(tokens[++ind])};
			++ind;
			pii removeRingPos = {stoi(tokens[++ind]), stoi(tokens[++ind])};
			++ind;
			from = board::convertCoordinatesForward(from);
			to   = board::convertCoordinatesForward(to);
			removeRingPos = board::convertCoordinatesForward(removeRingPos);
			B.removeSequence(1,from,to);
			B.removeRing(1,removeRingPos);
			rings_rem[1]--;
		}
	}
}

bool GameOver(int rings_rem[]){
	if(rings_rem[0] == 0 || rings_rem[1] == 0) return true;
	return false;
}

string removeMarkerStringOut(pair<pii,pii> p){
	string s;
	p.F = board::convertCoordinatesBackward(p.F);
	p.S = board::convertCoordinatesBackward(p.S);
	s += "RS ";
	s += to_string(p.F.F);
	s += " ";
	s += to_string(p.F.S);
	s += " ";
	s += "RE ";
	s += to_string(p.S.F);
	s += " ";
	s += to_string(p.S.S);
	s += " ";
	return s;
}

string removeRingStringOut(pii p){
	string s;
	s += "X ";
	p = board::convertCoordinatesBackward(p);
	s += to_string(p.F);
	s += " ";
	s += to_string(p.S);
	s += " ";
	return s;
}

string moveRingStringOut(pii from, pii to){
	string s;
	to = board::convertCoordinatesBackward(to);
	from = board::convertCoordinatesBackward(from);
	s += "S ";
	s += to_string(from.F); s += " "; s += to_string(from.S); s += " ";
	s += "M ";
	s += to_string(to.F); s += " "; s += to_string(to.S); s += " ";
	return s;
}

void ApnaMove(board& B, int rings_rem[]){
	vector< pair<pii,pii> > markerSeq;
	string outputStr;
	bool selectMoveDone = false;

	generateAllSequence:
	markerSeq = B.generateAllSequences(0);
	while(markerSeq.size()){

		pair<pii,pii> greedyBestSeq;
		if(markerSeq.size() == 1){
			greedyBestSeq = markerSeq[0];
		}
		else{
			greedyBestSeq = SelectMove3RemoveSeq(B);
		}
		
		outputStr += removeMarkerStringOut(greedyBestSeq);
		B.removeSequence(0,greedyBestSeq.F,greedyBestSeq.S);
		
		pii greedyBestRing;
		if(B.ringsPos[0].size() == 1){
			greedyBestRing = B.ringsPos[0][0];
		}
		else{
			greedyBestRing = SelectMove2RemoveRing(B);
		}
		
		outputStr += removeRingStringOut(greedyBestRing);
		B.removeRing(0,greedyBestRing);
		rings_rem[0]--;

		if(GameOver(rings_rem)) break;
		markerSeq = B.generateAllSequences(0);
	}

	if(GameOver(rings_rem)){
		outputStr.pop_back(); 
	 	cout<<outputStr<<endl;
	 	// degub output
	 	cerr<<outputStr<<endl;
		return;
	}

	if(!selectMoveDone)
	{
		pii from, to;
		pair<pii,pii> greedyBestMove = SelectMove1MoveRing(B);
		from = greedyBestMove.F;
		to   = greedyBestMove.S;
		outputStr += moveRingStringOut(from, to);
		B.moveRing(0,from,to);
		selectMoveDone = true;
		goto generateAllSequence;
	}


	outputStr.pop_back(); 
	cout<<outputStr<<endl;	
	// degub output
	cerr<<outputStr<<endl;
}
