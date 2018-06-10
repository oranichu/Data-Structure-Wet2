//
// Created by user on 10/06/2018.
//

#include "Player.h"

Player::Player(int id, int score) : id(id), score(score) {}

int Player::getId() const { return id; }

int Player::getScore() const { return score; }

int PlayerCompareId::operator()(Player p1, Player p2) {
    if (p1.getId() > p2.getId()) {
        return GO_LEFT;
    }
    if (p1.getId() < p2.getId()) {
        return GO_RIGHT;
    }
    return EQUAL;
}

int PlayerCompareScore::operator()(Player p1, Player p2) {
    if (p1.getScore() > p2.getScore()) {
        return GO_LEFT;
    }
    if (p1.getScore() < p2.getScore()) {
        return GO_RIGHT;
    }
    return EQUAL;
}
