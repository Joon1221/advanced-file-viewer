#ifndef ENCODED_ITEMS_H
#define ENCODED_ITEMS_H

#include <string>

using namespace std;

#include "encoded_item.h"

class EncodedItems { // abstract class = interface
public:
    EncodedItems() {}
    virtual ~EncodedItems() {}
    
    virtual void addItem(EncodedItem item) = 0;
    virtual void print() = 0;
    virtual string toString() = 0;
};


#endif
