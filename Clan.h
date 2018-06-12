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
    Clan(int id);

    void setHeapIndex(int i);

    int getId() const { return id ;}

    int getHeapIndex() const;

    void addPlayer(Player p);

    int getSize();

    bool operator!=(Clan c);


};

class updateIndex {
public:
    void operator()(Clan *clan, int i);
};

class getClanID {
public:
    void operator()(Clan *clan);
};


#endif //MIVNE_WET2_CLAN_H
