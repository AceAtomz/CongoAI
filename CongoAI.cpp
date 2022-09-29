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
    bool alive = true;

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
vector<Piece> BlackPieces;
vector<Piece> WhitePieces;

void readFENString(string fen){
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
                    BlackPieces.push_back(newPiece);
                }
            }
        }
        curRank--;
    }
}

void printFENString(vector<Piece> WhitePieces, vector<Piece> BlackPieces, char NextMove){
    cout << BlackPieces[0].getPosition()[1] << endl;
}



int main() {
    int N;
    cin >> N;
    cin.ignore(); //NB!
    for (int i = 0; i < N; ++i) {
        string fen;
        getline(cin, fen);
        readFENString(fen);
        //printFENString();
        cout << endl;
    }


    return 0;
}
