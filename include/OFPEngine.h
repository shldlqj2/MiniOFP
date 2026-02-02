#ifndef OFPENGINE_H
#define OFPENGINE_H
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Aircraft.h"
#include "NavDB.h"
#include "RouteFinder.h"

namespace OFP {
class OFPEngine {
  NavDB& db;
  RouteFinder routeFinder;

  std::string formatRouteString(const std::vector<std::string>& path) {
    if (path.size() < 2) return "";
    std::stringstream ss;
    ss << path[0];
    std::string currentAirway = "";

    for (size_t i = 0; i < path.size() - 1; ++i) {
      std::string airway = db.findAirwayName(path[i], path[i + 1]);
      if (airway != currentAirway) {
        if (!currentAirway.empty())
          ss << " " << currentAirway << " " << path[i];
        currentAirway = airway;
      }
    }
    ss << " " << currentAirway << " " << path.back();
    return ss.str();
  }

 public:
  OFPEngine(NavDB& database) : db(database), routeFinder(database) {}
  std::string generateFlightPlan(const std::string& callsign,
                                 const std::string& aircraftType,
                                 const std::string& depICAO,
                                 const std::string& arrICAO, double payloadKg,
                                 double windComponent) {
    std::stringstream ss;

    Aircraft aircraft("B777-300ER", 145500, 351500, 167800);
    aircraft.setPerformance({380, 10500}, {490, 7500}, {320, 2500});

    ss << "Status: Searching Route from " << depICAO << "to" << arrICAO
       << "...\n";
    std::vector<std::string> path = routeFinder.findPath(depICAO, arrICAO);

    if (path.empty()) {
      return "ERROR: No route found between airports.\n";
    }

    double totalDistance = 0.0;
    Coordinate prev = db.getCoordinate(path[0]);

    for (size_t i = 1; i < path.size(); ++i) {
      Coordinate curr = db.getCoordinate(path[i]);
      totalDistance += calculateDistance(prev, curr);
      prev = curr;
    }

    auto profile = aircraft.calculateProfile(totalDistance, windComponent);

    if (!profile.isPossible) {
      return "ERROR: Flight impossible. " + profile.message + "\n";
    }
    ss << "\n";
    ss << "============================================================\n";
    ss << "               OPERATIONAL FLIGHT PLAN (OFP)                \n";
    ss << "============================================================\n";
    ss << "FLT NO: " << std::left << std::setw(8) << callsign
       << " A/C: " << std::setw(10) << aircraft.modelName << " DATE: 29JAN26\n";
    ss << "DEP: " << depICAO << "   ARR: " << arrICAO << "\n";
    ss << "------------------------------------------------------------\n";
    ss << "ROUTE: " << formatRouteString(path) << "\n";
    ss << "------------------------------------------------------------\n";
    ss << "DIST: " << (int)totalDistance << " NM    ";
    ss << "WIND: " << (windComponent >= 0 ? "+" : "") << (int)windComponent
       << " KT    ";
    ss << "G/S: " << (int)profile.groundSpeed << " KT\n";
    ss << "TIME: " << std::fixed << std::setprecision(2) << profile.tripTime
       << " HRS\n";
    ss << "------------------------------------------------------------\n";
    ss << "FUEL CALCULATION (KG)                 \n";
    ss << "------------------------------------------------------------\n";
    ss << "TRIP FUEL      : " << std::setw(6) << (int)profile.tripFuel << "\n";
    ss << "CONTINGENCY 5% : " << std::setw(6) << (int)profile.contingencyFuel
       << "\n";
    ss << "ALTERNATE      : " << std::setw(6) << (int)profile.alternateFuel
       << "\n";
    ss << "FINAL RESV     : " << std::setw(6) << (int)profile.finalReserve
       << "\n";
    ss << "------------------------------------------------------------\n";
    ss << "MIN T/OFF FUEL : " << std::setw(6) << (int)profile.blockFuel << "\n";
    ss << "------------------------------------------------------------\n";
    ss << "PAYLOAD        : " << std::setw(6) << (int)payloadKg << "\n";
    ss << "EST T/OFF WT   : " << std::setw(6)
       << (int)(aircraft.oew + payloadKg + profile.blockFuel) << "\n";
    ss << "MAX T/OFF WT   : " << std::setw(6) << (int)aircraft.mtow << "\n";
    ss << "============================================================\n";

    return ss.str();
  }
};
}  // namespace OFP
#endif