#ifndef _CLIQUE_H
#define _CLIQUE_H

#include "../hdr/spec.h"

typedef struct CliqueNode CliqueNode;

struct CliqueNode // Θα υλοποιηθεί κυκλική λίστα. Αρχικά, το next θα δείχνει στον εαυτό του λόγω κυκλικότητας.
{                 // Όταν δημιουργούνται ζευγάρια, θα ενωποιούνται αυτά, κάνοντας update το *next.
    SpecInfo *specInfo;
    CliqueNode *next;
};

#endif