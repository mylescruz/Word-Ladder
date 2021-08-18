//
//  HashTable.h
//  Program 5
//
//  Created by Myles Cruz on 4/8/19.
//  Copyright © 2019 Program 5. All rights reserved.
//

#ifndef HashTable_h
#define HashTable_h

#include <iostream>
#include <string>
#include "Node.h"
using namespace std;

const int INUSE = 1;
const int NEVERUSED = 0;

class HashTable
{
public:
    HashTable(int s = 0);
    ~HashTable();
    
    void Insert(string val, string word);
    void InsertWordtoGraph(string word);
    void InsertinGraph(string word, int index);
    int Search(string val) const;
    Node * getIndex(unsigned int index) const;
    void Display(ostream & out) const;
    
private:
    int HashFcn(string val) const;
    
    Node * * table;
    int tSize;
};

#endif /* HashTable_h */
