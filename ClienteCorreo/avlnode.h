#ifndef AVLNODE_H
#define AVLNODE_H

#include <iostream>
#include <chrono>
#include <ctime>

template <class T>
class AVLNode{

private:

    T* dataPointer;
    AVLNode* left;
    AVLNode* right;

public:

    AVLNode();
    AVLNode(const T&);

    ~AVLNode();

    long long timestamp;

    T *getDataPointer() const;
    T& getData() const;
    AVLNode *&getLeft();
    AVLNode *&getRight();

    void setDataPointer(T *value);
    void setData(const T& value);
    void setLeft(AVLNode *&value);
    void setRight(AVLNode *&value);
};

template <class T>
AVLNode<T>::AVLNode() : dataPointer(nullptr), left(nullptr), right(nullptr){
    timestamp = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

template <class T>
AVLNode<T>::AVLNode(const T& d) : dataPointer(new T(d)), left(nullptr), right(nullptr){

}

template <class T>
AVLNode<T>::~AVLNode<T>(){
    delete dataPointer;
}

template <class T>
T* AVLNode<T>::getDataPointer() const{
    return dataPointer;
}

template <class T>
T& AVLNode<T>::getData() const{
    return *dataPointer;
}

template <class T>
AVLNode<T>*& AVLNode<T>::getLeft(){
    return left;
}

template <class T>
AVLNode<T>*& AVLNode<T>::getRight(){
    return right;
}

template <class T>
void AVLNode<T>::setDataPointer(T* value){
    dataPointer = value;
}

template <class T>
void AVLNode<T>::setData(const T& value){
    *dataPointer = value;
}

template <class T>
void AVLNode<T>::setLeft(AVLNode*& value){
    left = value;
}

template <class T>
void AVLNode<T>::setRight(AVLNode*& value){
    right = value;
}

#endif // AVLNODE_H
