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

#include "Reserver.hpp"

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
     * @brief Index of this item in the array (Storage), or Capacity() as sentinel.
     *
     * This field is also used by bucket roots as the head of the collision chain.
     * When acting as a bucket root, `Storage[hash & mask].Position` stores the
     * array index of the first item in the chain (or Capacity() if empty).
     *
     * When acting as an item, `Position` is simply its stable index in Storage,
     * typically equal to its insertion order.
     */
    SizeT Position;

    /**
     * @brief Link to the next item in the collision chain.
     *
     * Stores the index of the next item in Storage[], or Capacity() as end-of-chain.
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
template <typename Key_T, typename KeyUtils_T, typename HItem_T>
struct HashTable {
    /**
     * @brief Default constructor. Initializes an empty hash table.
     */
    QENTEM_INLINE HashTable() = default;

    /**
     * @brief Constructs a hash table with the given initial capacity.
     *
     * Reserves internal storage sufficient to hold at least @p capacity items,
     * enabling early insertion without immediate resizing.
     *
     * @param capacity Initial capacity (number of items to prepare for).
     */
    QENTEM_INLINE explicit HashTable(const SizeT capacity) {
        if (capacity != 0) {
            reserve(capacity);
        }
    }

    /**
     * @brief Move constructor. Transfers storage from another hash table.
     *
     * All contents are moved; the source is left empty.
     *
     * @param src HashTable to move from.
     */
    QENTEM_INLINE HashTable(HashTable &&src) noexcept
        : storage_{src.storage_}, size_{src.size_}, capacity_{src.capacity_} {
        src.setSize(0);
        src.setCapacity(0);
        src.clearStorage();
    }

    /**
     * @brief Copy constructor. Deep-copies all live entries and internal state.
     *
     * Reserve fresh storage and copies the [0..Size) region of @p src using item-level construction.
     * Hash structure is preserved as-is; no rehashing occurs.
     *
     * @param src HashTable to copy from.
     */
    QENTEM_INLINE HashTable(const HashTable &src) {
        if (src.IsNotEmpty()) {
            const HItem_T *src_item = src.First();
            const HItem_T *src_end  = (src_item + src.Size());
            HItem_T       *storage  = reserveOnly(src.Capacity());

            setSize(src.Size());

            do {
                storage->Construct(*src_item);
                ++storage;
                ++src_item;
            } while (src_item < src_end);
        }
    }

    /**
     * @brief Destructor. Disposes of all storage and items.
     */
    ~HashTable() {
        HItem_T *storage = Storage();
        MemoryUtils::Dispose(storage, (storage + Size()));
        release(storage, Capacity());
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
            HItem_T    *storage      = Storage();
            const SizeT old_capacity = Capacity();
            const SizeT size         = Size();

            // Take ownership of src's memory and bookkeeping.
            setStorage(src.Storage());
            setSize(src.Size());
            setCapacity(src.Capacity());

            // Reset src to an empty state so its destructor is safe.
            src.clearStorage();
            src.setSize(0);
            src.setCapacity(0);

            // Dispose of the old memory (after transfer, in case of derived/child arrays).
            MemoryUtils::Dispose(storage, (storage + size));
            release(storage, old_capacity);
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
            HItem_T    *storage      = Storage();
            const SizeT old_capacity = Capacity();
            const SizeT size         = Size();

            // Clear current state before copying new data.
            clearStorage();
            setSize(0);
            setCapacity(0);

            // Deep copy all items and hash table layout from src.
            copyTable(src);

            // Dispose of the old memory (after transfer, in case of derived/child arrays).
            MemoryUtils::Dispose(storage, (storage + size));
            release(storage, old_capacity);
        }

        return *this;
    }

    /**
     * @brief Move-adds all items from another hash table (@p src) into this table.
     *
     * Transfers ownership of each valid entry from @p src into this instance, replacing
     * any existing value with the same key. Once complete, @p src is cleared and its
     * internal buffer is released for reuse.
     *
     * @param src HashTable to move from (will be left empty and inert).
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
                    insert(index, QUtility::Move(*src_item));
                } else {
                    storage_item->MoveDoublecat(*src_item);
                }
            }
            ++src_item;
        }

        // Release src's memory and reset its bookkeeping.
        release(src.Storage(), src.Capacity());
        src.clearStorage();
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
    QENTEM_INLINE void Insert(const Key_T &key) {
        tryInsert(key); // Insert or do nothing if key already exists.
    }

    /**
     * @brief Inserts a key into the hash table using move semantics.
     *
     * If the key is already present, this operation has no effect.
     *
     * @param key The key to insert (moved).
     */
    QENTEM_INLINE void Insert(Key_T &&key) {
        tryInsert(QUtility::Move(key)); // Move-insert or do nothing if key exists.
    }

    /**
     * @brief Checks whether the table contains a given key.
     *
     * @param key The key to look up.
     * @return true if the key is found; false otherwise.
     */
    QENTEM_INLINE bool Has(const Key_T &key) const noexcept {
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
    QENTEM_INLINE const Key_T *GetKeyAt(const SizeT index) const noexcept {
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
    QENTEM_INLINE HItem_T *GetItem(const Key_T &key) noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            return find(index, key);
        }

        return nullptr; // Not found or table is empty.
    }

    QENTEM_INLINE const HItem_T *GetItem(const Key_T &key) const noexcept {
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
    QENTEM_INLINE HItem_T *GetItemAt(const SizeT index) noexcept {
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
    QENTEM_INLINE const HItem_T *GetItemAt(const SizeT index) const noexcept {
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
    QENTEM_INLINE bool GetIndex(SizeT &index, const Key_T &key) const noexcept {
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
    QENTEM_INLINE void Remove(const Key_T &key) noexcept {
        remove(key); // Dispatch to private remove logic.
    }

    /**
     * @brief Removes the item at the specified array-style index.
     *
     * If the index is valid and the slot is not already empty, removes the corresponding entry.
     *
     * @param index The array-style index of the item to remove.
     */
    QENTEM_INLINE void RemoveAt(const SizeT index) noexcept {
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
    bool Rename(const Key_T &from, Key_T &&to) const noexcept {
        HItem_T *item = prepareRename(from, to); // Unlink and ready for new key
        if (item != nullptr) {
            item->Key = QUtility::Move(to); // Move-assign new key

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
    bool Rename(const Key_T &from, const Key_T &to) {
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
    QENTEM_INLINE void Expect(const SizeT count) {
        const SizeT new_size = (count + Size()); // Total required size after 'count' more elements

        if (new_size > Capacity()) {
            resize(new_size); // Expand underlying storage if needed
        }
    }

    /**
     * @brief Adjusts the table's capacity to match exactly @p new_size elements.
     *
     * If @p new_size is less than the current number of stored items, excess entries
     * are discarded. If greater, a new buffer is reserved with sufficient space to
     * accommodate the requested size. When set to zero, the table is fully reset
     * and its memory is released.
     *
     * @param new_size The target number of items the table should support.
     */
    void Resize(const SizeT new_size) {
        if (new_size != 0) {
            if (Size() > new_size) {
                // Shrink: Dispose of elements outside new bounds
                HItem_T *storage = Storage();
                MemoryUtils::Dispose((storage + new_size), (storage + Size()));
                setSize(new_size); // Adjust logical size
            }

            resize(new_size); // Internal grow/shrink logic
        } else {
            Reset();
        }
    }

    /**
     * @brief Ensures the hash table has at least the requested capacity and clears existing contents.
     *
     * Resets the table and reserves internal storage sufficient to hold at least @p size items.
     * If @p size is zero, the table is cleared and no new storage is acquired.
     *
     * @param size The number of elements to reserve space for.
     */
    QENTEM_INLINE void Reserve(SizeT size) {
        Reset(); // Release any current storage and reset size/capacity

        if (size != 0) {
            reserve(size); // Reserves storage for at least 'size' items
        }
    }

    /**
     * @brief Removes all entries from the table, keeping current capacity.
     *
     * Sets the hash table to empty (size zero), disposes all stored items,
     * and resets the hash buckets to zero, but does not released the buffer.
     *
     * Safe to call on an already-empty table.
     */
    QENTEM_INLINE void Clear() noexcept {
        if (IsNotEmpty()) {
            HItem_T       *item = Storage();
            const HItem_T *end  = (Storage() + Size());

            while (item < end) {
                item->Position = Capacity();
                item->Next     = Capacity();
                item->Clear();
                ++item;
            }

            setSize(0); // Logical size is now zero
        }
    }

    /**
     * @brief Completely releases all storage and resets the table to an uninitialized state.
     *
     * Removes all entries, frees internal storage, and sets capacity and size to zero.
     * After this call, the table is as if default-constructed.
     *
     * Safe to call repeatedly.
     */
    QENTEM_INLINE void Reset() noexcept {
        HItem_T *storage = Storage();

        MemoryUtils::Dispose(storage, (storage + Size())); // Dispose all elements
        release(Storage(), Capacity());                    // Free the hash table & storage block

        clearStorage(); // Set pointer to nullptr
        setSize(0);     // Size is now zero
        setCapacity(0); // Capacity is now zero
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
    QENTEM_INLINE void Sort(const bool ascend = true) noexcept {
        HItem_T *storage = Storage();

        if (ascend) {
            QUtility::Sort<true>(storage, SizeT{0}, Size()); // Sort in ascending order
        } else {
            QUtility::Sort<false>(storage, SizeT{0}, Size()); // Sort in descending order
        }

        // Reset hash table mapping and rebuild
        resetLinks(storage, (storage + Capacity()), Capacity());

        generateHash();
    }

    /**
     * @brief Shrinks the internal storage to match the actual number of non-deleted items.
     *
     * If the actual item count is less than the logical size, resizes the table to remove
     * excess (deleted) items and reclaim memory. If all items have been deleted, the
     * table is fully reset and released.
     *
     * @note
     * This operation can be used after bulk removals to optimize memory use.
     */
    QENTEM_INLINE void Compress() {
        const SizeT size = ActualSize(); // Count of truly valid items

        if (size != 0) {
            if (size < Size()) {
                resize(size); // Shrink to fit
            }
            return;
        }

        Reset(); // If nothing remains, free all memory
    }

    /**
     * @brief Reorders hash table items so that all valid items occupy contiguous items at the start.
     *
     * Reorder() performs an in-place compaction of all non-deleted items (`Hash != 0`), shifting them
     * forward so that indices `[0, N)` form a dense block of valid entries and all deleted items are
     * pushed beyond the logical end. The logical size is updated to match the number of valid items.
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
                    storage[size] = QUtility::Move(*item); // Move live item to compacted position
                    item->Hash    = 0;                     // Mark old slot as dead/tombstone
                }

                ++size; // Increment count of live items and next slot for compaction
            }

            ++index; // Advance to next storage slot
        }

        setSize(size); // Update logical size to match number of live entries

        resetLinks(storage, (storage + Capacity()), Capacity());

        // Rebuild hash table mapping to match new item layout
        generateHash();
    }

    /**
     * @brief Returns the number of active (non-deleted) items in the hash table.
     *
     * Iterates over the storage array and counts items whose hash value is not zero,
     * effectively skipping items that are logically deleted but not yet removed
     * from memory.
     *
     * @return The count of live, valid entries in the table.
     *
     * @note
     * This value may be less than the logical size (Size()) if items were deleted.
     * Used by Compress() and other memory management routines to optimize storage.
     */
    QENTEM_INLINE SizeT ActualSize() const noexcept {
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
    QENTEM_INLINE SizeT Size() const noexcept {
        return size_;
    }

    /**
     * @brief Returns the storage capacity of the table.
     *
     * Indicates the total reserved number for items, which
     * may exceed the current logical size.
     *
     * @return Reserved capacity for items.
     */
    QENTEM_INLINE SizeT Capacity() const noexcept {
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
    QENTEM_INLINE HItem_T *Storage() noexcept {
        return storage_;
    }

    /**
     * @brief Returns a pointer to the first storage slot.
     *
     * Converts the base hash table pointer to a pointer to the first
     * HItem_T object in the underlying storage array.
     *
     * @return Const pointer to the start of item storage.
     */
    QENTEM_INLINE const HItem_T *Storage() const noexcept {
        return storage_;
    }

    /**
     * @brief Returns a const pointer to the first item.
     *
     * Provided for convenience; same as Storage().
     *
     * @return Pointer to the first item.
     */
    QENTEM_INLINE const HItem_T *First() const noexcept {
        return Storage();
    }

    /**
     * @brief Returns a pointer to the last valid item.
     *
     * Returns nullptr if the table is empty.
     *
     * @return Pointer to last item, or nullptr if empty.
     */
    QENTEM_INLINE HItem_T *Last() noexcept {
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
    QENTEM_INLINE const HItem_T *Last() const noexcept {
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
    QENTEM_INLINE const HItem_T *End() const noexcept {
        return (First() + Size());
    }

    /**
     * @brief Returns true if the container is empty.
     *
     * @return True if no items are stored.
     */
    QENTEM_INLINE bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    /**
     * @brief Returns true if the container contains at least one item.
     *
     * @return True if at least one item is stored.
     */
    QENTEM_INLINE bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // ===== STL-style Iterators =====

    /**
     * @brief Returns const iterator to the first item.
     */
    QENTEM_INLINE const HItem_T *begin() const noexcept {
        return First();
    }

    /**
     * @brief Returns const iterator to one past the last item.
     */
    QENTEM_INLINE const HItem_T *end() const noexcept {
        return End();
    }

    /**
     * @brief Returns mutable iterator to the first item.
     */
    QENTEM_INLINE HItem_T *begin() noexcept {
        return Storage();
    }

    /**
     * @brief Returns mutable iterator to one past the last item.
     */
    QENTEM_INLINE HItem_T *end() noexcept {
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
    QENTEM_INLINE SizeT getBase() const noexcept {
        SizeT base = Capacity();
        --base; // Convert capacity to mask for (hash & base)
        return base;
    }

    /**
     * @brief Reset all hash roots and collision links to the sentinel value.
     *
     * Postion is used as the root slot for each hash bucket,
     * Next is used for chaining. 'value' is always Capacity() here,
     * which acts as the invalid index marker.
     */
    QENTEM_INLINE static void resetLinks(HItem_T *item, const HItem_T *end, SizeT value) {
        while (item < end) {
            item->Position = value;
            item->Next     = value;
            ++item;
        }
    }

    /**
     * @brief Reserves storage for the hash table and item array.
     *
     * Requests a contiguous memory block large enough to contain both
     * the hash table and the item storage.
     *
     * @param capacity Number of items to reserve space for.
     * @return Pointer to the first item slot (immediately after hash table).
     */
    QENTEM_INLINE HItem_T *reserve(SizeT capacity) {
        HItem_T *storage = reserveOnly(capacity); // reserves raw memory and updates capacity_

        resetLinks(storage, (storage + Capacity()), Capacity());

        // Item storage comes directly after hash table in memory.
        return storage;
    }

    /**
     * @brief Reserves a contiguous memory block for the hash table.
     *
     * This reserves enough space for both the hash buckets and the item array.
     * The block is arranged as [hash table | items], enabling
     * cache-friendly access and efficient pointer arithmetic.
     *
     * @param capacity The number of items to reserve.
     * @return Pointer to the start of the hash table segment.
     */
    HItem_T *reserveOnly(SizeT capacity) {
        capacity = MemoryUtils::AlignToPow2(capacity); // Align to power-of-two

        setCapacity(capacity); // Record new capacity

        // Reserve a single block for [hash table][items]
        HItem_T *storage = Reserver::Reserve<HItem_T>(capacity);

        setStorage(storage); // Set hash table pointer

        return storage; // Return pointer to start of hash table
    }

    /**
     * @brief Set the internal hash table pointer.
     *
     * @param ptr Pointer to the start of the hash table segment.
     */
    QENTEM_INLINE void setStorage(HItem_T *storage) noexcept {
        storage_ = storage;
    }

    /**
     * @brief Nullify the internal hash table pointer.
     *
     * Sets the internal pointer to nullptr, signaling the storage is invalid or released.
     */
    QENTEM_INLINE void clearStorage() noexcept {
        storage_ = nullptr;
    }

    /**
     * @brief Set the current size (number of items) in the container.
     *
     * @param new_size New size to set.
     */
    QENTEM_INLINE void setSize(const SizeT new_size) noexcept {
        size_ = new_size;
    }

    /**
     * @brief Set the current capacity (total items) in the container.
     *
     * @param capacity New capacity to set.
     */
    QENTEM_INLINE void setCapacity(const SizeT capacity) noexcept {
        capacity_ = capacity;
    }

    /**
     * @brief Reserves a new storage block and migrates all live items into it.
     *
     * When increasing size, a fresh memory region is reserved, all live entries (where Hash ≠ 0)
     * are migrated, and the hash table is rebuilt accordingly. Deleted or empty keys
     * are excluded from the transfer.
     *
     * The previous storage is released after migration.
     *
     * @param new_size The new capacity (number of items) to reserve.
     */
    void resize(const SizeT new_size) {
        HItem_T       *old_storage  = Storage();
        HItem_T       *item         = old_storage; // Pointer to old item storage
        const SizeT    old_capacity = Capacity();
        const HItem_T *end          = (old_storage + Size()); // End of old storage
        HItem_T       *new_item     = reserve(new_size);      // Reserve a new storage+table

        setSize(0); // Reset size to repopulate with only live entries

        while (item < end) {
            if (item->Hash != 0) { // Only copy live items
                new_item->Construct(QUtility::Move(*item));
                ++new_item;
                ++size_; // Increment current count
            }

            ++item;
        }

        release(old_storage, old_capacity); // Free old hash table+storage
        generateHash();                     // Rebuild hash table from migrated entries
    }

    /**
     * @brief Double the capacity of the table, growing it to the next aligned power of two.
     */
    QENTEM_INLINE void expand() {
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
        HItem_T *storage = Storage();
        HItem_T *item;

        // Compute index in hash table using base mask.
        index = &((storage + (hash & getBase()))->Position);

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
        const HItem_T *storage = Storage();
        const HItem_T *item;
        // Compute index in hash table using base mask.
        const SizeT *index = &((storage + (hash & getBase()))->Position);

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
    QENTEM_INLINE HItem_T *find(SizeT *&index, const Key_T &key) noexcept {
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
    QENTEM_INLINE const HItem_T *find(SizeT &index, const Key_T &key) const noexcept {
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
    QENTEM_INLINE HItem_T *find(SizeT *&index, const HItem_T &item) noexcept {
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
    QENTEM_INLINE const HItem_T *find(SizeT &index, const HItem_T &item) const noexcept {
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
    QENTEM_INLINE HItem_T *hashAndFind(SizeT *&index, const Key_T &key, SizeT &hash) noexcept {
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
    QENTEM_INLINE const HItem_T *hashAndFind(SizeT &index, const Key_T &key, SizeT &hash) const noexcept {
        hash = KeyUtils_T::Hash(key);  // Compute hash for the key
        return find(index, key, hash); // Lookup with explicit hash
    }

    /**
     * @brief Inserts a fully constructed item at the specified index slot.
     *
     * This version is used when the full item is already constructed and (usually) moved in.
     * Constructs only key/value/hash; linkage fields are set externally.
     *
     * @param[in,out] index Pointer to the hash table slot where the item should be linked.
     * @param[in]     item  The item to insert (rvalue ref, moved).
     * @return Pointer to the newly inserted item in storage.
     */
    HItem_T *insert(SizeT *index, HItem_T &&item) noexcept {
        HItem_T *item_ptr = (Storage() + Size()); // Next available storage slot
        item_ptr->Construct(QUtility::Move(item));
        *index = Size(); // Store 0-based index in hash table slot
        ++size_;         // Increment count after using it

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
    QENTEM_INLINE HItem_T *insert(SizeT *index, const Key_T &key, const SizeT hash) noexcept {
        HItem_T item;
        item.Hash = hash;
        item.Key  = key; // Copy key

        return insert(index, QUtility::Move(item));
    }

    /**
     * @brief Constructs and inserts a new item using the given key (moved) and hash.
     *
     * @param[in,out] index Hash table slot for this key.
     * @param[in]     key   The key to insert (moved).
     * @param[in]     hash  The precomputed hash for the key.
     * @return Pointer to the newly inserted item.
     */
    QENTEM_INLINE HItem_T *insert(SizeT *index, Key_T &&key, const SizeT hash) noexcept {
        HItem_T item;
        item.Hash = hash;
        item.Key  = QUtility::Move(key); // Move key

        return insert(index, QUtility::Move(item));
    }

    /**
     * @brief Inserts a copy of the given item at the specified index slot.
     *
     * @param[in,out] index Hash table slot for this item.
     * @param[in]     item  The item to copy and insert.
     * @return Pointer to the newly inserted item.
     */
    QENTEM_INLINE HItem_T *insert(SizeT *index, const HItem_T &item) noexcept {
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
    HItem_T *tryInsert(const Key_T &key) noexcept {
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
    HItem_T *tryInsert(Key_T &&key) noexcept {
        if (Size() == Capacity()) {
            expand(); // Grow the table if needed
        }

        SizeT    hash;
        SizeT   *index;
        HItem_T *item = hashAndFind(index, key, hash);

        if (item == nullptr) {
            item = insert(index, QUtility::Move(key), hash); // Insert new item, moving key
            item->InitValue();                               // Ensure value is properly initialized
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
    QENTEM_INLINE void remove(SizeT *index, HItem_T *item) noexcept {
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
    QENTEM_INLINE void remove(HItem_T *item) noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            find(index, *item);  // Locate the slot for this item
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
    QENTEM_INLINE void remove(const Key_T &key) noexcept {
        if (IsNotEmpty()) {
            SizeT   *index;
            HItem_T *item = find(index, key); // Find by key
            remove(index, item);              // Remove by slot/item pointer
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
    HItem_T *prepareRename(const Key_T &from, const Key_T &to) {
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
     * @brief Deep-copies all valid items from another hash table, rebuilding internal links.
     *
     * Allocates new storage with capacity equal to src.Size(), and selectively copies
     * valid entries (where Hash ≠ 0) using Construct(). Linkage fields are re-generated
     * after all items are placed. Used for assignment and bulk copy.
     *
     * @param[in] src The source hash table to copy from.
     */
    void copyTable(const HashTable &src) {
        if (src.IsNotEmpty()) {
            const HItem_T *src_item = src.First();
            const HItem_T *src_end  = (src_item + src.Size());
            HItem_T       *new_item = reserve(src.Size());
            SizeT          index{0};

            do {
                if (src_item->Hash != 0) { // Only copy valid entries
                    new_item->Construct(*src_item);
                    ++new_item;
                    ++index;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(index); // Set the new item count
            generateHash(); // Rebuild hash links for new memory block
        }
    }

    /**
     * @brief Rebuilds hash slot chains for all active entries in storage.
     *
     * Traverses the item array and reconstructs the hash table links by:
     *   - Resetting each item's `Next` to Capacity(), marking end of chain.
     *   - Computing the correct slot for each hash using base masking.
     *   - Following existing slot chains (via `Next`) until an empty slot is found.
     *   - Linking the item into its corresponding slot chain.
     *
     * Used after operations that relocate or reorder items (e.g., resizing, sorting),
     * to ensure hash collisions are resolved against the current layout.
     *
     * This preserves O(1) average-time lookup while avoiding intrusive metadata.
     */
    void generateHash() noexcept {
        HItem_T       *src  = Storage();       // Pointer to start of storage array
        HItem_T       *item = src;             // Item pointer for iteration
        const HItem_T *end  = (item + Size()); // One-past-the-end
        SizeT          i    = SizeT{0};        // 0-based index for hash chains
        const SizeT    base = getBase();
        SizeT         *index;

        while (item < end) {
            index = &((src + (item->Hash & base))->Position); // Find base slot

            // Walk to the end of the chain, if needed
            while (*index != Capacity()) {
                index = &((src + *index)->Next);
            }

            *index = i; // Point slot (or chain) to this item
            ++i;
            ++item;
        }
    }

    QENTEM_INLINE static void release(HItem_T *storage, SizeT capacity) {
        Reserver::Release(storage, capacity);
    }

    HItem_T *storage_{nullptr};
    SizeT    size_{0};
    SizeT    capacity_{0};
};

} // namespace Qentem

#endif
