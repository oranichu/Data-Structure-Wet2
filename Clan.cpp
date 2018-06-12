//
// Created by user on 10/06/2018.
//

#include "Clan.h"

Clan::Clan(int id) : id(id), status(ABLE_TO_FIGHT),
                     heapIndex(-1) {}

void Clan::setHeapIndex(int i) { heapIndex = i; }

int Clan::getHeapIndex() const { return heapIndex; }

void Clan::addPlayer(Player p) { player_tree.insert(p, p.getScore()); }

int Clan::getSize() { return player_tree.getSize(); }

bool Clan::operator!=(Clan c) { return id != c.id; }

void updateIndex::operator()(Clan *clan, int i) {
    clan->setHeapIndex(i);
}

void getClanID::operator()(Clan *clan) {
    clan->getId();
}
