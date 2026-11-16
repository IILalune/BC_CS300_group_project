/** @file Equipment.cpp */
#include "Equipment.h"

Equipment::Equipment() {
    for (int index = 0; index < Equipment_Slot_Count; index++) {
        equipmentSlots[index] = nullptr;
    }
}

bool Equipment::isSlotEmpty(EquipmentSlot targetSlot) const {
    if (!isValidSlot(targetSlot)) {
        return false;
    }

    return equipmentSlots[targetSlot] == nullptr;
}

bool Equipment::equipItem(BaseItem* item, EquipmentSlot targetSlot) {
    if (item == nullptr || !isValidSlot(targetSlot) || !isSlotEmpty(targetSlot)) {
        return false;
    }

    equipmentSlots[targetSlot] = item;
    return true;
}

BaseItem* Equipment::unequipItem(EquipmentSlot targetSlot) {
    if (!isValidSlot(targetSlot) || isSlotEmpty(targetSlot)) {
        return nullptr;
    }

    BaseItem* removedItem = equipmentSlots[targetSlot];
    equipmentSlots[targetSlot] = nullptr;
    return removedItem;
}

BaseItem* Equipment::getItem(EquipmentSlot targetSlot) const {
    if (!isValidSlot(targetSlot)) {
        return nullptr;
    }

    return equipmentSlots[targetSlot];
}

int Equipment::getPhysicalAttackBonus() const {
    int total = 0;
    for (int index = 0; index < Equipment_Slot_Count; index++) {
        if (equipmentSlots[index] != nullptr) {
            total += equipmentSlots[index]->getPhysicalAttack();
        }
    }
    return total;
}

int Equipment::getMagicAttackBonus() const {
    int total = 0;
    for (int index = 0; index < Equipment_Slot_Count; index++) {
        if (equipmentSlots[index] != nullptr) {
            total += equipmentSlots[index]->getMagicAttack();
        }
    }
    return total;
}

int Equipment::getPhysicalDefenseBonus() const {
    int total = 0;
    for (int index = 0; index < Equipment_Slot_Count; index++) {
        if (equipmentSlots[index] != nullptr) {
            total += equipmentSlots[index]->getPhysicalDefense();
        }
    }
    return total;
}

int Equipment::getMagicDefenseBonus() const {
    int total = 0;
    for (int index = 0; index < Equipment_Slot_Count; index++) {
        if (equipmentSlots[index] != nullptr) {
            total += equipmentSlots[index]->getMagicDefense();
        }
    }
    return total;
}

bool Equipment::isValidSlot(EquipmentSlot targetSlot) const {
    return targetSlot >= Weapon_Slot && targetSlot < Equipment_Slot_Count;
}
