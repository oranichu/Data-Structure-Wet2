#ifndef GENTREE_AVLTREE_H
#define GENTREE_AVLTREE_H

#include <iostream>

#define GO_LEFT (-1)
#define GO_RIGHT (1)
#define EQUAL (0)


typedef enum {
    LEFT, RIGHT
} Place;

class AvlTreeExceptions : public std::exception {
};

class InvalidRank : public AvlTreeExceptions {
public:
    virtual const char *what() const throw() {
        return "Desired rank is bigger than number of nodes in tree";
    };
};

class NoElementsInTree : public AvlTreeExceptions {
public:
    virtual const char *what() const throw() {
        return "No elements in tree.";
    };
};

template<typename T, typename U>
class AvlNode {
    T data;
    AvlNode<T, U> *father;
    AvlNode<T, U> *lSon;
    AvlNode<T, U> *rSon;
    int height;
    int rank;
    U rankVal; //Initial rankVal , can't change unless thers's a data switch .
    U rankSum;
public:
    AvlNode(const T &data, const U &sum) : data(data), father(NULL), lSon(NULL),
                                           rSon(NULL), height(0), rank(1),
                                           rankVal(sum), rankSum(sum) {};

    void setData(const T &d) { data = d; }

    void setFather(AvlNode<T, U> *parent) { father = parent; }

    void setLeftSon(AvlNode<T, U> *left_son) { lSon = left_son; }

    void setRightSon(AvlNode<T, U> *right_son) { rSon = right_son; }

    const T &getData() {
        return data;
    }

    AvlNode<T, U> *getFather() const { return father; }

    AvlNode<T, U> *getLeftSon() const { return lSon; }

    AvlNode<T, U> *getRightSon() const { return rSon; }

    int getHeight() const { return height; }

    void setHeight(int h) { height = h; }

    int diff() const {
        if (lSon != NULL && rSon != NULL) {
            return lSon->height - rSon->height;
        }
        if (lSon != NULL) { //no right son
            return lSon->height + 1;
        }
        if (rSon != NULL) {
            return -1 - rSon->height; //no left son
        }
        return 0; //has no sons.
    }

    int getRank() const { return rank; };

    const U &getRankSum() const { return rankSum; };

    const U &getRankValue() const { return rankVal; };

    void setRankValue(const U &val) { rankVal = val; };

    void setRank(int r) { rank = r; };

    void setRankSum(const U &sum) { rankSum = sum; };


};

template<typename T, typename U, typename CompareFunction>
class AvlTree {

private:
    AvlNode<T, U> *root;
    /** cmp function for the tree ,returns -1 if we want x1 to be on the left
     * son return 1 if we want x1 to be on the right side , and 0 if x1 == x2 */
    CompareFunction cmp;
    int size;

    AvlNode<T, U> *
    sortInsert(AvlNode<T, U> *current, AvlNode<T, U> *new_node,
               const U &rankVal) {
        // if x1 <= x2 of the cmp function we want the new node to be on the left
        if (cmp(new_node->getData(), current->getData()) <= EQUAL) {
            if (current->getLeftSon() == NULL) {
                //Left son doesn't exist found place to insert !
                makeSon(current, new_node, LEFT);
            } else { //Left son does exist,continue searching .
                // rec call to the function , but makes sure so update pointers.
                makeSon(current,
                        sortInsert(current->getLeftSon(), new_node, rankVal),
                        LEFT);
            }
        } else {
            if (current->getRightSon() == NULL) {
                //Right son doesn't exist found place to insert !
                makeSon(current, new_node, RIGHT);
            } else { //Right son does exist,continue searching .
                makeSon(current,
                        sortInsert(current->getRightSon(), new_node, rankVal),
                        RIGHT);
            }
        }
        updateHeight(current);
        current->setRank(current->getRank() + 1);
        //Function f will hold the added node value to add .
        current->setRankSum(current->getRankSum() + rankVal);
        return balance(current, rankVal);
    }

    void makeSon(AvlNode<T, U> *father, AvlNode<T, U> *son, Place p) const {
        if (son != NULL) {
            son->setFather(father);
        }
        if (p == LEFT) {
            father->setLeftSon(son);
        } else { //p == RIGHT
            father->setRightSon(son);
        }
    }

    template<typename Func>
    //Function to determant what we want to do.
    void startInorder(AvlNode<T, U> *current, Func f) {
        if (current == NULL) {
            return;
        }
        startInorder(current->getLeftSon(), f);
        f(current);
        startInorder(current->getRightSon(), f);
    }

    template<typename Func>
    void startPreorder(AvlNode<T, U> *current, Func f) {
        if (current == NULL) {
            return;
        }
        f(current);
        startPreorder(current->getLeftSon(), f);
        startPreorder(current->getRightSon(), f);
    }

    template<typename Func>
    void startPostorder(AvlNode<T, U> *current, Func f) {
        if (current == NULL) {
            return;
        }
        startPostorder(current->getLeftSon(), f);
        startPostorder(current->getRightSon(), f);
        f(current);
    }

    AvlNode<T, U> *recFind(AvlNode<T, U> *current, T key) {
        if (current == NULL) {
            return NULL;
        }
        if (cmp(key, current->getData()) == EQUAL) {
            return current;
        }
        if (cmp(key, current->getData()) == GO_LEFT) {
            return recFind(current->getLeftSon(), key);
        }
        return recFind(current->getRightSon(), key);
    }

    AvlNode<T, U> *balance(AvlNode<T, U> *node, const U &rankVal) {
        int balance = node->diff();
        if (balance > 1) {
            if (node->getLeftSon()->diff() >= 0) {
                return llRotation(node, rankVal);
            } else {
                return lrRotation(node, rankVal);
            }
        } else if (balance < -1) {
            if (node->getRightSon()->diff() > 0) {
                return rlRotation(node, rankVal);
            } else {
                return rrRotation(node, rankVal);
            }
        }
        // Tree is balanced , return original root  .
        return node;
    }

    AvlNode<T, U> *rightRotation(AvlNode<T, U> *node, const U &rankVal) {
        AvlNode<T, U> *tempA = node->getLeftSon();
        tempA->setFather(node->getFather());
        makeSon(node, node->getLeftSon()->getRightSon(), LEFT);
        makeSon(tempA, node, RIGHT);
        updateHeight(node);
        updateHeight(tempA);
        updateRank(node);
        updateRank(tempA);
        updateRankSum(node, rankVal);
        updateRankSum(tempA, rankVal);
        return tempA;
    }

    AvlNode<T, U> *leftRotation(AvlNode<T, U> *node, const U &rankVal) {
        AvlNode<T, U> *tempA = node->getRightSon();
        tempA->setFather(node->getFather());
        makeSon(node, node->getRightSon()->getLeftSon(), RIGHT);
        makeSon(tempA, node, LEFT);
        updateHeight(node);
        updateHeight(tempA);
        updateRank(node);
        updateRank(tempA);
        updateRankSum(node, rankVal);
        updateRankSum(tempA, rankVal);
        return tempA;
    }

    void updateHeight(AvlNode<T, U> *node) {
        int leftSonH = -1, rightSonH = -1;
        if (node->getLeftSon() != NULL) {
            leftSonH = node->getLeftSon()->getHeight();
        }
        if (node->getRightSon() != NULL) {
            rightSonH = node->getRightSon()->getHeight();
        }
        node->setHeight(maxFunc(leftSonH, rightSonH) + 1);
    }

    void updateRank(AvlNode<T, U> *node) {
        int rightSonR = 0, leftSonR = 0;
        if (node->getLeftSon() != NULL) {
            leftSonR = node->getLeftSon()->getRank();
        }
        if (node->getRightSon() != NULL) {
            rightSonR = node->getRightSon()->getRank();
        }
        node->setRank(1 + rightSonR + leftSonR);
    }

    void updateRankSum(AvlNode<T, U> *node, const U &rankVal) {
        int rightSonRankSum = 0, leftSonRankSum = 0;
        if (node->getLeftSon() != NULL) {
            leftSonRankSum = node->getLeftSon()->getRankSum();
        }
        if (node->getRightSon() != NULL) {
            rightSonRankSum = node->getRightSon()->getRankSum();
        }
        node->setRankSum(
                leftSonRankSum + rightSonRankSum + node->getRankValue());
    }

    AvlNode<T, U> *llRotation(AvlNode<T, U> *node, const U &rankVal) {
        return rightRotation(node, rankVal);
    }

    AvlNode<T, U> *lrRotation(AvlNode<T, U> *node, const U &rankVal) {
        node->setLeftSon(leftRotation(node->getLeftSon(), rankVal));
        return rightRotation(node, rankVal);
    }

    AvlNode<T, U> *rlRotation(AvlNode<T, U> *node, const U &rankVal) {
        node->setRightSon(rightRotation(node->getRightSon(), rankVal));
        return leftRotation(node, rankVal);
    }

    AvlNode<T, U> *rrRotation(AvlNode<T, U> *node, const U &rankVal) {
        return leftRotation(node, rankVal);
    }

    AvlNode<T, U> *findMinReplace(AvlNode<T, U> *current) const {
        if (current == NULL) {
            return NULL;
        }
        while (current->getLeftSon() != NULL) {
            current = current->getLeftSon();
        }
        return current;
    }

    AvlNode<T, U> *
    removeTwoSons(AvlNode<T, U> *current, AvlNode<T, U> *bad_node,
                  const U &rankVal) {
        if (cmp(bad_node->getData(), current->getData()) == EQUAL) {
            AvlNode<T, U> *replace_node = findMinReplace(
                    bad_node->getRightSon());
            current->setData(replace_node->getData());
            current->setRankValue(replace_node->getRankValue());
            makeSon(current,
                    removeOneOrNoSons(current->getRightSon(), replace_node,
                                      RIGHT, replace_node->getRankValue()),
                    RIGHT);////////////
        } else if (cmp(bad_node->getData(), current->getData()) == GO_LEFT) {
            // rec call to the function , but makes sure so update pointers.
            makeSon(current,
                    removeTwoSons(current->getLeftSon(), bad_node, rankVal),
                    LEFT);
        } else { //go right.
            makeSon(current,
                    removeTwoSons(current->getRightSon(), bad_node, rankVal),
                    RIGHT);
        }
        updateHeight(current);
        current->setRank(current->getRank() - 1);
        current->setRankSum(current->getRankSum() - rankVal);
        return balance(current, rankVal);
    }

    AvlNode<T, U> *
    removeOneOrNoSons(AvlNode<T, U> *current, AvlNode<T, U> *bad_node,
                      Place p, const U &rankVal) {
        if (cmp(bad_node->getData(), current->getData()) == EQUAL) {
            AvlNode<T, U> *temp = (p == LEFT) ? current->getLeftSon()
                                              : current->getRightSon();
            delete (current);
            return temp;
        }
        if (cmp(bad_node->getData(), current->getData()) == GO_LEFT) {
            // rec call to the function , but makes sure so update pointers.
            makeSon(current,
                    removeOneOrNoSons(current->getLeftSon(), bad_node, p,
                                      rankVal),
                    LEFT);
        } else { //go right.
            makeSon(current,
                    removeOneOrNoSons(current->getRightSon(), bad_node, p,
                                      rankVal),
                    RIGHT);
        }
        updateHeight(current);
        current->setRank(current->getRank() - 1);
        current->setRankSum(current->getRankSum() - rankVal);
        return balance(current, rankVal);
    }

    int maxFunc(int x1, int x2) {
        if (x1 > x2) {
            return x1;
        }
        return x2;
    }

    U recGetKSum(AvlNode<T, U> *node, int k) const {
        if (k == node->getRank()) {
            return node->getRankSum();
        } //k can't be bigger than root rank.
        if (node->getLeftSon() != NULL) {
            //Rank is bigger than all left sub-tree , add it and go right.
            if (k > node->getLeftSon()->getRank() + 1) {
                return node->getLeftSon()->getRankSum() + node->getRankValue() +
                       recGetKSum(node->getRightSon(),
                                  k - node->getLeftSon()->getRank() - 1);
            }
            //Rank equals to root + left tree ;
            if (k == node->getLeftSon()->getRank() + 1) {
                return node->getLeftSon()->getRankSum() + node->getRankValue();
            }
            //Rank is smaller than left tree rank
            return recGetKSum(node->getLeftSon(), k);
        }
    }

public:
    AvlTree() : root(NULL), size(0) {};

    ~AvlTree() {
        if (root == NULL) {
            return;
        }
        treeDestroy(root);
    }

    AvlNode<T, U> *getRoot() const { return root; }

    int getSize() const {
        return size;
    }

    void insert(const T &data, const U &sum) {
        AvlNode<T, U> *new_node = new AvlNode<T, U>(data, sum);
        size++;
        if (root == NULL) {
            root = new_node;
            return;
        }
        root = sortInsert(root, new_node, sum);

    }

    AvlNode<T, U> *find(const T &key) {
        return recFind(root, key);
    }

    template<typename Func>
    void inorder(Func f) {
        startInorder(root, f);
    }

    template<typename Func>
    void preorder(Func f) {
        startPreorder(root, f);
    }

    template<typename Func>
    void postorder(Func f) {
        startPostorder(root, f);
    }

    bool removeNode(const T &key, const U &rankVal) {
        AvlNode<T, U> *node_to_remove = find(key);
        if (node_to_remove == NULL) {
            return false;
        }
        if (node_to_remove->getLeftSon() != NULL &&
            node_to_remove->getRightSon() != NULL) {
            root = removeTwoSons(root, node_to_remove, rankVal);
        } else if (node_to_remove->getLeftSon() != NULL) {
            root = removeOneOrNoSons(root, node_to_remove, LEFT, rankVal);
        } else if (node_to_remove->getRightSon() != NULL) {
            root = removeOneOrNoSons(root, node_to_remove, RIGHT, rankVal);
        } else { //for no sons it's same case as one son.
            root = removeOneOrNoSons(root, node_to_remove, LEFT, rankVal);
        }
        size--;
        return true;
    }

    U getKSum(int k) const {
        if (root == NULL) {
            throw NoElementsInTree();
        }
        if (k > root->getRank()) {
            throw InvalidRank();
        }
        recGetKSum(root, k);
    }

    void treeDestroy(AvlNode<T, U> *node) {
        if (node == NULL) {
            return;
        }
        treeDestroy(node->getLeftSon());
        treeDestroy(node->getRightSon());
        delete (node);
    }

};


#endif //GENTREE_AVLTREE_H
