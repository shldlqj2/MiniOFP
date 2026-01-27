#include "Aircraft.h"
#include "NavDB.h"
#include "geometry.h"
#include <iomanip>
#include <iostream>

// 결과 출력 헬퍼 함수
void printReport(const std::string &title,
                 const OFP::FlightProfileResult &res) {
  using namespace std;
  cout << "[" << title << "]" << endl;
  if (!res.isPossible) {
    cout << "  FAILED: " << res.message << endl << endl;
    return;
  }
  cout << "  Flight Time : " << fixed << setprecision(1) << res.tripTime
       << " hrs" << endl;
  cout << "  Ground Speed: " << (int)res.groundSpeed << " kts (Cruise)" << endl;
  cout << "  ------------------------" << endl;
  cout << "  Trip Fuel   : " << (int)res.tripFuel << " kg" << endl;
  cout << "  Contingency : " << (int)res.contingencyFuel << " kg (5%)" << endl;
  cout << "  Alternate   : " << (int)res.alternateFuel << " kg (200NM)" << endl;
  cout << "  Final Resv  : " << (int)res.finalReserve << " kg (30min)" << endl;
  cout << "  ========================" << endl;
  cout << "  BLOCK FUEL  : " << (int)res.blockFuel << " kg" << endl << endl;
}

int main() {
  using namespace OFP;

  NavDB db("../data/airports.csv");
  // 서울(ICN) -> 런던(LHR)
  const Airport *dep = db.findAirport("RKSI");
  const Airport *arr = db.findAirport("EGLL");

  if (!dep || !arr)
    return 1;

  double dist = calculateDistance(dep->loc, arr->loc);

  Aircraft b777("B777-300ER", 145500, 351500, 167800);
  // Climb(380kts, 10.5T), Cruise(490kts, 7.5T), Descent(320kts, 2.5T)
  b777.setPerformance({380, 10500}, {490, 7500}, {320, 2500});

  std::cout << "=== Mini-OFP Wind Analysis ===" << std::endl;
  std::cout << "Route: " << dep->icao << " -> " << arr->icao << " ("
            << (int)dist << " NM)" << std::endl;
  std::cout << "----------------------------------" << std::endl;

  // 시나리오 1: 바람 없음 (0 kts)
  printReport("No Wind", b777.calculateProfile(dist, 0));

  // 시나리오 2: 강한 맞바람 (Headwind 100 kts) - 제트기류를 정면으로 맞음
  // 490 - 100 = 390 kts로 기어감 -> 시간 늘어남 -> 연료 폭증
  printReport("Headwind 100 kts", b777.calculateProfile(dist, -100));

  // 시나리오 3: 강한 뒷바람 (Tailwind 100 kts) - 제트기류를 탐
  // 490 + 100 = 590 kts로 날아감 -> 시간 단축 -> 연료 절약
  printReport("Tailwind 100 kts", b777.calculateProfile(dist, 100));

  return 0;
}
