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
    Hash<Clan *, getClanID> *clans_hash;
    MinHeap<Clan *, int, updateIndex> *clans_min_heap;
    AvlTree<Player, int, PlayerCompareId> players_tree;
    int num_of_clans;
public:
    Oasis(int n, int *clanIDs) : num_of_clans(n) {
        Clan **clan_arr = new Clan *[n];
        for (int i = 0; i < n; i++) {
            clan_arr[i] = new Clan(clanIDs[i]);
        }
        clans_hash = new Hash(clan_arr, n, INVALID_CLAN_ID);
        clans_min_heap = new MinHeap(clan_arr, clanIDs,
                                     n, INVALID_CLAN_ID, updateIndex());
        delete[] clan_arr;
    }

    ~Oasis() {
        delete clans_min_heap;
        clans_hash->deleteElements(); //deletes all clans made.
        delete clans_hash;
    }

    void addClan(int clanId) {
        if (clanId < MIN_CLAN_ID) {
            throw InvalidInput();
        }
        Clan *new_clan = new Clan(clanId); //clan will be deleted in ~Oasis.
        if (clans_hash->find(new_clan) != NULL) {
            delete new_clan;
            throw OasisFailure();
        }
        clans_hash->insert(new_clan);
        clans_min_heap->insert(new_clan, clanId);
        num_of_clans++;
    }

    void addPlayer(int playerId, int score, int clanId) {
        if (clanId < 0 || playerId < 0 || score < 0) {
            throw InvalidInput();
        }
        Clan *clan = new Clan(clanId);
        Player p(playerId, score);
        if (clans_hash->find(clan) == NULL || players_tree.find(p) != NULL) {
            delete clan ;
            throw OasisFailure();
        }
        players_tree.insert(p,DEFAULT_SUM);
        clans_hash->find(clan)->getData()->addPlayer(p);
        delete clan ;
    }





};

#endif //MIVNE_WET2_OASIS_H
