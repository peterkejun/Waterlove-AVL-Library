#include "AVLNode.h"
#include "AVLTreeIterator.h"
#include <vector>
#include <iostream>
using namespace std;

#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

template<class T>
class AVLTree {
    
private:
    AVLNode<T> * root;
    unsigned int size;

    AVLNode<T> *rotateRR(AVLNode<T> *);
    AVLNode<T> *rotateLL(AVLNode<T> *);
    AVLNode<T> *rotateLR(AVLNode<T> *);
    AVLNode<T> *rotateRL(AVLNode<T> *);
    AVLNode<T> *balance(AVLNode<T> *);

    int heightDifference(AVLNode<T> *) const;

    void recursiveDeallocate(AVLNode<T> *);
    AVLNode<T> * recursiveInsert(AVLNode<T> *, T);
    int recursiveHeight(AVLNode<T> *) const;
    AVLNode<T> *recursiveFind(AVLNode<T> *, T) const;
    AVLNode<T> *recursiveDelete(AVLNode<T> *, T);
    AVLNode<T> *recursiveMinimum(AVLNode<T> *) const;
    AVLNode<T> *recursiveMaximum(AVLNode<T> *) const;
    void recursivePrint(AVLNode<T> *, int) const;
    AVLNode<T> *recursiveClone(AVLNode<T> *) const;
    AVLNode<T> *recursiveParent(AVLNode<T> *, T) const;
public:

    class Predicate {
    public:
        virtual bool evaluate(AVLNode<T> *) const;
    };

    class MapFunction {
    public:
        virtual void apply(AVLNode<T> *) const;
    };

    class ReduceFunction {
    public: 
        virtual int reduce(AVLNode<T> *, int) const;
    };

    AVLTree();
    AVLTree(AVLNode<T> *root);
    ~AVLTree();

    void insert(T);
    void delete_(T);
    AVLNode<T> * find(T) const;
    void print() const;

    // operations
    AVLTree<T> *clone() const;
    AVLTree<T> *split(T);   
    vector<AVLNode<T> *> toVector() const;
    void join(AVLTree<T> *);
    AVLNode<T> *max() const;
    AVLNode<T> *min() const;
    AVLNode<T> *popMax();
    AVLNode<T> *popMin();
    unsigned int count(T) const;
    vector<AVLNode<T> *> filter(Predicate *) const;
    unsigned int getSize() const;
    const AVLNode<T> *getRoot() const;
    void map(MapFunction *);
    int reduce(ReduceFunction *, int) const;
    AVLTreeIterator<T> iterator() const;
};

template<class T>
AVLTree<T>::AVLTree(): root(nullptr), size(0) {}

template<class T>
AVLTree<T>::AVLTree(AVLNode<T> *root): root(root) {
    AVLTreeIterator<T> iterator(this);
    unsigned int counter = 0;
    while (iterator.hasNext()) {
        counter++;
        iterator.next();
    }
    size = counter;
}

template<class T>
AVLTree<T>::~AVLTree() {
    recursiveDeallocate(root);
}

template<class T>
void AVLTree<T>::recursiveDeallocate(AVLNode<T> * node) {
    if (node == nullptr) {
        return;
    }
    recursiveDeallocate(node->getLeft());
    recursiveDeallocate(node->getRight());
    delete node;
}

template<class T>
int AVLTree<T>::recursiveHeight(AVLNode<T> * node) const {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = recursiveHeight(node->getLeft());
    int rightHeight = recursiveHeight(node->getRight());
    int max_height = leftHeight > rightHeight ? leftHeight : rightHeight;
    return max_height + 1;
}

template<class T>
int AVLTree<T>::heightDifference(AVLNode<T> * node) const {
    return recursiveHeight(node->getLeft()) - recursiveHeight(node->getRight());
}

template<class T>
AVLNode<T> * AVLTree<T>::balance(AVLNode<T> * node) {
    int heightDifference = this->heightDifference(node);
    // cout << "balancing " << node->getData() << " " << heightDifference << endl;
    if (heightDifference < -1) {
        if (this->heightDifference(node->getRight()) <= 0) {
            node = rotateRR(node);
        } else {
            node = rotateRL(node);
        }
    } else if (heightDifference > 1) {
        if (this->heightDifference(node->getLeft()) <= 0) {
            node = rotateLR(node);
        } else {
            node = rotateLL(node);
        }
    }
    // recursivePrint(node, 0);
    return node;
}

template<class T>
AVLNode<T> *AVLTree<T>::rotateRR(AVLNode<T> * parent){
    AVLNode<T> *temp = parent->getRight();
    parent->setRight(temp->getLeft());
    temp->setLeft(parent);
    return temp;
}

template<class T>
AVLNode<T> *AVLTree<T>::rotateLL(AVLNode<T> * parent){
    AVLNode<T> *temp = parent->getLeft();
    parent->setLeft(temp->getRight());
    temp->setRight(parent);
    return temp;
}

template<class T>
AVLNode<T> *AVLTree<T>::rotateLR(AVLNode<T> * parent){
    AVLNode<T> * temp = parent->getLeft();
    parent->setLeft(rotateRR(temp));
    return rotateLL(parent);
}

template<class T>
AVLNode<T> *AVLTree<T>::rotateRL(AVLNode<T> * parent){
    AVLNode<T> * temp = parent->getRight();
    parent->setRight(rotateRR(temp));
    return rotateRR(parent);
}

template<class T>
AVLNode<T> * AVLTree<T>::recursiveFind(AVLNode<T> *node, T data) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->compare(data) == AVLNode<T>::EQUAL) {
        return node;
    }
    AVLNode<T> *leftFind = recursiveFind(node->getLeft(), data);
    if (leftFind != nullptr) {
        return leftFind;
    }
    AVLNode<T> *rightFind = recursiveFind(node->getRight(), data);
    return rightFind;
}

template<class T>
AVLNode<T> * AVLTree<T>::find(T data) const {
    return recursiveFind(root, data);
}

template<class T>
void AVLTree<T>::recursivePrint(AVLNode<T> *node, int space) const {
    if (node == nullptr)
        return;

    space += 10;
    recursivePrint(node->getRight(), space);
    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";
    cout << node->getData() << "\n";
    recursivePrint(node->getLeft(), space);
}

template<class T>
void AVLTree<T>::print() const {
    recursivePrint(root, 0);
}

template<class T>
AVLNode<T> *AVLTree<T>::recursiveMinimum(AVLNode<T> * node) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->getLeft() == nullptr) {
        return node;
    } else {
        return recursiveMinimum(node->getLeft());
    }
}

template<class T>
AVLNode<T> *AVLTree<T>::recursiveMaximum(AVLNode<T> * node) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->getRight() == nullptr) {
        return node;
    } else {
        return recursiveMaximum(node->getRight());
    }
}

template<class T>
AVLNode<T> *AVLTree<T>::recursiveDelete(AVLNode<T> *node, T data) {
    if (node == nullptr) {
        return nullptr;
    } else if (node->compare(data) == AVLNode<T>::LESS) {
        node->setRight(recursiveDelete(node->getRight(), data));
    } else if (node->compare(data) == AVLNode<T>::GREATER) {
        node->setLeft(recursiveDelete(node->getLeft(), data));
    } else {
        if (node->getLeft() != nullptr && node->getRight() != nullptr) {
            AVLNode<T> *temp = recursiveMinimum(node->getRight());
            node->setData(temp->getData());
            node->setRight(recursiveDelete(node->getRight(), node->getData()));
        } else {
            AVLNode<T> * temp = node;
            if (node->getLeft() == nullptr) {
                node = node->getRight();
            } else if (node->getRight() == nullptr) {
                node = node->getLeft();
            }
            delete temp;
        }
    }

    if (node != nullptr) {
        return balance(node);
    }
    return nullptr;
}

template<class T>
void AVLTree<T>::delete_(T data) {
    root = recursiveDelete(root, data);
    size--;
}

template<class T>
AVLNode<T>* AVLTree<T>::recursiveInsert(AVLNode<T>* node, T data){
    if (root == nullptr) {
        root = new AVLNode<T>(data, nullptr, nullptr);
        return root;
    } else if (node->compare(data) == AVLNode<T>::GREATER) {
        if (node->getLeft() == nullptr) {
            AVLNode<T> * newNode = new AVLNode<T>(data, nullptr, nullptr);
            node->setLeft(newNode);
            node = balance(node);
            return newNode;
        } else {
            return recursiveInsert(node->getLeft(), data);
        }
   } else {
        if (node->getRight() == nullptr) {
            AVLNode<T> * newNode = new AVLNode<T>(data, nullptr, nullptr);
            node->setRight(newNode);
            // recursivePrint(node, 0);
        } else {
            node->setRight(recursiveInsert(node->getRight(), data));
        }
        // cout << "currently at " << node->getData() << endl;
        node = balance(node);
        return node;
   } 
}

template<class T>
void AVLTree<T>::insert(T data){
    root = recursiveInsert(root, data);
    size++;
}

template<class T>
AVLNode<T> *AVLTree<T>::recursiveClone(AVLNode<T> *node) const {
    if (node == nullptr) {
        return nullptr;
    }
    AVLNode<T> *copy = node->clone();
    copy->setLeft(recursiveClone(copy->getLeft()));
    copy->setRight(recursiveClone(copy->getRight()));
    return copy;
}

template<class T>
AVLTree<T> *AVLTree<T>::clone() const {
    if (root == nullptr) {
        return nullptr;
    }
    return new AVLTree<T>(recursiveClone(root));
}

template<class T>
AVLTree<T> *AVLTree<T>::split(T data) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->getData() == data) {
        return nullptr;
    }
    AVLNode<T> *parent = recursiveParent(root, data);
    if (parent == nullptr) {
        throw 1;
    }
    if (parent->getLeft() != nullptr && parent->getLeft()->getData() == data) {
        AVLTree<T> *tree = new AVLTree<T>(parent->getLeft());
        parent->setLeft(nullptr);
        size -= tree->getSize();
        return tree;
    } 
    if (parent->getRight() != nullptr && parent->getRight()->getData() == data) {
        AVLTree<T> *tree = new AVLTree<T>(parent->getRight());
        parent->setRight(nullptr);
        size -= tree->getSize();
        return tree;
    }
}

template<class T>
AVLNode<T> *AVLTree<T>::recursiveParent(AVLNode<T> *node, T data) const {
    if (node->getLeft() != nullptr && node->getLeft()->getData() == data) {
        return node;
    }
    if (node->getRight() != nullptr && node->getRight()->getData() == data) {
        return node;
    }
    if (node->getLeft() != nullptr) {
        AVLNode<T> *left = recursiveParent(node->getLeft(), data);
        if (left != nullptr) {
            return left;
        }
    }
    if (node->getRight() != nullptr) {
        AVLNode<T> *right = recursiveParent(node->getRight(), data);
        if (right != nullptr) {
            return right;
        }
    }
    return nullptr;
}

template<class T>
vector<AVLNode<T> *> AVLTree<T>::toVector() const {
    AVLTreeIterator<T> iterator(this);
    vector<AVLNode<T> *> v;
    while (iterator.hasNext()) {
        v.push_back(iterator.next());
    }
    return v;
}

template<class T>
void AVLTree<T>::join(AVLTree<T> *other) {
    AVLTreeIterator<T> iterator(other);
    while (iterator.hasNext()) {
        AVLNode<T> *next = iterator.next();
        insert(next->getData());
    }
    size += other->getSize();
}

template<class T>
AVLNode<T> *AVLTree<T>::max() const {
    return recursiveMaximum(root);
}

template<class T>
AVLNode<T> *AVLTree<T>::min() const {
    return recursiveMinimum(root);
}

template<class T>
AVLNode<T> *AVLTree<T>::popMax() {
    AVLNode<T> *node = max();
    if (node == nullptr) {
        return nullptr;
    }
    delete_(node->getData());
    return node;
}

template<class T>
AVLNode<T> *AVLTree<T>::popMin() {
    AVLNode<T> *node = min();
    if (node == nullptr) {
        return nullptr;
    }
    delete_(node->getData());
    return node;
}

template<class T>
unsigned int AVLTree<T>::count(T data) const {
    AVLTreeIterator<T> iterator(this);
    unsigned int counter = 0;
    while (iterator.hasNext()) {
        AVLNode<T> *node = iterator.next();
        if (node->getData() == data) {
            counter++;
        }
    }
    return counter;
}

template<class T>
vector<AVLNode<T> *> AVLTree<T>::filter(Predicate *predicate) const {
    AVLTreeIterator<T> iterator(this);
    vector<AVLNode<T> *> v;
    while (iterator.hasNext()) {
        AVLNode<T> *next = iterator.next();
        if (predicate->evaluate(next)) {
            v.push_back(next);
        }
    }
    return v;
}

template<class T>
const AVLNode<T> *AVLTree<T>::getRoot() const {
    return root;
}

template<class T>
unsigned int AVLTree<T>::getSize() const {
    return size;
}

template<class T>
void AVLTree<T>::map(MapFunction *function) {
    AVLTreeIterator<T> iterator(this);
    while (iterator.hasNext()) {
        AVLNode<T> *next = iterator.next();
        function->apply(next);
    }
}

template<class T>
int AVLTree<T>::reduce(ReduceFunction * function, int initialState) const {
    AVLTreeIterator<T> iterator(this);
    int state = initialState;
    while (iterator.hasNext()) {
        AVLNode<T> *next = iterator.next();
        state = function->reduce(next, state);
    }
    return state;
}

template<class T>
AVLTreeIterator<T> AVLTree<T>::iterator() const {
    AVLTreeIterator<T> iterator(this);
    return iterator;
}


#endif
