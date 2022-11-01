#include <iostream>
#include <vector>
#include <sstream>
#include<string>
#include<algorithm>
#include<iterator>
using namespace std;

#define BLACK 'b'
#define WHITE 'w'
#define DEPTH 2
class Piece;
struct Gamestate{
    vector<Piece> WhiteP;
    vector<Piece> BlackP;
    char currColor;
    int currTurn;
    int currScore;
    vector<vector<char>> currBoard;
    string currFEN;
};
struct Gamestate startState; //make a state of the game
vector<char> WP = {'P', 'S', 'G', 'M', 'E', 'L', 'C', 'Z'};
//Pawn(0-6) Superpawn(7-13) giraffe(14) monkey(15) elephant(16-17) lion(18) crocodile(19) zebra(20)
vector<char> allWhitePieces = {'P','P','P','P','P','P','P',
                             'S','S','S','S','S','S','S',
                             'G','M','E','E','L','C','Z'};
vector<char> BP = {'p', 's', 'g', 'm', 'e', 'l', 'c', 'z'};
vector<char> allBlackPieces = {'p','p','p','p','p','p','p',
                             's','s','s','s','s','s','s',
                             'g','m','e','e','l','c','z'};
vector<char> files = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
char nextMove;
int turnCount=0;
int rawScore=0;
vector<vector<char>> board;
vector<pair<int, int>> checkLionEat(vector<pair<int, int>> newAvailMoves, char color);
vector<pair<int, int>> checkGiraffeEat(vector<pair<int, int>> out, char color);
void sortPawns(char color);
void sortSuperPawns(char color);
void printBoard(vector<vector<char>> board);
char readFENString(string fen);
void resetBoard();
string printFENString(char NextMove);
string generateNewFENString(struct Gamestate currState);
string printLionMoves(struct Gamestate currState);
string printZebraMoves(struct Gamestate currState);
string printGiraffeMoves(struct Gamestate currState);
string printPawnMoves(struct Gamestate currState);
string printSuperPawnMoves(struct Gamestate currState);
vector<string> getAllMoves(struct Gamestate currState);

char convertFile(int newFile){
        return files[newFile];
}

int convertFileToInt(char file){
    auto fileIT = find(files.begin(), files.end(), file);
    if(fileIT!=files.end()){
        int index = fileIT - files.begin();
        return index;
    }else return -1;
}

class Piece{            //Base Piece class
public:
    //variables
    vector<int> position = {1,0};    //1-7 row
    char file = 'a';        //a-g column
    char color = WHITE;       //B or W
    bool alive = false;
    vector<pair<int, int>> availMoves;
    vector<pair<int, int>> allMoves;

    //Getters
    char getColor(){
        return color;
    }
    char getFile(){
        return file;
    }
    vector<int> getPosition(){
        return position;
    }
    bool getAlive(){
        return alive;
    }

    //Setters
    void setPosition(vector<int> newPosition){
        position = newPosition;
        setFile(position[1]);
    }
    void setAlive(bool newAlive){
        alive = newAlive;
    }

    //Remove own pieces from avail moves list
    vector<pair<int, int>> getOwnPieces(vector<pair<int, int>> out, char color){
        vector<pair<int, int>> notAvailMoves;
        vector<pair<int, int>> newAvailMoves;

        if(color==BLACK){
            for(int i=0;i<out.size();i++){
                if(board[out[i].second-1][out[i].first]!='0'){
                    for(int j=0;j<BP.size();j++){
                        if(board[out[i].second-1][out[i].first]==BP[j]){
                            notAvailMoves.push_back({out[i].first, out[i].second});
                            break;
                        }
                    }
                }
            }
        }else{
            for(int i=0;i<out.size();i++){
                if(board[out[i].second-1][out[i].first]!='0'){
                    for(int j=0;j<WP.size();j++){
                        if(board[out[i].second-1][out[i].first]==WP[j]){
                            notAvailMoves.push_back({out[i].first, out[i].second});
                            break;
                        }
                    }
                }
            }
        }

        for(int i=0; i<out.size();i++){
            bool flag = false;
            for(int j=0; j<notAvailMoves.size();j++){
                if(out[i]==notAvailMoves[j]) flag=true;
            }
            if(!flag){
                newAvailMoves.push_back(out[i]);
            }
        }
        return newAvailMoves;
    }

    //piece specific stuff
    void setAvailLionMoves(){
        if(color==nextMove){

            if(color==WHITE){ //check out of bounds
                if(position[0]>3 || position[1]<2 || position[1]>4){ //if not in white castle
                    availMoves.clear();
                    return;
                }

                if(position[0]-1>=1){ //set avail moves
                    allMoves.push_back({position[1],position[0]-1});
                    if(position[1]-1>=2) allMoves.push_back({position[1]-1,position[0]-1});
                    if(position[1]+1<=4) allMoves.push_back({position[1]+1,position[0]-1});
                }
                if(position[0]+1<=3){
                    allMoves.push_back({position[1],position[0]+1});
                    if(position[1]-1>=2) allMoves.push_back({position[1]-1,position[0]+1});
                    if(position[1]+1<=4) allMoves.push_back({position[1]+1,position[0]+1});
                }
                if(position[1]-1>=2){
                    allMoves.push_back({position[1]-1,position[0]});
                }
                if(position[1]+1<=4){
                    allMoves.push_back({position[1]+1,position[0]});
                }
            }else{
                if(position[0]<5 || position[1]<2 || position[1]>4){ //if not in black castle
                    availMoves.clear();
                    return;
                }

                if(position[0]-1>=5){ //set avail moves
                    allMoves.push_back({position[1],position[0]-1});
                    if(position[1]-1>=2) allMoves.push_back({position[1]-1,position[0]-1});
                    if(position[1]+1<=4) allMoves.push_back({position[1]+1,position[0]-1});
                }
                if(position[0]+1<=7){
                    allMoves.push_back({position[1],position[0]+1});
                    if(position[1]-1>=2) allMoves.push_back({position[1]-1,position[0]+1});
                    if(position[1]+1<=4) allMoves.push_back({position[1]+1,position[0]+1});
                }
                if(position[1]-1>=2){
                    allMoves.push_back({position[1]-1,position[0]});
                }
                if(position[1]+1<=4){
                    allMoves.push_back({position[1]+1,position[0]});
                }
            }

            availMoves = checkLionEat(allMoves, color);
            availMoves = getOwnPieces(availMoves,nextMove);
        }
    }
    void setAvailZebraMoves(){
        if(color==nextMove){
            if(position[0]-2>=1){
                if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]-2});
                if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]-2});
            }
            if(position[0]+2<=7){
                if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]+2});
                if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]+2});
            }
            if(position[1]-2>=0){
                if(position[0]-1>=1) allMoves.push_back({position[1]-2,position[0]-1});
                if(position[0]+1<=7) allMoves.push_back({position[1]-2,position[0]+1});
            }
            if(position[1]+2<=6){
                if(position[0]-1>=1) allMoves.push_back({position[1]+2,position[0]-1});
                if(position[0]+1<=7) allMoves.push_back({position[1]+2,position[0]+1});
            }
            availMoves = getOwnPieces(allMoves,nextMove);
        }
    }
    void setAvailGiraffeMoves(){
        if(color==nextMove){
            if(position[0]-1>=1){
                allMoves.push_back({position[1],position[0]-1});
                if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]-1});
                if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]-1});
            }
            if(position[0]+1<=7){
                allMoves.push_back({position[1],position[0]+1});
                if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]+1});
                if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]+1});
            }
            if(position[1]-1>=0){
                allMoves.push_back({position[1]-1,position[0]});
            }
            if(position[1]+1<=6){
                allMoves.push_back({position[1]+1,position[0]});
            }
            availMoves = checkGiraffeEat(allMoves,nextMove);

            if(position[0]-2>=1){
                availMoves.push_back({position[1],position[0]-2});
                if(position[1]-2>=0) availMoves.push_back({position[1]-2,position[0]-2});
                if(position[1]+2<=6) availMoves.push_back({position[1]+2,position[0]-2});
            }
            if(position[0]+2<=7){
                availMoves.push_back({position[1],position[0]+2});
                if(position[1]-2>=0) availMoves.push_back({position[1]-2,position[0]+2});
                if(position[1]+2<=6) availMoves.push_back({position[1]+2,position[0]+2});
            }
            if(position[1]-2>=0){
                availMoves.push_back({position[1]-2,position[0]});
            }
            if(position[1]+2<=6){
                availMoves.push_back({position[1]+2,position[0]});
            }

            availMoves = getOwnPieces(availMoves,nextMove);
        }
    }
    void setAvailPawnMoves(){
        if(color==nextMove){
            if(color==WHITE){
                if(position[0]+1<=7){
                    allMoves.push_back({position[1],position[0]+1});
                    if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]+1});
                    if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]+1});
                }
                if(position[0]>4){
                    if(board[position[0]-2][position[1]]=='0') allMoves.push_back({position[1],position[0]-1});
                    if(board[position[0]-3][position[1]]=='0' && board[position[0]-2][position[1]]=='0') allMoves.push_back({position[1],position[0]-2});
                }
            }else{
                if(position[0]-1>=1){
                    allMoves.push_back({position[1],position[0]-1});
                    if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]-1});
                    if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]-1});
                }
                if(position[0]<4){
                    if(board[position[0]][position[1]]=='0') allMoves.push_back({position[1],position[0]+1});
                    if(board[position[0]+1][position[1]]=='0' && board[position[0]][position[1]]=='0') allMoves.push_back({position[1],position[0]+2});
                }
            }
            availMoves = getOwnPieces(allMoves,nextMove);
        }
    }
    void setAvailSuperPawnMoves(){
        if(color==nextMove){
            if(color==WHITE){
                if(position[0]+1<=7){ //1 forward
                    allMoves.push_back({position[1],position[0]+1});
                    if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]+1}); //1 forward 1 left
                    if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]+1}); //1 forward 1 right
                }
                if(position[1]-1>=0){
                    allMoves.push_back({position[1]-1,position[0]}); //1 left
                }
                if(position[1]+1<=6){
                    allMoves.push_back({position[1]+1,position[0]}); //1 right
                }
                if(position[0]-1>=1){ //1 move back on board
                    if(board[position[0]-2][position[1]]=='0') allMoves.push_back({position[1],position[0]-1});//1 move back empty

                    if(position[1]-1>=0){//1 back 1 left on board
                        if(board[position[0]-2][position[1]-1]=='0') allMoves.push_back({position[1]-1,position[0]-1});//1 back 1 left empty
                    }
                    if(position[1]+1<=6){//1 back 1 right on board
                        if(board[position[0]-2][position[1]+1]=='0') allMoves.push_back({position[1]+1,position[0]-1});//1 back 1 right empty
                    }
                }

                if(position[0]-2>=1){ //2 back on board
                    if(board[position[0]-3][position[1]]=='0' && board[position[0]-2][position[1]]=='0') allMoves.push_back({position[1],position[0]-2});//2 back empty

                    if(position[1]-2>=0){//2 back 2 left on board
                        if(board[position[0]-3][position[1]-2]=='0' && board[position[0]-2][position[1]-1]=='0') allMoves.push_back({position[1]-2,position[0]-2});//2 back 2 left empty
                    }
                    if(position[1]+2<=6){//2 back 2 right on board
                        if(board[position[0]-3][position[1]+2]=='0' && board[position[0]-2][position[1]+1]=='0') allMoves.push_back({position[1]+2,position[0]-2});//2 back 2 right empty
                    }
                }
            }else{

                if(position[0]-1>=1){
                    allMoves.push_back({position[1],position[0]-1});
                    if(position[1]-1>=0) allMoves.push_back({position[1]-1,position[0]-1});
                    if(position[1]+1<=6) allMoves.push_back({position[1]+1,position[0]-1});
                }
                if(position[1]-1>=0){
                    allMoves.push_back({position[1]-1,position[0]});
                }
                if(position[1]+1<=6){
                    allMoves.push_back({position[1]+1,position[0]});
                }
                if(position[0]+1<=7){ //1 move back on board
                    if(board[position[0]][position[1]]=='0') allMoves.push_back({position[1],position[0]+1});//1 move back empty

                    if(position[1]-1>=0){//1 back 1 left on board
                        if(board[position[0]][position[1]-1]=='0') allMoves.push_back({position[1]-1,position[0]+1});//1 back 1 left empty
                    }
                    if(position[1]+1<=6){//1 back 1 right on board
                        if(board[position[0]][position[1]+1]=='0') allMoves.push_back({position[1]+1,position[0]+1});//1 back 1 right empty
                    }
                }

                if(position[0]+2<=7){ //2 back on board
                    if(board[position[0]+1][position[1]]=='0' && board[position[0]][position[1]]=='0') allMoves.push_back({position[1],position[0]+2});//2 back empty

                    if(position[1]-2>=0){//2 back 2 left on board
                        if(board[position[0]+1][position[1]-2]=='0' && board[position[0]][position[1]-1]=='0') allMoves.push_back({position[1]-2,position[0]+2});//2 back 2 left empty
                    }
                    if(position[1]+2<=6){//2 back 2 right on board
                        if(board[position[0]+1][position[1]+2]=='0' && board[position[0]][position[1]+1]=='0') allMoves.push_back({position[1]+2,position[0]+2});//2 back 2 right empty
                    }
                }
            }
            availMoves = getOwnPieces(allMoves,nextMove);
        }
    }

protected:
    void setFile(int newFile){
        file = files[newFile];
    }
};

class Pawn: public Piece{            //Pawn Piece subclass
public:
    string name = "Pawn";

    //Constructors
    Pawn(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class SuperPawn: public Piece{            //Pawn Piece subclass
public:
    string name = "Superpawn";

    //Constructors
    SuperPawn(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class Lion: public Piece{            //Lion Piece subclass
public:
    string name = "Lion";

    //Constructors
    Lion(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class Zebra: public Piece{            //Zebra Piece subclass
public:
    string name = "Zebra";

    //Constructors
    Zebra(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class Elephant: public Piece{            //Elephant Piece subclass
public:
    string name = "Elephant";

    //Constructors
    Elephant(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class Giraffe: public Piece{            //Giraffe Piece subclass
public:
    string name = "Giraffe";

    //Constructors
    Giraffe(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class Crocodile: public Piece{            //Crocodile Piece subclass
public:
    string name = "Crocodile";

    //Constructors
    Crocodile(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

class Monkey: public Piece{            //Monkey Piece subclass
public:
    string name = "Monkey";

    //Constructors
    Monkey(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile(position[1]);
        color = newColor;
    }
};

//-----------------------------------------------------------------------------------------------------
vector<Piece> BlackPieces; //Pawn(0-6) Superpawn(7-13) giraffe(14) monkey(15) elephant(16-17) lion(18) crocodile(19) zebra(20)
vector<Piece> WhitePieces;

int getPiece(struct Gamestate currState, char Tag, vector<int> pos, char color){
    if(color==WHITE){
        auto tagIT = find(allWhitePieces.begin(), allWhitePieces.end(), Tag);
        if(tagIT!=allWhitePieces.end()){
            int index = tagIT - allWhitePieces.begin();
            if(index<7){
                for(int i=0;i<7;i++){
                    if(!currState.WhiteP[i].alive) continue;
                    if(currState.WhiteP[i].position==pos) return i;
                }
            }else if(index<14){
                for(int i=7;i<14;i++){
                    if(!currState.WhiteP[i].alive) continue;
                    if(currState.WhiteP[i].position==pos) return i;
                }
            }else return index;
        }else return -1;
    }else{
        auto tagIT = find(allBlackPieces.begin(), allBlackPieces.end(), Tag);
        if(tagIT!=allBlackPieces.end()){
            int index = tagIT - allBlackPieces.begin();
            if(index<7){
                for(int i=0;i<7;i++){
                    if(!currState.BlackP[i].alive) continue;
                    if(currState.BlackP[i].position==pos) return i;
                }
            }else if(index<14){
                for(int i=7;i<14;i++){
                    if(!currState.BlackP[i].alive) continue;
                    if(currState.BlackP[i].position==pos) return i;
                }
            }else return index;
        }else return -1;
    }
}

struct Gamestate evolvePawns(struct Gamestate currState){
    if(currState.currColor==WHITE){
        for(int i=0;i<7;i++){
            if(!currState.WhiteP[i].alive) continue;
            if(currState.WhiteP[i].position[0]==7){
                currState.currBoard[6][currState.WhiteP[i].position[1]] = 'S';
                currState.WhiteP[i].setAlive(false);
                for(int j=7;j<14;j++){
                    if(!currState.WhiteP[j].alive){
                        currState.WhiteP[j].setAlive(true);
                        currState.WhiteP[j].setPosition(currState.WhiteP[i].getPosition());
                        break;
                    }
                }
            }
        }
    }else{
        for(int i=0;i<7;i++){
            if(!currState.BlackP[i].alive) continue;
            if(currState.BlackP[i].position[0]==1){
                currState.currBoard[0][currState.BlackP[i].position[1]] = 's';
                currState.BlackP[i].setAlive(false);
                for(int j=7;j<14;j++){
                    if(!currState.BlackP[j].alive){
                        currState.BlackP[j].setAlive(true);
                        currState.BlackP[j].setPosition(currState.BlackP[i].getPosition());
                        break;
                    }
                }
            }
        }
    }

    return currState;
}

struct Gamestate makeMove(struct Gamestate currState, string myMove){
    struct Gamestate nextState;
    int fileStart = convertFileToInt(myMove[0]);
    int rankStart = myMove[1] - '0';
    int fileEnd = convertFileToInt(myMove[2]);
    int rankEnd = myMove[3] - '0';
    string fen2 = " ";
    char color = currState.currColor;

    if(fileStart==-1 || fileEnd==-1) cout << "Invalid move" << endl;

    char startPiece = currState.currBoard[rankStart-1][fileStart];
    char endPiece = currState.currBoard[rankEnd-1][fileEnd];
    vector<int> startPos = {rankStart, fileStart};
    vector<int> endPos = {rankEnd, fileEnd};
    int PieceIndex = getPiece(currState, startPiece, startPos, color);

    if(color==WHITE){
        if(PieceIndex!=-1){
            currState.WhiteP[PieceIndex].setPosition(endPos); //update Piece pos
            currState.currBoard[rankStart-1][fileStart] = '0';         //update board
            currState.currBoard[rankEnd-1][fileEnd] = startPiece;
        }

        if(endPiece!='0'){ //if endPos is enemy piece
            int capturePiece = getPiece(currState, endPiece, endPos, BLACK);
            if(capturePiece!=-1){
                currState.BlackP[capturePiece].setAlive(false); //capture and set alive false
            }
        }

        for(int i=0;i<7;i++){
            if(currState.currBoard[3][i]!='0'){ //if piece in river is a piece
                vector<int> pawnPos = {4, i};
                int RiverPiece = getPiece(currState, currState.currBoard[3][i], pawnPos, color);
                if(RiverPiece!=-1){ //if piece in river is not the moved piece drown it
                    if(RiverPiece!=PieceIndex){ // if river piece is not the same as moved piece, drown
                        currState.WhiteP[RiverPiece].setAlive(false);
                        currState.currBoard[3][i] = '0';
                    }else if(rankStart==rankEnd){ //if moved piece started and ended in river, drown
                        currState.WhiteP[RiverPiece].setAlive(false);
                        currState.currBoard[3][i] = '0';
                    }
                }
            }
        }

        fen2 += BLACK;
        fen2 += " " + to_string(currState.currTurn);
    }else{
        nextState.currColor = WHITE;

        currState.currTurn++; //after black moves, turnCount increments
        if(PieceIndex!=-1){
            currState.BlackP[PieceIndex].setPosition(endPos); //update Piece pos
            currState.currBoard[rankStart-1][fileStart] = '0';         //update board
            currState.currBoard[rankEnd-1][fileEnd] = startPiece;
        }

        if(endPiece!='0'){ //if endPos is enemy piece
            int capturePiece = getPiece(currState, endPiece, endPos, WHITE);
            if(capturePiece!=-1){
                currState.WhiteP[capturePiece].setAlive(false); //capture and set alive false
            }
        }

        for(int i=0;i<7;i++){
            if(currState.currBoard[3][i]!='0'){ //if piece in river is a piece
                vector<int> pawnPos = {4, i};
                int RiverPiece = getPiece(currState, currState.currBoard[3][i], pawnPos, color);
                if(RiverPiece!=-1){ //if piece in river is not the moved piece drown it
                    if(RiverPiece!=PieceIndex){ // if river piece is not the same as moved piece, drown
                        currState.BlackP[RiverPiece].setAlive(false);
                        currState.currBoard[3][i] = '0';
                    }else if(rankStart==rankEnd){ //if moved piece started and ended in river, drown
                        currState.BlackP[RiverPiece].setAlive(false);
                        currState.currBoard[3][i] = '0';
                    }
                }
            }
        }

        fen2 += WHITE;
        fen2 += " " + to_string(currState.currTurn);
    }

    currState = evolvePawns(currState);
    nextState.currFEN = generateNewFENString(currState) + fen2;
    resetBoard();
    readFENString(nextState.currFEN);

    nextState.BlackP = BlackPieces;
    nextState.WhiteP = WhitePieces;
    nextState.currBoard = currState.currBoard;
    nextState.currTurn = turnCount;

    return nextState;
}

//rawScore = material + mobility + attack
//material
int calcMaterialScore(struct Gamestate currState){
    int WhiteScore=0;
    int BlackScore=0;
    char color = currState.currColor;

    //Winning Conditions
    if(!currState.WhiteP[18].alive){
        if(color==WHITE) rawScore = -10000;
        else rawScore = 10000;
        return rawScore;
    }
    if(!currState.BlackP[18].alive){
        if(color==WHITE) rawScore = 10000;
        else rawScore = -10000;
        return rawScore;
    }

    //White Score
    for(int i=0;i<7;i++){
        if(currState.WhiteP[i].alive) WhiteScore +=100;
    }
    for(int i=7;i<14;i++){
        if(currState.WhiteP[i].alive) WhiteScore +=350;
    }
    if(currState.WhiteP[14].alive) WhiteScore +=400;
    if(currState.WhiteP[20].alive) WhiteScore +=300;

    //Black Score
    for(int i=0;i<7;i++){
            if(currState.BlackP[i].alive) BlackScore +=100;
        }
        for(int i=7;i<14;i++){
            if(currState.BlackP[i].alive) BlackScore +=350;
        }
        if(currState.BlackP[14].alive) BlackScore +=400;
        if(currState.BlackP[20].alive) BlackScore +=300;

    //delta score
    if(color==WHITE) rawScore = WhiteScore-BlackScore;
    else rawScore = BlackScore-WhiteScore;
    return rawScore;
}
//mobility
int calcMobilityScore(struct Gamestate currState){
    int Mobility=0;
    int currMobility = getAllMoves(currState).size();
    stringstream ss(currState.currFEN);
    string tempMove;
    ss >> tempMove;

    struct Gamestate tempState;
    if(currState.currColor==WHITE){
        tempState.currFEN = tempMove + " " + BLACK + " " + to_string(currState.currTurn);
        tempState.currColor = BLACK;
    }else{
        tempState.currFEN = tempMove + " " + WHITE + " " + to_string(currState.currTurn);
        tempState.currColor = WHITE;
    }

    resetBoard();
    readFENString(tempState.currFEN);
    tempState.BlackP = BlackPieces;
    tempState.WhiteP = WhitePieces;
    tempState.currBoard = currState.currBoard;
    int nextMobility = getAllMoves(tempState).size();

    //delta score
    Mobility = currMobility-nextMobility;
    return Mobility;
}
//attack
int calcAttackScore(struct Gamestate currState){
    int attackScore =0;
    int currAttack =0;
    int nextAttack =0;
    vector<string> allMoves = getAllMoves(currState);
    //get current attack score
    for(int k=0; k<allMoves.size();k++){ //for every move
        vector<int> startPos = {allMoves[k][2] -49, convertFileToInt(allMoves[k][0])};
        char moveToBlock = currState.currBoard[allMoves[k][3] -49][convertFileToInt(allMoves[k][2])];
        if(moveToBlock!='0'){
            if(currState.currColor==WHITE){
                int index = getPiece(currState, moveToBlock, startPos, BLACK);
                if(index==18) currAttack +=10;
                if(index!=-1) currAttack +=1;
            }else{
                int index = getPiece(currState, moveToBlock, startPos, WHITE);
                if(index==18) currAttack +=10;
                if(index!=-1) currAttack +=1;
            }
        }
    }

    //generate next state
    stringstream ss(currState.currFEN);
    string tempMove;
    ss >> tempMove;

    struct Gamestate tempState;
    if(currState.currColor==WHITE){
        tempState.currFEN = tempMove + " " + BLACK + " " + to_string(currState.currTurn);
        tempState.currColor = BLACK;
    }else{
        tempState.currFEN = tempMove + " " + WHITE + " " + to_string(currState.currTurn);
        tempState.currColor = WHITE;
    }
    resetBoard();
    readFENString(tempState.currFEN);
    tempState.BlackP = BlackPieces;
    tempState.WhiteP = WhitePieces;
    tempState.currBoard = currState.currBoard;
    vector<string> tempMoves = getAllMoves(tempState);
    //get next state score
    for(int k=0; k<tempMoves.size();k++){ //for every move
        vector<int> startPos = {tempMoves[k][2] -49, convertFileToInt(tempMoves[k][0])};
        char moveToBlock = tempState.currBoard[tempMoves[k][3] -49][convertFileToInt(tempMoves[k][2])];
        if(moveToBlock!='0'){
            if(tempState.currColor==WHITE){
                int index = getPiece(tempState, moveToBlock, startPos, BLACK);
                if(index==18) nextAttack +=10;
                if(index!=-1) nextAttack +=1;
            }else{
                int index = getPiece(tempState, moveToBlock, startPos, WHITE);
                if(index==18) nextAttack +=10;
                if(index!=-1) nextAttack +=1;
            }
        }
    }

    //delta score
    attackScore = currAttack-nextAttack;
    return attackScore;
}
//rawScore
int calcRawScore(struct Gamestate currState){
    int materialScore = calcMaterialScore(currState);
    if(materialScore==0){
        for(int i=0;i<20;i++){
            if(i!=18 && (currState.WhiteP[i].alive || currState.BlackP[i].alive)) break;
            return 0;
        }
    }

    if(materialScore==10000) return 10000;
    if(materialScore==-10000) return -10000;

    int mobilityScore = calcMobilityScore(currState);
    int attackScore = calcAttackScore(currState);
    rawScore = materialScore + mobilityScore + attackScore;
    return rawScore;
}

bool isGameOver(struct Gamestate currState){
    if(!currState.WhiteP[18].alive || !currState.BlackP[18].alive) return true;
    return false;
}

int performMinMax(struct Gamestate currState, int currDepth){
    if(isGameOver(currState) || currDepth <=0){ //if any lions are dead, or if reached end of depth search
        //cout << calcRawScore(currState) <<endl;
        return calcRawScore(currState);     //calc score of current state
    }
    int tempVal = -1000000; //very large negative number
    vector<string> allMoves = getAllMoves(currState); //get all moves for all pieces

    for(int i=0; i<allMoves.size();i++){ //for every move
        struct Gamestate nextState = makeMove(currState, allMoves[i]); //get next state for each move
        //cout << allMoves[i] << " " << nextState.currFEN << endl;

        //printBoard(nextState.currBoard);
        //cout << endl;

        int eval = -performMinMax(nextState, currDepth-1); //recurse
        tempVal = max(tempVal, eval);

    }
    return tempVal;
}

vector<pair<int, int>> checkLionEat(vector<pair<int, int>> newAvailMoves, char color){
    /*if(!WhitePieces[18].alive || !BlackPieces[18].alive){
        return newAvailMoves;
    }*/

    int l = BlackPieces[18].position[0] - WhitePieces[18].position[0]; //length between 2 lions
    bool blocked = false; //blocked flag variable
    if(color==WHITE){
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==2){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==4 && board[3][3]=='0'){
                newAvailMoves.insert(newAvailMoves.begin(), 1, {BlackPieces[18].position[1], BlackPieces[18].position[0]});
                return newAvailMoves;
            }
        }
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==4){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==2 && board[3][3]=='0'){
                newAvailMoves.insert(newAvailMoves.begin(), 1, {BlackPieces[18].position[1], BlackPieces[18].position[0]});
                return newAvailMoves;
            }
        }
        if(WhitePieces[18].position[1]==BlackPieces[18].position[1]){
            for(int i=WhitePieces[18].position[0]; i<l+WhitePieces[18].position[0]-1;i++){
                if(board[i][WhitePieces[18].position[1]]!='0'){
                    blocked=true;
                    break;
                }
            }
            if(!blocked)  newAvailMoves.insert(newAvailMoves.begin(), 1, {BlackPieces[18].position[1], BlackPieces[18].position[0]});
        }
    }else{
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==2){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==4 && board[3][3]=='0'){
                newAvailMoves.insert(newAvailMoves.begin(), 1, {WhitePieces[18].position[1], WhitePieces[18].position[0]});
                return newAvailMoves;
            }
        }
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==4){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==2 && board[3][3]=='0'){
                newAvailMoves.insert(newAvailMoves.begin(), 1, {WhitePieces[18].position[1], WhitePieces[18].position[0]});
                return newAvailMoves;
            }
        }
        if(BlackPieces[18].position[1]==WhitePieces[18].position[1] && l==2 && board[3][WhitePieces[18].position[1]]=='0'){
            newAvailMoves.insert(newAvailMoves.begin(), 1, {WhitePieces[18].position[1], WhitePieces[18].position[0]});
            return newAvailMoves;
        }
        if(WhitePieces[18].position[1]==BlackPieces[18].position[1]){
            for(int i=WhitePieces[18].position[0]; i<l+WhitePieces[18].position[0]-1;i++){
                if(board[i][WhitePieces[18].position[1]]!='0'){
                    blocked=true;
                    break;
                }
            }
            if(!blocked)  newAvailMoves.insert(newAvailMoves.begin(), 1, {WhitePieces[18].position[1], WhitePieces[18].position[0]});
        }
    }
    return newAvailMoves;
}

vector<pair<int, int>> checkGiraffeEat(vector<pair<int, int>> out, char color){
        vector<pair<int, int>> notAvailMoves;
        vector<pair<int, int>> newAvailMoves;

        for(int i=0;i<out.size();i++){
            if(board[out[i].second-1][out[i].first]!='0'){
                notAvailMoves.push_back({out[i].first, out[i].second});
            }
        }

        for(int i=0; i<out.size();i++){
            bool flag = false;
            for(int j=0; j<notAvailMoves.size();j++){
                if(out[i]==notAvailMoves[j]) flag=true;
            }
            if(!flag){
                newAvailMoves.push_back(out[i]);
            }
        }
        return newAvailMoves;
    }

void setupPieces(){
    //Pawns
    for(int i=0;i<7;i++){
        Pawn pb({6,i},BLACK);
        Pawn pw({6,i},WHITE);
        BlackPieces.push_back(pb);
        WhitePieces.push_back(pw);
    }

    //SuperPawns
    for(int i=0;i<7;i++){
        SuperPawn spb({6,i},BLACK);
        SuperPawn spw({6,i},WHITE);
        BlackPieces.push_back(spb);
        WhitePieces.push_back(spw);
    }

    //Giraffes
    Giraffe gb({7,0}, BLACK);
    Giraffe gw({0,0}, WHITE);
    BlackPieces.push_back(gb);
    WhitePieces.push_back(gw);

    //Monkey
    Monkey mb({7,1}, BLACK);
    Monkey mw({0,1}, WHITE);
    BlackPieces.push_back(mb);
    WhitePieces.push_back(mw);

    //Elephant
    Elephant eb1({7,2}, BLACK);
    Elephant eb2({7,4}, BLACK);
    Elephant ew1({0,2}, WHITE);
    Elephant ew2({0,4}, WHITE);
    BlackPieces.push_back(eb1);
    BlackPieces.push_back(eb2);
    WhitePieces.push_back(ew1);
    WhitePieces.push_back(ew2);

    //Lion
    Lion lb({0,0}, BLACK); //7,3
    Lion lw({0,0}, WHITE); //0,3
    BlackPieces.push_back(lb);
    WhitePieces.push_back(lw);

    //Crocodile
    Crocodile cb({7,5}, BLACK);
    Crocodile cw({0,5}, WHITE);
    BlackPieces.push_back(lb);
    WhitePieces.push_back(lw);

    //Zebra
    Zebra zb({7,6}, BLACK);
    Zebra zw({0,6}, WHITE);
    BlackPieces.push_back(zb);
    WhitePieces.push_back(zw);

    vector<char> def= {'0','0','0','0','0','0','0'};
    board = {def, def, def, def, def, def, def};
}

void resetBoard(){
    for(int i=0;i<21;i++){
        WhitePieces[i].setAlive(false);
        WhitePieces[i].allMoves.clear();
        WhitePieces[i].availMoves.clear();
        BlackPieces[i].setAlive(false);
        BlackPieces[i].allMoves.clear();
        BlackPieces[i].availMoves.clear();
    }

    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            board[i][j] = '0';
        }
    }
    turnCount=0;
    rawScore=0;
}

string generateNewFENString(struct Gamestate currState){
    string fen="";
    int tempLine = 0;

    for(int i=6; i>=0;i--){
        for(int j=0;j<7;j++){
            char piece = currState.currBoard[i][j];
            if(piece=='0') tempLine++;
            else{
                if (tempLine!=0) fen += to_string(tempLine);
                fen += piece;
                tempLine=0;
            }
            if(tempLine==7 || j==6){
                if (tempLine!=0) fen += to_string(tempLine);
                tempLine=0;
            }
        }
        if(i!=0) fen += '/';
    }
    return fen;
}

char readFENString(string fen){
    int curRank = 7;
    string boardSetup;
    string color;
    string turnTemp;
    stringstream ss(fen);
    getline(ss, boardSetup, ' ');
    getline(ss, color, ' ');
    nextMove = color[0];
    getline(ss, turnTemp, ' ');
    turnCount = stoi(turnTemp);

    stringstream ss1(boardSetup);
    string row;
    while (!ss1.eof()) {
        int curFile=0;
        getline(ss1, row, '/');

        if(row=="7"){
            curRank--;
            continue;
        }
        for(int i=0;i<row.length();i++){
            if(isdigit(row[i])){
                curFile+= (row[i] - '0');
                continue;
            }else{
                if(row[i]=='p'){ //black pawn
                    for(int j=0;j<7;j++){
                            if(!BlackPieces[j].getAlive()){
                                BlackPieces[j].setAlive(true);
                                BlackPieces[j].setPosition({curRank, curFile});
                                board[curRank-1][curFile] = 'p';
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='P'){ //white pawn
                    for(int j=0;j<7;j++){
                            if(!WhitePieces[j].getAlive()){
                                WhitePieces[j].setAlive(true);
                                WhitePieces[j].setPosition({curRank, curFile});
                                board[curRank-1][curFile] = 'P';
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='s'){ //black superpawn
                    for(int j=7;j<14;j++){
                            if(!BlackPieces[j].getAlive()){
                                BlackPieces[j].setAlive(true);
                                BlackPieces[j].setPosition({curRank, curFile});
                                board[curRank-1][curFile] = 's';
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='S'){ //white superpawn
                    for(int j=7;j<14;j++){
                            if(!WhitePieces[j].getAlive()){
                                WhitePieces[j].setAlive(true);
                                WhitePieces[j].setPosition({curRank, curFile});
                                board[curRank-1][curFile] = 'S';
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='g'){ // black giraffe
                    BlackPieces[14].setAlive(true);
                    BlackPieces[14].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'g';
                    curFile++;
                    continue;
                }
                if(row[i]=='G'){ // white giraffe
                    WhitePieces[14].setAlive(true);
                    WhitePieces[14].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'G';
                    curFile++;
                    continue;
                }
                if(row[i]=='m'){ //black monkey
                    BlackPieces[15].setAlive(true);
                    BlackPieces[15].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'm';
                    curFile++;
                    continue;
                }
                if(row[i]=='M'){ //white monkey
                    WhitePieces[15].setAlive(true);
                    WhitePieces[15].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'M';
                    curFile++;
                    continue;
                }
                if(row[i]=='e'){ //black elephant
                    for(int j=16;j<18;j++){
                            if(!BlackPieces[j].getAlive()){
                                BlackPieces[j].setAlive(true);
                                BlackPieces[j].setPosition({curRank, curFile});
                                board[curRank-1][curFile] = 'e';
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='E'){ //white elephant
                    for(int j=16;j<18;j++){
                            if(!WhitePieces[j].getAlive()){
                                WhitePieces[j].setAlive(true);
                                WhitePieces[j].setPosition({curRank, curFile});
                                board[curRank-1][curFile] = 'E';
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='l'){ //black lion
                    BlackPieces[18].setAlive(true);
                    BlackPieces[18].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'l';
                    curFile++;
                    continue;
                }
                if(row[i]=='L'){ //white lion
                    WhitePieces[18].setAlive(true);
                    WhitePieces[18].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'L';
                    curFile++;
                    continue;
                }
                if(row[i]=='c'){ //black crocodile
                    BlackPieces[19].setAlive(true);
                    BlackPieces[19].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'c';
                    curFile++;
                    continue;
                }
                if(row[i]=='C'){ //white crocodile
                    WhitePieces[19].setAlive(true);
                    WhitePieces[19].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'C';
                    curFile++;
                    continue;
                }
                if(row[i]=='z'){ //black zebra
                    BlackPieces[20].setAlive(true);
                    BlackPieces[20].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'z';

                    curFile++;
                    continue;
                }
                if(row[i]=='Z'){ //white zebra
                    WhitePieces[20].setAlive(true);
                    WhitePieces[20].setPosition({curRank, curFile});
                    board[curRank-1][curFile] = 'Z';
                    curFile++;
                    continue;
                }
            }
        }
        curRank--;
    }

    if(WhitePieces[14].alive) WhitePieces[14].setAvailGiraffeMoves();
    if(BlackPieces[14].alive) BlackPieces[14].setAvailGiraffeMoves();
    if(WhitePieces[18].alive) WhitePieces[18].setAvailLionMoves();
    if(BlackPieces[18].alive) BlackPieces[18].setAvailLionMoves();
    if(WhitePieces[20].alive) WhitePieces[20].setAvailZebraMoves();
    if(BlackPieces[20].alive) BlackPieces[20].setAvailZebraMoves();
    sortPawns(nextMove);
    for(int a=0;a<7;a++){
        if(WhitePieces[a].alive) WhitePieces[a].setAvailPawnMoves();
        if(BlackPieces[a].alive) BlackPieces[a].setAvailPawnMoves();
    }
    sortSuperPawns(nextMove);
    for(int a=7;a<14;a++){
        if(WhitePieces[a].alive) WhitePieces[a].setAvailSuperPawnMoves();
        if(BlackPieces[a].alive) BlackPieces[a].setAvailSuperPawnMoves();
    }

    return nextMove;
}

vector<string> sortPiece(vector<string> pieces){
    vector<pair<char, int>> temp;
    for(int i=0; i<pieces.size();i++){
        temp.push_back(pair<char, int>(pieces[i][0], pieces[i][1]-'0'));
    }

    sort(temp.begin(), temp.end());
    vector<string> out;
    for(int i=0; i<pieces.size();i++){
        out.push_back(temp[i].first + to_string(temp[i].second));
    }
    return out;
}

string printFENString(char NextMove){ //Pawn(0-6) Superpawn(7-13) giraffe(14) monkey(15) elephant(16-17) lion(18) crocodile(19) zebra(20)
    string output = "white pawn: ";
    string temp = "";
    vector<string> sorted;
    for(int i=0;i<7;i++){
        if(WhitePieces[i].getAlive()){
            temp = WhitePieces[i].getFile() + to_string(WhitePieces[i].getPosition()[0]);
            sorted.push_back(temp);
        }else continue;
    }
    sorted = sortPiece(sorted);
    for(int i=0;i<sorted.size();i++){
        output+= sorted[i];
        if(i!=sorted.size()-1) output+= " ";
    }
    output+="\n";
    sorted.clear();
    temp = "";

    output+="black pawn: ";
    for(int i=0;i<7;i++){
        if(BlackPieces[i].getAlive()){
            temp = BlackPieces[i].getFile() + to_string(BlackPieces[i].getPosition()[0]);
            sorted.push_back(temp);
        }else continue;
    }
    sorted = sortPiece(sorted);
    for(int i=0;i<sorted.size();i++){
        output+= sorted[i];
        if(i!=sorted.size()-1) output+= " ";
    }
    output+="\n";
    sorted.clear();
    temp = "";

    output += "white superpawn: ";
    for(int i=7;i<14;i++){
        if(WhitePieces[i].getAlive()){
            temp = WhitePieces[i].getFile() + to_string(WhitePieces[i].getPosition()[0]);
            sorted.push_back(temp);
        }else continue;
    }
    sorted = sortPiece(sorted);
    for(int i=0;i<sorted.size();i++){
        output+= sorted[i];
        if(i!=sorted.size()-1) output+= " ";
    }
    output+="\n";
    sorted.clear();
    temp = "";

    output+="black superpawn: ";
    for(int i=7;i<14;i++){
        if(BlackPieces[i].getAlive()){
            temp = BlackPieces[i].getFile() + to_string(BlackPieces[i].getPosition()[0]);
            sorted.push_back(temp);
        }else continue;
    }
    sorted = sortPiece(sorted);
    for(int i=0;i<sorted.size();i++){
        output+= sorted[i];
        if(i!=sorted.size()-1) output+= " ";
    }
    output+="\n";
    sorted.clear();
    temp = "";

    output += "white giraffe: ";
    if(WhitePieces[14].getAlive()) output += WhitePieces[14].getFile() + to_string(WhitePieces[14].getPosition()[0]);
    output+="\nblack giraffe: ";
    if(BlackPieces[14].getAlive()) output += BlackPieces[14].getFile() + to_string(BlackPieces[14].getPosition()[0]);

    output+="\nwhite monkey: ";
    if(WhitePieces[15].getAlive()) output += WhitePieces[15].getFile() + to_string(WhitePieces[15].getPosition()[0]);
    output+="\nblack monkey: ";
    if(BlackPieces[15].getAlive()) output += BlackPieces[15].getFile() + to_string(BlackPieces[15].getPosition()[0]);

    output+="\nwhite elephant: ";
    for(int i=16;i<18;i++){
        if(WhitePieces[i].getAlive()){
            temp = WhitePieces[i].getFile() + to_string(WhitePieces[i].getPosition()[0]);
            sorted.push_back(temp);
        }else continue;
    }
    sorted = sortPiece(sorted);
    for(int i=0;i<sorted.size();i++){
        output+= sorted[i];
        if(i!=sorted.size()-1) output+= " ";
    }
    output+="\n";
    sorted.clear();
    temp = "";
    output+="black elephant: ";
    for(int i=16;i<18;i++){
        if(BlackPieces[i].getAlive()){
            temp = BlackPieces[i].getFile() + to_string(BlackPieces[i].getPosition()[0]);
            sorted.push_back(temp);
        }else continue;
    }
    sorted = sortPiece(sorted);
    for(int i=0;i<sorted.size();i++){
        output+= sorted[i];
        if(i!=sorted.size()-1) output+= " ";
    }
    output+="\n";
    sorted.clear();
    temp = "";

    output+="white lion: ";
    if(WhitePieces[18].getAlive()) output += WhitePieces[18].getFile() + to_string(WhitePieces[18].getPosition()[0]);
    output+="\nblack lion: ";
    if(BlackPieces[18].getAlive()) output += BlackPieces[18].getFile() + to_string(BlackPieces[18].getPosition()[0]);

    output+="\nwhite crocodile: ";
    if(WhitePieces[19].getAlive()) output += WhitePieces[19].getFile() + to_string(WhitePieces[19].getPosition()[0]);
    output+="\nblack crocodile: ";
    if(BlackPieces[19].getAlive()) output += BlackPieces[19].getFile() + to_string(BlackPieces[19].getPosition()[0]);

    output+="\nwhite zebra: ";
    if(WhitePieces[20].getAlive()) output += WhitePieces[20].getFile() + to_string(WhitePieces[20].getPosition()[0]);
    output+="\nblack zebra: ";
    if(BlackPieces[20].getAlive()) output += BlackPieces[20].getFile() + to_string(BlackPieces[20].getPosition()[0]);

    output+="\nside to play: ";
    if(NextMove==BLACK) output += "black";
    else output += "white";

    return output;
}

void printBoard(vector<vector<char>> board){
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

string printLionMoves(struct Gamestate currState){
    string out = "";
    vector<string> sorted;
    if(currState.currColor==WHITE){
        Piece L = currState.WhiteP[18];
        if(L.availMoves.size()==0) return out;
        for(int i=0; i<L.availMoves.size();i++){
            sorted.push_back(convertFile(L.availMoves[i].first) + to_string(L.availMoves[i].second));
        }

        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(L.position[1]) + to_string(L.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }else{
        Piece L = currState.BlackP[18];
        if(L.availMoves.size()==0) return out;
        for(int i=0; i<L.availMoves.size();i++){
            sorted.push_back(convertFile(L.availMoves[i].first) + to_string(L.availMoves[i].second));
        }
        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(L.position[1]) + to_string(L.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }
    sorted.clear();
    return out;
}

string printZebraMoves(struct Gamestate currState){
    string out = "";
    vector<string> sorted;

    if(currState.currColor==WHITE){
        Piece Z = currState.WhiteP[20];
        if(Z.availMoves.size()==0) return out;
        for(int i=0; i<Z.availMoves.size();i++){
            sorted.push_back(convertFile(Z.availMoves[i].first) + to_string(Z.availMoves[i].second));
        }

        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(Z.position[1]) + to_string(Z.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }else{
        Piece z = currState.BlackP[20];
        if(z.availMoves.size()==0) return out;
        for(int i=0; i<z.availMoves.size();i++){
            sorted.push_back(convertFile(z.availMoves[i].first) + to_string(z.availMoves[i].second));
        }
        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(z.position[1]) + to_string(z.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }
    sorted.clear();
    return out;
}

string printGiraffeMoves(struct Gamestate currState){
    string out = "";
    vector<string> sorted;

    if(currState.currColor==WHITE){
        Piece Z = currState.WhiteP[14];
        if(Z.availMoves.size()==0) return out;
        for(int i=0; i<Z.availMoves.size();i++){
            sorted.push_back(convertFile(Z.availMoves[i].first) + to_string(Z.availMoves[i].second));
        }

        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(Z.position[1]) + to_string(Z.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }else{
        Piece z = currState.BlackP[14];
        if(z.availMoves.size()==0) return out;
        for(int i=0; i<z.availMoves.size();i++){
            sorted.push_back(convertFile(z.availMoves[i].first) + to_string(z.availMoves[i].second));
        }
        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(z.position[1]) + to_string(z.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }
    sorted.clear();
    return out;
}

void sortPawns(char color){
    vector<Piece> TempPawns;
    vector<string> sorted;

    if(color==WHITE){
        for(int j=0;j<7;j++){ //sorts pawns by starting position
            Piece Z = WhitePieces[j];
            if(!Z.alive) continue;
            sorted.push_back(convertFile(Z.position[1]) + to_string(Z.position[0]));
        }
        sorted = sortPiece(sorted);

        for(int j=0;j<sorted.size();j++){
            for(int i=0;i<7;i++){
                if(!WhitePieces[i].alive) continue;
                string pawnPosTemp = convertFile(WhitePieces[i].position[1]) + to_string(WhitePieces[i].position[0]);
                if(pawnPosTemp==sorted[j]){
                    TempPawns.push_back(WhitePieces[i]);
                }
            }
        }

        for(int i=0;i<TempPawns.size();i++){
            WhitePieces[i] = TempPawns[i];
        }
    }else{
        for(int j=0;j<7;j++){ //sorts pawns by starting position
            Piece Z = BlackPieces[j];
            if(!Z.alive) continue;
            sorted.push_back(convertFile(Z.position[1]) + to_string(Z.position[0]));
        }
        sorted = sortPiece(sorted);

        for(int j=0;j<sorted.size();j++){
            for(int i=0;i<7;i++){
                if(!BlackPieces[i].alive) continue;
                string pawnPosTemp = convertFile(BlackPieces[i].position[1]) + to_string(BlackPieces[i].position[0]);
                if(pawnPosTemp==sorted[j]){
                    TempPawns.push_back(BlackPieces[i]);
                }
            }
        }

        for(int i=0;i<TempPawns.size();i++){
            BlackPieces[i] = TempPawns[i];
        }
    }
}

void sortSuperPawns(char color){
    vector<Piece> TempPawns;
    vector<string> sorted;

    if(color==WHITE){
        for(int j=7;j<14;j++){ //sorts pawns by starting position
            Piece Z = WhitePieces[j];
            if(!Z.alive) continue;
            sorted.push_back(convertFile(Z.position[1]) + to_string(Z.position[0]));
        }
        sorted = sortPiece(sorted);

        for(int j=0;j<sorted.size();j++){
            for(int i=7;i<14;i++){
                if(!WhitePieces[i].alive) continue;
                string pawnPosTemp = convertFile(WhitePieces[i].position[1]) + to_string(WhitePieces[i].position[0]);
                if(pawnPosTemp==sorted[j]){
                    TempPawns.push_back(WhitePieces[i]);
                }
            }
        }

        for(int i=7;i<TempPawns.size()+7;i++){
            WhitePieces[i] = TempPawns[i-7];
        }
    }else{
        for(int j=7;j<14;j++){ //sorts pawns by starting position
            Piece Z = BlackPieces[j];
            if(!Z.alive) continue;
            sorted.push_back(convertFile(Z.position[1]) + to_string(Z.position[0]));
        }
        sorted = sortPiece(sorted);

        for(int j=0;j<sorted.size();j++){
            for(int i=7;i<14;i++){
                if(!BlackPieces[i].alive) continue;
                string pawnPosTemp = convertFile(BlackPieces[i].position[1]) + to_string(BlackPieces[i].position[0]);
                if(pawnPosTemp==sorted[j]){
                    TempPawns.push_back(BlackPieces[i]);
                }
            }
        }

        for(int i=7;i<TempPawns.size()+7;i++){
            BlackPieces[i] = TempPawns[i-7];
        }
    }
}

string printPawnMoves(struct Gamestate currState){
    string out = "";
    vector<string> sorted;

    if(currState.currColor==WHITE){
        for(int j=0;j<7;j++){
            sorted.clear();
            Piece Z = currState.WhiteP[j];
            if(Z.availMoves.size()==0) continue; //if pawn is dead or has no avail moves
            if(j!=0)out+= " ";
            for(int i=0; i<Z.availMoves.size();i++){ //sorts moves in alpha-numeric order
                sorted.push_back(convertFile(Z.availMoves[i].first) + to_string(Z.availMoves[i].second));
            }
            sorted = sortPiece(sorted);

            for(int i=0;i<sorted.size();i++){
                out+= convertFile(Z.position[1]) + to_string(Z.position[0]) + sorted[i];
                if(i!=sorted.size()-1) out+= " ";
            }
        }
    }else{
        for(int j=0;j<7;j++){
            sorted.clear();
            Piece z = currState.BlackP[j];
            if(z.availMoves.size()==0) continue;
            if(j!=0)out+= " ";
            for(int i=0; i<z.availMoves.size();i++){
                sorted.push_back(convertFile(z.availMoves[i].first) + to_string(z.availMoves[i].second));
            }
            sorted = sortPiece(sorted);

            for(int i=0;i<sorted.size();i++){
                out+= convertFile(z.position[1]) + to_string(z.position[0]) + sorted[i];
                if(i!=sorted.size()-1) out+= " ";
            }
        }
    }
    sorted.clear();
    return out;
}

string printSuperPawnMoves(struct Gamestate currState){
    string out = "";
    vector<string> sorted;

    if(currState.currColor==WHITE){
        for(int j=7;j<14;j++){
            sorted.clear();
            Piece Z = currState.WhiteP[j];
            if(Z.availMoves.size()==0) continue; //if pawn is dead or has no avail moves
            if(j!=7)out+= " ";
            for(int i=0; i<Z.availMoves.size();i++){ //sorts moves in alpha-numeric order
                sorted.push_back(convertFile(Z.availMoves[i].first) + to_string(Z.availMoves[i].second));
            }
            sorted = sortPiece(sorted);

            for(int i=0;i<sorted.size();i++){
                out+= convertFile(Z.position[1]) + to_string(Z.position[0]) + sorted[i];
                if(i!=sorted.size()-1) out+= " ";
            }
        }
    }else{
        for(int j=7;j<14;j++){
            sorted.clear();
            Piece z = currState.BlackP[j];
            if(z.availMoves.size()==0) continue;
            if(j!=7)out+= " ";
            for(int i=0; i<z.availMoves.size();i++){
                sorted.push_back(convertFile(z.availMoves[i].first) + to_string(z.availMoves[i].second));
            }
            sorted = sortPiece(sorted);

            for(int i=0;i<sorted.size();i++){
                out+= convertFile(z.position[1]) + to_string(z.position[0]) + sorted[i];
                if(i!=sorted.size()-1) out+= " ";
            }
        }
    }
    sorted.clear();
    return out;
}

vector<string> getAllMoves(struct Gamestate currState){
    string allMoves="";
    string lionMoves = printLionMoves(currState); //lion moves
    allMoves+= lionMoves;
    if(lionMoves!="") allMoves+= " "; //if not empty add a space
    string zebraMoves = printZebraMoves(currState);  //zebra moves
    allMoves+= zebraMoves;
    if(zebraMoves!="") allMoves+= " ";
    string giraffeMoves = printGiraffeMoves(currState);  //giraffe moves
    allMoves+= giraffeMoves;
    if(giraffeMoves!="") allMoves+= " ";
    string pawnMoves = printPawnMoves(currState);  //pawn moves
    allMoves+= pawnMoves;
    if(pawnMoves!="") allMoves+= " ";
    string superpawnMoves = printSuperPawnMoves(currState);  //pawn moves
    allMoves+= superpawnMoves;

    //make vector
    stringstream ss(allMoves);
    string tempMove;
    vector<string> vecMoves;
    while (ss >> tempMove) {
        vecMoves.push_back(tempMove);
    }
    return vecMoves;
}

int main() {
    setupPieces();
    string output1="";
    string output2="";
    int N;
    cin >> N;
    cin.ignore(); //NB!
    for (int i = 0; i < N; ++i) {
        resetBoard();
        string fen;
        string myMove;
        getline(cin, fen);

        //Sub1 stuff
        nextMove = readFENString(fen);
        output1+=printFENString(nextMove);

        //Sub4 stuff
        startState.WhiteP=WhitePieces;
        startState.BlackP=BlackPieces;
        startState.currBoard=board;
        startState.currFEN=fen;
        startState.currColor=nextMove;
        startState.currTurn=turnCount;

        /*printBoard(startState.currBoard);
        cout << endl;

        rawScore = calcMaterialScore(startState);
        output2 += "\nMaterial: " + to_string(rawScore) + "\n";
        rawScore = calcMobilityScore(startState);
        output2 += "Mobility: " + to_string(rawScore) + "\n";
        rawScore = calcAttackScore(startState);
        output2 += "Attack: " + to_string(rawScore) + "\n";
        //rawScore = performMinMax(startState, DEPTH);
        */
        rawScore = calcRawScore(startState);
        output2 += to_string(rawScore);


        if(i!=N-1){
            output1+="\n\n";
            output2+="\n";
        }
    }
    //cout << output1;
    cout << output2;

    return 0;
}
