#ifndef ENCODED_ITEM_H
#define ENCODED_ITEM_H

#include <string>

using namespace std;

class EncodedItem {
private:
    int dist;
    int len;
    char ch;
public:
    EncodedItem();
    EncodedItem(int dist, int len, char ch);
    virtual ~EncodedItem();
    
    void setDist(int dist);
    void setLen(int len);
    void setChar(char ch);
    
    void print();
    string toString();
};

#endif
