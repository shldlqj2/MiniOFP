#include "NavDB.h"
#include "RouteFinder.h"
#include <iomanip>
#include <iostream>

int main() {
  using namespace OFP;

  // 1. DB 로딩
  NavDB db("../data/airports.csv", "../data/waypoints.csv",
           "../data/airways.csv");

  // 2. 경로 탐색 엔진 생성
  RouteFinder router(db);

  // 출발: 서울(RKSI), 도착: BIGOB (아까 CSV에서 정의한 끝점)
  std::string start = "RKSI";
  std::string end = "BIGOB";

  std::cout << "=== Mini-OFP Dijkstra Search ===" << std::endl;
  std::cout << "Searching path from " << start << " to " << end << "..."
            << std::endl;

  // 3. 길 찾기 수행
  std::vector<std::string> path = router.findPath(start, end);

  if (path.empty()) {
    std::cout << "Path NOT found!" << std::endl;
  } else {
    std::cout << "Path Found! (" << path.size() << " points)" << std::endl;
    std::cout << "------------------------------" << std::endl;

    double totalDist = 0.0;
    Coordinate prev = db.getCoordinate(path[0]);

    for (size_t i = 0; i < path.size(); ++i) {
      std::string id = path[i];
      Coordinate curr = db.getCoordinate(id);

      double legDist = 0;
      if (i > 0) {
        legDist = calculateDistance(prev, curr);
        totalDist += legDist;
      }

      std::cout << " [" << i << "] " << id;
      if (i > 0)
        std::cout << " (+" << (int)legDist << " NM)";
      std::cout << std::endl;

      prev = curr;
    }
    std::cout << "------------------------------" << std::endl;
    std::cout << "Total Distance: " << totalDist << " NM" << std::endl;
  }

  return 0;
}
