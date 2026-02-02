#include <iostream>
#include <string>

#include "NavDB.h"
#include "OFPEngine.h"

int main() {
  using namespace OFP;

  // 1. 시스템 초기화
  std::cout << "[System] Initializing Navigation Database..." << std::endl;
  NavDB db("../data/airports.csv", "../data/waypoints.csv",
           "../data/airways.csv");
  OFPEngine engine(db);

  // 2. 사용자 입력 시나리오
  std::string callsign = "KAL027";
  std::string dep = "RKSI";
  std::string arr = "OLMEN";  // DB에 있는 목적지로 설정
  double payload = 18000.0;   // 승객/화물 무게
  double wind = -15.0;        // 맞바람 15노트

  // 3. OFP 생성
  std::string ofpReport =
      engine.generateFlightPlan(callsign, "B777", dep, arr, payload, wind);

  // 4. 결과 출력
  std::cout << ofpReport << std::endl;

  return 0;
}