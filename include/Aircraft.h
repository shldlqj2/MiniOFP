#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cmath>
#include <iostream>
#include <string>
#include <utility>

namespace OFP {
class Aircraft {
public:
  std::string modelName;

  double maxFuelCapacity;
  double mtow;
  double oew;

  double cruiseSpeedKts;
  double fuelFlow;

  Aircraft(std::string name, double maxFuel, double mtowVal, double oewVal,
           double speed, double ff)
      : modelName(name), maxFuelCapacity(maxFuel), mtow(mtowVal), oew(oewVal),
        cruiseSpeedKts(speed), fuelFlow(ff) {}

  std::pair<double, double> calculateTrip(double distanceNM) const {
    double timeHours = distanceNM / cruiseSpeedKts;
    double tripFuel = timeHours * fuelFlow;

    return {tripFuel, timeHours};
  }

  bool canFly(double distanceNM, double payloadKg) const {
    auto [neededFuel, _] = calculateTrip(distanceNM);
    if (neededFuel > maxFuelCapacity) {
      std::cout << "[Fail] 연료 용량이 부족합니다 Need: " << neededFuel << " kg"
                << std::endl;
      return false;
    }
    double takeoffWeight = oew + payloadKg + neededFuel;

    if (takeoffWeight > mtow) {
      std::cout << "[Fail] 너무 무겁습니다. 현재 무게 : " << takeoffWeight
                << ", 이륙 가능 무게 : " << mtow << std::endl;
      return false;
    }
    return true;
  }
};
} // namespace OFP

#endif // !AIRCRAFT_H
#define AIRCRAFT_H

#include <cmath>
#include <iostream>
#include <string>
