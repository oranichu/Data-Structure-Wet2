//
// Created by shay-halimi on 07-Jun-18.
//

#ifndef PROJECT_LIST_H
#define PROJECT_LIST_H

#include <iostream>



/**
 *
 * @tparam T : the data type
 * data : the data in the node
 * next : pointer to the next node
 * prev : pointer to the previous node
 */
template<typename T>
class Node {
    T data;
    Node<T> *next;
    Node<T> *prev;
public:
    explicit Node(const T &data) : data(data), next(NULL), prev(NULL) {}

    const T &getData() { return data; }

    void setData(const T &data) { this->data = data; };

    Node<T> *getNext() { return next; }

    void setNext(Node<T> *next) { this->next = next; }

    Node<T> *getPrev() { return prev; }

    void setPrev(Node<T> *prev) { this->prev = prev; }
};

/**
 *
 * @tparam T : the data type
 * @tparam Func : the Objuect Function type of the compare function. compare
 * between 2 T objects and return true if they are the same.
 * head : pointer to the first node
 * itr : pointer to the current node
 * size : the size of the list (number of nodes)
 * compareT : an object function that checks if 2 T objects have the same key.
 */
template<typename T,typename Func>
class List {
    Node<T> *head;
    Node<T> *itr;
    int size;
    Func compareT;

private:
    /*
     * destroy the nodes that come with and after "node"
     */
    void destroyList(Node<T> *node) {
        if (node == NULL) {
            return;
        }
        destroyList(node->getNext());
        delete node;
    }

public:
    List() : head(NULL),itr(NULL), size(0) {}

    explicit List(const T &emptyValue) : head(new Node<T>(emptyValue)),
                                         size(0) {}

    ~List() {
        destroyList(head);
    }

    /*
     * insert the data to the beginning of the list
     */
    void blindInsert(const T &data) {
        Node<T> *node = new Node<T>(data);
        if (head==NULL) {
            head = node ;
            return ;
        }
        node->setNext(head);
        head->setPrev(node);
        head=node ;
        size++;
    }

    /*
     * insert the data to the beginning of the list, but only if the data
     * wasn't in the list before.
     * return : true if it is a new data.
     */
    bool insert(const T &data) {
        if (find(data) == NULL) {
            blindInsert(data);
            return true;
        }
        return false;
    }

    /*
     * return a pointer to a node that contain data with the same key as "data"
     * NULL if there is not such node.
     */
    Node<T> *find(const T &data) {
        if (head==NULL) {
            return NULL ;
        }
        Node<T> *curr = head;
        while (compareT(curr->getData(),data)==false) {
            curr = curr->getNext();
            if (curr == NULL) {
                return NULL;
            }
        }
        return curr;
    }

    /*
     * destroy the node with the same key as "data" and tie the previous
     * and next nodes together.
     */
    bool destroy(const T &data) {
        Node<T> *node = find(data);
        if (node == NULL) {
            return false;
        }
        Node<T> *prev = node->getPrev();
        Node<T> *next = node->getNext();
        prev->setNext(next);
        if (next != NULL) {
            next->setPrev(prev);
        }
        size--;
        delete node;
        return true;
    }

    /*
     * return a pointer to the first node.
     */
    Node<T> *getFirst() {
        if (head==NULL) {
            return NULL ;
        }
        itr = head;
        return itr;
    }

    /*
     * return a pointer to the current node.
     */
    Node<T> *getCurr() {
        return itr;
    }

    /*
     * advance the current node indicator and return it.
     */
    Node<T> *getNext() {
        itr = itr->getNext();
        return itr;
    }
};

#endif //PROJECT_LIST_H
