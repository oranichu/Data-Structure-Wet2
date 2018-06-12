//
// Created by shay-halimi on 07-Jun-18.
//

#ifndef PROJECT_LIST_H
#define PROJECT_LIST_H
#include <iostream>


template <typename T>
class Node{
    T data;
    Node<T>* next;
    Node<T>* prev;
public:
    explicit Node(const T& data) : data(data), next(NULL), prev(NULL){}
    const T& getData(){ return data;}
    void setData(const T& data){this->data = data;};
    Node<T>* getNext(){ return next;}
    void setNext(Node<T>* next){this->next = next;}
    Node<T>* getPrev(){ return prev;}
    void setPrev(Node<T>* prev){this->prev = prev;}
};

//not complete
template <typename T>
class List{
    Node<T>* head;
    Node<T>* itr;
    int size;

private:
    void destroyList(Node<T>* node){
        if(node==NULL){
            return;
        }
        destroyList(node->getNext());
        delete node;
    }
public:
    List () : size(0){}
    explicit List(const T& emptyValue) : head(new Node<T>(emptyValue)), size(0){}
    ~List(){
        destroyList(head);
    }
    void blindInsert(const T& data){
        Node<T>* node = new Node<T>(data);
        Node<T>* next = head->getNext();
        node->setNext(next);
        node->setPrev(head);
        head->setNext(node);
        if (next != NULL){
            next->setPrev(node);
        }
        size++;
    }
    bool insert(const T& data){
        if (find(data) == NULL){
            blindInsert(data);
            return true;
        }
        return false;
    }
    Node<T>* find(const T& data){
        Node<T>* curr = head;
        T key(data);
        while (curr->getData() != key){
            curr = curr->getNext();
            if (curr == NULL){
                return NULL;
            }
        }
        return curr;
    }
    bool destroy(const T& data){
        Node<T>* node = find(data);
        if (node == NULL){
            return false;
        }
        Node<T>* prev = node->getPrev();
        Node<T>* next = node->getNext();
        prev->setNext(next);
        if(next != NULL){
            next->setPrev(prev);
        }
        size--;
        delete node;
        return true;
    }
    Node<T>* getFirst(){
        itr = head->getNext();
        return itr;
    }
    Node<T>* getCurr(){
        return itr;
    }
    Node<T>* getNext(){
        itr = itr->getNext();
        return itr;
    }

    /// for testing ///
    void printInt(){
        Node<T>* curr = head;
        while (curr != NULL){
            cout << "[ " << curr->getData() << " ] -> ";
            curr = curr->getNext();
        }
        cout << "NULL" << endl;
    }
};
#endif //PROJECT_LIST_H
