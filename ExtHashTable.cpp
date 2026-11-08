//Implementation of a extensible hash table of pointers to (std::)strings

#include "ExtHashTable.hpp"
#include "HashTableException.hpp"
#include <iostream>

#ifndef __EXTHASHTABLE__CPP_
#define __EXTHASHTABLE__CPP_

using namespace std;

//Constructor
ExtHashTable::ExtHashTable(double upper_bound_ratio, double lower_bound_ratio, int capacity): HashTable(capacity){
    this->upper_bound_ratio = upper_bound_ratio;
    this->lower_bound_ratio = lower_bound_ratio;
}

//Copy constructor
ExtHashTable::ExtHashTable(const ExtHashTable &t): HashTable(t) {
    capacity = t.getCapacity();
    size = t.getSize();
    upper_bound_ratio = t.upper_bound_ratio;
    lower_bound_ratio = t.lower_bound_ratio;
}

//Function that determines if the hash table needs to be rehashed or not
//Updates the table.
void ExtHashTable::rehash() {
    double loadFactor;
    string** newTable;
    int newCapacity,n,newPos;
    unsigned long hashCode;

    if (size == 0) {
        return;
    }

    loadFactor = (double)size/capacity;
    if (loadFactor >= upper_bound_ratio) {
        newCapacity = capacity*2;
    }
    else if (loadFactor <= lower_bound_ratio ) {
        newCapacity = capacity/2;
    }
    else {
        return;
    }

    //init new hash table
    try {
        newTable = new string*[newCapacity];

        for (int i=0; i<newCapacity; i++) {
            newTable[i] = nullptr;
        }
    }
    catch (bad_alloc& ex) {
        throw ex;
    }

    //copy the strings of the previous hash table to the new hash table
    for(int pos=0; pos<capacity; pos++) {
        if (!isAvailable(pos)) {
            hashCode = getHashCode((*table[pos]).c_str());
            newPos = hashFunction(hashCode,0,newCapacity);
            n=1;
            while (newTable[newPos] != nullptr) {
                newPos = hashFunction(hashCode,n,newCapacity);
                n++;
            }

            newTable[newPos] = table[pos];
            table[pos] = nullptr;
        }
    }
    delete[] table;
    table = newTable;
    capacity = newCapacity;

    cout<<"--> Size: "<<size<<", New capacity: "<<capacity<<endl;
}

bool ExtHashTable::add(const string& str) {
    return add(str.c_str());
}

bool ExtHashTable::add(const char* s) {
    unsigned long hashCode;
    int pos,n;

    if (contains(s) || !isWord(s)) {
        return false;
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
    rehash();
    return true;
}

bool ExtHashTable::remove(const string& str) {
    return remove(str.c_str());
}

bool ExtHashTable::remove(const char* s) {
    if (HashTable::remove(s)) {
        rehash();
        return true;
    }
    return false;
}

ExtHashTable& ExtHashTable::operator = (const ExtHashTable &t) {
    for (int i=0; i<capacity; i++) {
        if (!isAvailable(i)) {
            delete table[i];
        }
    }
    delete[] table;

    size = t.getSize();
    capacity = t.getCapacity();
    upper_bound_ratio = t.upper_bound_ratio;
    lower_bound_ratio = t.lower_bound_ratio;

    try {
        table = new string*[capacity];
    }
    catch (bad_alloc& ex) {
        throw ex;
    }

    //copy the hash table
    for(int pos=0; pos<capacity; pos++) {
        if (!t.isAvailable(pos)) {
            table[pos] = new string(*(t.table[pos]));
        }
        else {
            table[pos] = t.table[pos];
        }
    }

    return *this;
}

//add new string to the current hash table
//the current ht doesn't change
//return a new hash table
ExtHashTable ExtHashTable::operator + (const string& str) const {
    ExtHashTable newExtHT(*this);

    newExtHT.add(str);
    return newExtHT;
}

ExtHashTable ExtHashTable::operator + (const char* s) const {
    ExtHashTable newExtHT(*this);

    newExtHT.add(s);
    return newExtHT;
}

//remove existing string to the current hash table
//the current ht doesn't change
//return a new hash table
ExtHashTable ExtHashTable::operator - (const string& str) const {
    ExtHashTable newExtHT(*this);

    newExtHT.remove(str);
    return newExtHT;
}

ExtHashTable ExtHashTable::operator - (const char* s) const {
    ExtHashTable newExtHT(*this);

    newExtHT.remove(s);
    return newExtHT;
}

//add new string to the current hash table
ExtHashTable& ExtHashTable::operator += (const string& str) {
    add(str);
    return *this;
}

ExtHashTable& ExtHashTable::operator += (const char* s) {
    add(s);
    return *this;
}

//remove existing string of the current hash table
ExtHashTable& ExtHashTable::operator -= (const string& str) {
    remove(str);
    return *this;
}

ExtHashTable& ExtHashTable::operator -= (const char *s){
    remove(s);
    return *this;
}

//Union
ExtHashTable ExtHashTable::operator + (const ExtHashTable &t) const{
    //HashTable newHashT(*this);
    ExtHashTable newHashT(*this);

    for(int pos = 0; pos<t.getCapacity(); pos++) {
        if (!t.isAvailable(pos)) {
            newHashT.HashTable::add(*(t.table[pos]));
        }
    }

    return newHashT;

}

ExtHashTable& ExtHashTable::operator += (const ExtHashTable &t) {
    for(int pos = 0; pos<t.getCapacity(); pos++) {
        if (!t.isAvailable(pos)) {
            HashTable::add(*(t.table[pos]));
        }
    }

    return *this;
}
#endif














