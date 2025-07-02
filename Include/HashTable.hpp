/**
 * @file HashTable.hpp
 * @brief Implements the core hash table container for Qentem Engine.
 *
 * This header defines the HashTable class, which provides the underlying
 * associative data structure used by both HList and HArray. HashTable offers
 * efficient storage, retrieval, and management of unique keys, supporting
 * fast lookups and ordered key handling as required by its derived containers.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_HASH_TABLE_H
#define QENTEM_HASH_TABLE_H

#include "Memory.hpp"

namespace Qentem {

/**
 * @brief Base hash table item for associative containers.
 *
 * This struct defines the core data members and comparison operators used by
 * all hash table entries in Qentem containers. Stores the hash value, chain link,
 * and the key itself. Implements relational and equality operators to enable
 * sorting and key comparisons within the table.
 *
 * @tparam Key_T The key type. Must support comparison operators.
 */
template <typename Key_T>
struct HTableItem_T {
    /**
     * @brief Hash value for the key.
     */
    SizeT Hash;

    /**
     * @brief Link to the next item in the collision chain.
     */
    SizeT Next;

    /**
     * @brief The key associated with this item.
     */
    Key_T Key;

    /**
     * @brief Less-than comparison by key.
     */
    QENTEM_INLINE bool operator<(const HTableItem_T &item) const noexcept {
        return (Key < item.Key);
    }

    /**
     * @brief Greater-than comparison by key.
     */
    QENTEM_INLINE bool operator>(const HTableItem_T &item) const noexcept {
        return (Key > item.Key);
    }

    /**
     * @brief Less-than-or-equal comparison by key.
     */
    QENTEM_INLINE bool operator<=(const HTableItem_T &item) const noexcept {
        return (Key <= item.Key);
    }

    /**
     * @brief Greater-than-or-equal comparison by key.
     */
    QENTEM_INLINE bool operator>=(const HTableItem_T &item) const noexcept {
        return (Key >= item.Key);
    }

    /**
     * @brief Equality comparison by key.
     */
    QENTEM_INLINE bool operator==(const HTableItem_T &item) const noexcept {
        return (Key == item.Key);
    }
};

/**
 * @brief Generic, flexible hash table base for Qentem Engine.
 *
 * HashTable is the core, policy-based implementation used by all associative
 * containers in Qentem (such as HArray and HList). It supports efficient
 * storage, lookup, and management of unique keys—preserving order and enabling
 * customization of key behavior and item storage.
 *
 * This class is not meant to be used directly by end users. Instead, it forms
 * the basis for specialized containers (like StringHashTable or NumberHashTable)
 * by accepting custom key utilities and item structures.
 *
 * @tparam Key_T      Key type. Must provide comparison and access functions.
 * @tparam KeyUtils_T Policy type that defines static Hash() and IsEqual() for Key_T.
 * @tparam HItem_T    Storage type for each entry (must have members: Hash, Next, Key).
 */
// +------------------------+------------------------+
// |      Hash Table        |        Items           |
// +========================+========================+
// |  [ 0, 1, 2, ... n-1 ]  | [ item0, item1, ... ]  |
// +------------------------+------------------------+
// |  (Hash positions/bins) | (Stored entries; each  |
// |                        |  with key, hash, link, |
// |                        |  and optionally value) |
// +------------------------+------------------------+
template <typename Key_T, typename KeyUtils_T, typename HItem_T>
struct HashTable {
    /**
     * @brief Default constructor. Initializes an empty hash table.
     */
    HashTable() = default;

    /**
     * @brief Constructs a hash table with the given initial capacity.
     *
     * Allocates storage for at least @p size elements.
     *
     * @param size Initial capacity (number of items).
     */
    explicit HashTable(const SizeT capacity) {
        if (capacity != 0) {
            allocate(capacity);
        }
    }

    /**
     * @brief Move constructor. Transfers storage from another hash table.
     *
     * All contents are moved; the source is left empty.
     *
     * @param src HashTable to move from.
     */
    inline HashTable(HashTable &&src) noexcept
        : hashTable_{src.hashTable_}, size_{src.size_}, capacity_{src.capacity_} {
        src.setSize(0);
        src.setCapacity(0);
        src.clearHashTable();
    }

    /**
     * @brief Copy constructor. Deep-copies all entries and internal state.
     *
     * @param src HashTable to copy from.
     */
    inline HashTable(const HashTable &src) {
        copyTableWithHash(src);
    }

    /**
     * @brief Destructor. Disposes of all storage and items.
     */
    inline ~HashTable() {
        SizeT   *hashTable = getHashTable();
        HItem_T *storage   = Storage();

        if (hashTable != nullptr) {
            Memory::Dispose(storage, (storage + Size()));
            Memory::Deallocate(hashTable);
        }
    }

    /**
     * @brief Move assignment operator. Transfers all data from another hash table.
     *
     * Releases any existing storage, then takes ownership of @p src's resources.
     * Leaves @p src empty. Safe for self-assignment.
     *
     * @param src HashTable to move from.
     * @return Reference to this hash table.
     */
    HashTable &operator=(HashTable &&src) noexcept {
        if (this != &src) {
            // Save current pointers and size for cleanup after reassignment.
            SizeT      *ht      = getHashTable();
            HItem_T    *storage = Storage();
            const SizeT size    = Size();

            // Take ownership of src's memory and bookkeeping.
            setHashTable(src.getHashTable());
            setSize(src.Size());
            setCapacity(src.Capacity());

            // Reset src to an empty state so its destructor is safe.
            src.clearHashTable();
            src.setSize(0);
            src.setCapacity(0);

            if (ht != nullptr) {
                // Dispose of the old memory (after transfer, in case of derived/child arrays).
                Memory::Dispose(storage, (storage + size));
                Memory::Deallocate(ht);
            }
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator. Deep-copies all entries from another hash table.
     *
     * Releases any existing storage, clears the current contents, and performs a deep copy
     * of @p src. Safe for self-assignment.
     *
     * @param src HashTable to copy from.
     * @return Reference to this hash table.
     */
    HashTable &operator=(const HashTable &src) {
        if (this != &src) {
            // Save old pointers and size for safe cleanup after copying.
            SizeT      *ht      = getHashTable();
            HItem_T    *storage = Storage();
            const SizeT size    = Size();

            // Clear current state before copying new data.
            clearHashTable();
            setSize(0);
            setCapacity(0);

            // Deep copy all items and hash table layout from src.
            copyTable(src);

            if (ht != nullptr) {
                // Dispose of the old memory (after transfer, in case of derived/child arrays).
                Memory::Dispose(storage, (storage + size));
                Memory::Deallocate(ht);
            }
        }

        return *this;
    }

    /**
     * @brief Move-adds all items from another hash table (src) into this table.
     *
     * Moves every valid item from @p src into this table, overwriting values if the key exists.
     * After completion, @p src is cleared and its storage deallocated.
     *
     * @param src HashTable to move from (will be left empty).
     */
    void operator+=(HashTable &&src) {
        // Compute the total size after merge.
        const SizeT    n_size   = (Size() + src.Size());
        HItem_T       *src_item = src.Storage();
        const HItem_T *src_end  = (src_item + src.Size());

        // Expand capacity if needed to fit all items.
        if (n_size > Capacity()) {
            resize(n_size);
        }

        // Move each valid item (Hash != 0) from src to this table.
        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT   *index;
                HItem_T *storage_item = find(index, *src_item);

                // If not found, insert as a new item; otherwise, overwrite value (MoveDoublecat).
                if (storage_item == nullptr) {
                    insert(index, Memory::Move(*src_item));
                } else {
                    storage_item->MoveDoublecat(*src_item);
                }
            }
            ++src_item;
        }

        // Release src's memory and reset its bookkeeping.
        Memory::Deallocate(src.getHashTable());
        src.clearHashTable();
        src.setSize(0);
        src.setCapacity(0);
    }

    /**
     * @brief Adds (copies) all items from another hash table into this table.
     *
     * For every valid item in @p src, inserts or updates the corresponding item in this table.
     * If a key already exists, its value is overwritten with a copy from @p src.
     *
     * @param src HashTable to copy from (remains unchanged).
     */
    void operator+=(const HashTable &src) {
        // Compute the total size after merge.
        const SizeT    n_size   = (Size() + src.Size());
        const HItem_T *src_item = src.First();
        const HItem_T *src_end  = src_item + src.Size();

        // Expand capacity if necessary.
        if (n_size > Capacity()) {
            resize(n_size);
        }

        // Insert or update each valid item from src.
        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT   *index;
                HItem_T *storage_item = find(index, *src_item);

                // Insert new item if not found; otherwise update value.
                if (storage_item == nullptr) {
                    storage_item = insert(index, *src_item);
                }
                storage_item->CopyValue(*src_item);
            }
            ++src_item;
        }
    }

    /**
     * @brief Inserts a key into the hash table if it does not exist.
     *
     * If the key is already present, this operation has no effect.
     *
     * @param key The key to insert (copied).
     */
    inline void Insert(const Key_T &key) {
        tryInsert(key); // Insert or do nothing if key already exists.
    }

    /**
     * @brief Inserts a key into the hash table using move semantics.
     *
     * If the key is already present, this operation has no effect.
     *
     * @param key The key to insert (moved).
     */
    inline void Insert(Key_T &&key) {
        tryInsert(Memory::Move(key)); // Move-insert or do nothing if key exists.
    }

    /**
     * @brief Checks whether the table contains a given key.
     *
     * @param key The key to look up.
     * @return true if the key is found; false otherwise.
     */
    inline bool Has(const Key_T &key) const noexcept {
        if (IsNotEmpty()) {
            SizeT index;
            return (find(index, key) != nullptr); // Return true if found, false if not.
        }

        return false; // Table is empty.
    }

    /**
     * @brief Retrieves the key at a given index in insertion order.
     *
     * @param index The array-style index to access.
     * @return Pointer to the key at the given index, or nullptr if index is invalid or empty.
     */
    inline const Key_T *GetKeyAt(const SizeT index) const noexcept {
        const HItem_T *src = First();

        // Check bounds and ensure this slot is not deleted.
        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Key);
        }

        return nullptr; // Not found or empty.
    }

    /**
     * @brief Looks up an item by key.
     *
     * @param key The key to find.
     * @return Pointer to the found item, or nullptr if not found.
     */
    inline HItem_T *GetItem(const Key_T &key) noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            return find(index, key);
        }

        return nullptr; // Not found or table is empty.
    }

    inline const HItem_T *GetItem(const Key_T &key) const noexcept {
        if (IsNotEmpty()) {
            SizeT index;
            return find(index, key);
        }

        return nullptr; // Not found or table is empty.
    }

    /**
     * @brief Retrieves an item by its array index.
     *
     * @param index The array-style index to access.
     * @return Pointer to the item at the given index, or nullptr if index is invalid or empty.
     */
    inline HItem_T *GetItemAt(const SizeT index) noexcept {
        HItem_T *src = Storage();

        // Check bounds and ensure this slot is not deleted.
        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return (src + index);
        }

        return nullptr; // Not found or empty.
    }

    /**
     * @brief Retrieves an item by its array index.
     *
     * @param index The array-style index to access.
     * @return Const pointer to the item at the given index, or nullptr if index is invalid or empty.
     */
    inline const HItem_T *GetItemAt(const SizeT index) const noexcept {
        const HItem_T *src = First();

        // Check bounds and ensure this slot is not deleted.
        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return (src + index);
        }

        return nullptr; // Not found or empty.
    }

    /**
     * @brief Looks up the index of a key in insertion order.
     *
     * If the key is present, its index is stored in @p index and true is returned.
     *
     * @param[out] index (Set to the array-style index of the key if found.)
     * @param key The key to search for.
     * @return true if the key is found and index set; false otherwise.
     */
    inline bool GetIndex(SizeT &index, const Key_T &key) const noexcept {
        if (IsNotEmpty()) {
            find(index, key);
            return (index != Capacity());
        }

        return false; // Key not found or table is empty.
    }

    /**
     * @brief Removes the entry for a given key from the table.
     *
     * Does nothing if the key is not present.
     *
     * @param key The key to remove.
     */
    inline void Remove(const Key_T &key) noexcept {
        remove(key); // Dispatch to private remove logic.
    }

    /**
     * @brief Removes the item at the specified array-style index.
     *
     * If the index is valid and the slot is not already empty, removes the corresponding entry.
     *
     * @param index The array-style index of the item to remove.
     */
    inline void RemoveAt(const SizeT index) noexcept {
        // Only act if index is within bounds.
        if (index < Size()) {
            HItem_T *item = (Storage() + index);

            // Only remove if this slot contains a valid item.
            if (item->Hash != 0) {
                remove(item); // Use internal remove logic.
            }
        }
    }

    /**
     * @brief Renames a key to a new (non-existing) key, moving the item and updating hash links.
     *
     * Attempts to rename the item with key 'from' to 'to', only if 'from' exists and 'to' does not.
     * The hash table’s internal chains are updated, and the key’s storage is moved (rvalue version) or copied (const
     * lvalue version).
     *
     * @param from The existing key to rename.
     * @param to   The new key name (rvalue).
     * @return true if renaming is successful; false otherwise.
     */
    inline bool Rename(const Key_T &from, Key_T &&to) const noexcept {
        HItem_T *item = prepareRename(from, to); // Unlink and ready for new key
        if (item != nullptr) {
            item->Key = Memory::Move(to); // Move-assign new key

            return true;
        }

        return false; // Either table empty, source missing, or destination exists
    }

    /**
     * @brief Renames a key to a new (non-existing) key, moving the item and updating hash links.
     *
     * Same as above, but copies the new key instead of moving.
     *
     * @param from The existing key to rename.
     * @param to   The new key name (const lvalue).
     * @return true if renaming is successful; false otherwise.
     */
    inline bool Rename(const Key_T &from, const Key_T &to) {
        HItem_T *item = prepareRename(from, to); // Unlink and ready for new key
        if (item != nullptr) {
            item->Key = to; // Copy-assign new key

            return true;
        }

        return false; // Either table empty, source missing, or destination exists
    }

    /**
     * @brief Ensures that the table can accommodate at least 'count' more elements.
     *
     * If the current capacity is not sufficient to store the current size plus the requested
     * number of additional elements, expands the table to the necessary size.
     *
     * @param count Number of additional elements to expect.
     */
    inline void Expect(const SizeT count) {
        const SizeT new_size = (count + Size()); // Total required size after 'count' more elements

        if (new_size > Capacity()) {
            resize(new_size); // Expand underlying storage if needed
        }
    }

    /**
     * @brief Changes the table's capacity to hold exactly 'new_size' elements.
     *
     * If the new size is smaller than the current number of elements, shrinks the array
     * and disposes any items beyond the new limit. If larger, expands capacity as needed.
     * If new_size is zero, completely resets (deallocates) the table.
     *
     * @param new_size The new maximum number of items for the table.
     */
    void Resize(const SizeT new_size) {
        if (new_size != 0) {
            if (Size() > new_size) {
                // Shrink: Dispose of elements outside new bounds
                HItem_T *storage = Storage();
                Memory::Dispose((storage + new_size), (storage + Size()));
                setSize(new_size); // Adjust logical size
            }

            resize(new_size); // Internal grow/shrink logic
        } else {
            Reset(); // If new_size is zero, fully deallocate
        }
    }

    /**
     * @brief Ensures the hash table has at least the requested capacity and clears existing contents.
     *
     * Resets the table and (re)allocates internal storage to hold at least 'size' items.
     * If 'size' is zero, the table is left empty and unallocated.
     *
     * @param size The number of elements to reserve space for.
     */
    inline void Reserve(SizeT size) {
        Reset(); // Deallocate any current storage and reset size/capacity

        if (size != 0) {
            allocate(size); // Allocate storage for at least 'size' items
        }
    }

    /**
     * @brief Removes all entries from the table, keeping current capacity.
     *
     * Sets the hash table to empty (size zero), disposes all stored items,
     * and resets the hash buckets to zero, but does not free the allocation.
     *
     * Safe to call on an already-empty table.
     */
    void Clear() noexcept {
        if (IsNotEmpty()) {
            SizeT   *ht      = getHashTable();
            HItem_T *storage = Storage();
            Memory::SetToValue(ht, Capacity(), Capacity()); // Reset hash buckets
            Memory::Dispose(storage, (storage + Size()));   // Dispose each item (calls destructors)
            setSize(0);                                     // Logical size is now zero
        }
    }

    /**
     * @brief Completely deallocates all storage and resets the table to an uninitialized state.
     *
     * Removes all entries, frees internal storage, and sets capacity and size to zero.
     * After this call, the table is as if default-constructed.
     *
     * Safe to call repeatedly.
     */
    void Reset() noexcept {
        if (Capacity() != 0) {
            HItem_T *storage = Storage();

            Memory::Dispose(storage, (storage + Size())); // Dispose all elements
            Memory::Deallocate(getHashTable());           // Free the hash table & storage block

            clearHashTable(); // Set pointer to nullptr
            setSize(0);       // Size is now zero
            setCapacity(0);   // Capacity is now zero
        }
    }

    /**
     * @brief Sorts the items in the hash table in ascending or descending order by key.
     *
     * After sorting, the hash table mapping is regenerated to reflect the new order.
     *
     * @param ascend If true (default), sort in ascending order; if false, descending.
     *
     * @note
     * The sorting is stable and preserves key/value associations. After sorting,
     * all hash buckets are reset and rebuilt for consistency.
     */
    void Sort(const bool ascend = true) noexcept {
        if (ascend) {
            Memory::Sort<true>(Storage(), SizeT{0}, Size()); // Sort in ascending order
        } else {
            Memory::Sort<false>(Storage(), SizeT{0}, Size()); // Sort in descending order
        }

        // Reset hash table mapping and rebuild
        Memory::SetToValue(getHashTable(), Capacity(), Capacity());

        generateHash();
    }

    /**
     * @brief Shrinks the internal storage to match the actual number of non-deleted items.
     *
     * If the actual item count is less than the logical size, resizes the table to remove
     * excess (deleted) items and reclaim memory. If all items have been deleted, the
     * table is fully reset and deallocated.
     *
     * @note
     * This operation can be used after bulk removals to optimize memory use.
     */
    void Compress() {
        const SizeT size = ActualSize(); // Count of truly live items

        if (size != 0) {
            if (size < Size()) {
                resize(size); // Shrink to fit
            }
            return;
        }

        Reset(); // If nothing remains, free all memory
    }

    /**
     * @brief Reorders hash table items so that all live entries occupy contiguous slots at the start.
     *
     * Reorder() performs an in-place compaction of all non-deleted items (`Hash != 0`), shifting them
     * forward so that indices `[0, N)` form a dense block of valid entries and all deleted slots are
     * pushed beyond the logical end. The logical size is updated to match the number of live items.
     *
     * After compaction, the hash table's mapping is reset and hash chains are regenerated to match the
     * new storage layout. Storage capacity is unchanged, but all item indices and pointers may change.
     *
     * @usage
     *   Call after bulk deletions to ensure items are contiguous and iteration is efficient, with no gaps
     *   in the storage array. Especially recommended before exporting, serializing, or enumerating all items.
     *
     * @warning
     *   Any pointers or indices to items obtained before calling Reorder() will be invalid after this call.
     *   Use with caution if external code relies on storage layout or indices.
     */
    void Reorder() {
        HItem_T *storage = Storage(); // Pointer to start of item storage array
        SizeT    index   = 0;         // Current scan position in the array
        SizeT    size    = 0;         // Next position to place a live item (and final live count)

        // Scan all current items in storage
        while (index < Size()) {
            // If current slot contains a live item
            if (storage[index].Hash != 0) {
                if (index != size) {
                    HItem_T *item = (storage + index);
                    storage[size] = Memory::Move(*item); // Move live item to compacted position
                    item->Hash    = 0;                   // Mark old slot as dead/tombstone
                }

                ++size; // Increment count of live items and next slot for compaction
            }

            ++index; // Advance to next storage slot
        }

        setSize(size); // Update logical size to match number of live entries

        // Reset all hash table slots to empty
        Memory::SetToValue(getHashTable(), Capacity(), Capacity());

        // Rebuild hash table mapping to match new item layout
        generateHash();
    }

    /**
     * @brief Returns the number of active (non-deleted) items in the hash table.
     *
     * Iterates over the storage array and counts items whose hash value is not zero,
     * effectively skipping slots that are logically deleted but not yet removed
     * from memory.
     *
     * @return The count of live, valid entries in the table.
     *
     * @note
     * This value may be less than the logical size (Size()) if items were deleted.
     * Used by Compress() and other memory management routines to optimize storage.
     */
    SizeT ActualSize() const noexcept {
        const HItem_T *item = First();         // Pointer to start of item storage
        const HItem_T *end  = (item + Size()); // Pointer to end of logical array
        SizeT          size = 0;               // Live item counter

        while (item < end) {
            if (item->Hash != 0) { // Only count active entries
                ++size;
            }
            ++item;
        }

        return size;
    }

    /**
     * @brief Returns the current logical number of items in the table.
     *
     * This reflects the number of entries tracked by the container,
     * including both live and logically deleted entries.
     *
     * @return Number of items (live + logically deleted).
     */
    inline SizeT Size() const noexcept {
        return size_;
    }

    /**
     * @brief Returns the storage capacity of the table.
     *
     * Indicates the total number of slots allocated for items, which
     * may be larger than the logical size.
     *
     * @return Allocated capacity for items.
     */
    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    /**
     * @brief Returns a pointer to the first storage slot.
     *
     * Converts the base hash table pointer to a pointer to the first
     * HItem_T object in the underlying storage array.
     *
     * @return Pointer to the start of item storage.
     */
    inline HItem_T *Storage() noexcept {
        return reinterpret_cast<HItem_T *>(getHashTable() + Capacity());
    }

    /**
     * @brief Returns a pointer to the first storage slot.
     *
     * Converts the base hash table pointer to a pointer to the first
     * HItem_T object in the underlying storage array.
     *
     * @return Const pointer to the start of item storage.
     */
    inline const HItem_T *Storage() const noexcept {
        return reinterpret_cast<const HItem_T *>(getHashTable() + Capacity());
    }

    /**
     * @brief Returns a const pointer to the first item.
     *
     * Provided for convenience; same as Storage().
     *
     * @return Pointer to the first item.
     */
    inline const HItem_T *First() const noexcept {
        return reinterpret_cast<const HItem_T *>(getHashTable() + Capacity());
    }

    /**
     * @brief Returns a pointer to the last valid item.
     *
     * Returns nullptr if the table is empty.
     *
     * @return Pointer to last item, or nullptr if empty.
     */
    inline HItem_T *Last() noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }
        return nullptr;
    }

    /**
     * @brief Returns a pointer to the last valid item.
     *
     * Returns nullptr if the table is empty.
     *
     * @return Const pointer to last item, or nullptr if empty.
     */
    inline const HItem_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (Size() - SizeT{1}));
        }
        return nullptr;
    }

    /**
     * @brief Returns a const pointer to one past the last item (the end).
     *
     * Used for iteration purposes.
     *
     * @return Pointer to one past the last item.
     */
    inline const HItem_T *End() const noexcept {
        return (First() + Size());
    }

    /**
     * @brief Returns true if the container is empty.
     *
     * @return True if no items are stored.
     */
    inline bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    /**
     * @brief Returns true if the container contains at least one item.
     *
     * @return True if at least one item is stored.
     */
    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // ===== STL-style Iterators =====

    /**
     * @brief Returns const iterator to the first item.
     */
    inline const HItem_T *begin() const noexcept {
        return First();
    }

    /**
     * @brief Returns const iterator to one past the last item.
     */
    inline const HItem_T *end() const noexcept {
        return End();
    }

    /**
     * @brief Returns mutable iterator to the first item.
     */
    inline HItem_T *begin() noexcept {
        return Storage();
    }

    /**
     * @brief Returns mutable iterator to one past the last item.
     */
    inline HItem_T *end() noexcept {
        return (Storage() + Size());
    }

  protected:
    /**
     * @brief Returns the mask value for the hash table base.
     *
     * Used for fast modulo operations (hash & base) when indexing.
     * Assumes capacity is always a power of two.
     *
     * @return The base mask for hashing (Capacity() - 1).
     */
    inline SizeT getBase() const noexcept {
        SizeT base = Capacity();
        --base; // Convert capacity to mask for (hash & base)
        return base;
    }

    /**
     * @brief Allocates storage for the hash table and item array.
     *
     * Allocates a contiguous memory block to hold both the hash table
     * (buckets) and the item storage. Sets the hash table to zero.
     *
     * @param capacity Number of items to allocate space for.
     * @return Pointer to the first item slot (after hash table).
     */
    inline HItem_T *allocate(SizeT capacity) {
        SizeT *ht = allocateOnly(capacity); // Allocates raw memory and updates capacity_

        Memory::SetToValue(ht, Capacity(), Capacity());

        // Item storage comes directly after hash table in memory.
        return reinterpret_cast<HItem_T *>(ht + Capacity());
    }

    /**
     * @brief Allocates a contiguous memory block for the hash table.
     *
     * This allocates enough space for both the hash buckets and the item array.
     * The block is laid out as [hash table | items], allowing for
     * cache-friendly and pointer-arithmetic-based access.
     *
     * @param capacity The number of item slots to allocate.
     * @return Pointer to the start of the hash table segment.
     */
    inline SizeT *allocateOnly(SizeT capacity) {
        constexpr SizeT32 size     = sizeof(SizeT);                 // Size of one hash entry
        constexpr SizeT   size_sum = SizeT{size + sizeof(HItem_T)}; // Total size per entry

        capacity = Memory::AlignToPow2(capacity); // Align to power-of-two

        setCapacity(capacity); // Record new capacity

        // Allocate a single block for [hash table][items]
        SizeT *ht = reinterpret_cast<SizeT *>(Memory::Allocate<char>(size_sum * capacity));

        setHashTable(ht); // Set hash table pointer

        return ht; // Return pointer to start of hash table
    }

    /**
     * @brief Set the internal hash table pointer.
     *
     * @param ptr Pointer to the start of the hash table segment.
     */
    inline void setHashTable(SizeT *ptr) noexcept {
        hashTable_ = ptr;
    }

    /**
     * @brief Get the internal hash table pointer.
     *
     * @return Pointer to the start of the hash table segment.
     */
    inline SizeT *getHashTable() noexcept {
        return hashTable_;
    }

    /**
     * @brief Get const pointer to the internal hash table.
     *
     * @return Pointer to the start of the hash table segment.
     */
    inline const SizeT *getHashTable() const noexcept {
        return hashTable_;
    }

    /**
     * @brief Nullify the internal hash table pointer.
     *
     * Sets the internal pointer to nullptr, signaling the storage is invalid or released.
     */
    inline void clearHashTable() noexcept {
        hashTable_ = nullptr;
    }

    /**
     * @brief Set the current size (number of items) in the container.
     *
     * @param new_size New size to set.
     */
    inline void setSize(const SizeT new_size) noexcept {
        size_ = new_size;
    }

    /**
     * @brief Set the current capacity (total item slots) in the container.
     *
     * @param capacity New capacity to set.
     */
    inline void setCapacity(const SizeT capacity) noexcept {
        capacity_ = capacity;
    }

    /**
     * @brief Reallocate and migrate all live items to a new storage block of the given size.
     *
     * If increasing the size, creates a new memory region, migrates all live entries (Hash != 0),
     * and resets the hash table. Deleted or empty slots are not carried over.
     * The previous storage is deallocated after migration. The hash table is rebuilt for the new layout.
     *
     * @param new_size The new capacity (number of slots) to allocate.
     */
    void resize(const SizeT new_size) {
        SizeT         *ht      = getHashTable();     // Old hash table pointer
        HItem_T       *item    = Storage();          // Pointer to old item storage
        const HItem_T *end     = (item + Size());    // End of old storage
        HItem_T       *storage = allocate(new_size); // Allocate new storage+table

        setSize(0); // Reset size to repopulate with only live entries

        while (item < end) {
            if (item->Hash != 0) {                                // Only copy live items
                Memory::Initialize(storage, Memory::Move(*item)); // Move construct in new storage
                ++storage;
                ++size_; // Increment current count
            }
            ++item;
        }

        Memory::Deallocate(ht); // Free old hash table+storage
        generateHash();         // Rebuild hash table from migrated entries
    }

    /**
     * @brief Double the capacity of the table, growing it to the next aligned power of two.
     */
    inline void expand() {
        resize(Capacity() * SizeT{2});
    }

    /**
     * @brief Finds an item in the hash table by key and explicit hash value.
     *
     * Performs a lookup by traversing the collision chain for the given hash value.
     * If a matching key is found (matching both hash and actual key), returns a pointer to the item.
     * Otherwise, updates the index pointer to where a new item could be inserted.
     *
     * @tparam KeyType_T  The type of key used for lookup.
     * @param[out] index  Pointer to the slot in the hash table chain where the item is found or would be inserted.
     * @param[in]  key    The key to look up.
     * @param[in]  hash   The precomputed hash value for this key.
     * @return Pointer to the found item if present; nullptr if not found.
     */
    template <typename KeyType_T>
    HItem_T *find(SizeT *&index, const KeyType_T &key, const SizeT hash) noexcept {
        SizeT *ht = getHashTable();

        // Storage area starts after hash table area.
        HItem_T *storage = reinterpret_cast<HItem_T *>(ht + Capacity());
        HItem_T *item;
        // Compute index in hash table using base mask.
        index = (ht + (hash & getBase()));

        // Traverse the collision chain for this hash bucket.
        while (*index != Capacity()) {
            item = (storage + *index);

            // Check for hash and key equality using KeyUtils.
            if (KeyUtils_T::IsEqual(hash, item->Hash, key, item->Key)) {
                return item; // Match found!
            }

            index = &(item->Next); // Follow the collision chain.
        }

        return nullptr; // Not found.
    }

    /**
     * @brief Finds a const item in the hash table by key and explicit hash value.
     *
     * Performs a const lookup by traversing the collision chain for the given hash value.
     * If a matching key is found (matching both hash and actual key), sets out_index and returns a pointer to the item.
     * Otherwise, sets out_index to Capacity() and returns nullptr.
     *
     * @tparam KeyType_T   The type of key used for lookup.
     * @param[out] out_index Output reference to the slot index where the item is found.
     * @param[in]  key      The key to look up.
     * @param[in]  hash     The precomputed hash value for this key.
     * @return Const pointer to the found item if present; nullptr if not found.
     */
    template <typename KeyType_T>
    const HItem_T *find(SizeT &out_index, const KeyType_T &key, const SizeT hash) const noexcept {
        const SizeT *ht = getHashTable();

        // Storage area starts after hash table area.
        const HItem_T *storage = reinterpret_cast<const HItem_T *>(ht + Capacity());
        const HItem_T *item;
        // Compute index in hash table using base mask.
        const SizeT *index = (ht + (hash & getBase()));

        // Traverse the collision chain for this hash bucket.
        while (*index != Capacity()) {
            item = (storage + *index);

            // Check for hash and key equality using KeyUtils.
            if (KeyUtils_T::IsEqual(hash, item->Hash, key, item->Key)) {
                out_index = *index;
                return item; // Match found!
            }

            index = &(item->Next); // Follow the collision chain.
        }

        // Not found: set out_index to Capacity() to indicate failure.
        out_index = Capacity();
        return nullptr;
    }

    /**
     * @brief Finds an item in the hash table matching a given key, using default hash computation.
     *
     * Computes the hash for the key, then performs a lookup in the hash table.
     *
     * @param[out] index Pointer to the slot in the hash table chain, updated to the location where the item is found,
     *                   or where a new item would be inserted if not found.
     * @param[in]  key   The key to look up.
     * @return Pointer to the matching item if found; nullptr otherwise.
     */
    HItem_T *find(SizeT *&index, const Key_T &key) noexcept {
        // Delegate to the main find(), using the computed hash for this key
        return find(index, key, KeyUtils_T::Hash(key));
    }

    /**
     * @brief Finds a const item in the hash table matching a given key, using default hash computation.
     *
     * Computes the hash for the key, then performs a const lookup in the hash table.
     *
     * @param[out] index Reference to the slot index in the hash table chain, updated to where the item is found
     *                   or where it would be inserted if not found.
     * @param[in]  key   The key to look up.
     * @return Const pointer to the matching item if found; nullptr otherwise.
     */
    const HItem_T *find(SizeT &index, const Key_T &key) const noexcept {
        // Delegate to the main find(), using the computed hash for this key
        return find(index, key, KeyUtils_T::Hash(key));
    }

    /**
     * @brief Finds an item in the table by an item reference (using its stored key and hash).
     *
     * Useful when you already have an item and wish to find its slot in the table,
     * leveraging the item's existing key and hash.
     *
     * @param[out] index Pointer to the slot in the hash table chain.
     * @param[in]  item  The item to search for (provides both key and hash).
     * @return Pointer to the matching item if found; nullptr otherwise.
     */
    inline HItem_T *find(SizeT *&index, const HItem_T &item) noexcept {
        // Lookup using the item's key and its precomputed hash value
        return find(index, item.Key, item.Hash);
    }

    /**
     * @brief Finds a const item in the table by an item reference (using its stored key and hash).
     *
     * Useful when you already have an item and wish to find its slot in the table,
     * leveraging the item's existing key and hash.
     *
     * @param[out] index Reference to the slot in the hash table chain.
     * @param[in]  item  The item to search for (provides both key and hash).
     * @return Const pointer to the matching item if found; nullptr otherwise.
     */
    inline const HItem_T *find(SizeT &index, const HItem_T &item) const noexcept {
        // Lookup using the item's key and its precomputed hash value
        return find(index, item.Key, item.Hash);
    }

    /**
     * @brief Computes the hash for a given key and performs a lookup, also returning the hash.
     *
     * This helper is used when the caller wants both the computed hash and the item pointer.
     *
     * @param[out] index Pointer to the slot in the hash table chain, updated by the search.
     * @param[in]  key   The key to look up.
     * @param[out] hash  The computed hash value for this key.
     * @return Pointer to the matching item if found; nullptr otherwise.
     */
    HItem_T *hashAndFind(SizeT *&index, const Key_T &key, SizeT &hash) noexcept {
        hash = KeyUtils_T::Hash(key);  // Compute hash for the key
        return find(index, key, hash); // Lookup with explicit hash
    }

    /**
     * @brief Computes the hash for a given key and performs a lookup, also returning the hash.
     *
     * This helper is used when the caller wants both the computed hash and the item pointer.
     *
     * @param[out] index Reference to the slot in the hash table chain, updated by the search.
     * @param[in]  key   The key to look up.
     * @param[out] hash  The computed hash value for this key.
     * @return Const pointer to the matching item if found; nullptr otherwise.
     */
    const HItem_T *hashAndFind(SizeT &index, const Key_T &key, SizeT &hash) const noexcept {
        hash = KeyUtils_T::Hash(key);  // Compute hash for the key
        return find(index, key, hash); // Lookup with explicit hash
    }

    /**
     * @brief Inserts a fully constructed item at the specified index slot.
     *
     * This version is used when the full item is already constructed and (usually) moved in.
     *
     * @param[in,out] index Pointer to the hash table slot where the item should be linked.
     * @param[in]     item  The item to insert (rvalue ref, moved).
     * @return Pointer to the newly inserted item in storage.
     */
    HItem_T *insert(SizeT *index, HItem_T &&item) noexcept {
        HItem_T *item_ptr = (Storage() + Size()); // Next available storage slot
        *index            = Size();               // Store 0-based index in hash table slot
        ++size_;                                  // Increment count after using it
        item.Next = Capacity();                   // End of collision chain

        Memory::Initialize(item_ptr, Memory::Move(item)); // Move or copy-construct in place

        return item_ptr;
    }

    /**
     * @brief Constructs and inserts a new item using the given key and hash.
     *
     * @param[in,out] index Hash table slot for this key.
     * @param[in]     key   The key to insert (copied).
     * @param[in]     hash  The precomputed hash for the key.
     * @return Pointer to the newly inserted item.
     */
    HItem_T *insert(SizeT *index, const Key_T &key, const SizeT hash) noexcept {
        HItem_T item;
        item.Hash = hash;
        item.Key  = key; // Copy key

        return insert(index, Memory::Move(item));
    }

    /**
     * @brief Constructs and inserts a new item using the given key (moved) and hash.
     *
     * @param[in,out] index Hash table slot for this key.
     * @param[in]     key   The key to insert (moved).
     * @param[in]     hash  The precomputed hash for the key.
     * @return Pointer to the newly inserted item.
     */
    HItem_T *insert(SizeT *index, Key_T &&key, const SizeT hash) noexcept {
        HItem_T item;
        item.Hash = hash;
        item.Key  = Memory::Move(key); // Move key

        return insert(index, Memory::Move(item));
    }

    /**
     * @brief Inserts a copy of the given item at the specified index slot.
     *
     * @param[in,out] index Hash table slot for this item.
     * @param[in]     item  The item to copy and insert.
     * @return Pointer to the newly inserted item.
     */
    inline HItem_T *insert(SizeT *index, const HItem_T &item) noexcept {
        return insert(index, HItem_T{item});
    }

    /**
     * @brief Finds or inserts an item by key (copy).
     *
     * Looks up the given key in the hash table. If the key is not found, inserts a new default-initialized item
     * at the appropriate slot and returns its pointer. Expands the table if full.
     *
     * @param[in] key The key to find or insert (copied).
     * @return Pointer to the existing or newly inserted item.
     */
    inline HItem_T *tryInsert(const Key_T &key) noexcept {
        if (Size() == Capacity()) {
            expand(); // Grow the table if needed
        }

        SizeT    hash;
        SizeT   *index;
        HItem_T *item = hashAndFind(index, key, hash);

        if (item == nullptr) {
            item = insert(index, key, hash); // Insert new item if not found
            item->InitValue();               // Ensure value is properly initialized
        }

        return item;
    }

    /**
     * @brief Finds or inserts an item by key (move).
     *
     * Looks up the given key in the hash table. If the key is not found, inserts a new default-initialized item,
     * moving in the provided key, and returns its pointer. Expands the table if full.
     *
     * @param[in] key The key to find or insert (moved).
     * @return Pointer to the existing or newly inserted item.
     */
    inline HItem_T *tryInsert(Key_T &&key) noexcept {
        if (Size() == Capacity()) {
            expand(); // Grow the table if needed
        }

        SizeT    hash;
        SizeT   *index;
        HItem_T *item = hashAndFind(index, key, hash);

        if (item == nullptr) {
            item = insert(index, Memory::Move(key), hash); // Insert new item, moving key
            item->InitValue();                             // Ensure value is properly initialized
        }

        return item;
    }

    /**
     * @brief Removes the given item from the hash table by direct pointer and index slot.
     *
     * Updates the hash table slot to bypass the removed item, resets its links and hash,
     * and clears its value. No effect if the item is null.
     *
     * @param[in,out] index Pointer to the hash slot that points to this item.
     * @param[in,out] item  Pointer to the item to remove.
     */
    void remove(SizeT *index, HItem_T *item) noexcept {
        if (item != nullptr) {
            *index     = item->Next; // Unlink from hash chain
            item->Hash = 0;          // Mark as deleted
            item->Next = Capacity(); // Clear item's hash chain

            item->Clear(); // Zero/reset user key/value fields
        }
    }

    /**
     * @brief Removes an item from the table by direct pointer.
     *
     * Finds the hash slot for this item and removes it using the slot-aware remover.
     * No effect if the table is empty.
     *
     * @param[in,out] item Pointer to the item to remove.
     */
    void remove(HItem_T *item) noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            find(index, *item); // Locate the slot for this item

            remove(index, item); // Remove via index-aware logic
        }
    }

    /**
     * @brief Removes the item matching the given key from the table, if present.
     *
     * Locates the item by key and removes it if found. No effect if key is not found or table is empty.
     *
     * @param[in] key The key of the item to remove.
     */
    void remove(const Key_T &key) noexcept {
        if (IsNotEmpty()) {
            SizeT   *index;
            HItem_T *item = find(index, key); // Find by key

            remove(index, item); // Remove by slot/item pointer
        }
    }

    /**
     * @brief Prepares the hash table for renaming a key, rewiring chains and returning the item if possible.
     *
     * Detaches the item associated with 'from' and prepares for reassignment to 'to' if
     * and only if 'from' exists and 'to' does not.
     *
     * @param from The existing key to be renamed.
     * @param to   The destination key that must not already exist.
     * @return Pointer to the item if rename is possible and links are updated, nullptr otherwise.
     */
    inline HItem_T *prepareRename(const Key_T &from, const Key_T &to) {
        if (IsNotEmpty()) {
            SizeT *left_index;  // Pointer to the hash table slot for 'from'
            SizeT *right_index; // Pointer to the hash table slot for 'to'

            find(left_index, from);

            if (*left_index != Capacity()) { // Ensure 'from' exists
                SizeT to_hash;
                hashAndFind(right_index, to, to_hash); // Find/compute slot for 'to'

                if (*right_index == Capacity()) { // Ensure 'to' does not exist
                    SizeT index = *left_index;

                    HItem_T *item = (Storage() + index);

                    // Re-link the hash table chains for the moved key
                    *right_index = *left_index;
                    item->Hash   = to_hash;
                    *left_index  = item->Next;
                    item->Next   = Capacity();

                    return item;
                }
            }
        }

        return nullptr;
    }

    /**
     * @brief Deep copies all non-empty items from another hash table, rebuilding the hash table structure.
     *
     * Allocates a new storage array, copies each valid item, then re-generates the hash links
     * to match the new memory layout. Used for copy construction and copy assignment.
     *
     * @param[in] src The source hash table to copy from.
     */
    void copyTable(const HashTable &src) {
        if (src.IsNotEmpty()) {
            const HItem_T *src_item = src.First();
            const HItem_T *src_end  = (src_item + src.Size());
            HItem_T       *storage  = allocate(src.Size());
            SizeT          index{0};

            do {
                if (src_item->Hash != 0) {                  // Only copy valid entries
                    Memory::Initialize(storage, *src_item); // Copy-construct in new storage
                    ++storage;
                    ++index;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(index); // Set the new item count
            generateHash(); // Rebuild hash links for new memory block
        }
    }

    /**
     * @brief Deep copies all items and hash table links from another hash table, preserving the hash state.
     *
     * This is used in the copy constructor to exactly duplicate both the entries and
     * the current hash table layout (for strong exception safety).
     *
     * @param[in] src The source hash table to clone.
     */
    void copyTableWithHash(const HashTable &src) {
        if (src.IsNotEmpty()) {
            setCapacity(src.Capacity());

            const HItem_T *src_item = src.First();
            const HItem_T *src_end  = (src_item + src.Size());
            const SizeT   *src_ht   = src.getHashTable();
            SizeT         *ht       = allocateOnly(Capacity());
            HItem_T       *storage  = reinterpret_cast<HItem_T *>(ht + Capacity());
            SizeT          index{0};

            Memory::CopyTo(ht, src_ht, Capacity()); // Duplicate hash table slot array

            do {
                if (src_item->Hash != 0) {
                    Memory::Initialize(storage, *src_item);
                    ++storage;
                    ++index;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(index);
        }
    }

    /**
     * @brief Rebuilds the hash table slot links for all valid items.
     *
     * This method traverses the storage array and updates the hash table's
     * slot pointers (`ht`) and each item's `Next` pointer to correctly form
     * the hash chains. Used after resizing, sorting, or when items are
     * re-allocated/copied so that the slot chains match the new memory layout.
     *
     * Algorithm:
     *  - For each valid item (with non-zero hash), resets its `Next` pointer to Capacity().
     *  - Finds the correct hash slot for the item's hash.
     *  - If the slot is occupied, follows the `Next` chain to the end.
     *  - Adds the new item to the chain, updating slot indices accordingly.
     */
    void generateHash() noexcept {
        SizeT         *ht   = getHashTable();  // Pointer to hash table slot array
        HItem_T       *src  = Storage();       // Pointer to start of storage array
        HItem_T       *item = src;             // Item pointer for iteration
        const HItem_T *end  = (item + Size()); // One-past-the-end
        SizeT         *index;
        SizeT          i    = SizeT{0}; // 0-based index for hash chains
        const SizeT    base = getBase();

        while (item < end) {
            item->Next = Capacity();                 // Reset chain
            index      = (ht + (item->Hash & base)); // Find base slot

            // Walk to the end of the chain, if needed
            while (*index != Capacity()) {
                index = &((src + *index)->Next);
            }

            *index = i; // Point slot (or chain) to this item
            ++i;
            ++item;
        }
    }

    SizeT *hashTable_{nullptr};
    SizeT  size_{0};
    SizeT  capacity_{0};
};

} // namespace Qentem

#endif
