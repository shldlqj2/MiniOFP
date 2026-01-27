#include "Aircraft.h"
#include "NavDB.h"
#include "geometry.h"
#include <iomanip>
#include <iostream>

int main() {
  using namespace OFP;

  NavDB db("../data/airports.csv");
  const Airport *dep = db.findAirport("RKSI");
  const Airport *arr = db.findAirport("EGLL");

  if (!dep || !arr)
    return 1;

  double dist = calculateDistance(dep->loc, arr->loc);

  // B777-300ER 생성 (기본 스펙)
  Aircraft b777("B777-300ER", 145500, 351500, 167800);

  // 상세 성능 설정 (속도 kts, 연료소모 kg/hr)
  // Climb: 평균 380kts, 연료 10,500 kg/hr (엔진 풀가동)
  // Cruise: 평균 490kts, 연료 7,500 kg/hr
  // Descent: 평균 320kts, 연료 2,500 kg/hr (거의 공회전)
  b777.setPerformance({380, 10500}, {490, 7500}, {320, 2500});

  std::cout << "=== Mini-OFP Detailed Profile ===" << std::endl;
  std::cout << "Route: " << dep->icao << " -> " << arr->icao << std::endl;
  std::cout << "Total Distance: " << std::fixed << std::setprecision(1) << dist
            << " NM" << std::endl;
  std::cout << "---------------------------------" << std::endl;

  auto res = b777.calculateProfile(dist);

  if (res.isPossible) {
    std::cout << " [Phase 1: Climb  ] Dist: " << std::setw(6) << res.climbDist
              << " NM (20 min)" << std::endl;
    std::cout << " [Phase 2: Cruise ] Dist: " << std::setw(6) << res.cruiseDist
              << " NM" << std::endl;
    std::cout << " [Phase 3: Descent] Dist: " << std::setw(6) << res.descentDist
              << " NM (30 min)" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << " Total Flight Time : " << res.totalTime << " hours"
              << std::endl;
    std::cout << " Total Block Fuel  : " << res.totalFuel << " kg" << std::endl;
  } else {
    std::cout << "Calculation Failed: " << res.message << std::endl;
  }

  return 0;
}
