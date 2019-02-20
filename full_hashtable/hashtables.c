#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
    char *key;
    char *value;
    struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
    int capacity;
    LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
    LinkedPair *pair = malloc(sizeof(LinkedPair));
    pair->key = strdup(key);
    pair->value = strdup(value);
    pair->next = NULL;

    return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
    if (pair != NULL) {
        free(pair->key);
        free(pair->value);
        free(pair);
    }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
    unsigned long hash = 5381;
    int c;
    unsigned char * u_str = (unsigned char *)str;

    while ((c = *u_str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
    HashTable *ht = malloc(sizeof(HashTable));
    ht->capacity = capacity;
    ht->storage = calloc(capacity, sizeof(LinkedPair *));
    return ht;
}

void append_to_end_of_pairs(LinkedPair *slp, LinkedPair *elp)
{
    if (strcmp(elp->key, slp->key) == 0)
    {
        // key already exists so return
        return;
    }
    LinkedPair *next = slp->next;
    if (!next)
    {
        slp->next = elp;
        return;
    }
    append_to_end_of_pairs(slp, elp);
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
    unsigned int i = hash(key, ht->capacity);
    LinkedPair *slp = ht->storage[i];
    LinkedPair *elp = create_pair(key, value);
    if (slp)
    {
        append_to_end_of_pairs(slp, elp);
    }
    ht->storage[i] = elp;
}

/// Returns pair before query
LinkedPair *find_pair(LinkedPair *lp, char *query)
{
    LinkedPair *next = lp->next;
    if (lp->next)
    {
        if (strcmp(query, next->key) == 0)
        {
            return lp;
        }
        find_pair(lp, query);
    }
    // Should only be true if a single pair is present and is the same as the query
    if (strcmp(query, lp->key) == 0)
    {
        return lp;
    }
    // Couldn't find the query
    return NULL;
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
    unsigned int i = hash(key, ht->capacity);
    LinkedPair *slp = ht->storage[i];
    if (!slp)
    {
        fprintf(stderr, "Key not found\n");
        return;
    }
    LinkedPair *plp = find_pair(slp, key);
    if (!plp)
    {
        fprintf(stderr, "Key not found\n");
        return;
    }
    LinkedPair *lp = plp->next;
    LinkedPair *nlp = lp->next;
    plp->next = nlp;
    destroy_pair(lp);
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
    unsigned int i = hash(key, ht->capacity);
    LinkedPair *slp = ht->storage[i];
    if (!slp)
    {
        fprintf(stderr, "Key not found\n");
        return NULL;
    }
    LinkedPair *lp = find_pair(slp, key);
    if (!lp)
    {
        return NULL;
    }
    return lp->next->value;
}

void destroy_all_pairs(LinkedPair *slp)
{
    LinkedPair *next = slp->next;
    free(slp);
    if (next)
    {
        destroy_all_pairs(next);
    }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
    for (int i = 0; i < ht->capacity; i++)
    {
        destroy_all_pairs(ht->storage[i]);
    }
    free(ht->storage);
    free(ht);
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
    HashTable *new_ht = malloc(sizeof(HashTable));
    new_ht->capacity = ht->capacity * 2;
    new_ht->storage = realloc(ht->storage, ht->capacity * sizeof(LinkedPair *));
    destroy_hash_table(ht);
    return new_ht;
}


#ifndef TESTING
int main(void)
{
    struct HashTable *ht = create_hash_table(2);

    hash_table_insert(ht, "line_1", "Tiny hash table\n");
    hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
    hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

    printf("%s\n", hash_table_retrieve(ht, "line_1"));
    printf("%s", hash_table_retrieve(ht, "line_2"));
    printf("%s", hash_table_retrieve(ht, "line_3"));

    int old_capacity = ht->capacity;
    ht = hash_table_resize(ht);
    int new_capacity = ht->capacity;

    printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

    destroy_hash_table(ht);

    return 0;
}
#endif
