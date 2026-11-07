/** @file Affect.h*/

#ifndef ELEMENTAL_EFFECT_
#define ELEMENTAL_EFFECT_


#include<string>

enum class ElementalType {AIR, FIRE, EARTH, WATER};


class Affect {

    // Air (add critical damage 3x buff , can attack again) 
    // Water (flat damage, less mana, double attack at level 2) 
    // Earth (Each earth consercutively attack add more damage)
    // Fire (Extra damage tick each turn)

    //Field        
        ElementalType affectType;
        int level;
};

#endif // ELEMENTAL_EFFECTS_

