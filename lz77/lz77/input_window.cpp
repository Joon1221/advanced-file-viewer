//#define DEBUG_MSG 1

#include <iostream>
#include <fstream>

using namespace std;

#include "input_window.h"

InputWindow::InputWindow(istream &in, int fileSize) : in(in) {
    this->fileSize = fileSize;
    fileNumCharsProcessed = 0;
    fileNumCharsRemained = fileSize;
    init(WINDOW_SIZE);
}

InputWindow::InputWindow(istream &in, int fileSize, int windowSize) : in(in) {
    this->fileSize = fileSize;
    fileNumCharsProcessed = 0;
    fileNumCharsRemained = fileSize;
    init(windowSize);
}

std::ifstream::pos_type InputWindow::getFileSize(const char* filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void InputWindow::init(int windowSize) {
    this->windowSize = windowSize;
    buffSize = windowSize * 2 + 1;
    
    buff = new char[buffSize];
    head = 0;
    pointer = 0;
    tail = 0; // char를 in으로부터 한개 빼서 넣을 자리.

    numChars = 0; // numCharsProcessed + numCharsRemained
    numCharsProcessed = 0;
    numCharsRemained = 0;
    
    while (numChars < windowSize && appendNextChar()) {
        numChars++;
        numCharsRemained++;
    }
//    cout << "Window::Window(): numChars = " << numChars << endl;
//
//    for (int i = 0; i < numChars; i++) {
//        cout << "Window::Window(): buff[" << i << "] = " << buff[i] << endl;
//    }
}

void InputWindow::printBuffWithPointer() {
#ifdef DEBUG_MSG
    cout << "numChars = " << numChars << " | head = " << head << " | tail = " << tail << " | pointer = " << pointer << endl;
    cout << "[";
    for (int i = 0; i < buffSize; i++) {
        if (numChars > 0) {
            if (head < tail) {
                if (i >= head && i < tail) {
                    cout << buff[i];
                }
                else {
                    cout << " ";
                }
            }
            else {
                if (i >= head || i < tail) {
                    cout << buff[i];
                }
                else {
                    cout << " ";
                }
            }
        }
        else {
            cout << " ";
        }
    }
    cout << "]" << endl;
    
    cout << " ";
    for (int i = 0; i < buffSize; i++) {
        if (i == pointer) {
            cout << "^";
        }
        else if (i == head) {
            cout << "h";
        }
        else if (i == tail) {
            cout << "t";
        }
        else {
            cout << " ";
        }
    }
    cout << endl;
#endif
}

bool InputWindow::appendNextChar() {
    buff[tail] = in.get();
    if (!in.eof()) {
//        cout << "InputWindow::appendNextChar(): tail = " << tail << " | buff[tail] = " << buff[tail] << endl;
        tail = (tail + 1) % buffSize;
        return true;
    }
    else {
        return false;
    }
}

void InputWindow::appendNextCharByNumChars(int numCharsToAppend) {
    for (int i = 0; i < numCharsToAppend; i++) {
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
}

// precondition: doesCharExistByOffset(offset-1) is true
void InputWindow::moveByOffset(int offset) {
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
    fileNumCharsProcessed += offset;
    numCharsRemained -= offset;
    fileNumCharsRemained -= offset; // 이렇게 해야만, 전체 남은 byte를 알 수 있고, 그래야만, doesCharExistByOffset()가 더 처리할 것이 있는지 정확히 알 수 있다.
}

bool InputWindow::doesCharExistByDist(int dist) {
    return numCharsProcessed >= dist;
}

// precondition: doesCharExistByDist(dist) is true
char InputWindow::getCharByDist(int dist) {
    return buff[(pointer-dist+buffSize) % buffSize];
}

bool InputWindow::doesCharExistByOffset(int offset) {
    return fileNumCharsRemained > offset;
//    return numCharsRemained > offset; // 이렇게 하면, file에 더 처리할 것이 있는데, 없는 것으로 되어 버린다. 왜냐하면, window size와 똑같은 offset일 경우.. 에러.
}

char InputWindow::getCharByOffset(int offset) {
    int diff = offset + 1 - numCharsRemained;
    
    appendNextCharByNumChars(diff);
    
    return buff[(pointer + offset) % buffSize];
}

int InputWindow::getNumProcessedChars() {
    return numCharsProcessed;
}

int InputWindow::getNumRemainedChars() {
    return numCharsRemained;
}
