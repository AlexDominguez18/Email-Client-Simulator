#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <exception>
#include <string>
#include <iostream>

template <class T>
class List {
public:
    class Node {
    private:
        T data;
        Node* next;

    public:
        Node();
        Node(const T& e);
        T& getData();
        T getDataC();
        Node* getNext();
        void setData(T& e);
        void setNext(Node* p);
    };
private:
    Node* anchor;

    int nodesCounter;

    bool isValidPos(Node*) const;
    void copyAll(const List<T>&) const;

public:
    typedef Node* Pos;
    class ListException : public std::exception{
    private:
        std::string msg;
    public:
        explicit ListException(const char* message) : msg(message){}

        explicit ListException(const std::string& message) : msg(message){}

        virtual ~ListException()throw() {}

        virtual const char* what() const throw () {
            return msg.c_str();
        }
    };

    List();
    List(const List<T>& l);
    ~List();
    bool isEmpty() const;
    void insertData(Node*, const T&);
    void deleteData(Node*);
    Node* getFirstPos() const;
    Node* getLastPos() const;
    Node* getPrevPos(Node*) const;
    Node* getNextPost(Node*) const;
    Node* findData(const T&) const;
    T retrieve(Node*) const;
    void deleteAll();
    List<T>& operator = (const List<T>&);
    int getSize();
};

///Node
template <class T>
List<T>::Node::Node() : next(nullptr) { }

template <class T>
List<T>::Node::Node(const T& e) : data(e), next(nullptr) { }

template <class T>
T& List<T>::Node::getData() {
    return data;
}

template <class T>
T List<T>::Node::getDataC(){
    return data;
}

template <class T>
typename List<T>::Node* List<T>::Node::getNext() {
    return next;
}

template <class T>
void List<T>::Node::setData(T& e) {
    data = e;
}

template <class T>
void List<T>::Node::setNext(Node* p) {
    next = p;
}

///Lista

template <class T>
bool List<T>::isValidPos(Node* p) const{
    Node* aux(anchor);
    while(aux!=nullptr) {
        if(aux==p) {
            return true;
        }
        aux=aux->getNext();
    }
    return false;
}

template <class T>
void List<T>::copyAll(const List<T>& l) const{
    Node* aux(l.anchor);
    Node* last(nullptr);
    Node* newNode;
    while(aux!=nullptr) {
        if((newNode=new Node(aux->getData()))==nullptr) {
            throw ListException("Memoria no disponible, copyAll");
        }
        if(last==nullptr) {
            anchor->setData(newNode->getData());
            anchor->setNext(newNode->getNext());
        } else {
            last->setNext(newNode);
        }
        last=newNode;
        aux=aux->getNext();
    }
}

template <class T>
List<T>::List() : anchor(nullptr) {
    nodesCounter = 0;
}

template <class T>
List<T>::List(const List<T>& l) : anchor(nullptr) {
    copyAll(l);
}

template <class T>
List<T>::~List() {
    deleteAll();
}

template <class T>
bool List<T>::isEmpty() const{
    return anchor==nullptr;
}

template <class T>
void List<T>::insertData(Node* p, const T& e) {
    if(p!=nullptr and !isValidPos(p)) {
        throw ListException("Posicion invalida, insertData");
    }
    Node* aux(new Node(e));
    if(aux==nullptr) {
        throw ListException("Memoria no disponisble, insertData");
    }
    if(p==nullptr) {
        aux->setNext(anchor);
        anchor=aux;
    } else {
        aux->setNext(p->getNext());
        p->setNext(aux);
    }
    nodesCounter++;
}

template <class T>
void List<T>::deleteData(Node* p) {
    if(!isValidPos(p)) {
        throw ListException("Posicion invalida, deleteData");
    }
    if(p==anchor) {
        anchor = anchor->getNext();
    } else {
        getPrevPos(p)->setNext(p->getNext());
    }
    delete p;
    nodesCounter--;
}

template <class T>
typename List<T>::Node* List<T>::getFirstPos() const{
    return anchor;
}

template <class T>
typename List<T>::Node* List<T>::getLastPos() const{
    if(isEmpty()) {
        return nullptr;
    }
    Node* aux(anchor);
    while(aux->getNext()!=nullptr){
        aux = aux->getNext();
    }
    return aux;
}

template <class T>
typename List<T>::Node* List<T>::getPrevPos(Node* p) const{
    if(p==anchor) {
        return nullptr;
    }
    Node* aux(anchor);
    while(aux!=nullptr and aux->getNext()!=p) {
        aux = aux->getNext();
    }
    return aux;
}

template <class T>
typename List<T>::Node* List<T>::getNextPost(Node* p) const{
    if(!isValidPos(p)) {
        return nullptr;
    }
    return p->getNext();
}

template <class T>
typename List<T>::Node* List<T>::findData(const T& e) const{
    Node* aux(anchor);
    while(aux!=nullptr and aux->getData()!=e) {
        aux = aux->getNext();
    }
    return aux;
}

template <class T>
T List<T>::retrieve(Node* p) const{
    if(!isValidPos()) {
        throw ListException("Posicion invalida, retrieve");
    }
    return p->getData();
}

template <class T>
void List<T>::deleteAll() {
    Node* aux;
    while(anchor!=nullptr) {
        aux = anchor;
        anchor = anchor->getNext();
        delete aux;
    }
    nodesCounter = 0;
}

template <class T>
List<T>& List<T>::operator=(const List<T>& l) {
    deleteAll();
    copyAll(l);
    return *this;
}

template <typename T>
int List<T>::getSize(){
    return nodesCounter;
}


#endif // LIST_H_INCLUDED
