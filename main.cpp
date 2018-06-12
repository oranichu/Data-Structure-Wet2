#include <iostream>
#include "AvlRankTree.h"
#include "Player.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    AvlTree<Player,int,PlayerCompareScore> t ;
    for (int i=1 ; i<50 ; i++) {
        t.insert(Player(i,i),i);
    }
    if (t.getKSum(10) != 445) {
        std::cout << "***********ERROR**************" << std::endl ;
    };
    if (t.getKSum(49) != 1225) {
        std::cout << "***********ERROR**************" << std::endl ;
    };
    if (t.getKSum(1) != 49) {
        std::cout << "***********ERROR**************" << std::endl ;
    };
    t.removeNode(Player(47,47),47);
    if (t.getKSum(10) != 437) {
        std::cout << "***********ERROR**************" << std::endl ;
    };
    return 0;
}