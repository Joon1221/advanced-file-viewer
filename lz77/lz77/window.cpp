#include <iostream>

using namespace std;

#include "window.h"

Window::Window(istream &in) : in(in) {
    init(WINDOW_SIZE);
}

Window::Window(istream &in, int windowSize) : in(in) {
    init(windowSize);
}

void Window::init(int windowSize) {

    this->windowSize = windowSize;
    buffSize = windowSize * 2;
    
    buff = new char[buffSize];
    head = 0;
    pointer = 0;
    tail = 0; // char를 in으로부터 한개 빼서 넣을 자리.

    numChars = 0;
    numCharsProcessed = 0;
    numCharsRemained = 0;
    
    while (numChars < windowSize && appendNextChar()) {
        numChars++;
        numCharsRemained++;
    }
    cout << "Window::Window(): numChars = " << numChars << endl;

    for (int i = 0; i < numChars; i++) {
        cout << "Window::Window(): buff[" << i << "] = " << buff[i] << endl;
    }
}

bool Window::appendNextChar() {
    buff[tail] = in.get();
    if (!in.eof()) {
        tail = (tail + 1) % buffSize;
        return true;
    }
    else {
        return false;
    }
}

// precondition: doesCharExistByOffset(offset-1) is true
void Window::moveByOffset(int offset) {
    for (int i = 0; i < offset; i++) {
        if (appendNextChar()) {
            if (numChars < buffSize) {
                numChars++;
            }
            else {
                head = (head + 1) % buffSize;
                numCharsProcessed--;
            }
            numCharsRemained++;
        }
        else {
            break;
        }
    }
    pointer = (pointer + offset) % buffSize;
    numCharsProcessed += offset;
    numCharsRemained -= offset;
}

bool Window::doesCharExistByDist(int dist) {
    return numCharsProcessed >= dist;
}

// precondition: doesCharExistByDist(dist) is true
char Window::getCharByDist(int dist) {
    return buff[(pointer-dist+buffSize) % buffSize];
}

bool Window::doesCharExistByOffset(int offset) {
    return numCharsRemained > offset;
}

char Window::getCharByOffset(int offset) {
    return buff[(pointer + offset) % buffSize];
}

int Window::getNumProcessedChars() {
    return numCharsProcessed;
}

int Window::getNumRemainedChars() {
    return numCharsRemained;
}
