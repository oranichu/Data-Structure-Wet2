#ifndef MINHEAP_MINHEAP_H
#define MINHEAP_MINHEAP_H

#include <iostream>

class MinHeapException : public std::exception {
};

class BadArgument : public MinHeapException {
public:
    virtual const char *what() const throw() {
        return "Invalid Argument";
    };
};

class NoElements : public MinHeapException {
public:
    virtual const char *what() const throw() {
        return "No Min in heap , there are no elements.";
    };
};

/**
 *
 * @tparam T - main data class type.
 * @tparam U - index class type.
 * @tparam Func - function object to help update the data index in class T.
 */
template<typename T, typename U, typename Func>
class MinHeap {
private:
    // The node of the heap ,contains the data and the key.
    class MinHeapNode {
        T data;
        U key;
    public:
        MinHeapNode(const T &d, const U &k) : data(d), key(k) {};

        const T &getData() const { return data; };

        const U &getKey() const { return key; };

        void setData(const T &d) { data = d; };

        void setKey(const U &k) { key = k; };

    };

    int size; //Total array size.
    int last; //The index of the last node , last <= size always ! .
    //Minimum key value for delete , needs to be smaller than all possible keys.
    U minKeyVal;
    MinHeapNode **arr;
    Func updateIndexFunc;

    void switchNodes(int i, int j) {
        MinHeapNode *temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        updateIndexFunc(arr[i]->getData(), i);
        updateIndexFunc(arr[j]->getData(), j);
    }

    int sonToSwitch(int i) {
        if ((i * 2 + 1) <= last) {
            if (((arr[i]->getKey() > arr[2 * i]->getKey())) ||
                (arr[i]->getKey() > arr[2 * i + 1]->getKey())) {
                if (arr[2 * i]->getKey() < arr[2 * i + 1]->getKey()) {
                    return 2 * i; //left son is min.
                }
                return 2 * i + 1; //right son is min.
            }
            return -1; //father is smaller than 2 sons
        } //else (i*2+1 > last) so either 1 son or none .
        if ((i * 2 == last)) {
            if (arr[i]->getKey() > arr[2 * i]->getKey()) {
                return 2 * i; //only left son which is smaller.
            }
        }
        return -1;//father is leaf or smaller than left son.
    }

    void expandArray() {
        size *= 2;
        MinHeapNode **temp = new MinHeapNode *[size];
        temp[0]=NULL ;
        for (int i = 1; i < last; i++) {
            temp[i] = arr[i];
        }
        delete[]arr;
        arr = temp;
    }

public:
    //T and U array sizes must be in size of n !
    MinHeap(const T *dataArr, const U *keyArr, int n, int minKey, const Func &f)
            : size(4 * n), last(n), minKeyVal(minKey), updateIndexFunc(f) {
        if (n < 1 || dataArr == NULL || keyArr == NULL) {
            throw BadArgument();
        }
        arr = new MinHeapNode *[size];
        for (int i = 1; i <= last; i++) {
            arr[i] = new MinHeapNode(dataArr[i - 1], keyArr[i - 1]);
//updates the data to have the current index of the data place in the array.
            updateIndexFunc(dataArr[i - 1], i);
        }
        for (int i = (last / 2); i >= 1; i--) {
            siftDown(i);
        }
    };

    ~MinHeap() {
        for (int i = 1; i <= last; i++) {
            delete arr[i];
        }
        delete[] arr;
    }

    void siftDown(int i) {
        int smaller_son_index = sonToSwitch(i);
        if (smaller_son_index == -1) {
            return;
        }
        switchNodes(i, smaller_son_index);
        siftDown(smaller_son_index);
    }

    void siftUp(int i) {
        if (i <= 1) { //reached the top.
            return;
        }
        if (arr[i]->getKey() > arr[i / 2]->getKey()) {
            return; //he's bigger than his father , stop .
        }
        switchNodes(i, i / 2);
        siftUp(i / 2);
    }

    int getSize() const { return size; };

    int getLastIndex() const { return last; };

    const T &getMin() const {
        if (arr == NULL || last == 0) {
            throw NoElements();
        }
        return arr[1]->getData();
    }

    void printHeap() const {
        for (int i = 1; i <= last; i++) {
            std::cout << arr[i]->getData() << " ";
        }
        std::cout << std::endl;
    }

    void insert(const T &new_data, const U &new_key) {
        if ((++last) >= size / 2) {
            expandArray();
        }
        arr[last] = new MinHeapNode(new_data, new_key);
        updateIndexFunc(new_data, last);
        siftUp(last);
    }

    void delMin() {
        if (last == 1) {
            updateIndexFunc(arr[last]->getData(), -1);//puts -1 in data index.
            delete arr[last];
            last--;
            return;
        }
        switchNodes(1, last);
        updateIndexFunc(arr[last]->getData(), -1);
        delete arr[last];
        last--;
        siftDown(1);
    }

    void delNode(int i) {
        if (i > last) {
            return;
        }
        arr[i]->setKey(minKeyVal);
        siftUp(i);
        delMin();
    }


};

#endif //MINHEAP_MINHEAP_H
