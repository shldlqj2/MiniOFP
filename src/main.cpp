#include "Aircraft.h"
#include "NavDB.h"
#include "geometry.h"
#include <iomanip> // 소수점 예쁘게 찍기 위해
#include <iostream>

int main() {
  using namespace OFP;

  // 1. DB 로딩 & 공항 찾기
  NavDB db("../data/airports.csv");
  const Airport *dep = db.findAirport("RKSI"); // 인천
  const Airport *arr = db.findAirport("EGLL"); // 런던

  if (!dep || !arr) {
    std::cerr << "Airport not found!" << std::endl;
    return 1;
  }

  double dist = calculateDistance(dep->loc, arr->loc);

  std::cout << "=== Flight Feasibility Check ===" << std::endl;
  std::cout << "Route: " << dep->name << " -> " << arr->name << std::endl;
  std::cout << "Distance: " << std::fixed << std::setprecision(1) << dist
            << " NM" << std::endl;
  std::cout << "--------------------------------" << std::endl;

  // 2. 비행기 객체 생성 (대략적인 실제 스펙)

  // 비행기 A: Boeing 737-800 (작은 비행기)
  // MaxFuel: 20,800kg, MTOW: 79,000kg, OEW: 41,000kg, Speed: 450kts, FF:
  // 2,400kg/h
  Aircraft b737("B737-800", 20800, 79000, 41000, 450, 2400);

  // 비행기 B: Boeing 777-300ER (큰 비행기)
  // MaxFuel: 145,500kg, MTOW: 351,500kg, OEW: 167,800kg, Speed: 490kts, FF:
  // 7,500kg/h
  Aircraft b777("B777-300ER", 145500, 351500, 167800, 490, 7500);

  // 가정: 승객 150명 + 짐 = 약 15,000kg Payload
  double currentPayload = 15000.0;

  // 3. 검증
  std::cout << "[Test 1] " << b737.modelName << " with " << currentPayload
            << "kg payload:" << std::endl;
  if (b737.canFly(dist, currentPayload)) {
    std::cout << " -> SUCCESS! Ready for takeoff." << std::endl;
  } else {
    std::cout << " -> FAILED." << std::endl;
  }

  std::cout << "\n[Test 2] " << b777.modelName << " with " << currentPayload
            << "kg payload:" << std::endl;
  if (b777.canFly(dist, currentPayload)) {
    auto result = b777.calculateTrip(dist); // {연료, 시간}
    std::cout << " -> SUCCESS! Flight Time: " << result.second << " hrs"
              << std::endl;
    std::cout << " -> Trip Fuel: " << result.first << " kg" << std::endl;
  } else {
    std::cout << " -> FAILED." << std::endl;
  }

  return 0;
}
