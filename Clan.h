//
// Created by user on 10/06/2018.
//

#ifndef MIVNE_WET2_CLAN_H
#define MIVNE_WET2_CLAN_H

#include "AvlRankTree.h"

#include "MinHeap.h"
#include "Player.h"

typedef enum {
    ABLE_TO_FIGHT, DEFETED
} FightStatus;

class Clan {
    int id;
    int heapIndex;
    FightStatus status;
    AvlTree<Player, int, PlayerCompareScore> player_tree;


public:
    Clan(int id, int size) : id(id), status(ABLE_TO_FIGHT),
                             heapIndex(-1) {}

    void setHeapIndex(int i) { heapIndex = i; }

    int getHeapIndex() const { return heapIndex; }

    void addPlayer(Player p) { player_tree.insert(p, p.getScore()); }

    int getSize() { return player_tree.getSize(); }

    bool operator!=(Clan c) { return id != c.id; }


};

class updateIndex {
public:
    void operator()(Clan *clan, int i) {
        clan->setHeapIndex(i);
    }
};


#endif //MIVNE_WET2_CLAN_H
