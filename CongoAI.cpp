#include <iostream>
#include <vector>
#include <sstream>
#include<string>
using namespace std;

#define BLACK 'B'
#define WHITE 'W'

class Piece{            //Base Piece class
public:
    //variables
    vector<int> position = {0,0};    //0-6 row
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
        setFile();
    }
    void setAlive(bool newAlive){
        alive = newAlive;
    }

protected:
    void setFile(){
        vector<char> files = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
        file = files[position[1]];
    }
};

class Pawn: public Piece{            //Pawn Piece subclass
public:
    string name = "Pawn";

    //Constructors
    Pawn(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class SuperPawn: public Piece{            //Pawn Piece subclass
public:
    string name = "Superpawn";

    //Constructors
    SuperPawn(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class Lion: public Piece{            //Lion Piece subclass
public:
    string name = "Lion";

    //Constructors
    Lion(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class Zebra: public Piece{            //Zebra Piece subclass
public:
    string name = "Zebra";

    //Constructors
    Zebra(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class Elephant: public Piece{            //Elephant Piece subclass
public:
    string name = "Elephant";

    //Constructors
    Elephant(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class Giraffe: public Piece{            //Giraffe Piece subclass
public:
    string name = "Giraffe";

    //Constructors
    Giraffe(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class Crocodile: public Piece{            //Crocodile Piece subclass
public:
    string name = "Crocodile";

    //Constructors
    Crocodile(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }
};

class Monkey: public Piece{            //Monkey Piece subclass
public:
    string name = "Monkey";

    //Constructors
    Monkey(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
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
            continue;
        }

        for(int i=0;i<row.length();i++){
            if(isdigit(row[i])){
                curFile+= (row[i] - '0');
                continue;;
            }else{
                if(row[i]=='l'){
                    vector<int> pos = {curRank, curFile};
                    Lion newPiece(pos, BLACK);
                    BlackPieces[19] = newPiece;
                }
            }
        }
        curRank--;
    }
    return color[0];
}

void printFENString(char NextMove){
    cout << BlackPieces[19].getFile() << BlackPieces[19].getPosition()[0] << endl;
}


int main() {
    setupPieces();
    int N;
    cin >> N;
    cin.ignore(); //NB!
    for (int i = 0; i < N; ++i) {
        string fen;
        getline(cin, fen);
        char col = readFENString(fen);
        printFENString(col);
        cout << endl;
    }


    return 0;
}
