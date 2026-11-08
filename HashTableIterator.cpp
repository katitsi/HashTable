#include "HashTable.hpp"

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

//Constructor
HashTable::Iterator::Iterator(const HashTable* t) : ht(t){
    if (ht->getSize() == 0) {
        curr = t->table;
        position = 0;
    }
    else {
        for (int pos=0; pos<t->getCapacity(); pos++) {
            if(!t->isAvailable(pos)) {
                position = pos;
                curr = &(t->table[pos]);
                break;
            }
        }
    }
}

//Copy constructor
HashTable::Iterator::Iterator(const Iterator& it) : curr(it.curr),
                                                    ht(it.ht),
                                                    position(it.position){}

HashTable::Iterator& HashTable::Iterator::operator = (const Iterator& it) {
    ht = it.ht;
    curr = it.curr;
    position = it.position;

    return *this;
}

//move the iterator to the next saved word and change the
//value of the pointer curr to the new one.
HashTable::Iterator HashTable::Iterator::operator ++ () {
    int pos=0;

    if (ht->getSize() == 0) {
        return *this;
    }

    for (pos=position+1; pos < ht->capacity; pos++) {
        if (!ht->isAvailable(pos)) {
            curr = &(ht->table[pos]);
            break;
        }
    }

    position = pos;
    if(pos >= ht->capacity) {
        setCurr(ht->table + ht->capacity);
    }


    return *this;
}

//move the iterator to the next saved word but doesn't
//change the value of the pointer curr to the new one.
HashTable::Iterator HashTable::Iterator::operator ++ (int a) {
    HashTable::Iterator newIT(*this);
    int pos=0;

    if (ht->getSize() == 0) {
        return *this;
    }

    for (pos=position+1; ht->capacity; pos++) {
        if (!ht->isAvailable(pos)) {
            curr = &(ht->table[pos]);
            break;
        }
    }

    position = pos;
    if(pos >= ht->capacity) {
        setCurr(ht->table + ht->capacity);
    }

    return newIT;
}

//checks if two iterators points at the same position in the table
bool HashTable::Iterator::operator == (const Iterator& it) const {
    if (curr == it.curr && position == it.position && ht == it.ht) {
        return true;
    }
    return false;
}

//ckecks if two iterators points at different position in the table
bool HashTable::Iterator::operator != (const Iterator& it) const {
    if (curr != it.curr && position!=it.position && ht == it.ht) {
        return true;
    }
    return false;
}

//returns the string that the iterator is pointing
const string& HashTable::Iterator::operator * () {
    return **curr;
}

const string* HashTable::Iterator::operator -> () {
    return *curr;
}

int HashTable::Iterator::pos() const {
    return position;
}

void HashTable::Iterator::setPos(int pos) {
    position = pos;
}

void HashTable::Iterator::setCurr(string** current) {
    curr = current;
}


#endif
