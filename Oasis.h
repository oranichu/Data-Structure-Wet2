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
    Hash<Clan , getClanID> *clans_hash;
    MinHeap<Clan *, int, updateIndex> *clans_min_heap;
    AvlTree<Player, int, PlayerCompareId> players_tree;
public:
    Oasis(int n, int *clanIDs) {
        Clan **clan_arr = new Clan *[n];
        for (int i = 0; i < n; i++) {
            clan_arr[i] = new Clan(clanIDs[i]);
        }
        int min = -1;
        clans_hash = new Hash<Clan , getClanID>(clan_arr, n );
        clans_min_heap = new MinHeap<Clan *, int, updateIndex>(clan_arr,
                                                               clanIDs, n,
                                                               INVALID_CLAN_ID,
                                                               updateIndex());
        delete[] clan_arr;
    }

    ~Oasis() {
        delete clans_min_heap;

      //  clans_hash->deleteElements(); //deletes all clans made.
        delete clans_hash;
    }

    void addClan(int clanId) {
        if (clanId < MIN_CLAN_ID) {
            throw InvalidInput();
        }
        Clan new_clan(clanId); //clan will be deleted in ~Oasis.
        if (clans_hash->find(new_clan) != NULL) {
            throw OasisFailure();
        }
        clans_hash->insert(new_clan);
        clans_min_heap->insert(&new_clan, clanId);
    }

    void addPlayer(int playerId, int score, int clanId) {
        if (clanId < 0 || playerId < 0 || score < 0) {
            throw InvalidInput();
        }
        Clan clan(clanId);
        Player p(playerId, score);
        if (clans_hash->find(clan) == NULL || players_tree.find(p) != NULL) {
            throw OasisFailure();
        }
        players_tree.insert(p, DEFAULT_SUM);
        clans_hash->find(clan)->getData().addPlayer(p);
    }

    void clanFight(int clan1Id, int clan2Id, int k1, int k2) {
        if (clan1Id < MIN_CLAN_ID || clan2Id < MIN_CLAN_ID || k1 <= 0 ||
            k2 <= 0) {
            throw InvalidInput();
        }
        Clan clan1(clan1Id);
        Clan clan2(clan2Id);
        Node<Clan > *clan1_node = clans_hash->find(clan1);
        Node<Clan > *clan2_node = clans_hash->find(clan2);
        if (clan1_node == NULL || clan2_node == NULL) {
            throw OasisFailure();
        }
        if (clan1_node->getData().canFight() == false ||
            clan2_node->getData().canFight() == false) {
            throw OasisFailure();
        }
        try { // getting k1 sum and k2 sum .
            int sum1 = clan1_node->getData().getPlayersTree().getKSum(k1);
            int sum2 = clan1_node->getData().getPlayersTree().getKSum(k2);
            if (sum1 > sum2) {//clan1 won.
                clan2_node->getData().clanDefeted();
            } else if (sum1 < sum2) { //clan2 won.
                clan1_node->getData().clanDefeted();
            } else if (clan1_node->getData().getId() < //Tie
                       clan2_node->getData().getId()) {
                clan2_node->getData().clanDefeted(); //clan1 id is smaller.
            } else { //clan2 id is smaller.
                clan1_node->getData().clanDefeted();
            }
        } catch (InvalidRank e) { //if k1 or k2 are smaller than clan size.
            throw OasisFailure();
        }
    }

    void getMinClan(int *clan) {
        if (clan==NULL) {
            throw OasisFailure() ;
        }
        try {
           *clan = clans_min_heap->getMin()->getId();
        }
        catch (NoElements e){
            throw OasisFailure() ;
        }
    }

};

#endif //MIVNE_WET2_OASIS_H
