#include <u.h>
#include <libc.h>
#include <stdio.h>
#include "dict.h"



/* dictionary initialization code used in both dictcreate and grow */
Dict
internaldictcreate(int size)
{
    Dict d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != 0);

    for(i = 0; i < d->size; i++) d->table[i] = 0;

    return d;
}

Dict
dictcreate(void)
{
    return internaldictcreate(INITIAL_SIZE);
}

void
dictdestroy(Dict d)
{
    int i;
    struct elt *e;
    struct elt *next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            free(e->value);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static unsigned long
hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void
grow(Dict d)
{
    Dict d2;            /* new dictionary we'll create */
    struct dict swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = internaldictcreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside dictinsert
             * to avoid this problem */
            dictinsert(d2, e->key, e->value);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call dictdestroy on d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;

    dictdestroy(d2);
}

/* insert a new key-value pair into an existing dictionary */
void
dictinsert(Dict d, const char *key, const char *value)
{
    struct elt *e;
    unsigned long h;

    assert(key);
    assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = strdup(value);

    h = hash_function(key) % d->size;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */ 
char *
dictsearch(Dict d, const char *key)
{
    struct elt *e;

    for(e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
        if(!strcmp(e->key, key)) {
            /* got it */
            return e->value;
        }
    }

    return 0;
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void
dictdelete(Dict d, const char *key)
{
    struct elt **prev;          /* what to change when elt is deleted */
    struct elt *e;              /* what to delete */

    for(prev = &(d->table[hash_function(key) % d->size]); 
        *prev != 0; 
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->key, key)) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->value);
            free(e);

            return;
        }
    }
}

/*
int
main()
{
    Dict d;
    char buf[512];
    int i;

    d = dictcreate();

    dictinsert(d, "foo", "hello world");
    puts(dictsearch(d, "foo"));
    dictinsert(d, "foo", "hello world2");
    puts(dictsearch(d, "foo"));
    dictdelete(d, "foo");
    puts(dictsearch(d, "foo"));
    dictdelete(d, "foo");
  
    dictdelete(d, "foo");

    for(i = 0; i < 10000; i++) {
        sprintf(buf, "%d", i);
        dictinsert(d, buf, buf);
    }

    dictdestroy(d);

    return 0;
}
*/