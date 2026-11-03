// Character that contain information of the character 
// and the action perform each turn in the room
// A BOSS is also a character and have HIS own invetory. IF we defeat the boss we claim the inventory. 
// 3 boos mass. 1 for the end, 1 for half way and 1 for a secret room. 

// + attack() : void
// +castSpell() : void
// + defense() : ...   (Should magic and physical count to the same defense point for the sake of simplicity?)
// + useItem(): bool
// + changeEquippededWeapon(): bool ( if fail, send message game reach error, reset game, report bug to developer ( easter egg))
// - levelUp(): void ()

// Accessors:
// +getAttackDamage():
// +getHealth():
// +getMana():
// +getDefense(): 
// +getItemDetail(): 
// 

/** Field
 * - int Level
 * - int Health
 * - int maxHealth
 * - int Mana
 * - int maxMana
 * - int Defense
 * - int physicalAttack
 * - int magicalAttack
 * - Status status 
 * - Inventory inventory
 * - Item equippedWeapon
*/
