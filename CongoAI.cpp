#include <iostream>
#include <vector>
#include <sstream>
#include<string>
using namespace std;

class Piece{            //Base Piece class
public:
    //variables
    vector<int> position = {0,0};    //0-6 row
    char file = 'a';        //a-g column
    char color = 'W';       //B or W
    bool alive = true;

    //Constructors
    Piece(vector<int> newPosition, char newColor){
        position = newPosition;
        setFile();
        color = newColor;
    }

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
        if(position[1]==0) file = 'a';
        if(position[1]==1) file = 'b';
        if(position[1]==2) file = 'c';
        if(position[1]==3) file = 'd';
        if(position[1]==4) file = 'e';
        if(position[1]==5) file = 'f';
        if(position[1]==6) file = 'g';
    }
};

class Pawn: public Piece{            //Pawn Piece subclass
public:
    string name = "Pawn";
};

class Lion: public Piece{            //Lion Piece subclass
public:
    string name = "Lion";
};

class Zebra: public Piece{            //Zebra Piece subclass
public:
    string name = "Zebra";
};

class Elephant: public Piece{            //Elephant Piece subclass
public:
    string name = "Elephant";
};

class Giraffe: public Piece{            //Giraffe Piece subclass
public:
    string name = "Giraffe";
};

class Crocodile: public Piece{            //Crocodile Piece subclass
public:
    string name = "Crocodile";
};

class Monkey: public Piece{            //Monkey Piece subclass
public:
    string name = "Monkey";
};

//-----------------------------------------------------------------------------------------------------
vector<Piece> BlackPieces;
vector<Piece> WhitePieces;

void readFENString(string fen){
    int curRank =0;
    int curFile=0;

    stringstream ss(fen);
    string boardSetup;
    string color;
    getline(ss, boardSetup, ' ');
    getline(ss, color, ' ');

    stringstream ss1(boardSetup);
    string row;
    while (!ss1.eof()) {
        getline(ss1, row, '/');

        if(row=='7'){
            continue;
        }

        for(int i=0;i<row.length();i++){
            if(isdigit(row[i])){
                curFile+= row[i] - '0';
                continue;;
            }else{

                //BlackPieces.push_back()
            }
        }

        curRank++;
    }
}

void printFENString(vector<Piece> WhitePieces, vector<Piece> BlackPieces, char NextMove){

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
