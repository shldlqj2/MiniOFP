#include <iostream>
#include <vector>
#include <string>
#include <sstream> // 문자열 조립용
#include "NavDB.h"
#include "RouteFinder.h"
// geometry.h, Aircraft.h 등 필요한 헤더 포함

// [New] 경로 압축 함수
std::string formatICAORoute(const OFP::NavDB& db, const std::vector<std::string>& path) {
    if (path.size() < 2) return "";

    std::stringstream ss;
    
    // 1. 출발 공항 출력
    ss << path[0]; 

    std::string currentAirway = "";
    std::string entryPoint = path[0];

    // 2. 경로 순회 (1번 인덱스부터 시작)
    for (size_t i = 0; i < path.size() - 1; ++i) {
        std::string from = path[i];
        std::string to = path[i+1];

        // DB에서 두 점 사이의 항로 이름 가져오기
        std::string airway = db.findAirwayName(from, to);

        // A. 첫 구간이거나, 항로가 바뀌는 경우
        if (airway != currentAirway) {
            // 이전 항로가 있었다면, 그 항로의 끝지점(현재 from)을 찍어줘야 함
            // 하지만 첫 시작점(entryPoint)과 겹치면 생략 (RKSI Y711... 처럼)
            if (!currentAirway.empty()) {
               // 항로 이름 출력
               ss << " " << currentAirway << " " << from;
            } else {
               // 맨 처음 시작 부분
            }
            
            // 항로 갱신
            currentAirway = airway;
        }
        
        // B. 항로가 같으면? -> 아무것도 안 하고 그냥 통과 (압축)
    }

    // 3. 마지막 구간 처리
    // 루프가 끝나면 마지막 항로와 도착지점을 출력해줘야 함
    ss << " " << currentAirway << " " << path.back();

    return ss.str();
}

// 기존 main 함수 수정
int main() {
    using namespace OFP;

    NavDB db("../data/airports.csv", "../data/waypoints.csv", "../data/airways.csv");
    RouteFinder router(db);

    // 테스트: RKSI -> BIGOB
    std::string start = "RKSI";
    std::string end = "OLMEN"; 

    std::vector<std::string> path = router.findPath(start, end);

    std::cout << "=== Mini-OFP Route String Generator ===" << std::endl;
    
    if (path.empty()) {
        std::cout << "Path not found." << std::endl;
        return 0;
    }

    // 1. Raw Path 출력 (압축 전)
    std::cout << "[Raw Path]: ";
    for (const auto& p : path) std::cout << p << " ";
    std::cout << std::endl;

    // 2. ICAO Format 출력 (압축 후)
    std::string icaoRoute = formatICAORoute(db, path);
    std::cout << "[ICAO Format]: " << icaoRoute << std::endl;

    // 기대 결과: RKSI Y711 BIGOB
    // (설명: GOPDO, ANKOR는 Y711에 포함되므로 생략됨)

    return 0;
}