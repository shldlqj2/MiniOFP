#ifndef NAVDB_H
#define NAVDB_H

#include "geometry.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace OFP {
struct Airport {
  std::string icao;
  std::string name;
  Coordinate loc;
};
class NavDB {
  std::vector<Airport> airports;

public:
  NavDB(const std::string &filepath) { loadFromCSV(filepath); }
  void loadFromCSV(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      std::cerr << "Error: Failed to open file " << filepath << std::endl;
      return;
    }
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string token;
      Airport apt;
      if (!std::getline(ss, apt.icao, ','))
        continue;
      if (!std::getline(ss, apt.name, ','))
        continue;

      std::string latStr, lonStr;
      if (!std::getline(ss, latStr, ','))
        continue;
      apt.loc.lat = std::stod(latStr);

      if (!std::getline(ss, lonStr, ','))
        continue;
      apt.loc.lon = std::stod(lonStr);
      airports.push_back(apt);
    }
    std::cout << "Data Loaded: " << airports.size() << "airports. "
              << std::endl;
  }
  const Airport *findAirport(const std::string &icaoCode) const {
    for (const auto &apt : airports) {
      if (apt.icao == icaoCode) {
        return &apt;
      }
    }
    return nullptr;
  }
};
} // namespace OFP
#endif // !NAVDB_H
#define NAVDB_H
