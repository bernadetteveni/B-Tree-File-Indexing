//
//  BTree.cpp
//  Bernadette Veninata
//
#include "BTree.h"

BTree::BTree(){
    root = NULL;
    p = NULL;
    i = -1;
}

bool BTree::isPSet(){
    if(p == NULL)
        return false;
    return true;
}

bool BTree::isISet(){
    if(i == -1)
        return false;
    return true;
}

Data BTree::read(){
    return p -> page[i].value;
}

void BTree::write(Data x){
    p -> page [i].value = x;
}

bool BTree::isEmpty(){
    if(root == NULL)
        return true;
    return false;
}

bool BTree::isFull(){
    try{
        BTreeNode* q = new BTreeNode;
        delete q;
    }
    catch(std::bad_alloc){
        return true;
    }
    return false;
}

void BTree::find(int x){
    Data data;
    data.id = x;
    find(data, root);
    if(data.id == read().id){
        cout << "Found: " << read().id << endl;
    }
    else{
        cout << "Did not find " << data.id << endl;
    }
}

void BTree::find(Data x, BTreeNode* q){
    if(q->leaf == true){
        for(int j = q -> n - 1; j >= 0; j--){
            if(x.id == q -> page [j].value.id){
                p = q;
                i = j;
            }
        }
        if(q -> n - 1 == -1){
            p = NULL;
            i = -1;
        }
    }
    else{
        int j = q -> n - 1;
        while(j > 0 and x.id < q -> page [j].value.id){
            j--;
        }
        find(x, q -> page [j].next);
    }
}

void BTree::insertRoot(Data x){
    root = new BTreeNode;
    
    root -> root = true;
    root -> leaf = true;
    
    for(int j = 0; j <= PAGE_SIZE - 1; j++){
        root -> page [j].value.id = 0;
        root -> page [j].value.offset = 0;
        root -> page [j].next = NULL;
    }
    root -> page[0].value = x;
    root -> n = 1;
    p = root;
    i = 0;
}

void BTree::insert(Data x, BTreeNode* q){
    if(debug){
        cout << "called insert with " << x.id << " " << q->page[0].value.id << endl;
        print();
        cout << "end of print" << endl;
    }
    if(q -> n == PAGE_SIZE && q -> root == true){
        if(debug)
            cout << "q -> n == PAGE_SIZE && q -> root == true" << endl;
        if(splitRoot()){
            if(debug){
                cout << "after split root" << endl;
                print();
                cout << "end of print" << endl;
            }
            insert (x, root);
        }
        else{
            p = NULL;
            i = -1;
        }
    }
    else if(q -> leaf == true){
        if(debug)
            cout << "leaf == true" << endl;
        int j = q -> n - 1;
        while(j >= 0 and x.id < q -> page [j].value.id){
            q -> page [j + 1].value = q -> page [j].value;
            j--;
        }
        j++;
        q -> page [j].value = x;
        q -> n++;
        p = q;
        i = j;
    }
    else{
        if(debug)
            cout << "leaf == false" << endl;
        int j = q -> n - 1;
        while(j > 0 and x.id < q -> page [j].value.id){
            j--;
        }
        if(q -> page [j].next -> n == PAGE_SIZE){
            if(splitChild (q, j)){
                if(debug){
                    cout << "after split child" << endl;
                    print();
                    cout << "end of print" << endl;
                }
                insert(x, q);
            }
            else{
                p = NULL;
                i = -1;
            }
        }
        else{
            if(debug)
                cout << "insert into child of" << q -> page [j].value.id << endl;
            insert (x, q -> page [j].next);
            q -> page [j].value = q -> page [j].next -> page [0].value;
        }
    }
}

void BTree::insert(Data x){
    insert(x, root);
}

bool BTree::splitRoot(){
    if(debug)
        cout << "splitting root" << endl;
    BTreeNode* q1 = new BTreeNode;
    q1 = root;
    if(isFull())
        return false;
    BTreeNode* q2 = new BTreeNode;
    if(isFull())
        return false;
    root = new BTreeNode;
    for(int j = 0; j <= PAGE_SIZE - 1; j++){
        root -> page [j].value.id = 0;
        root -> page [j].next = NULL;
        q2 -> page [j].value.id = 0;
        q2 -> page [j].next = NULL;
    }
    for(int  j = 0; j <= ((PAGE_SIZE / 2) - 1); j++){
        q2 -> page [j].value = q1 -> page [((PAGE_SIZE + 1) / 2) + j].value;
        q2 -> page [j].next = q1 -> page [((PAGE_SIZE + 1) / 2) + j].next;
    }
    q1 -> root = false;
    q2 -> root = false;
    if(q2 -> page [0].next == NULL){
        q1 -> leaf = true;
        q2 -> leaf = true;
    }
    else{
        q1 ->leaf = false;
        q2 -> leaf = false;
    }
    q1 -> n = (PAGE_SIZE + 1) / 2;
    q2 -> n = PAGE_SIZE / 2;
    root -> root = true;
    root -> leaf = false;
    root -> n = 2;
    root -> page [0].value = q1 -> page [0].value;
    root -> page [0].next = q1;
    root -> page [1].value = q2 -> page [0].value;
    root -> page [1].next = q2;
    return true;
}

bool BTree::splitChild(BTreeNode* q1, int k){
    if(debug)
        cout << "splitting child" << endl;
    if(isFull())
        return false;
    BTreeNode* q2 = new BTreeNode;
    for(int j = 0; j <= PAGE_SIZE - 1; j++){
        q2 -> page [j].value.id = 0;
        q2 -> page [j].value.offset = 0;
        q2 -> page [j].next = NULL;
    }
    for(int j = 0; j <= ((PAGE_SIZE / 2) - 1); j++){
        q2 -> page [j].value = q1 -> page [k].next -> page [((PAGE_SIZE + 1) / 2) + j].value;
        q2 -> page [j].next = q1 -> page [k].next -> page [((PAGE_SIZE  + 1) / 2) + j].next;
    }
    q2 -> root = false;
    if(q2 -> page [0].next == NULL){
        q2 -> leaf = true;
    }
    else{
        q2 -> leaf = false;
    }
    q2 -> n = PAGE_SIZE / 2;
    q1 -> page [k].next -> n = (PAGE_SIZE + 1) / 2;
    int  j = q1 -> n - 1;
    while(j > k){
        q1 -> page [j + 1].value = q1 -> page [j].value;
        q1 -> page [j + 1].next = q1 -> page [j].next;
        j--;
    }
    j++;
    q1 -> page [j].value = q2 -> page [0].value;
    q1 -> page [j].next = q2;
    q1 -> n++;
    return true;
}

void BTree::print(){
    printHelper(root);
}

void BTree::printHelper(BTreeNode* p){
    if(p == NULL){
        return;
    }
    firstInsert = true;
    
    if(firstInsert){
        cout << "R (";
        cout << p->page[i].value.id << ", ";
        cout << p->page[i].value.offset;
        cout << ") ";
        firstInsert = false;
    }
    
    cout << endl;
    if(p->leaf){
        for(int i = 1; i <= p->n-1; i++){
            cout << "L (";
            cout << p->page[i].value.id << ", ";
            cout << p->page[i].value.offset;
            cout << ") ";
        }
        cout << "| ";
        cout << endl;
    }
    
    for(int i = 0; i <= p->n-1; i++){
        printHelper(p->page[i].next);
    }
}
