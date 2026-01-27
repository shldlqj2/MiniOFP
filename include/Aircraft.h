#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace OFP {
struct PerformanceData {
  double speedKts;
  double fuelFlow;
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

  struct FlightProfileResult {
    double totalFuel;
    double totalTime;
    double climbDist, cruiseDist, descentDist;
    bool isPossible;
    std::string message;
  };

  FlightProfileResult calculateProfile(double totalDistanceNM) const {
    FlightProfileResult res;
    res.isPossible = true;

    // 상승과 하강은 고정된 시간 동안 이루어진다고 단순화(상용X, 개발 Level)
    // 실제 무게/온도가 아닌 평균치 적용
    // 상승: 20분 동안 진행
    // 하강: 0분 동안 진행
    //
    double timeClimb = 20.0 / 60.0;
    double timeDescent = 30.0 / 60.0;

    res.climbDist = timeClimb * climbData.speedKts;
    res.descentDist = timeDescent * descentData.speedKts;

    res.cruiseDist = totalDistanceNM - (res.cruiseDist + res.descentDist);

    if (res.cruiseDist < 0) {
      res.isPossible = false;
      res.message = "비행하기엔 거리가 너무 짧습니다. 적절한 항공 프로필을 "
                    "작성 해 주세요";
      return res;
    }

    double timeCruise = res.cruiseDist / cruiseData.speedKts;

    double fuelClimb = timeClimb * climbData.fuelFlow;
    double fuelDescent = timeDescent * descentData.fuelFlow;
    double fuelCruise = timeCruise * cruiseData.fuelFlow;

    res.totalFuel = fuelClimb + fuelCruise + fuelDescent;
    res.totalTime = timeClimb + timeCruise + timeDescent;

    if (res.totalFuel > maxFuelCapacity) {
      res.isPossible = false;
      res.message = "필요한 연료가 연료 총량보다 많습니다.";
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
} // namespace OFP

#endif // !AIRCRAFT_H
#define AIRCRAFT_H

#include <cmath>
#include <iostream>
#include <string>
