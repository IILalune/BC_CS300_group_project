#include "MapGenerator.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
int drawInt(std::mt19937& rng, int low, int high) {
  return std::uniform_int_distribution<int>(low, high)(rng);
}

void connect(DungeonMap& map, int from, int to) {
  if (!map.ConnectBothRoom(from, to)) {
    throw std::logic_error("Could not connect rooms " + std::to_string(from) +
                           " and " + std::to_string(to));
  }
}
}  // namespace

std::unique_ptr<DungeonMap> GenerateFloor(std::mt19937& rng, std::ostream& report) {
  auto floor = std::make_unique<DungeonMap>(report, std::mt19937(rng()));

  // Main route: entrance, three to six guard posts, then the dragon.
  std::vector<int> mainRoute;
  mainRoute.push_back(floor->GenerateNewRoom(RoomType::Start, "Dungeon entrance"));
  const int combatCount = drawInt(rng, 3, 6);
  for (int i = 1; i <= combatCount; ++i) {
    const int room = floor->GenerateNewRoom(RoomType::Normal,
                                            "Guard post " + std::to_string(i));
    connect(*floor, mainRoute.back(), room);
    mainRoute.push_back(room);
  }
  const int boss = floor->GenerateNewRoom(RoomType::Boss, "The dragon's chamber");
  connect(*floor, mainRoute.back(), boss);

  // Leave one guard post ahead of every fork so a branch can rejoin the main route.
  std::vector<int> branchStarts;
  for (int i = 1; i < combatCount; ++i) branchStarts.push_back(i);
  std::shuffle(branchStarts.begin(), branchStarts.end(), rng);
  const int branchCount = drawInt(rng, 1, std::min(3, combatCount - 1));
  std::bernoulli_distribution rejoin(0.5);
  // Each branch starts at a distinct main-route index and ends at a treasure room.
  for (int i = 0; i < branchCount; ++i) {
    const int start = branchStarts[i];
    int previous = mainRoute[start];
    const int length = drawInt(rng, 1, 3);
    for (int step = 1; step <= length; ++step) {
      int room;
      if (step == length) {
        room = floor->GenerateNewRoom(RoomType::Treasure, "Sealed vault " + std::to_string(i + 1));
      } else {
        const auto name = "Side passage " + std::to_string(i + 1) + "." + std::to_string(step);
        room = floor->GenerateNewRoom(RoomType::Normal, name);
      }
      connect(*floor, previous, room);
      previous = room;
    }
    if (rejoin(rng)) connect(*floor, previous, mainRoute[drawInt(rng, start + 1, combatCount)]);
  }

  if (!floor->AllRoomsConnected()) {
    throw std::logic_error("Generated floor contains an unreachable room");
  }
  report << "Floor ready. All " << floor->GetRoomCount()
         << " rooms are reachable from the entrance.\n";
  floor->PrintCurrentRoom();
  return floor;
}
