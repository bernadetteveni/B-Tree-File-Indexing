//
//  BTree.hpp
//  Bernadette Veninata
//  index implemented using the B-tree algorithms as a C++ class.

#ifndef BTree_h
#define BTree_h

#include <stdio.h>
#include <iostream>

using namespace std;

const int PAGE_SIZE = 16;

struct Data{
    int offset;
    int id;
};

struct BTreeNode;

struct Keys{
    Data value; 
    BTreeNode* next;
};

struct BTreeNode{
    bool root;
    bool leaf;
    int n;
    Keys page[PAGE_SIZE];
};

class BTree{
public:
    BTree();
    bool isPSet();
    bool isISet();
    Data read();
    void write(Data x);
    bool isEmpty();
    bool isFull();
    void find(int x);
    void find(Data x, BTreeNode* q);
    void insertRoot(Data x);
    void insert(Data x, BTreeNode* q);
    bool splitRoot();
    bool splitChild(BTreeNode* q1, int k);
    void insert(Data x);
    void print();
    void printHelper(BTreeNode* p);

    
private:
    bool debug = 1;
    bool firstInsert=1;
    BTreeNode* root;
    BTreeNode* p;
    int i;
};


#endif /* BTree_h */
