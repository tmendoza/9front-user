
typedef struct dict *Dict;

/* create a new empty dictionary */
Dict dictcreate(void);

/* destroy a dictionary */
void dictdestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void dictinsert(Dict, const char *key, const char *value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char *dictsearch(Dict, const char *key);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void dictdelete(Dict, const char *key);

struct elt {
    struct elt *next;
    char *key;
    char *value;
};

struct dict {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt **table;
};

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)