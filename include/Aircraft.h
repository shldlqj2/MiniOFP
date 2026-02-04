#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace OFP {
struct PerformanceData {
  double speedKts;
  double fuelFlow;
};

struct FlightProfileResult {
  double climbDist, cruiseDist, descentDist;

  double groundSpeed;

  double tripTime;

  double tripFuel;
  double contingencyFuel;
  double alternateFuel;
  double finalReserve;
  double blockFuel;

  bool isPossible;
  std::string message;
};

class Aircraft {
public:
  std::string modelName;

  double maxFuelCapacity;
  double mtow;
  double oew;

  PerformanceData climbData;
  PerformanceData cruiseData;
  PerformanceData descentData;

  Aircraft(std::string name, double maxFuel, double mtowVal, double oewVal)
      : modelName(name), maxFuelCapacity(maxFuel), mtow(mtowVal), oew(oewVal) {}

  void setPerformance(PerformanceData climb, PerformanceData cruise,
                      PerformanceData descent) {
    climbData = climb;
    cruiseData = cruise;
    descentData = descent;
  }

  FlightProfileResult calculateProfile(double totalDistanceNM,
                                       double windComponent) const {
    FlightProfileResult res;
    res.isPossible = true;

    double timeClimb = 20.0 / 60.0;
    double timeDescent = 30.0 / 60.0;
    res.climbDist = timeClimb * climbData.speedKts;
    res.descentDist = timeDescent * descentData.speedKts;

    res.cruiseDist = totalDistanceNM - (res.climbDist + res.descentDist);

    if (res.cruiseDist < 0) {
      res.isPossible = false;
      res.message = "거리가 너무 짧습니다.";
      return res;
    }

    res.groundSpeed = cruiseData.speedKts + windComponent;

    if (res.groundSpeed <= 0) {
      res.isPossible = false;
      res.message = "맞바람이 너무 강해 운항속도가 0이거나 0보다 작습니다.";
      return res;
    }

    double timeCruise = res.cruiseDist / res.groundSpeed;

    double fuelClimb = timeClimb * climbData.fuelFlow;
    double fuelDescent = timeDescent * descentData.fuelFlow;
    double fuelCruise = timeCruise * cruiseData.fuelFlow;

    res.tripFuel = fuelClimb + fuelCruise + timeDescent;
    res.tripTime = timeClimb + timeCruise + timeDescent;

    res.contingencyFuel = res.tripFuel * 0.05;
    double altTime = 200.0 / cruiseData.speedKts;
    res.alternateFuel = altTime * cruiseData.fuelFlow;

    res.finalReserve = 0.5 * cruiseData.fuelFlow;

    res.blockFuel = res.tripFuel + res.contingencyFuel + res.alternateFuel +
                    res.finalReserve;

    if (res.blockFuel > maxFuelCapacity) {
      res.isPossible = false;
      res.message = " Block Fuel exceeds Tank Capacity";
    }
    return res;
  }

  // std::pair<double, double> calculateTrip(double distanceNM) const {
  //   double timeHours = distanceNM / cruiseData.cruiseSpeedKts;
  //   double tripFuel = timeHours * cruiseData.fuelFlow;
  //
  //   return {tripFuel, timeHours};
  // }
  //
  // bool canFly(double distanceNM, double payloadKg) const {
  //   auto [neededFuel, _] = calculateTrip(distanceNM);
  //   if (neededFuel > maxFuelCapacity) {
  //     std::cout << "[Fail] 연료 용량이 부족합니다 Need: " << neededFuel << "
  //     kg"
  //               << std::endl;
  //     return false;
  //   }
  //   double takeoffWeight = oew + payloadKg + neededFuel;
  //
  //   if (takeoffWeight > mtow) {
  //     std::cout << "[Fail] 너무 무겁습니다. 현재 무게 : " << takeoffWeight
  //               << ", 이륙 가능 무게 : " << mtow << std::endl;
  //     return false;
  //   }
  //   return true;
  // }
};
class AircraftDB {
  std::map<std::string, Aircraft> fleet;

public:
  AircraftDB(const std::string &filepath) { loadAircraft(filepath); }
  void loadAircraft(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
      return;

    std::string line;
  std:
    std::getline(file, line);

    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string model, token;
      std::vector<double> vals;

      std::getline(ss, model, ',');
      while (std::getline(ss, token, ',')) {
        vals.push_back(std::stod(token));
      }

      if (vals.size() >= 9) {
        Aircraft ac(model, vals[0], vals[1], vals[2]);
        ac.setPerformance({vals[3], vals[4]}, {vals[5], vals[6]},
                          {vals[7], vals[8]});
        fleet.insert({model, ac});
      }
    }
    std::cout << "[DB] Aircraft Loaded: " << fleet.size() << "types."
              << std::endl;
  }
  const Aircraft *getAircraft(const std::string &modelName) const {
    auto it = fleet.find(modelName);
    if (it != fleet.end()) {
      return &it->second;
    }
    return nullptr;
  } // class AircraftDB
};
} // namespace OFP

#endif // !AIRCRAFT_H
#define AIRCRAFT_H

#include <cmath>
#include <iostream>
#include <string>
