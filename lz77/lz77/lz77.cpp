//#define DEBUG_MSG 1

#include <iostream>

using namespace std;

#include "encoded_item.h"
#include "encoded_items.h"
#include "vector_encoded_items.h"
#include "input_window.h"
#include "output_window.h"

#include "lz77.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// lz77Decompress()
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int lz77Decompress(istream &in, ostream &out) {
    int curDist = 0;
    int curLen = 0;
    char curChar = 0;
    
    int outputByteSize = 0;
    
    OutputWindow *output = new OutputWindow(out, 100);
    
    while (true) {
        curDist = (int)((unsigned char)in.get());
        curLen = (int)((unsigned char)in.get());
        curChar = in.get();
        
        if (in.eof()) {
            break;
        }
//        
//        cout << "curDist: " << curDist << endl;
//        cout << "curLen: " << curLen << endl;
//        cout << "curChar: " << curChar << endl;
        
        output->appendNextString(curDist, curLen, curChar);
    }
    return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// lz77Compress(): binary version
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int lz77Compress(istream &in, int inputFileSize,  ostream &out) {
    int outputByteSize = 0;
    
    InputWindow *input = new InputWindow(in, inputFileSize, 50);
#ifdef DEBUG_MSG
    cout << "---------------------------------------------------------------" << endl;
    cout << "InputWindow *input = new InputWindow(in, inputFileSize, 1);" << endl;
    input->printBuffWithPointer();
#endif

    while (input->doesCharExistByOffset(0)) {
        
        char curSearchingChar = input->getCharByOffset(0);
#ifdef DEBUG_MSG
        cout << "---------------------------------------------------------------" << endl;
        cout << "char curSearchingChar = input->getCharByOffset(0);" << endl;
        cout << "curSearchingChar: " << curSearchingChar << endl;
        input->printBuffWithPointer();
#endif

        int curLen = 0;
        
        
        // i번째 character를 input의 맨 처음부터 찾기.
        int maxLen = 0;
        int maxDist = 0;
        
        for (int curDist = input->getNumProcessedChars(); curDist > 0; curDist--) {
            char curInspectingChar = input->getCharByDist(curDist);
            
            if (curSearchingChar == curInspectingChar) {
                curLen = 0;

                for (int i = curDist; i > 0 && input->doesCharExistByOffset(curLen); i--) {
                    char curSearchingChar = input->getCharByDist(i);
                    char curInspectingChar = input->getCharByOffset(curLen);
#ifdef DEBUG_MSG
                    cout << "---------------------------------------------------------------" << endl;
                    cout << "char curInspectingChar = input->getCharByOffset(curLen);" << endl;
                    cout << "curInspectingChar: " << curInspectingChar << endl;
                    input->printBuffWithPointer();
#endif

                    if (curSearchingChar != curInspectingChar) {
                        break;
                    }
                    curLen++;
                }
                
                if (maxLen < curLen) {
                    maxLen = curLen;
                    maxDist = curDist;
                }
            }
        }

#ifdef DEBUG_MSG
        cout << "maxLen: " << maxLen << endl;
        cout << "maxDist: " << maxDist << endl;
        if (input->doesCharExistByOffset(maxLen)) {
//            cout << "Next Char: " << input->getCharByOffset(maxLen) << endl;
            cout << "Next Char: exist" << endl;
        }
        else {
            cout << "Next Char: " << "_" << endl;
        }
#endif

        // 만약 마지막 char이면..
        if (!input->doesCharExistByOffset(maxLen)) {
            out.put((unsigned char)maxDist | 0b10000000);
            out.put((unsigned char)maxLen);
            out.put((unsigned char)'_'); // 마지막 버젼은 '_' 대신에 '\0'
            outputByteSize += 3;
        }
        // 만약 마지막 char이 아니면..
        else {
            out.put((unsigned char)maxDist);
            out.put((unsigned char)maxLen);
            char curEndingChar = input->getCharByOffset(maxLen);
#ifdef DEBUG_MSG
            cout << "---------------------------------------------------------------" << endl;
            cout << "char curEndingChar = input->getCharByOffset(maxLen);" << endl;
            cout << "curEndingChar: " << curEndingChar << endl;
            input->printBuffWithPointer();
#endif
            out.put((unsigned char)curEndingChar);
            outputByteSize += 3;
        }
        
        input->moveByOffset(maxLen + 1);
#ifdef DEBUG_MSG
        cout << "---------------------------------------------------------------" << endl;
        cout << "input->moveByOffset(maxLen + 1);" << endl;
        input->printBuffWithPointer();
#endif

//        maxIteration--;
//        if (maxIteration <= 0) {
//            break;
//        }
    }
    
    return outputByteSize;
}
