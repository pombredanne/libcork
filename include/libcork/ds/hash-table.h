/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef LIBCORK_DS_HASH_TABLE_H
#define LIBCORK_DS_HASH_TABLE_H

/**
 * @file
 * @brief Implementation of the @ref hash_table submodule
 */

#include <libcork/core/allocator.h>
#include <libcork/core/hash.h>
#include <libcork/core/types.h>
#include <libcork/ds/dllist.h>

/**
 * @addtogroup hash_table
 *
 * <tt>#%include \<libcork/ds/hash-table.h\></tt>
 *
 * This section defines a hash table data structure.
 *
 * @{
 */


/**
 * @brief A function that computes hash values for the keys in a hash
 * table.
 *
 * @param [in] key  A hash table key
 * @returns The hash value for that key
 *
 * @since 0.2
 */

typedef cork_hash_t
(*cork_hash_table_hasher_t)(const void *key);


/**
 * @brief A function that compares two keys for equality.
 *
 * @param [in] key1  A hash table key
 * @param [in] key2  A hash table key
 * @returns Whether the two keys are equal
 *
 * @since 0.2
 */

typedef bool
(*cork_hash_table_comparator_t)(const void *key1, const void *key2);


/**
 * @brief The contents of each entry in a hash table.
 */

typedef struct cork_hash_table_entry_t
{
    /** @brief The hash of this entry's key. */
    cork_hash_t  hash;

    /** @brief This entry's key */
    void  *key;

    /** @brief This entry's value */
    void  *value;

    /**
     * @brief A link to the other entries in the same hash bucket.
     * @private
     */
    cork_dllist_item_t  siblings;
} cork_hash_table_entry_t;


/**
 * @brief A hash table class.
 *
 * @since 0.2
 */

typedef struct cork_hash_table_t
{
    /**
     * @brief The current array of bins in the hash table.
     * @private
     */
    cork_dllist_t  *bins;

    /**
     * @brief The number of bins in the hash table.
     * @private
     */
    size_t  bin_count;

    /**
     * @brief The number of entries in the hash table.
     * @private
     */
    size_t  entry_count;

    /**
     * @brief A hashing function.
     * @private
     */
    cork_hash_table_hasher_t  hasher;

    /**
     * @brief A comparator function.
     * @private
     */
    cork_hash_table_comparator_t  comparator;

    /**
     * @brief The custom allocator to use with this hash table.
     * @private
     */
    cork_allocator_t  *alloc;
} cork_hash_table_t;

/* end of hash_table group */
/**
 * @}
 */


/**
 * @brief Initialize a new hash table.
 *
 * @param [in] alloc  A custom allocator
 * @param [in] table  An uninitialized hash table
 * @param [in] initial_size  An estimated initial number of elements
 * that you'll want to put into the table, or @c 0 to use a default
 * initial size
 * @param [in] hasher  A hashing function
 * @param [in] comparator  A comparator function
 * @returns Whether we could successfully allocate the hash table.
 *
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

bool
cork_hash_table_init(cork_allocator_t *alloc,
                     cork_hash_table_t *table,
                     size_t initial_size,
                     cork_hash_table_hasher_t hasher,
                     cork_hash_table_comparator_t comparator);

/**
 * @brief Allocate and initialize a new hash table.
 *
 * @param [in] alloc  A custom allocator
 * @param [in] initial_size  An estimated initial number of elements
 * that you'll want to put into the table, or @c 0 to use a default
 * initial size
 * @param [in] hasher  A hashing function
 * @param [in] comparator  A comparator function
 *
 * @returns A new hash table, or @c NULL if the hash table couldn't be
 * allocated.
 *
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

cork_hash_table_t *
cork_hash_table_new(cork_allocator_t *alloc,
                    size_t initial_size,
                    cork_hash_table_hasher_t hasher,
                    cork_hash_table_comparator_t comparator);


/**
 * @brief Finalize a hash table.
 *
 * Nothing special is done to any remaining keys or values in the table;
 * if they need to be finalized, you should do that yourself before
 * calling this function.
 *
 * @param [in] table  A hash table
 *
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

void
cork_hash_table_done(cork_hash_table_t *table);

/**
 * @brief Finalize and deallocate a hash table.
 *
 * Nothing special is done to any remaining keys or values in the table;
 * if they need to be finalized, you should do that yourself before
 * calling this function.
 *
 * @param [in] table  A hash table
 *
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

void
cork_hash_table_free(cork_hash_table_t *table);


/**
 * @brief Remove all entries from a hash table, without finalizing the
 * hash table itself.
 *
 * Nothing special is done to any remaining keys or values in the table;
 * if they need to be finalized, you should do that yourself before
 * calling this function.
 *
 * @param [in] table  A hash table
 *
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

void
cork_hash_table_clear(cork_hash_table_t *table);


/**
 * @brief Ensure that a hash table has enough space to store a certain
 * number of elements.
 *
 * @param [in] table  A hash table
 * @param [in] desired_count  The desired number of elements to reserve
 * space for
 *
 * @returns @c true if we were able to reserve enough space; @c false
 * otherwise.
 *
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

bool
cork_hash_table_ensure_size(cork_hash_table_t *table,
                            size_t desired_count);


/**
 * @brief Return the number of elements in a hash table.
 * @param [in] table  A hash table
 * @returns The number of elements in the hash table
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

#if defined(CORK_DOCUMENTATION)
size_t
cork_hash_table_size(const cork_hash_table_t *table);
#else
#define cork_hash_table_size(table) ((table)->entry_count)
#endif


/**
 * @brief Retrieve the value in a hash table with the given key.
 *
 * We return @c NULL if there is no corresponding entry in the table.
 *
 * @param [in] table  A hash table
 * @param [in] key  The key to search for
 * @returns A pointer to the corresponding value, or @c NULL if there is
 * no entry with the given key.
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

void *
cork_hash_table_get(const cork_hash_table_t *table,
                    const void *key);


/**
 * @brief Retrieve the entry in a hash table with the given key.
 *
 * If you update the entry's @a key field, you must ensure that the new
 * value is considered “equal” to the old value, according to the hash
 * table's hasher and comparator functions.
 *
 * @param [in] table  A hash table
 * @param [in] key  The key to search for
 * @returns A pointer to the corresponding entry, or @c NULL if there is
 * no entry with the given key.
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

cork_hash_table_entry_t *
cork_hash_table_get_entry(const cork_hash_table_t *table,
                          const void *key);


/**
 * @brief Retrieve an entry from a hash table, creating it if necessary.
 *
 * If there is no entry with the given key, it will be created.  The new
 * entry's @a value will initially be empty, but you can fill it in.
 *
 * You can also update the entry's @a key; this is necessary, for
 * instance, if the @a key parameter that we search for was allocated on
 * the stack.  We can't save this stack key into the hash table, since
 * it will disappear as soon as the calling function finishes.  However,
 * you can create a new key on the heap, and save it into the new entry.
 *
 * @param [in] table  A hash table
 * @param [in] key  The key to search for
 * @param [out] is_new  Whether the entry is new
 *
 * @returns The entry for the given key, or @c NULL if we needed to
 * create a new entry and could not
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

cork_hash_table_entry_t *
cork_hash_table_get_or_create(cork_hash_table_t *table,
                              void *key, bool *is_new);

/**
 * @brief Add an entry to a hash table, overwriting it if necessary.
 *
 * If there is already an entry with the given key, we will overwrite
 * its key and value.  The existing key and value will be returned in
 * the @a old_key and @a old_value parameters, to let you finalize them,
 * if needed.
 *
 * @param [in] table  A hash table
 * @param [in] key  The key to add
 * @param [in] value  The corresponding value
 * @param [out] old_key  The previous entry's key, if any
 * @param [out] old_value  The previous entry's value, if any
 *
 * @returns Whether the entry was successfully added to the table.
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

bool
cork_hash_table_put(cork_hash_table_t *table,
                    void *key, void *value,
                    void **old_key, void **old_value);

/**
 * @brief Remove an entry from a hash table.
 *
 * @param [in] table  A hash table
 * @param [in] key  The key to remove
 * @param [out] deleted_key  The key from the deleted entry
 * @param [out] deleted_value  The value from the deleted entry
 *
 * @returns Whether there was an entry to remove.
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

bool
cork_hash_table_delete(cork_hash_table_t *table, const void *key,
                       void **deleted_key, void **deleted_value);


/**
 * @brief The return value from a @ref cork_hash_table_mapper_t
 * function.
 * @ingroup hash_table
 * @since 0.2
 */

typedef enum cork_hash_table_map_result_t
{
    /** @brief Abort the current @ref cork_hash_table_map operation. */
    CORK_HASH_TABLE_MAP_ABORT = 0,
    /** @brief Continue on to the next entry in the hash table. */
    CORK_HASH_TABLE_MAP_CONTINUE = 1,
    /** @brief Delete the entry that was just processed, and then
     * continue on to the next entry in the hash table. */
    CORK_HASH_TABLE_MAP_DELETE = 2
} cork_hash_table_map_result_t;

/**
 * @brief A function that can be applied to each entry in a hash table.
 * @param [in] entry  A hash table entry
 * @returns A @ref cork_hash_table_map_result_t value, indicating
 * whether we should abort or continue the map operation, and whether we
 * should delete the current entry before proceeding.
 * @ingroup hash_table
 * @since 0.2
 */

typedef cork_hash_table_map_result_t
(*cork_hash_table_mapper_t)(cork_hash_table_entry_t *entry, void *user_data);

/**
 * @brief Apply a function to each entry in a hash table.
 * @param [in] table  A hash table
 * @param [in] mapper  The function to apply to each entry
 * @param [in] user_data  An additional parameter to pass into @a mapper
 * @public @memberof cork_hash_table_t
 * @since 0.2
 */

void
cork_hash_table_map(cork_hash_table_t *table,
                    cork_hash_table_mapper_t mapper, void *user_data);


#endif /* LIBCORK_DS_HASH_TABLE_H */