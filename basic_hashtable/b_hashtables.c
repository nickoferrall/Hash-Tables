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
  BasicHashTable *ht = calloc(1, sizeof(BasicHashTable));

  ht->capacity = capacity;
  ht->storage = calloc(1, capacity * sizeof(char *));

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

  // store the newly created pair into the storage pointer. Insert at the index of the hashed key
  ht->storage[returned_key] = create_pair(key, value);
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

  // remove the key from the storage pointer
  free(ht->storage[returned_key]);
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

  // if the key exists, return the key. Otherwise, return NULL
  int *my_pointer = ht->storage[returned_key];
  if (strcmp(my_pointer[0], key) == 0)
  {
    return *my_pointer;
  }
  else
  {
    return NULL;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != 0)
    {
      destroy_pair(ht->storage[i]);
      break;
    }
  };
  free(ht->storage);
  // free(ht->capacity); // not able to remove capacity?
  free(ht);
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  // printf("%s", hash_table_retrieve(ht, "line"));

  // hash_table_remove(ht, "line");

  // if (hash_table_retrieve(ht, "line") == NULL)
  // {
  //   printf("...gone tomorrow. (success)\n");
  // }
  // else
  // {
  //   fprintf(stderr, "ERROR: STILL HERE\n");
  // }

  // destroy_hash_table(ht);

  return 0;
}
#endif
