#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <fstream>
#include <string>

#include "avlnode.h"
#include "list.h"

#define MAX_NODES 10

template <class T>
class AVLTree{

private:

    AVLNode<T>* root;

    void insert(AVLNode<T>*&, const T&);

    void deleteAll(AVLNode<T>*&);

    AVLNode<T>*& find(AVLNode<T>*&, const T&);

    AVLNode<T>*& getLowest(AVLNode<T>*&);
    AVLNode<T>*& getHighest(AVLNode<T>*&);

    void preOrder(AVLNode<T>*&);
    void inOrder(AVLNode<T>*&);
    void postOrder(AVLNode<T>*&);

    void getLeastRecentlyUsed(AVLNode<T>*& lru,AVLNode<T>*& r);

    void saveTree(std::fstream&, AVLNode<T>*&);

    int getBalanceFactor(AVLNode<T>*&);

    void slRotation(AVLNode<T>*&);
    void srRotation(AVLNode<T>*&);
    void dlRotation(AVLNode<T>*&);
    void drRotation(AVLNode<T>*&);

    void balance(AVLNode<T>*&);

public:

    AVLTree();
    ~AVLTree();

    int nodes;
    bool isPaginated;
    bool isEmpty();
    bool isLeaf(AVLNode<T>*&);

    void insert(const T&);

    void deleteData(AVLNode<T>*&);

    T& retrieve(AVLNode<T>*&) const;

    int getHeight();
    int getHeight(AVLNode<T>*&);

    AVLNode<T>*& find(const T&);

    AVLNode<T>*& getLowest();
    AVLNode<T>*& getHighest();

    void preOrder();
    void inOrder();
    void postOrder();

    AVLNode<T>*& getLeastRecentlyUsed();

    void saveTree(std::fstream&);

    void deleteAll();

    AVLTree& operator = (const AVLTree&);

};

template <class T>
AVLTree<T>::AVLTree() : root(nullptr) {
    isPaginated = false;
    nodes = 0;
}

template <class T>
AVLTree<T>::~AVLTree(){
    deleteAll();
}

template <class T>
bool AVLTree<T>::isEmpty(){
    return root == nullptr;
}

template <class T>
T& AVLTree<T>::retrieve(AVLNode<T>*& r) const{
    return r->getData();
}

template <class T>
AVLNode<T>*& AVLTree<T>::find(const T& d){
    return find(root,d);
}

template <class T>
AVLNode<T>*& AVLTree<T>::find(AVLNode<T>*& r, const T& d){
    if (r == nullptr or r->getData() == d){
        return r;
    }
    if (d < r->getData()){
        return find(r->getLeft(),d);
    }
    return find(r->getRight(), d);
}

template <class T>
AVLNode<T>*& AVLTree<T>::getLowest(AVLNode<T>*& r){
    if (r == nullptr or r->getLeft() == nullptr){
        return r;
    }
    return getLowest(r->getLeft());
}

template <class T>
AVLNode<T>*& AVLTree<T>::getLowest(){
    return getLowest(root);
}

template <class T>
AVLNode<T>*& AVLTree<T>::getHighest(AVLNode<T>*& r){
    if (r == nullptr or r->getRight() == nullptr){
        return r;
    }
    return getHighest(r->getRight());
}

template <class T>
AVLNode<T>*& AVLTree<T>::getHighest(){
    return getHighest(root);
}

template <class T>
bool AVLTree<T>::isLeaf(AVLNode<T>*& r){
    return r != nullptr and r->getLeft() == r->getRight();
}

template  <class T>
int AVLTree<T>::getHeight(AVLNode<T>*& r){
    if (r == nullptr){
        return 0;
    }

    int leftHeight(getHeight(r->getLeft()));
    int rightHeight(getHeight(r->getRight()));

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

template <class T>
int AVLTree<T>::getHeight(){
    return getHeight(root);
}

template  <class T>
void AVLTree<T>::preOrder(AVLNode<T>*& r){
    if (r == nullptr){
        return;
    }
    std::cout << r->getData() << "|";
    preOrder(r->getLeft());
    preOrder(r->getRight());

}

template <class T>
void AVLTree<T>::preOrder(){
    preOrder(root);
}

template <class T>
void AVLTree<T>::inOrder(AVLNode<T>*& r){
    if (r == nullptr){
        return;
    }

    inOrder(r->getLeft());
    std::cout << r->getData() << "|";
    inOrder(r->getRight());
}

template <class T>
void AVLTree<T>::inOrder(){
    inOrder(root);
}

template <class T>
void AVLTree<T>::postOrder(AVLNode<T>*& r){
    if (r == nullptr){
        return;
    }

    postOrder(r->getLeft());
    postOrder(r->getRight());
    std::cout << r->getData() << "|";
}

template <class T>
void AVLTree<T>::postOrder(){
    postOrder(root);
}

template <class T>
void AVLTree<T>::getLeastRecentlyUsed(AVLNode<T>*& lru,AVLNode<T>*& r){
    if (r == nullptr){
        return;
    }
    if (r->timestamp < lru->timestamp){
        lru = r;
    }
    getLeastRecentlyUsed(lru,r->getLeft());
    getLeastRecentlyUsed(lru,r->getRight());
}

template <class T>
AVLNode<T>*& AVLTree<T>::getLeastRecentlyUsed(){
    AVLNode<T>*& lru = root;
    getLeastRecentlyUsed(lru,root);
    return lru;
}

template <class T>
void AVLTree<T>::saveTree(std::fstream& file, AVLNode<T>*& r){
    if (r == nullptr){
        return;
    }
    saveTree(file,r->getLeft());
    file.write((char*)&r->getData(),sizeof (T));
    saveTree(file,r->getRight());
}

template <class T>
void AVLTree<T>::saveTree(std::fstream& file){
    saveTree(file,root);
}

template <class T>
void AVLTree<T>::insert(AVLNode<T>*& r, const T& d){
    if (r == nullptr){
        r = new AVLNode<T>(d);
    }
    else{
        if (d < r->getData()){
            insert(r->getLeft(), d);
        }
        else{
            insert(r->getRight(), d);
        }
        balance(r);
    }
}

template <class T>
void AVLTree<T>::insert(const T& d){
    insert(root,d);
    if (isPaginated){
        nodes++;
    }
}

template <class T>
void AVLTree<T>::deleteData(AVLNode<T>*& r){
    if (isLeaf(r)){
        delete r;
        r = nullptr;
    }
    else{
        AVLNode<T>*& temp(r->getLeft() ? getHighest(r->getLeft()) : getHighest(r->getRight()));
        r->setData(temp->getData());
        deleteData(temp);
    }
    if (isPaginated){
        nodes--;
    }
}

template <class T>
void AVLTree<T>::deleteAll(AVLNode<T>*& r){
    if (r == nullptr){
        return;
    }
    deleteAll(r->getLeft());
    deleteAll(r->getRight());
    delete r;
}

template <class T>
void AVLTree<T>::deleteAll(){
    deleteAll(root);
    root = nullptr;
    nodes = 0;
}

template <class T>
int AVLTree<T>::getBalanceFactor(AVLNode<T>*& r){
    return getHeight(r->getRight()) - getHeight(r->getLeft());
}

template <class T>
void AVLTree<T>::slRotation(AVLNode<T>*& r){
    AVLNode<T>* aux(r->getRight());

    r->setRight(aux->getLeft());
    aux->setLeft(r);
    r = aux;
}

template <class T>
void AVLTree<T>::srRotation(AVLNode<T>*& r){
    AVLNode<T>* aux(r->getLeft());

    r->setLeft(aux->getRight());
    aux->setRight(r);
    r = aux;
}

template <class T>
void AVLTree<T>::dlRotation(AVLNode<T>*& r){
    srRotation(r->getRight());
    slRotation(r);
}

template <class T>
void AVLTree<T>::drRotation(AVLNode<T>*& r){
    slRotation(r->getLeft());
    srRotation(r);
}

template <class T>
void AVLTree<T>::balance(AVLNode<T>*& r){
    switch (getBalanceFactor(r)) {
        case -2: //Left loading
            if (getBalanceFactor(r->getLeft()) == -1){
                srRotation(r);
            }else{
                drRotation(r);
            }
            break;
        case 2: //Right Loading
            if (getBalanceFactor(r->getRight()) == 1){
                slRotation(r);
            }else{
                dlRotation(r);
            }
            break;
    }
}

#endif // AVLTREE_H
