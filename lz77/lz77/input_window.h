#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

// dist byte의 MSB를 사용하기 때문에, 절대 compress용 window의 크기는 128보다 작아야 한다.
#define WINDOW_SIZE 127
//#define BUFF_SIZE (WINDOW_SIZE*2)

class InputWindow {
private:
    istream &in;

    int fileSize;
    int fileNumCharsProcessed;
    int fileNumCharsRemained;

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
    void appendNextCharByNumChars(int numCharsToAppend);

public:
    InputWindow(istream &in, int fileSize);
    InputWindow(istream &in, int fileSize, int windowSize);

    void printBuffWithPointer();

    void moveByOffset(int offset);
    
    bool doesCharExistByDist(int dist);
    char getCharByDist(int dist);
    
    bool doesCharExistByOffset(int offset);
    char getCharByOffset(int offset);
    
    int getNumProcessedChars();
    int getNumRemainedChars();
    
    static std::ifstream::pos_type getFileSize(const char* filename);
};

#endif

