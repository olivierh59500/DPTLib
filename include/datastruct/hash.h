/* hash.h -- associative arrays */

/**
 * \file hash.h
 *
 * Hash is an associative array.
 *
 * The interface presently forces you to malloc all keys, and values passed
 * in, yourself.
 */

#ifndef DATASTRUCT_HASH_H
#define DATASTRUCT_HASH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "base/result.h"

/* ----------------------------------------------------------------------- */

#define result_HASH_END      (result_BASE_HASH + 0) /* Indicates final element */
#define result_HASH_BAD_CONT (result_BASE_HASH + 1) /* Invalid continuation value */

/* ----------------------------------------------------------------------- */

#define T hash_t

typedef struct hash T;

/* ----------------------------------------------------------------------- */

/**
 * A function called to hash the specified key.
 */
typedef unsigned int (hash_fn_t)(const void *a);

/**
 * A function called to compare the two specified keys.
 */
typedef int (hash_compare_t)(const void *a, const void *b);

/**
 * A function called to destroy the specified key.
 */
typedef void (hash_destroy_key_t)(void *key);

/**
 * A function called to destroy the specified value.
 */
typedef void (hash_destroy_value_t)(void *value);

/**
 * A no-op function for use where a hash_destroy_key is expected.
 */
hash_destroy_key_t hash_no_destroy_key;

/**
 * A no-op function for use where a hash_destroy_value is expected.
 */
hash_destroy_value_t hash_no_destroy_value;

/**
 * Create a hash.
 *
 * \param      default_value Value to return for failed lookups.
 * \param      nbins         Suggested number of hash bins to allocate.
 * \param      fn            Function to hash keys.
 * \param      compare       Function to compare keys.
 * \param      destroy_key   Function to destroy a key.
 * \param      destroy_value Function to destroy a value.
 * \param[out] hash          Created hash.
 *
 * \return Error indication.
 */
result_t hash_create(const void            *default_value,
                     int                    nbins,
                     hash_fn_t             *fn,
                     hash_compare_t        *compare,
                     hash_destroy_key_t    *destroy_key,
                     hash_destroy_value_t  *destroy_value,
                     T                    **hash);

/**
 * Destroy a hash.
 *
 * \param doomed Hash to destroy.
 */
void hash_destroy(T *doomed);

/* ----------------------------------------------------------------------- */

/**
 * Return the value associated with the specified key.
 *
 * \param hash Hash.
 * \param key  Key to look up.
 *
 * \return Value associated with the specified key.
 */
const void *hash_lookup(T *hash, const void *key);

/**
 * Insert the specified key:value pair into the hash.
 *
 * The hash takes ownership of the key and value pointers. It will call the
 * destroy functions passed to hash_create when the keys and values are to
 * be destroyed.
 *
 * \param hash  Hash.
 * \param key   Key to insert.
 * \param value Associated value.
 *
 * \return Error indication.
 */
result_t hash_insert(T *hash, const void *key, const void *value);

/**
 * Remove the specified key from the hash.
 *
 * \param hash Hash.
 * \param key  Key to remove.
 */
void hash_remove(T *hash, const void *key);

/**
 * Return the count of items stored in the hash.
 *
 * \param hash Hash.
 *
 * \return Count of items in the hash.
 */
int hash_count(T *hash);

/* ----------------------------------------------------------------------- */

/**
 * A function called for every key:value pair in the hash.
 *
 * Return an error to halt the walk operation.
 */
typedef int (hash_walk_callback_t)(const void *key,
                                   const void *value,
                                   void       *opaque);

/**
 * Walk the hash, calling the specified routine for every element.
 *
 * \param hash   Hash.
 * \param cb     Callback routine.
 * \param opaque Opaque pointer to pass to callback routine.
 *
 * \return Error indication.
 * \retval result_OK If the walk completed successfully.
 */
result_t hash_walk(const T *hash, hash_walk_callback_t *cb, void *opaque);

/* ----------------------------------------------------------------------- */

/**
 * Walk the hash, returning each element in turn.
 *
 * \param      hash             Hash.
 * \param      continuation     Continuation value. Zero for initial call.
 * \param[out] nextcontinuation Next continuation value.
 * \param[out] key              Pointer to receive key.
 * \param[out] value            Pointer to receive value.
 *
 * \return Error indication.
 * \retval result_OK       If an element was found.
 * \retval result_HASH_END If no elements remain.
 */
result_t hash_walk_continuation(T           *hash,
                                int          continuation,
                                int         *nextcontinuation,
                                const void **key,
                                const void **value);

/* ----------------------------------------------------------------------- */

#undef T

#ifdef __cplusplus
}
#endif

#endif /* DATASTRUCT_HASH_H */
