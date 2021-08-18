//
//  Program5Main.cpp
//  Program 5
//
//  Created by Myles Cruz on 4/5/19.
//  Copyright © 2019 Program 5. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "HashTable.h"
#include "Node.h"
using namespace std;

const int TABLESIZE = 28541; // Closest prime number to 27*32^2 + 27*32 + 27 = 28539 (Horner's Rule) 28541
const int NUM_WORDS = 100;
const int GRAPHSIZE = 2131; // Number found to cause no collisions in the Graph
const int WORDLENGTH = 3;
const int UNVISITED = 0;
const int VISITED = 1;
const int DONE = 2;

void Initialize(HashTable & HT, HashTable & Graph);
void CreateGraph(HashTable & Graph, HashTable & HT);
void StartGame(HashTable & Graph);
void Game(HashTable & Graph, string * parent, int * status, int & parentSize, string word, string endWord, bool & found);
void OutputFile(HashTable & HT, HashTable & Graph);

int main() {
    
    HashTable HT(TABLESIZE);
    HashTable Graph(GRAPHSIZE);
    
    // CREATE HASHTABLE (PART I)
    Initialize(HT, Graph);
    
    // CREATE GRAPH (PART II)
    CreateGraph(Graph, HT);
    
    //OutputFile(HT, Graph);
    
    // GAME (PART III)
    StartGame(Graph);
    
    return 0;
}

void Initialize(HashTable & HT, HashTable & Graph)
{
    ifstream theFile;
    string word, temp;
    
    theFile.open("wordlist.txt");
    if (!theFile.is_open())
    {
        cout << "Unable to open file." << endl;
        return;
    }
    
    while (!theFile.eof())
    {
        theFile >> word;
        if (word == "STOP")
            break;
        
        for (int i = 0; i < WORDLENGTH; i++) // for loops replaces each letter in word with @
        {
            temp = word;
            temp[i] = '@';
            HT.Insert(temp, word);
        }
        
        Graph.InsertWordtoGraph(word);
    }
    theFile.close();
    //HT.Display(cout);
}
void CreateGraph(HashTable & Graph, HashTable & HT)
{
    string word, temp;
    for (int i = 0; i < GRAPHSIZE; i++)
    {
        Node * p = Graph.getIndex(i); // gets Node of each index of the HashTable Graph
        if (p != 0) // if index is not empty
        {
            string word = p->data;
            for (int j = 0; j < WORDLENGTH; j++) // searches through HashTable the way it was inserted
            {
                temp = word;
                temp[j] = '@';
                
                int pos = HT.Search(temp);
                Node * q = HT.getIndex(pos);
                
                while (q != 0)
                {
                    temp = q->data;
                    // if word does not have the @ in it or is not the word at the Graph's index, then insert
                    if (temp[0] != '@' && temp[1] != '@' && temp[2] != '@' && temp != word)
                        Graph.InsertinGraph(temp, i);
                    q = q->next;
                }
            }
        }
    }
    Graph.Display(cout);
}
void OutputFile(HashTable & HT, HashTable & Graph) // Output File
{
    ofstream HashTableFile, GraphFile;
    
    HashTableFile.open("Program5PartI.txt");
    HT.Display(HashTableFile); // outputs contents of HashTable
    HashTableFile.close();
    cout << "HashTable outputted to file: Program5PartI.txt" << endl;
    
    GraphFile.open("Program5PartII.txt");
    Graph.Display(GraphFile); // outputs contents of Graph
    GraphFile.close();
    cout << "Graph outputted to file: Program5PartII.txt" << endl << endl;
}
void StartGame(HashTable & Graph)
{
    cout << "                       STARTING GAME" << endl;
    cout << "*******************************************************************" << endl;
    
    string * parent = new string[NUM_WORDS];
    // parent array keeps array of words in the path from startWord to endWord
    int * status = new int[GRAPHSIZE];
    // status array tells if index of Graph has been visited or not
    
    for (int i = 0; i < GRAPHSIZE; i++)
        status[i] = UNVISITED;
    
    string startWord, endWord;
    int parentSize = 0;
    bool found = false; // used to determine if path is found
    
    cout << "Enter word to start: ";
    cin >> startWord;
    cout << "Enter word to find: ";
    cin >> endWord;
    
    if (startWord != endWord)
        Game(Graph, parent, status, parentSize, startWord, endWord, found);
    else
    {
        cout << startWord << " and " << endWord << " are the same." << endl;
        found = true;
    }
    
    if (!found)
        cout << "No path found from " << startWord << " to " << endWord << endl;
    
    cout << "*******************************************************************" << endl;
}
void Game(HashTable & Graph, string * parent, int * status, int & parentSize, string word, string endWord, bool & found)
{
    if (found == true) // if path found, return until original call to exit
        return;
    
    found = false; // if no path found, found will continue to be false
    int index = Graph.Search(word);
    if (index == -1) // if Search returns -1, word is not in Graph
    {
        cout << word << " is not in the Graph." << endl;
        return;
    }
    Node * p = Graph.getIndex(index); // get Node of index at startWord
    
    if (status[index] == UNVISITED)
    {
        status[index] = VISITED; // marks index as VISITED
        parent[parentSize] = word; // gives parent array value of word being called in the Game
        parentSize++;
        while (p != 0)
        {
            if (p->data == endWord) // if word is in Node, found is true
            {
                parent[parentSize] = endWord;
                cout << "Path found: " << endl;
                found = true;
                // prints out parent array which is list of order of previous words in graph
                for (int i = 0; i < parentSize; i++)
                    cout << parent[i] << " -> ";
                cout << endWord << endl;
                
                return;
            }
            
            // checks the location of next word and calls Game if UNVISITED
            string temp = p->data;
            int loc = Graph.Search(temp);
            if (status[loc] == UNVISITED)
            {
                Game(Graph, parent, status, parentSize, temp, endWord, found);
                if (found)
                    return;
            }
            
            p = p->next;
        }
        // if reaches the end of the Node, marks index as DONE so it is no longer revisited
        // decreases parentSize because that word is no longer part of the path
        status[index] = DONE;
        parentSize--;
        string temp = parent[parentSize];
        
        if (found)
            return;
    }
}
