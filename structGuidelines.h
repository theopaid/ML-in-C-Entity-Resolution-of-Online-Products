
HashItem *hashArray[n_buckets];

typedef struct HashItem
{
    SpecNode *specPtr;
};

typedef struct SpecNode
{
    CliqueNode *headOfClique;
    SpecNode *nextSpec; // Θα δείχνει στο επόμενο spec, στην περίπτωση που το hashFunction μας δείξει στο ίδιο bucket.
};

typedef struct CliqueNode // Θα υλοποιηθεί κυκλική λίστα. Αρχικά, το next θα δείχνει στον εαυτό του λόγω κυκλικότητας.
{                         // Όταν δημιουργούνται ζευγάρια, θα ενωποιούνται αυτά, κάνοντας update το *next.
    SpecInfo *specInfo;
    CliqueNode *next;
};

typedef struct SpecInfo
{
    char  *specId;
    DscrNode *infoList;
};

typedef struct DscrNode // Περιέχει την περιγραφή (descirption) του spec.
{                       // Σε μορφή λίστας θα αποθηκεύονται.
    char *key;          
    char *value;        // key:value αντιστοίχιση των γραμμών του .json
    DscrNode *next;
};




