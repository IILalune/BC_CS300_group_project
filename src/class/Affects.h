/** @file Affect.h*/

#include<string>

enum class AffectType {AIR, FIRE, EARTH, WATER};


class Affect {

    // Air (add critical damage 3x buff , can attack again) 
    // Water (flat damage, less mana, double attack at level 2) 
    // Earth (Each earth consercutively attack add more damage)
    // Fire (Extra damage tick each turn)

    //Field        
        AffectType affectType;
        int level;
}