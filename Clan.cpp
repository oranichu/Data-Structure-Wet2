//
// Created by user on 10/06/2018.
//

#include "Clan.h"

Clan::Clan(int id) : id(id), heapIndex(-1) {status=ABLE_TO_FIGHT;}

void Clan::setHeapIndex(int i) { heapIndex = i; }

int Clan::getHeapIndex() const { return heapIndex; }

void Clan::addPlayer(Player p) { player_tree.insert(p, p.getScore()); }

int Clan::getSize() { return player_tree.getSize(); }

bool Clan::operator!=(Clan c) { return id != c.id; }

bool Clan::canFight() {
    return status == ABLE_TO_FIGHT;
}

void Clan::clanDefeted() {
    status = DEFETED;
}

const AvlTree<Player, int, PlayerCompareScore> &Clan::getPlayersTree() const {
    return player_tree;
}

void updateIndex::operator()(Clan *clan, int i) {
    clan->setHeapIndex(i);
}

int getClanID::operator()(Clan *clan) {
    return clan->getId();
}

bool ClanCompareId::operator()(Clan *clan1, Clan *clan2) {
    return clan1->getId() == clan2->getId();
}
