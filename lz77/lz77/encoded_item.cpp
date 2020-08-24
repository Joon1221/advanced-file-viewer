#include <iostream>

#include "encoded_item.h"

using namespace std;

EncodedItem::EncodedItem() {
    dist = 0;
    len = 0;
    ch = '?';
}

EncodedItem::EncodedItem(int dist, int len, char ch) {
    this->dist = dist;
    this->len = len;
    this->ch = ch;
}

EncodedItem::~EncodedItem() {
}

void EncodedItem::setDist(int dist) {
    this->dist = dist;
}

void EncodedItem::setLen(int len) {
    this->len = len;
}

void EncodedItem::setChar(char ch) {
    this->ch = ch;
}

void EncodedItem::print() {
    cout << "(" << dist << "," << len << ")" << ch;
}

string EncodedItem::toString() {
    char buf[100];
    sprintf(buf, "(%d,%d)%c", dist, len, ch);
    return string(buf);
}
