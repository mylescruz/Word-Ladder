//
//  HashTable.cpp
//  Program 5
//
//  Created by Myles Cruz on 4/8/19.
//  Copyright © 2019 Program 5. All rights reserved.
//

#include "HashTable.h"
#include "Node.h"
#include <iostream>
#include <string>
using namespace std;

HashTable::HashTable(int s)
{
    if (s < 0)
        s = 100;
    
    table = new Node*[s];
    if (!table)
    {
        cout << "Unable to allocate hashtable." << endl;
        exit(1);
    }
    
    tSize = s;
    for (int i = 0; i < tSize; i++)
        table[i] = 0;   // each list is now the empty list
}
HashTable::~HashTable()
{
    if (table != 0)
    {
        for (int i = 0; i < tSize; i++)
        {
            if (table[i] != 0)  // get rid of nodes in list
            {
                Node * p = table[i];
                while (p != 0)
                {
                    table[i] = table[i]->next;
                    delete p;
                    p = table[i];
                }
            }
        }
        delete [] table;
    }
}
// Insert uses two parameters to take in the original word and the val with the @ in the word
void HashTable::Insert(string val, string word)
{
    int index = HashFcn(val); // gets the index of the word using a HashFunction
    //cout << "Index of " << val << ": " << index << endl;
    
    Node * p = new Node;
    p->data = val; // creates new node with modified word in it
    p->next = 0;
    Node * r = new Node;
    r->data = word; // creates new node with original word in it
    
    if (table[index] == 0) // if empty, place key in there first, then place original word after it
    {
        table[index] = p;
        r->next = 0;
        table[index]->next = r;
    }
    else if (table[index]->data == p->data) // if keys match, place original word after the key and in front of the previous word
    {
        Node * q = table[index];
        r->next = q->next;
        q->next = r;
    }
    else // if indeces are the same, collision which means the table size is too small
        cout << "Collision! " << val << "'s spot taken at index: " << index << endl;
}
// InsertWordtoGraph: inserts the word by a new Node into the index given by HashFcn
void HashTable::InsertWordtoGraph(string word)
{
    int index = HashFcn(word);
    
    Node * p = new Node;
    p->data = word;
    p->next = 0;
    
    if (table[index] == 0)
        table[index] = p;
    else
        cout << "Collision! " << endl;
}
// InsertinGraph: Creates new node with the word parameter, then places it at the parameter index
void HashTable::InsertinGraph(string word, int index)
{
    Node * p = new Node;
    p->data = word;
    p->next = 0;
    
    Node * q = table[index];
    p->next = q->next;
    q->next = p;
}
// Search: returns the index that the HashFcn calculates if it is in that Node
int HashTable::Search(string val) const
{
    int index = HashFcn(val);
    
    Node * p = table[index];
    while (p != 0)
    {
        if (p->data == val)
            return index;
        
        p = p->next;
    }

    return -1;
}
// getIndex: returns the Node at the index parameter
Node * HashTable::getIndex(unsigned int index) const
{
    return table[index];
}
void HashTable::Display(ostream & out) const
{
    for (int i = 0; i < tSize; i++)
    {
        Node * p = table[i];
        if (table[i] != 0) // only prints members of HashTable with values
        {
            out << "i: " << i << " ";
            while (p != 0)
            {
                if (p->next != 0)
                    out << p->data << " -> ";
                else
                    out << p->data << " ";
                p = p->next;
            }
            out << endl;
        }
    }
}
int HashTable::HashFcn(string keyword) const
{
    int sum = 0, base = 32;
    
    sum = keyword[0] - '@' + 1; // use '@' because it is the smallest of the 27 characters
    for (int i = 1; i < keyword.length(); i++)
        sum = ((keyword[i] - '@' + 1) + base * sum) % tSize;
    
    return sum % tSize;
}

