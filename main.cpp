//
//  main.cpp
//  Bernadette Veninata
//  A program which indexes a file containing U.S. census records for 100 adults, implemented using the B-tree algorithms
//

    
#include <iostream>
#include <fstream>
#include <cmath>
#include "BTree.h"
    
using namespace std;

int main(){
    BTree tree1;
    Data data;
    
    data.id = 100;
    data.offset = 100;
    
    tree1.insertRoot(data);
    
    data.id = 2;
    data.offset = 2;
    tree1.insert(data);
    
    data.id = 87;
    data.offset = 87;
    tree1.insert(data);
    
    data.id = 17;
    data.offset = 17;
    tree1.insert(data);
    
    data.id = 45;
    data.offset = 45;
    tree1.insert(data);
    
    data.id = 99;
    data.offset = 99;
    tree1.insert(data);
    
    data.id = 33;
    data.offset = 33;
    tree1.insert(data);

    tree1.print();
    
    return 0;
}
