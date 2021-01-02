// db_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <new>
#include <memory>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <queue>
#include <deque>
#include <list>
#include <unordered_map>
#include <math.h>

using namespace std;

static int MAX = 4;
class Node;

class mapping {
public:
    int key;
    Node* ptr;
    mapping()
    {
        key = 0;
        ptr = nullptr;
    }
};
// BP node 
class Node {
public:
    bool IS_LEAF;
    int size;
    Node* parent;
    mapping array[5];
    Node();
    void insertKey(int x);
    Node* Splitleaf(Node*, bool, int);
    void MoveHalfleaf(Node*, int);
    void InsertNodeAfter(Node*, int, Node*);
    int pointerindex(Node*);
    void insertleaf(int);
    int KeyIndex(int);
    void Splitinternal(Node*);
    void MoveHalfinternal(Node*);
};

// BP tree 
class BPTree {
public:
    Node* root;
    void insertInternal(int,
        Node*,
        Node*);
    Node* findParent(Node*, Node*);
    Node* searchNode(int);
    void InitRoot(int);
    void InsertIntoLeaf(int);
    void InsertIntoParent(Node*, int, Node*);

    BPTree();
    void insert(int);
    void display(Node*);
};

// Constructor of Node 
Node::Node()
{
    for (int i = 0; i < MAX; i++)
    {
        array[i].key = 0;
        array[i].ptr = nullptr;
    }
    
    this->IS_LEAF = true;
    size = 0;
}

int Node::KeyIndex(int x)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i].key >= x)
            return i;
    }
    return size;
}

void Node::insertleaf(int x)
{
    int index = KeyIndex(x);
    for (int i = size; i > index; i--)
    {
        array[i].key = array[i - 1].key;
    }
    array[index].key = x;
    size++;
}
void Node::MoveHalfleaf(Node* recipient, int newkey)
{
    int total = MAX;
    int copyidx = total / 2;
    for (int i = copyidx; i < total; i++)
    {
        recipient->array[i - copyidx].key = array[i].key;
    }
    size = copyidx;
    recipient->size = total - copyidx;
}

void Node::MoveHalfinternal(Node* recipient)
{
    int totalsize = MAX+ 1;
    int copyidx = ceil((double)totalsize / 2);
    for (int i = copyidx; i < totalsize; i++)
    {
        recipient->array[i - copyidx].key = array[i].key;
        recipient->array[i - copyidx].ptr = array[i].ptr;
    }
    size = copyidx;
    recipient -> size = totalsize - copyidx;
}

Node* Node::Splitleaf(Node* leaf, bool leafnode, int newkey)
{
    Node* newnode = new Node;
    newnode->IS_LEAF = true;
    leaf->MoveHalfleaf(newnode, newkey);
    return newnode;
}

void Node::Splitinternal(Node* new_node)
{
    new_node->IS_LEAF = false;
    MoveHalfinternal(new_node);
}

int Node::pointerindex(Node* node)
{
    for (int i = 0; i < size + 1; i++)
    {
        if (array[i].ptr == node)
            return i;
    }
    return -1;
}

void Node::InsertNodeAfter(Node* old_node, int key, Node* new_node)
{
    int idx = pointerindex(old_node) + 1;
    for (int i = size; i > idx; i--)
    {
        array[i].key = array[i - 1].key;
        array[i].ptr = array[i - 1].ptr;
    }
    array[idx].key = key;
    array[idx].ptr = new_node;
    size++;
}

// Initialise the BPTree Node 
BPTree::BPTree()
{
    root = NULL;
}


Node* BPTree::searchNode(int x)
{
    // If tree is empty 
    if (root == NULL)
    {
        cout << "Tree is empty" << endl;
        return nullptr;
    }
    // Traverse to find the value 
    else
    {
        Node* cursor = root;
        // Till we reach leaf node 
        while (cursor->IS_LEAF == false)
        {
            for (int i = 1; i < cursor->size; i++) {

                // If the element to be 
                // found is not present 
                if (x < cursor->array[i].key) {
                    cursor = cursor->array[i-1].ptr;
                    break;
                }

                // If reaches end of the 
                // cursor node 
                if (i == cursor->size - 1) {
                    cursor = cursor->array[cursor->size - 1].ptr;
                    break;
                }
            }
        }
        // Traverse the leaf cursor and find the node with value x 
        return cursor;
    }
}

void BPTree::InitRoot(int x)
{
    this->root = new Node;
    root->IS_LEAF = true;
    root->insertleaf(x);
/*    root->key[0] = x;
    root->size = 1;
    root->array[1].key = x;
*/
}


// Function to implement the Insert 
// Operation in B+ Tree
void BPTree::insert(int x)
{
    if (this->root == nullptr)
    {
        InitRoot(x);
        return;
    }
    Node* leafnode = searchNode(x);
    leafnode->insertleaf(x);
    if (leafnode->size > MAX - 1)
    {
        Node* new_leaf = leafnode->Splitleaf(leafnode, true, x);
        Node* parent = findParent(this->root, leafnode);
        new_leaf->parent = parent;
        int midkey = new_leaf->array[0].key;
        InsertIntoParent(leafnode, midkey, new_leaf);
    }
}

void BPTree::InsertIntoParent(Node* old_node, int midkey, Node* new_node)
{
    if (old_node == root)
    {
        Node* new_root = new Node();
        new_root->size = 2;
        new_root->IS_LEAF = false;
        new_root->array[0].ptr = old_node;
        new_root->array[1].ptr = new_node;
        new_root->array[1].key = midkey;
        root = new_root;
    }
    else
    {
        Node* parent = findParent(root, old_node);
        parent->InsertNodeAfter(old_node, midkey, new_node);
        if (parent->size > MAX)
        {
            Node* new_internal = new Node;
            parent->Splitinternal(new_internal);
            InsertIntoParent(parent, new_internal->array[0].key, new_internal);
        }
#if 0
        if (parent->size < MAX)
        {
            
        }
        else
        {
            Node* new_internal = new Node;
            parent->Splitinternal(new_internal);
            InsertIntoParent(parent, new_internal->array[0].key, new_internal);
        }
#endif
    }
}


// Function to find the parent node 
Node* BPTree::findParent(Node* cursor, Node* child)
{
    if (cursor == child)
        return cursor;
    Node* parent = nullptr;
    queue<Node*>myQ;
    myQ.push(cursor);
    while (!myQ.empty())
    {
        int size = myQ.size();
        for (int i = 0; i < size; i++)
        {
            Node* first = myQ.front();
            myQ.pop();
            for (int j = 0; j < MAX; j++)
            {
                if (first->array[j].ptr == child)
                    return first;
                else if (first->array[j].ptr != nullptr)
                    myQ.push(first->array[j].ptr);
            }
        }
    }
    return nullptr;
}

// Driver Code 
int main()
{
    // Create B+ Tree
    BPTree Btree;
#if 0
    Node* a = new Node;
    Node* b = new Node;
    a->size = 3; 
    a->array[0].key = 3;
    a->array[1].key = 4;
    a->array[2].key = 6;
    a->array[3].key = 7;
    a->MoveHalfleaf(b, 0);
    cout << b->size << endl;
    cout << a->size << endl;
#endif
#if 0
    
   
    Btree.insert(1);
    Btree.insert(3);
    Btree.insert(5);
    Btree.insert(7);
    Btree.insert(9);
    Btree.insert(2);
    Btree.insert(4);
    Btree.insert(6);
    Btree.insert(8);
    Btree.insert(10);

    cout << "here" << endl;
    auto a = Btree.searchNode(8);
    cout << a->size << endl;
#endif
#if 1
    Btree.root = new Node;
    Btree.root->size = 2;

    Btree.root->array[0].key = 0;
    Btree.root->array[1].key = 16;
    Btree.root->IS_LEAF = false;
    
    Node* A = new Node;
    Node* B = new Node;    
    A->size = 3;
    A->IS_LEAF = true;
    A->array[0].key = 1;
    A->array[1].key = 4;
    A->array[2].key = 9;
 
    B->size = 2;
    B->IS_LEAF = true;
    B->array[0].key = 16;
    B->array[1].key = 25;

    Btree.root->array[0].ptr = A;
    Btree.root->array[1].ptr = B;
    Btree.insert(20);
    Btree.insert(13);
    Btree.insert(15);
    Btree.insert(10);
    Btree.insert(11);
    Btree.insert(12);
    cout << Btree.root->size << endl;
#endif

    return 0;
}
