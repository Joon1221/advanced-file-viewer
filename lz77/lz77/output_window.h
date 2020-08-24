#ifndef OUTPUT_WINDOWS_H
#define OUTPUT_WINDOWS_H

#include <iostream>
#include <stdio.h>

using namespace std;

#define WINDOW_SIZE 500
//#define BUFF_SIZE ((WINDOW_SIZE*2)+1)

class OutputWindow {
private:
    ostream &out;
    
    int windowSize;
    int buffSize;
    
    char *buff;
    int head;
    int pointer;
    int tail;
    
    // private helper functions.
    void init(int windowSize);
    
public:
    OutputWindow(ostream &in);
    OutputWindow(ostream &in, int windowSize);
    
    bool appendNextString(int dist, int len, char ch);
};

#endif
