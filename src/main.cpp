#include <iostream>
#include <string>

#include "NavDB.h"
#include "OFPEngine.h"

int main() {
  using namespace OFP;

  std::cout << "[System] Booting Mini-OFP Engine..." << std::endl;

  // DB 로딩
  NavDB db("../data/airports.csv", "../data/waypoints.csv",
           "../data/airways.csv");
  AircraftDB acDb("../data/aircraft.csv"); // [New]

  OFPEngine engine(db, acDb);

  // 사용자 입력
  std::string callsign = "KAL027";
  std::string type = "B737-800"; // [New] 이번엔 작은 비행기로 테스트
  std::string dep = "RKSI";
  std::string arr = "OLMEN";
  double payload = 15000.0;
  double wind = 0.0;

  std::string report =
      engine.generateFlightPlan(callsign, type, dep, arr, payload, wind);

  std::cout << report << std::endl;

  // 파일 저장
  engine.saveToFile("OFP_" + callsign + ".txt", report);

  return 0;
}
