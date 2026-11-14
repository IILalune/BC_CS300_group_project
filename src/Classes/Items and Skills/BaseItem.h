/**
 * @file item_interface.h
 * @brief interface for all items
 */

// INCOMPLETE: WILL FINISH WITH STAT/CHARACTER IMPLEMENTATION

#ifndef ITEM_INTERFACE_H_
#define ITEM_INTERFACE_H_

#include "Classes/Items and Skills/ItemType.h"

#include <string>

//TODO: finish useItem when Character classes are finished

class Character; //forward declaration

class BaseItem {
    public:

        /**
         * can add more if needed.
         * could also put them in item subclasses if design allows. ex: only 
         *   weapons affect attack.
         */
        BaseItem   (int itemId, 
                    const std::string& name,
                    int physicalAttack = 0,
                    int magicAttack = 0,
                    int physicalDefense = 0,
                    int magicDefense = 0,
                    int heal = 0);

        /**
         * destructor
         */
        virtual ~BaseItem() = default;

        /**
         * @return item id
         */
        int getItemId() const;

        /**
         * @return name of item
         */
        const std::string& getItemName() const;

        /**
         * @return physical attack
         */
        int getPhysicalAttack() const;

        /**
         * @return magic attack
         */
        int getMagicAttack() const;

        /**
         * @return physical Defense
         */
        int getPhysicalDefense() const;        

        /**
         * @return magic Defense
         */
        int getMagicDefense() const;

        /**
         * @return heal amount
         */
        int getHeal() const;


    //These will change dependent on ItemType
        /**
         * @return true if equipable
         */
        virtual bool isEquipable() const;

        /**
         * @return true if useable
         * POSSIBLY REDUNDANT - we may just use isEquipable() for all useable items
         */
        virtual bool isUsable() const;

        /**
         * @return item's ItemType
         * Might not be used depending on implementation
         */
        virtual ItemType getItemType() const = 0;

        /**
         * @param character the character that is using the potion
         * @return message when used
         * This method's parameters will be changed depending on what the implementation of 
         *    the player/monster classes
         * 
         * Currently designed to only ever affect the Character that uses it
         */
        virtual std::string useItem(Character& character) const;

    private:
            int             itemId;
            std::string     name;
            int             physicalAttack;
            int             magicAttack;
            int             physicalDefense;
            int             magicDefense;
            int             heal;

};

#endif
