#include "output_window.h"

OutputWindow::OutputWindow(ostream &out) : out(out) {
    init(WINDOW_SIZE);
}

OutputWindow::OutputWindow(ostream &out, int windowSize) : out(out) {
    init(windowSize);
}

void OutputWindow::init(int windowSize) {
    this->windowSize = windowSize;
    buffSize = windowSize * 2;
    
    buff = new char[buffSize];
    head = 0;
    pointer = 0;
    tail = 0; // char를 in으로부터 한개 빼서 넣을 자리.
    
}

bool OutputWindow::appendNextString(int dist, int len, char ch) {
    bool lastPattern = dist & 0b10000000;
    dist = dist & 0b01111111;
    

    for (int i = 0; i < len; i++) {
        out.put(buff[(pointer-dist+buffSize+i) % buffSize]);
        buff[(pointer+i) % buffSize] = buff[(pointer-dist+buffSize+i) % buffSize];
   
    }
    tail = (tail + len) % buffSize;
    pointer = (pointer + len) % buffSize;
    
    if (!lastPattern) {

        buff[pointer] = ch;
        tail = (tail + 1) % buffSize;
        pointer = (pointer + 1) % buffSize;
        
        out.put(ch);
    }
    
    return true;
}
