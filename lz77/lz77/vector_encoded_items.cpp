#include <iostream>

#include "vector_encoded_items.h"

VectorEncodedItems:: VectorEncodedItems() {
    
}

VectorEncodedItems::~ VectorEncodedItems() {
    
}

void VectorEncodedItems ::addItem(EncodedItem item) {
    items.push_back(item);
}

void VectorEncodedItems::print() {
    for (int i = 0; i < items.size(); i++) {
        items[i].print();
        cout << " ";
    }
    cout << endl;
}

string VectorEncodedItems::toString() {
    string s = "";
    for (int i = 0; i < items.size(); i++) {
        s += items[i].toString();
        if (i < items.size() - 1) {
            s += " ";
        }
    }
    return s;
}
