#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <stdio.h>

using namespace std;

#define WINDOW_SIZE 500
//#define BUFF_SIZE (WINDOW_SIZE*2)

class Window {
private:
    istream &in;
    
    int windowSize;
    int buffSize;
    
    char *buff;
    int head;
    int pointer;
    int tail;
    
    int numChars; // numCharsProcessed + numCharsRemained
    int numCharsProcessed;
    int numCharsRemained;

    // private helper functions.
    void init(int windowSize);
    bool appendNextChar();
    
public:
    Window(istream &in);
    Window(istream &in, int windowSize);

    void moveByOffset(int offset);
    
    bool doesCharExistByDist(int dist);
    char getCharByDist(int dist);
    
    bool doesCharExistByOffset(int offset);
    char getCharByOffset(int offset);
    
    int getNumProcessedChars();
    int getNumRemainedChars();
};

#endif

