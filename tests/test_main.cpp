#include "../include/Aircraft.h"
#include "../include/NavDB.h"
#include "../include/geometry.h"
#include "SimpleTest.h"
#include <iostream>

void test_geometry() {
  using namespace OFP;
  Coordinate p1 = {0.0, 0.0};
  Coordinate p2 = {0.0, 1.0};
  double dist = calculateDistance(p1, p2);

  // 오차범위 0.1NM이내면 통과
  ASSERT_NEAR(dist, 60.0, 0.1);
}

void test_aircraft_fuel() {
  using namespace OFP;
  Aircraft ac("TEST-JET", 1000, 5000, 2000);

  ac.setPerformance({100, 100}, {100, 100}, {100, 100});

  auto res = ac.calculateProfile(200.0, 0.0);

  // 대략 200kg 근처인지 확인
  // 오차범위를 50kg 줌
  ASSERT_TRUE(res.isPossible);
  ASSERT_NEAR(res.tripFuel, 200.0, 50.0)
}

// 3. DB 로딩 테스트
void test_db_loading() {
  using namespace OFP;
  // 실제 파일 대신 없는 파일로 에러 처리 확인은 어려우므로,
  // 여기서는 상대 경로가 맞는지 확인용
  NavDB db("../data/airports.csv", "../data/waypoints.csv",
           "../data/airways.csv");

  // RKSI가 있어야 함 (data 파일이 존재한다는 가정 하에)
  const Airport *apt = db.findAirport("RKSI");
  ASSERT_TRUE(apt != nullptr);

  if (apt) {
    ASSERT_NEAR(apt->loc.lat, 37.4602, 0.01);
  }
}

int main() {
  std::cout << "=== Running Mini-OFP Unit Tests ===" << std::endl;

  test_geometry();
  test_aircraft_fuel();
  test_db_loading();

  std::cout << "===================================" << std::endl;
  if (failed_tests == 0) {
    std::cout << "ALL TESTS PASSED!" << std::endl;
    return 0;
  } else {
    std::cout << failed_tests << " TESTS FAILED!" << std::endl;
    return 1; // 실패 시 종료 코드 1
  }
}
