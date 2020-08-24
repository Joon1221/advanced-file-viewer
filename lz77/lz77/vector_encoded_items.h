#ifndef VECTOR_ENCODED_ITEMS_H
#define VECTOR_ENCODED_ITEMS_H

#include <stdio.h>
#include <vector>

using namespace std;

#include "encoded_items.h"

class VectorEncodedItems : public EncodedItems {
private:
    vector<EncodedItem> items;

public:
    VectorEncodedItems();
    virtual ~ VectorEncodedItems();
    
    void addItem(EncodedItem item);
    void print();
    string toString();
};

#endif
