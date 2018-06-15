//
// Created by user on 12/06/2018.
//

#include "library2.h"
#include "Oasis.h"

void *init(int n, int *clanIDs) {
    if (n < 2 || clanIDs == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        if (clanIDs[i] < 0) {
            return NULL;
        }
    }
    Oasis *oasis;
    try {
        oasis = new Oasis(n, clanIDs);
    } catch (std::bad_alloc &) {
        return NULL;
    }
    return oasis;
}

StatusType addClan(void *DS, int clanID) {
    if (clanID < 0 || DS == NULL) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis *) DS)->addClan(clanID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (InvalidInput &e) {
        return INVALID_INPUT;
    } catch (OasisFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType addPlayer(void *DS, int playerID, int score, int clan) {
    if (playerID < 0 || clan < 0 || score < 0 || DS == NULL) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis *) DS)->addPlayer(playerID, score, clan);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (InvalidInput &e) {
        return INVALID_INPUT;
    } catch (OasisFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType clanFight(void *DS, int clan1, int clan2, int k1, int k2) {
    if (clan1 < 0 || clan2 < 0 || k1 <= 0 || k2 <= 0 || DS == NULL) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis *) DS)->clanFight(clan1, clan2, k1, k2);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (InvalidInput &e) {
        return INVALID_INPUT;
    } catch (OasisFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType getMinClan(void *DS, int *clan) {
    if (clan == NULL || DS == NULL) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis *) DS)->getMinClan(clan);
    } catch (InvalidInput &e) {
        return INVALID_INPUT;
    } catch (OasisFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}

void quit(void **DS) {
    if (DS == NULL || *DS==NULL) {
        return;
    }
    delete ((Oasis*) *DS);
    *DS=NULL ;
}
