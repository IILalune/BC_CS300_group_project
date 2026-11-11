/** @file Character.h */
// Character that contain information of the character 
// and the action perform each turn in the room
// A BOSS is also a character and have HIS own invetory. IF we defeat the boss we claim the inventory. 
// 3 bosses max. 1 for the end, 1 for half way and 1 for a secret room. 



#ifndef CHARACTER_
#define CHARACTER_

#include <string>



#include "Dice.h"



class Character {
    public:
        // Constructor
        Character(std::string name);


        // Character Actions
        int physicalAttack();

        // Accessors
        std::string getName(); 
        int getMinPhysicalAttack();
        int getMaxPhysicalAttack();
        // Add on slowly

    private:
    /** Field*/
        std::string characterName;
        
        //
        int Level;
        //int levelPoint;

        // Character Stats :
        //int Health;       
        //int maxHealth;
        //int Mana;
        //int maxMana;
       // int Defense;    
        int minPhysicalAttack;
        int maxPhysicalAttack;
        int criticalScalePoint;
        //int magicalAttack;   // Magic attack is low, but item have higher damage mutiplier. ??
    
        
        // Character default objects:
        Dice dice;

        //Inventory inventory;
        //Item equippedWeapon;
        

};

#endif // CHARACTER_