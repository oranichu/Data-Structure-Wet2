//
// Created by user on 12/06/2018.
//

#include "Oasis.h"

Oasis::Oasis(int n, int *clanIDs) {
    Clan **clan_arr = new Clan *[n];
    for (int i = 0; i < n; i++) {
        clan_arr[i] = new Clan(clanIDs[i]);
        clans_list.insert(clan_arr[i]);
    }
    clans_hash = new Hash<Clan *, getClanID,ClanCompareId>(clan_arr, n );
    clans_min_heap = new MinHeap<Clan *, int, updateIndex>(clan_arr,
                                                           clanIDs, n,
                                                           INVALID_CLAN_ID,
                                                           updateIndex());
    delete[] clan_arr;
}

Oasis::~Oasis() {
    Node<Clan*> *node = clans_list.getFirst();
    while (node != NULL ) {
        delete(node->getData());
        node=node->getNext();
    }
    delete clans_min_heap;

    //  clans_hash->deleteElements(); //deletes all clans made.
    delete clans_hash;
}

void Oasis::addClan(int clanId) {
    if (clanId < MIN_CLAN_ID) {
        throw InvalidInput();
    }
    Clan *new_clan = new Clan(clanId); //clan will be deleted in ~Oasis.
    if (clans_hash->find(new_clan) != NULL) {
        delete new_clan;
        throw OasisFailure();
    }
    clans_hash->insert(new_clan);
    clans_list.insert(new_clan);
    clans_min_heap->insert(new_clan, clanId);
}

void Oasis::addPlayer(int playerId, int score, int clanId) {
    if (clanId < 0 || playerId < 0 || score < 0) {
        throw InvalidInput();
    }
    Clan *clan = new Clan(clanId);
    Player p(playerId, score);
    if (clans_hash->find(clan) == NULL || players_tree.find(p) != NULL) {
        delete clan;
        throw OasisFailure();
    }
    players_tree.insert(p, DEFAULT_SUM);
    clans_hash->find(clan)->getData()->addPlayer(p);
    delete clan;
}

void Oasis::clanFight(int clan1Id, int clan2Id, int k1, int k2) {
    if (clan1Id < MIN_CLAN_ID || clan2Id < MIN_CLAN_ID || k1 <= 0 ||
        k2 <= 0) {
        throw InvalidInput();
    }
    if (clan1Id == clan2Id ) {
        throw OasisFailure();
    }
    Clan *clan1 = new Clan(clan1Id);
    Clan *clan2 = new Clan(clan2Id);
    Node<Clan *> *clan1_node = clans_hash->find(clan1);
    Node<Clan *> *clan2_node = clans_hash->find(clan2);
    delete (clan1);
    delete (clan2);
    if (clan1_node == NULL || clan2_node == NULL) {
        throw OasisFailure();
    }
    if (clan1_node->getData()->canFight() == false ||
        clan2_node->getData()->canFight() == false) {
        throw OasisFailure();
    }
    try { // getting k1 sum and k2 sum .
        int sum1 = clan1_node->getData()->getPlayersTree().getKSum(k1);
        int sum2 = clan2_node->getData()->getPlayersTree().getKSum(k2);
        if (sum1 > sum2) {//clan1 won.
            clan2_node->getData()->clanDefeted();
            clans_min_heap->delNode(clan2_node->getData()->getHeapIndex());
        } else if (sum1 < sum2) { //clan2 won.
            clan1_node->getData()->clanDefeted();
            clans_min_heap->delNode(clan1_node->getData()->getHeapIndex());
        } else if (clan1_node->getData()->getId() < //Tie
                   clan2_node->getData()->getId()) {
            clan2_node->getData()->clanDefeted(); //clan1 id is smaller.
            clans_min_heap->delNode(clan2_node->getData()->getHeapIndex());
        } else { //clan2 id is smaller.
            clan1_node->getData()->clanDefeted();
            clans_min_heap->delNode(clan1_node->getData()->getHeapIndex());
        }
    } catch (std::exception& e) { //if k1 or k2 are smaller than clan size.
        throw OasisFailure();
    }

}

void Oasis::getMinClan(int *clan) {
    if (clan==NULL) {
        throw InvalidInput() ;
    }
    try {
        *clan = clans_min_heap->getMin()->getId();
    }
    catch (const NoElements& e){
        throw OasisFailure() ;
    }
}
