#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/
typedef struct Pair
{
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable
{
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL)
  {
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
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  // since we're overriding values, we don't need calloc as it's slightly slower

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair *)); // create space for capacity (16) size of pair pointers as that's what we're storing in storage

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // hash the key
  int returned_key = 0;
  returned_key = hash(key, ht->capacity);

  // check if there's already a pair at the index
  // if there is, then we have a hash table collision
  // we'll send an error, destroy the existing pair and add the new pair
  if (ht->storage[returned_key] != NULL)
  {
    printf("Warning: a pair already exists at that index.");
    destroy_pair(ht->storage[returned_key]);
    ht->storage[returned_key] = create_pair(key, value);
  }
  // if there's no pair at the storage index, store the newly created pair at the storage pointer. Insert at the index of the hashed key
  else
  {
    ht->storage[returned_key] = create_pair(key, value);
  };
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // hash the key
  int returned_key = 0;
  returned_key = hash(key, ht->capacity);

  // if the pointer index is null, print error
  if (ht->storage[returned_key] == NULL)
  {
    printf("Key not found.");
  }
  // if the pointer index is not null, destroy the pair
  else if (ht->storage[returned_key] != NULL)
  {
    destroy_pair(ht->storage[returned_key]);
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // hash the key
  int returned_key = 0;
  returned_key = hash(key, ht->capacity);

  // if the key doesn't exist, return error
  if (ht->storage[returned_key] == NULL)
  {
    printf("Key not found.");
  }
  // if the key of the index and the key are the same, return the index value
  else if (strcmp(ht->storage[returned_key]->key, key) == 0)
  {
    return ht->storage[returned_key]->value;
  }
  // otherwise return null
  else
  {
    return NULL;
  }
  return 0;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // go through the storage checking for values that aren't null and destroy them
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      destroy_pair(ht->storage[i]);
      break;
    }
  };
  // we call free on pointers. We don't free capacity as it's not a pointer
  free(ht->storage);
  free(ht);
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
