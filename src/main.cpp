
#include "NavDB.h"
#include "geometry.h"
#include <iostream>

int main() {
  OFP::NavDB db("../data/airports.csv");

  std::string depCode = "RKSI";
  std::string arrCode = "EGLL"; // 런던 히드로 공항

  const OFP::Airport *dep = db.findAirport(depCode);
  const OFP::Airport *arr = db.findAirport(arrCode);

  if (dep == nullptr || arr == nullptr) {
    std::cerr << "Error: Airport not found in DB." << std::endl;
    return 1;
  }

  double dist = OFP::calculateDistance(dep->loc, arr->loc);

  std::cout << "=== Mini-OFP Route Engine ===" << std::endl;
  std::cout << "Dep: " << dep->name << " (" << dep->icao << ")" << std::endl;
  std::cout << "Arr: " << arr->name << " (" << arr->icao << ")" << std::endl;
  std::cout << "Flight Distance: " << dist << " NM" << std::endl;

  return 0;
}
