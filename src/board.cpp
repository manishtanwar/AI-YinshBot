#include "board.h"

int board::n;
int board::m;
int board::k;
int board::l;
map<pii,pii> board::forwardConversionTable;
map<pii,pii> board::backwardConversionTable;

// helper  vector for converting coordinates
vector<pii> dir = {{1,0}, {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,1}};
void board :: init(){
	pii p,ans;
	
	p = mp(0,0);
	ans = mp(0,0);
	ans.F += n; ans.S += n;
	forwardConversionTable[p] = ans;
	backwardConversionTable[ans] = p;
	mat[ans.F][ans.S] = 0;

	for(int i=1;i<=n;i++){
		for(int j=0;j<6*i;j++){
			p = mp(i,j);
			int lineNum = p.S / p.F;
			int pointNumOnLine = p.S % p.F;
			ans = mp(0,p.F);
			for(int k=0;k<lineNum;k++){
				ans.F += dir[k].F * p.F;
				ans.S += dir[k].S * p.F;
			}
			ans.F += dir[lineNum].F * pointNumOnLine;
			ans.S += dir[lineNum].S * pointNumOnLine;

			ans.F += n; ans.S += n;
			forwardConversionTable[p] = ans;
			backwardConversionTable[ans] = p;

			// checking if the board position is valid or not
			if( !(i == n && j%n == 0) )
				mat[ans.F][ans.S] = 0;
		}
	}
}

board::board(){
	mat = new int*[2*n+1];
	for(int i=0; i<2*n+1; i++)
		mat[i] = new int[2*n+1];

	for(int i=0; i<2*n+1; i++)
		for(int j=0; j<2*n+1; j++)
			mat[i][j] = -3;
	init();

	markerCnt[0] = markerCnt[1] = 0;
}

board :: board(board const& original){
	mat = new int*[2*n+1];
	for(int i=0; i<2*n+1; i++)
		mat[i] = new int[2*n+1];

	for(int i=0; i<2*n+1; i++)
		for(int j=0; j<2*n+1; j++)
			mat[i][j] = original.mat[i][j];

	markerCnt[0] = original.markerCnt[0];
	markerCnt[1] = original.markerCnt[1];

	for(int j=0;j<2;j++)
	for(int i=0;i < original.ringsPos[j].size(); i++)
		ringsPos[j].push_back(original.ringsPos[j][i]);
}

board :: ~board(){
    for(int i=0; i<2*n+1; i++){
        delete[] mat[i];
    }
    delete[] mat;
}

// it converts (hexNumber,PointNumber) -> (x,y) as in our coordinate system
pii board :: convertCoordinatesForward(pii p){
	return forwardConversionTable[p];
}
pii board :: convertCoordinatesBackward(pii p){
	return backwardConversionTable[p];
}


bool board :: inTheBoard(pii p){
	if(p.F < 0 || p.S < 0) return false;
	if(p.F >= 2 * (board::n) + 1 || p.S >= 2 * (board::n) + 1) return false;
	return true;
}

// helper vector for generating neighbours
vector<pii> neighboursDir = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,-1}};

vector<pii> board :: generateNeighbours(pii ringPos){
	vector<pii> ans; pii curr;
	for(auto z : neighboursDir){
		curr = {ringPos.F + z.F, ringPos.S + z.S};
		bool metAMarker = 0;

		while(inTheBoard(curr) && (mat[curr.F][curr.S] == 0 || abs(mat[curr.F][curr.S]) == 2)){
			if(abs(mat[curr.F][curr.S]) == 2) metAMarker = 1;
			if(mat[curr.F][curr.S] == 0 && metAMarker){
				ans.pb(curr); break;
			}
			if(mat[curr.F][curr.S] == 0) ans.pb(curr);
			curr.F += z.F; curr.S += z.S;
		}
	}
	return ans;
}

bool board :: isEndOfSeq(pii p, pii dir, int coeff){
	p.F += coeff * dir.F;
	p.S += coeff * dir.S;
	if(!inTheBoard(p)) return true;
	if(mat[p.F][p.S] == 2) return false;
	return true;
}

// helper vector for generating sequences
vector<pii> neighboursDirOneSided = {{1,0},{0,1},{1,1}};

vector< pair<pii,pii> > board :: generateAllSequences(int player){
	vector< pair<pii,pii> > ans;
	int reqThing = (player == 0) ? 2 : -2;

	for(int i=0;i<2*n+1;i++){
		for(int j=0;j<2*n+1;j++){
			if(mat[i][j] != reqThing) continue;
			pii st = {i,j};
			for(auto z : neighboursDirOneSided){
				pii en = {st.F + z.F, st.S + z.S}; 
				int length = 1;
				while(inTheBoard(en) && length < k){
					if(mat[en.F][en.S] != reqThing) break; 
					length++;
					en.F += z.F; en.S += z.S;
				}
				en.F -= z.F; en.S -= z.S;
				// pushing this sequence if it is valid
				if(length == k) ans.pb({st,en});
			}
		}
	}
	return ans;
}

void board :: placeRing( int player, pii at ){
	// add ring to ringPos(itions)
	ringsPos[player].push_back(at);
	// add ring to board
	mat[at.first][at.second] = ( player == 0 ? 1 : -1 );
}

void board :: moveRing( int player, pii from, pii to ){

	// STEP 1: remove ring and place a marker on ring's position
	removeRing( player, from );
	mat[from.first][from.second] = ( player == 0 ? 2 : -2 );
	markerCnt[player]++;

	// get directions of movement
	int xDir = (to.first == from.first ? 0 : ( to.first - from.first )/(abs( to.first - from.first )) ); 
	int yDir = (to.second == from.second ? 0 : ( to.second - from.second )/(abs( to.second - from.second )) ); 
	
	int startX = from.first + xDir, startY = from.second + yDir;
	int endX = to.first, endY = to.second;
	// STEP 2: move from start to end flipping markers and skipping empty places

	while(!(startX == endX && startY == endY)){
		if( mat[startX][startY] == 2  ){
			mat[startX][startY] = -2;
			markerCnt[0]--; markerCnt[1]++;
		}
		else if( mat[startX][startY] == -2 ){ 
			mat[startX][startY] = 2;
			markerCnt[1]--; markerCnt[0]++;
		}
		startX += xDir;
		startY += yDir;
	}

	// STEP 3: place the ring on final positon
	placeRing( player, to );
}

void board :: removeRing( int player, pii from ){
	// remove ring from ringPos(itions)
	int pos = 0;
	for(int i=0; i < ringsPos[player].size(); i++){
		if(from == ringsPos[player][i]) {pos = i; break;}
	}
	swap(ringsPos[player][pos], ringsPos[player][ (int)ringsPos[player].size() - 1 ]);
	ringsPos[player].pop_back();
	// ringsPos[player].erase( remove( ringsPos[player].begin(), ringsPos[player].end(), from ), ringsPos[player].end() );
	// empty the board position
	mat[from.first][from.second] = 0;
}

void board :: removeSequence( int player, pii from, pii to ){
	// get directions of movement
	int xDir = (to.first == from.first ? 0 : ( to.first - from.first )/(abs( to.first - from.first )) ); 
	int yDir = (to.second == from.second ? 0 : ( to.second - from.second )/(abs( to.second - from.second )) ); 
	
	int startX = from.first, startY = from.second;
	int endX = to.first + xDir, endY = to.second + yDir;
	// move from start to end removing markers
	while(!(startX == endX && startY == endY)){
		mat[startX][startY] = 0;
		markerCnt[player]--;
		startX += xDir;
		startY += yDir;
	}
}
