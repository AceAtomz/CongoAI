# CongoAI
An AI designed to play the board game Congo

Congo is a board game, similar to chess but it has a 7x7 board, different pieces and slightly different rules.

The program outputs the evaluated score of the best possible move to make.
Key notes about the algorithm:
- Minmax function 
- Alpha Beta pruning implementation
- Depth of 4
- Advanced evaluation function
- Priority sorted movelist

# Get Started

To download the repo you must first clone it with the command

```
git clone https://github.com/AceAtomz/CongoAI.git
```

To begin using the program either:
run Congo.exe with terminal input of the form
```
1
1z5/pPp1lP1/5sp/4P1s/4L1p/2p2pP/7 w 35
```
- The first line indicating how many FEN strings to be read
- The following n lines being the FEN strings

Instead you can call the followning in a unix terminal:
```
./CongoAI.exe < random.in | tee myOut.txt 
```
with all outputs sent to myOut.txt

Output is of the form:
```
-669
```

### Developed by AtomZ - Claudio Da Mata
Advanced Analysis of Algorithms - 2022
