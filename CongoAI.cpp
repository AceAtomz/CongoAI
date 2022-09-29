#include <iostream>
#include <vector>
#include <sstream>
#include<string>
using namespace std;

class Piece{            //Base Piece class
public:
    //variables
    int myRank = 0;         //0-6 row
    char myFile = 'a';        //a-g column
    char color = 'W';       //B or W
    bool alive = true;

    //Constructors
    Piece(int newRank, char newFile, char newColor){
        myRank = newRank;
        myFile = newFile;
        color = newColor;
    }

    //Getters
    char getColor(){
        return color;
    }
    char getFile(){
        return myFile;
    }
    int getRank(){
        return myRank;
    }
    bool getAlive(){
        return alive;
    }

    //Setters
    void setPosition(char newFile, int newRank){
        myRank = newRank;
        myFile = newFile;
    }
    void setAlive(bool newAlive){
        alive = newAlive;
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
