//
// Created by user on 10/06/2018.
//

#ifndef MIVNE_WET2_PLAYER_H
#define MIVNE_WET2_PLAYER_H

#include "AvlRankTree.h"


class Player {
    int id;
    int score;
public:

    Player(int id, int score);;

    int getId() const;;

    int getScore() const;;

};

class PlayerCompareScore {
public:
    Direction operator()(Player p1, Player p2);
};

class PlayerCompareId {
public:
    Direction operator()(Player p1, Player p2);
};


#endif //MIVNE_WET2_PLAYER_H
