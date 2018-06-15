//
// Created by user on 12/06/2018.
//

#ifndef MIVNE_WET2_OASIS_H
#define MIVNE_WET2_OASIS_H

#include "List.h"
#include "Hash.h"
#include "AvlRankTree.h"
#include "MinHeap.h"
#include "Player.h"
#include "Clan.h"

#define MIN_CLAN_ID (0)
#define INVALID_CLAN_ID (-1)
#define DEFAULT_SUM (0)


class OasisExceptions : public std::exception {
};

class OasisFailure : public OasisExceptions {
public:
    virtual const char *what() const throw() {
        return "Clan is already in Oasis.";
    };
};

class InvalidInput : public OasisExceptions {
public:
    virtual const char *what() const throw() {
        return "Invalid Input.";
    };
};


class Oasis {
    Hash<Clan *, getClanID,ClanCompareId> *clans_hash;
    MinHeap<Clan *, int, updateIndex> *clans_min_heap;
    AvlTree<Player, int, PlayerCompareId> players_tree;

    List<Clan *,ClanCompareId> clans_list;

public:

    Oasis(int n, int *clanIDs);

    ~Oasis();

    void addClan(int clanId);

    void addPlayer(int playerId, int score, int clanId);

    void clanFight(int clan1Id, int clan2Id, int k1, int k2);

    void getMinClan(int *clan);

};

#endif //MIVNE_WET2_OASIS_H
