#include <iostream>
#include <vector>
#include <sstream>
#include<string>
#include<algorithm>
#include<iterator>
using namespace std;

#define BLACK 'b'
#define WHITE 'w'
char nextMove;
vector<vector<char>> board;
void checkLionEat(char color);

char convertFile(int newFile){
        vector<char> files = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
        return files[newFile];
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
        vector<char> WP = {'P', 'S', 'G', 'M', 'E', 'L', 'C', 'Z'};
        vector<char> BP = {'p', 's', 'g', 'm', 'e', 'l', 'c', 'z'};
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
            if(color==WHITE){
                if(position[0]>3){ //if not in white castle
                    availMoves.clear();
                    return;
                }
                if(position[1]<2 || position[1]>4){ //if not in white castle
                    availMoves.clear();
                    return;
                }
            }else{
                if(position[0]<5){ //if not in black castle
                    availMoves.clear();
                    return;
                }
                if(position[1]<2 || position[1]>4){ //if not in black castle
                    availMoves.clear();
                    return;
                }
            }

            for(int i=0;i<allMoves.size();i++){
                if(position[1]==allMoves[i].first){ // if in same column
                    if(position[0]-1==allMoves[i].second || position[0]+1==allMoves[i].second){
                        availMoves.push_back(allMoves[i]);
                    }
                }

                if(position[1]-1==allMoves[i].first){ // if left column in castle
                    if(position[0]-1==allMoves[i].second || position[0]+1==allMoves[i].second){
                        availMoves.push_back(allMoves[i]);
                    }
                }

                if(position[1]+1==allMoves[i].first){ // if right column in castle
                    if(position[0]-1==allMoves[i].second || position[0]+1==allMoves[i].second){
                        availMoves.push_back(allMoves[i]);
                    }
                }

                if(position[0]==allMoves[i].second){ // if in same row
                    if(position[1]-1==allMoves[i].first || position[1]+1==allMoves[i].first){
                        availMoves.push_back(allMoves[i]);
                    }
                }

            }
            checkLionEat(color);
            availMoves = getOwnPieces(availMoves,nextMove);
        }
    }

protected:
    void setFile(int newFile){
        vector<char> files = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
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
        setAllMoves();
    }

    //Setters
    void setAllMoves(){
        for(int i=2;i<5;i++){
            for(int j=1;j<8;j++){
                if(j==4) continue;
                allMoves.push_back({i,j});
            }
        }
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

void checkLionEat(char color){
    int l = BlackPieces[18].position[0] - WhitePieces[18].position[0]; //length between 2 lions
    bool blocked = false; //blocked flag variable
    if(color==WHITE){
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==2){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==4){
                WhitePieces[18].availMoves.push_back({BlackPieces[18].position[1], BlackPieces[18].position[0]});
                return;
            }
        }
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==4){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==2){
                WhitePieces[18].availMoves.push_back({BlackPieces[18].position[1], BlackPieces[18].position[0]});
                return;
            }
        }
        if(BlackPieces[18].position[1]==WhitePieces[18].position[1] && l==2){
            WhitePieces[18].availMoves.push_back({BlackPieces[18].position[1], BlackPieces[18].position[0]});
            return;
        }
        for(int i=WhitePieces[18].position[0]; i<l;i++){

            if(board[i][WhitePieces[18].position[1]]!='0'){
                blocked=true;
                break;
            }
        }
        if(!blocked)  WhitePieces[18].availMoves.push_back({BlackPieces[18].position[1], BlackPieces[18].position[0]});
    }else{
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==2){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==4){
                BlackPieces[18].availMoves.push_back({WhitePieces[18].position[1], WhitePieces[18].position[0]});
                return;
            }
        }
        if(WhitePieces[18].position[0]==3 && WhitePieces[18].position[1]==4){
            if(BlackPieces[18].position[0]==5 && BlackPieces[18].position[1]==2){
                BlackPieces[18].availMoves.push_back({WhitePieces[18].position[1], WhitePieces[18].position[0]});
                return;
            }
        }
        if(BlackPieces[18].position[1]==WhitePieces[18].position[1] && l==2){
            BlackPieces[18].availMoves.push_back({WhitePieces[18].position[1], WhitePieces[18].position[0]});
            return;
        }
        for(int i=BlackPieces[18].position[0]-2; i>1;i--){
            if(board[i][BlackPieces[18].position[1]]!='0'){
                blocked=true;
                break;
            }
        }
        if(!blocked)  BlackPieces[18].availMoves.push_back({WhitePieces[18].position[1], WhitePieces[18].position[0]});
    }
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
    Lion lb({7,3}, BLACK);
    Lion lw({0,3}, WHITE);
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
        BlackPieces[i].setAlive(false);
    }

    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            board[i][j] = '0';
        }
    }
}

char readFENString(string fen){
    int curRank = 7;
    stringstream ss(fen);
    string boardSetup;
    string color;
    getline(ss, boardSetup, ' ');
    getline(ss, color, ' ');
    nextMove = color[0];

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

    WhitePieces[18].setAvailLionMoves();
    BlackPieces[18].setAvailLionMoves();

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

void printBoard(){
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

string printLionMoves(){
    string out = "";
    vector<string> sorted;
    if(nextMove==WHITE){
        Piece L = WhitePieces[18];
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
        Piece L = BlackPieces[18];
        if(L.availMoves.size()==0) return out;
        for(int i=0; i<BlackPieces[18].availMoves.size();i++){
            sorted.push_back(convertFile(L.availMoves[i].first) + to_string(L.availMoves[i].second));
        }
        sorted = sortPiece(sorted);
        for(int i=0;i<sorted.size();i++){
            out+= convertFile(L.position[1]) + to_string(L.position[0]) + sorted[i];
            if(i!=sorted.size()-1) out+= " ";
        }
    }

    return out;
}

int main() {
    setupPieces();
    string output1="";
    string outputLion="";
    int N;
    cin >> N;
    cin.ignore(); //NB!
    for (int i = 0; i < N; ++i) {
        resetBoard();
        string fen;
        getline(cin, fen);

        //Sub1 stuff
        char col = readFENString(fen);
        output1+=printFENString(col);

        //Sub2 stuff
        outputLion+=printLionMoves();

        if(i!=N-1){
          output1+="\n\n";
          outputLion+="\n";
        }
        //printBoard();
    }
    //cout << output1;

    cout << outputLion << endl;

    return 0;
}
