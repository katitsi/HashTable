//Impimetation of a hash table of pointers to (std::)strings

#include "HashTable.hpp"
#include "HashTableException.hpp"
#include <iostream>

#ifndef __HASHTABLE_CPP__
#define __HASHTABLE_CPP__

using namespace std;

//Constuctor
HashTable::HashTable(int capacity) {
    size = 0;
    this->capacity = capacity;

    try {
        table = new string*[capacity];

        for (int i=0; i<capacity; i++) {
            table[i] = nullptr;
        }
    }
    catch (std::bad_alloc& ex){
        throw ex;
    }
}

//Copy constuctor
HashTable::HashTable(const HashTable &ht) {
    size = ht.getSize();
    capacity = ht.getCapacity();

    try {
        table = new string*[capacity];

        for (int i=0; i<capacity; i++) {
            if (!ht.isAvailable(i)) {
                table[i] = new string(*ht.table[i]);
            } else {
                table[i] = ht.table[i];
            }
        }
    }
    catch(std::bad_alloc& ex) {
        throw ex;
    }
}

//Deconstuctor
HashTable::~HashTable() {
    for (int i=0; i<capacity; i++) {
        if (table[i] != nullptr && table[i] != TOMB) {
            delete table[i];
        }
    }

    delete[] table;
}

bool HashTable::isEmpty(int pos) const{
    if (pos >= capacity) {
        return false;
    }

    if(table[pos] != nullptr) {
        return false;
    }

    return true;
}

bool HashTable::isTomb(int pos) const {
    if (pos >= capacity) {
        return false;
    }

    if (this->table[pos] != TOMB) {
        return false;
    }

    return true;
}

bool HashTable::isAvailable(int pos) const {
    return (isEmpty(pos) || isTomb(pos));
}

int HashTable::getSize() const {
    return size;
}

int HashTable::getCapacity() const {
    return capacity;
}

//check if a string is in hash table
bool HashTable::contains(const char *s) const {
    unsigned long hashCode;
    int initPos,pos,n;

    if (!isWord(s)) {
        return false;
    }

    hashCode = getHashCode(s);

    initPos = hashFunction(hashCode,0,capacity);
    pos = initPos;
    n=1;
    while(table[pos]!=nullptr) {
        if (table[pos] != TOMB && *table[pos] == s) {
                return true;
        }

        pos = hashFunction(hashCode,n,capacity);
        if (pos == initPos) {
            break;
        }
        n++;
    }

    return false;
}

bool HashTable::contains(const string &s) const {
    return contains(s.c_str());
}

string HashTable::print() const {
    string str;
    char buf[128];

    for(int i=0; i<capacity; i++) {
        if (!isAvailable(i)) {
            sprintf(buf, "%2d. -%s-\n", i,(*table[i]).c_str());
            str.append(buf);
        }
    }

    sprintf(buf, " --- CAPACITY: %d, SIZE: %d ---\n", capacity, size);
    str.append(buf);

    return str;
}

//add new word
bool HashTable::add(const char *s) {
    unsigned long hashCode;
    int pos,n;

    if (!isWord(s) || contains(s)) {
        return false;
    }

    if (size == capacity) {
        throw  HashTableException();
    }

    hashCode = getHashCode(s);
    pos = hashFunction(hashCode,0,capacity);
    n=1;
    while(!isAvailable(pos)) {
        pos = hashFunction(hashCode,n,capacity);
        n++;
    }

    table[pos] = new string(s);
    size++;
    return true;
}

bool HashTable::add(const string &s) {
    return add(s.c_str());
}

bool HashTable::remove(const char *s){
    unsigned long hashCode;
    int pos,n;

    if(!isWord(s) || !contains(s)) {
        return false;
    }

    hashCode = getHashCode(s);
    pos = hashFunction(hashCode,0,capacity);
    n=1;
    while(table[pos] != nullptr) {
        if (table[pos]!=TOMB && *table[pos] == s) {
            delete table[pos];
            table[pos] = TOMB;
            size--;
            break;
        }

        pos = hashFunction(hashCode,n,capacity);
        n++;
    }


    return true;
}

bool HashTable::remove(const string &s) {
    return remove(s.c_str());
}

HashTable& HashTable::operator = (const HashTable &ht) {
    //delete the previous table
    for(int i=0; i<capacity; i++) {
        if (!isAvailable(i)) {
            delete table[i];
        }
    }
    delete[] table;

    size = ht.getSize();
    capacity = ht.getCapacity();

    try {
        table = new string*[capacity];
    }
    catch(bad_alloc& ex) {
        throw ex;
    }

    //copy table
    for(int pos=0; pos<capacity; pos++) {
        if (!ht.isAvailable(pos)) {
            table[pos] = new string(*(ht.table[pos]));
        }
        else {
            table[pos] = ht.table[pos];
        }
    }

    return *this;
}

//add new word
HashTable& HashTable::operator += (const string& str) {
    add(str);
    return *this;
}

HashTable& HashTable::operator += (const char *s) {
    add(s);
    return *this;
}

//remove existing word
HashTable& HashTable::operator -= (const string& str) {
    remove(str);
    return *this;
}

HashTable& HashTable::operator -= (const char* s) {
    remove(s);
    return *this;
}

//add new word to current table
//returns new hash table, doesn't change the current
HashTable HashTable::operator + (const string& str) const {
    HashTable newHashtable(*this);
    newHashtable.add(str);
    return newHashtable;
}

HashTable HashTable::operator + (const char* s) const {
    HashTable newHashtable(*this);
    newHashtable.add(s);
    return newHashtable;

}

//remove existing word to current table
//returns new hash table, doesn't change the current
HashTable HashTable::operator - (const string& str) const {
    HashTable newHashtable(*this);
    newHashtable.remove(str);
    return newHashtable;

}

HashTable HashTable::operator - (const char* s) const {
    HashTable newHashtable(*this);
    newHashtable.remove(s);
    return newHashtable;

}

std::ostream& operator << (std::ostream& stream, const HashTable &t) {
    return stream<<t.print();
}

//Iterator tha points in the first saved word
HashTable::Iterator HashTable::begin() const {
    HashTable::Iterator it(this);

    return it;
}

//Iterator that points after the last position of the table
HashTable::Iterator HashTable::end() const {
    HashTable::Iterator it(this);

    if (size==0) {
        return it;
    }

    it.setPos(capacity);
    it.setCurr(table + capacity);

    return it;
}

#endif