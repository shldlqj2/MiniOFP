#ifndef NAVDB_H
#define NAVDB_H

#include "geometry.h"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace OFP {
struct Airport {
  std::string icao;
  std::string name;
  Coordinate loc;
};

struct Waypoint {
  std::string id;
  Coordinate loc;
};

struct AirwaySegment {
  std::string toID;
  std::string airwayName;
  double distanceNM;
};

class NavDB {
  std::map<std::string, Airport> airports;
  std::map<std::string, Waypoint> waypoints;
  std::map<std::string, std::vector<AirwaySegment>> adjList;

public:
  NavDB(const std::string &aptPath, const std::string &wptPath,
        const std::string &awyPath) {
    loadAirports(aptPath);
    loadWaypoints(wptPath);
    loadAirways(awyPath);
  }
  void loadAirports(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
      return;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string token, latStr, lonStr;
      Airport apt;

      std::getline(ss, apt.icao, ',');
      std::getline(ss, apt.name, ',');
      std::getline(ss, latStr, ',');
      std::getline(ss, lonStr, ',');

      apt.loc.lat = std::stod(latStr);
      apt.loc.lon = std::stod(lonStr);

      airports[apt.icao] = apt;
    }
    std::cout << "[DB] Airports Loaded: " << airports.size() << std::endl;
  }

  void loadWaypoints(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      std::cerr << "Error: Waypoint file not fount!" << std::endl;
      return;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string latStr, lonStr;
      Waypoint wpt;

      std::getline(ss, wpt.id, ',');
      std::getline(ss, latStr, ',');
      std::getline(ss, lonStr, ',');

      wpt.loc.lat = std::stod(latStr);
      wpt.loc.lon = std::stod(lonStr);

      waypoints[wpt.id] = wpt;
    }
    std::cout << "[DB] Waypoints Loaded: " << waypoints.size() << std::endl;
  }

  Coordinate getCoordinate(const std::string &id) const {
    if (airports.count(id))
      return airports.at(id).loc;
    if (waypoints.count(id))
      return waypoints.at(id).loc;
    return {0.0, 0.0};
  }

  bool hasPoint(const std::string &id) const {
    return airports.count(id) || waypoints.count(id);
  }

  void loadAirways(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      std::cerr << "Error: Airway file not found!" << std::endl;
      return;
    }

    std::string line;
    std::getline(file, line);

    int count = 0;
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string name, fromID, toID;

      std::getline(ss, name, ',');
      std::getline(ss, fromID, ',');
      std::getline(ss, toID, ',');

      if (hasPoint(fromID) && hasPoint(toID)) {
        Coordinate p1 = getCoordinate(fromID);
        Coordinate p2 = getCoordinate(toID);
        double dist = calculateDistance(p1, p2);

        AirwaySegment seg = {toID, name, dist};
        adjList[fromID].push_back(seg);

        AirwaySegment segRev = {fromID, name, dist};
        adjList[toID].push_back(segRev);

        count++;
      }
    }
    std::cout << "[DB] Airways Loaded: " << count << " segments linked."
              << std::endl;
  }

  const std::vector<AirwaySegment> *getNeighbors(const std::string &id) const {
    auto it = adjList.find(id);
    if (it != adjList.end()) {
      return &it->second;
    }
    return nullptr;
  }

  const Airport *findAirport(const std::string &key) const {
    auto it = airports.find(key);
    if (it != airports.end()) {
      return &it->second;
    }
    return nullptr;
  }

  const Waypoint *findWaypoint(const std::string &key) const {
    auto it = waypoints.find(key);
    if (it != waypoints.end()) {
      return &it->second;
    }
    return nullptr;
  }

  std::string findAirwayName(const std::string &fromID, const std::string &toID) const{
    auto it = adjList.find(fromID);
    if (it != adjList.end()) {
      for(const auto &segment : it->second){
        if(segment.toID == toID){
          return segment.airwayName;
        }
      }

    }
    return "DCT";
  }
};
} // namespace OFP
#endif // !NAVDB_H
#define NAVDB_H
