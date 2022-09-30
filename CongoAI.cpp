#include <iostream>
#include <vector>
#include <sstream>
#include<string>
#include<algorithm>
using namespace std;

#define BLACK 'b'
#define WHITE 'w'

class Piece{            //Base Piece class
public:
    //variables
    vector<int> position = {1,0};    //1-7 row
    char file = 'a';        //a-g column
    char color = WHITE;       //B or W
    bool alive = false;

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
}

void resetBoard(){
    for(int i=0;i<21;i++){
        WhitePieces[i].setAlive(false);
        BlackPieces[i].setAlive(false);
    }
}

char readFENString(string fen){
    int curRank = 7;
    stringstream ss(fen);
    string boardSetup;
    string color;
    getline(ss, boardSetup, ' ');
    getline(ss, color, ' ');

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
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='g'){ // black giraffe
                    BlackPieces[14].setAlive(true);
                    BlackPieces[14].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='G'){ // white giraffe
                    WhitePieces[14].setAlive(true);
                    WhitePieces[14].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='m'){ //black monkey
                    BlackPieces[15].setAlive(true);
                    BlackPieces[15].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='M'){ //white monkey
                    WhitePieces[15].setAlive(true);
                    WhitePieces[15].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='e'){ //black elephant
                    for(int j=16;j<18;j++){
                            if(!BlackPieces[j].getAlive()){
                                BlackPieces[j].setAlive(true);
                                BlackPieces[j].setPosition({curRank, curFile});
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
                                curFile++;
                                break;
                            }
                    }
                }
                if(row[i]=='l'){ //black lion
                    BlackPieces[18].setAlive(true);
                    BlackPieces[18].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='L'){ //white lion
                    WhitePieces[18].setAlive(true);
                    WhitePieces[18].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='c'){ //black crocodile
                    BlackPieces[19].setAlive(true);
                    BlackPieces[19].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='C'){ //white crocodile
                    WhitePieces[19].setAlive(true);
                    WhitePieces[19].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='z'){ //black zebra
                    BlackPieces[20].setAlive(true);
                    BlackPieces[20].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
                if(row[i]=='Z'){ //white zebra
                    WhitePieces[20].setAlive(true);
                    WhitePieces[20].setPosition({curRank, curFile});
                    curFile++;
                    continue;
                }
            }
        }
        curRank--;
    }
    return color[0];
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


int main() {
    setupPieces();
    string output="";
    int N;
    cin >> N;
    cin.ignore(); //NB!
    for (int i = 0; i < N; ++i) {
        resetBoard();
        string fen;
        getline(cin, fen);
        char col = readFENString(fen);
        output+=printFENString(col);
        //output+="\n";
        if(i!=N-1) output+="\n\n";
    }
    cout << output;

    return 0;
}
