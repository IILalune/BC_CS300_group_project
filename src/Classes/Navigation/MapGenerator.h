#ifndef DRAGON_MAP_GENERATOR_H
#define DRAGON_MAP_GENERATOR_H

#include "DungeonMap.h"

// Pass a seeded engine to reproduce a floor and its encounters.
std::unique_ptr<DungeonMap> GenerateFloor(std::mt19937& rng,
                                          std::ostream& report = std::cout);

#endif
