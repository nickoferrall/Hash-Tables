#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  // we create space in memory of the size of the hashtable
  HashTable *ht = malloc(sizeof(HashTable));

  // we set the capacity of the newly created hashtable as the capacity of the variable passed in
  ht->capacity = capacity;

  // calloc creates space for capacity * size of a LinkedPair pointer
  // it then sets the values to NULL
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // create the index by hashing the key
  unsigned int index = hash(key, ht->capacity);

  // check if bucket at the index is occupied
  LinkedPair *currentPair = ht->storage[index];
  LinkedPair *last_pair;

  // while there's something at index and the index doesn't match the key
  while (currentPair != NULL && strcmp(currentPair->key, key) != 0)
  {
    // last pair is empty and now stores what's at index
    last_pair = currentPair;

    // current pair is now equal to what was a currentPair->next
    // why not currentPair = currentPair-> next and delete line above?
    currentPair = last_pair->next;
  };

  // if the current index is occupied with the same key, update the value
  if (currentPair != NULL)
  {
    currentPair->value = value;
  }
  // if the index is NULL, create a new pair; it's next should be the index
  else
  {
    LinkedPair *new_pair = create_pair(key, value);

    // new_pair->next is saved as the current index which is null
    new_pair->next = ht->storage[index];

    // the current index is now the new_pair
    ht->storage[index] = new_pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  // create the index by hashing the key
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];
  while (current_pair != NULL)
  {
    if (strcmp(current_pair->key, key) == 0)
    {
      destroy_pair(current_pair);
      ht->storage[index] = NULL;
    }
    else
    {
      current_pair = current_pair->next;
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // create the index by hashing the key
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];
  while (current_pair != NULL)
  {
    if (strcmp(current_pair->key, key) == 0)
    {
      return current_pair->value;
    }
    current_pair = current_pair->next;
  }

  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{

  for (int i = 0; i < ht->capacity; i++)
  {
    while (ht->storage[i] != NULL)
    {
      LinkedPair *current_pair = ht->storage[i];
      ht->storage[i] = ht->storage[i]->next;
      destroy_pair(current_pair);
    }
  };
  // we call free on pointers. We don't free capacity as it's not a pointer
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(2 * ht->capacity);

  // copy all elements into the new hash table
  for (int i = 0; i < ht->capacity; i++)
  {
    LinkedPair *current_pair = ht->storage[i];
    while (current_pair != NULL)
    {
      hash_table_insert(new_ht, current_pair->key, current_pair->value);
      current_pair = current_pair->next;
    }
  }
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

  printf("%s", hash_table_retrieve(ht, "line_1"));
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
