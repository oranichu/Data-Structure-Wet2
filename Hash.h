//
// Created by shay-halimi on 08-Jun-18.
//

#ifndef GENTREE_HASH_H
#define GENTREE_HASH_H

#include "List.h"
#include "math.h"

template<typename T, typename GETKEY>
class Hash {
    int size;
    int occupancy;
    List<T> **arr;
    GETKEY getKey;

private:
    int hash(const T &data) {
        /* int key = getKey(data);
         key *= 0.61803398875;
         key = fmod(key, 1);
         key = key * size;
         return (int) key;
         */
        return getKey(data) % size;
    }

    void createListArr() {
        arr = new List<T> *[occupancy * 4];
        for (int i = 0; i < occupancy * 4; ++i) {
            arr[i] = new List<T>();
        }
    }

    void destroyListArr(List<T> **listArr, int size) {
        for (int i = 0; i < size; ++i) {
            delete listArr[i];
        }
        delete[] listArr;
    }

    void resize() {
        int tempSize = size;
        size = occupancy * 4;
        List<T> **tempArr = arr;
        createListArr();
        int place;
        Node<T> *curr;
        for (int i = 0; i < tempSize; ++i) {
            curr = tempArr[i]->getFirst();
            while (curr != NULL) {
                place = hash(curr->getData());
                arr[place]->blindInsert(curr->getData());
                curr = curr->getNext();
            }
        }
        destroyListArr(tempArr, tempSize);
    }

public:
    Hash(T **array, int size) : size(size * 4), occupancy(size) {
        createListArr();
        int place;
        for (int i = 0; i < size; ++i) {
            place = hash(**(array + i));
            arr[place]->blindInsert(**array);
        }
    }

    ~Hash() {
        destroyListArr(arr, size);
    }

    bool insert(const T &data) {
        int place = hash(data);
        bool added = arr[place]->insert(data);
        if (added) {
            occupancy++;
            if (occupancy * 2 >= size) {
                resize();
            }
        }
        return false;
    }

    Node<T> *find(const T &key) {
        int place = hash(key);
        return arr[place]->find(key);
    }

    bool destroy(const T &key) {
        int place = hash(key);
        bool destroyed = arr[place]->destroy(key);
        if (destroyed) {
            occupancy--;
            if (occupancy * 8 <= size) {
                resize();
            }
        }
        return false;
    }

    int getSize() {
        return occupancy;
    }

    
};

#endif //GENTREE_HASH_H
