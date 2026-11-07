/**
 *  An interface for the ADT Map. (Distinct search keys.)
 * @file  MapInterface.h 
 *
 *     #### TAKEN FROM THE TEXTBOOK ####
 */

#ifndef MAP_INTERFACE_
#define MAP_INTERFACE_

//#include "NotFoundException.h" //implement later if we use the map

template<class KeyType, class ValueType>
class MapInterface {
  public:

   /** 
    * Sees whether this map is empty.
    *@return True if the map is empty, otherwise returns false. 
    */
   virtual bool isEmpty() const = 0;

   /** 
    * Gets the number of items in this map.
    * @return The number of items in the map. 
    */
   virtual int getNumberOfItems() const = 0;

   /** Inserts an item into this map according to the item's search key.
    *@pre  The search key of the new item differs from all search
    *      keys presently in the map.
    *@post If the insertion is successful, the new item is in its
    *      proper position within the map.
    *@param searchKey  The search key associated with the item to be inserted.
    *@param someValue  The value associated with searchKey that together form
    *                  the item to add to the map.
    *@return  True if the item was successfully added, or false if not. 
    */
   virtual bool add(const KeyType& searchKey, const ValueType& someValue) = 0;

   /** Removes an item with the given search key from this map.
    *@post  If the item whose search key equals searchKey existed in the map,
    *       the item was removed.
    *@param targetKey  The search key of the item to be removed.
    *@return  True if the item was successfully removed, or false if not. 
    */
   virtual bool remove(const KeyType& targetKey) = 0;

   /** 
    * Removes all items from this map. 
    */
   virtual void clear() = 0;

   /** 
    * Retrieves an item with a given search key from this map.
    *@post  If the retrieval is successful, the item is returned.
    *@param targetKey  The search key of the item to be retrieved.
    *@return  The item associated with the search key.
    *@throw  NotFoundException if the item does not exist. 
    */
   virtual ValueType getItem(const KeyType& targetKey) const = 0;   // was ItemType

   /** 
    * Sees whether this map contains an item with a given search key.
    *@post  The map is unchanged.
    *@param targetKey  The search key of the item to be found.
    *@return  True if an item with the given search key exists in the map. 
    */
   virtual bool contains(const KeyType& targetKey) const = 0;

   /** 
    * Traverses this map and calls a given client function once for each item.
    *@post  The given function's action occurs once for each item in the
    *      map and possibly alters the item.
    *@param visit A client function. */
   virtual void traverse(void visit(ValueType&)) const = 0;         // was ItemType&

   /** 
    * Deletes this map and frees its allocated memory. 
    */
   virtual ~MapInterface() { }

}; // end MapInterface

#endif
