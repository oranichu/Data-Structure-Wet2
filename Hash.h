//
// Created by shay-halimi on 08-Jun-18.
//

#ifndef GENTREE_HASH_H
#define GENTREE_HASH_H

#include "List.h"
#include "math.h"

/**
 *
 * @tparam T : the data type
 * @tparam GETKEY : Object Function Type of the function that return an int key
 * for the data. the function is injective.
 * @tparam CompareT : Object Function Type of the function that check if 2 T
 * objects have the same key.
 *
 * size : the size of arr
 * occupancy : how many T objects arr contains.
 * getKey : an injective function that return an int key for every T object.
 * arr : an array of lists the contains all the T objects.
 */
template<typename T, typename GETKEY,typename CompareT>
class Hash {
    int size;
    int occupancy;
    GETKEY getKey;
    List<T,CompareT> **arr;

private:
    int hash(const T &data) {
         int key = getKey(data);
         key *= 0.61803398875;
         key = fmod(key, 1);
         key = key * size;
         return (int) key;
    }

    /*
     * create a new lists array for arr the is twice the size of occupancy.
     */
    void createListArr() {
        arr = new List<T,CompareT> *[occupancy * 4];
        for (int i = 0; i < occupancy * 4; ++i) {
            arr[i] = new List<T,CompareT>();
        }
    }

    /*
     * destroy a lists array with all the T objects inside it.
     */
    void destroyListArr(List<T,CompareT> **listArr, int size) {
        for (int i = 0; i < size; ++i) {
            delete listArr[i];
        }
        delete[] listArr;
    }

    /*
     * create new lists array for arr with all the previous data inside it and
     * delete the previous lists array.
     */
    void resize() {
        int tempSize = size;
        size = occupancy * 4;
        List<T,CompareT> **tempArr = arr;
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
    Hash(T *array, int size) : size(size * 4), occupancy(size) {
        createListArr();
        int place;
        for (int i = 0; i < size; ++i) {
            place = hash(array[i]);
            arr[place]->blindInsert(array[i]);
        }
    }

    ~Hash() {
        destroyListArr(arr, size);
    }

    /*
     * insert a new data to Hash.
     * return : false if the data was already in there.
     */
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

    /*
     * return a pointer to the node that contains data with the same key.
     * NULL if there is no such node in Hash.
     */
    Node<T> *find(const T &key) {
        int place = hash(key);
        return arr[place]->find(key);
    }

    /*
     * delete data from Hash
     * return : false if there is no such data.
     */
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

    /*
     * return the occupancy of Hash.
     */
    int getSize() {
        return occupancy;
    }

};

#endif //GENTREE_HASH_H
