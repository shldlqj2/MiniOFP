#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846

#endif // !M_PI
#define M_PI 3.14159265358979323846

namespace OFP {
const double EarthRadiusNM = 3440.065;

struct Coordinate {
  double lat;
  double lon;
};

// inline을 왜 썼는가?
// 오버헤드 감소 + 중복정의 방지
// 오버헤드 감소는 최신 c++에는 의미 없음 왜냐면 알아서 최적화 또는 무시
// 중복정의 방지가 핵심
inline double toRadians(double degree) { return degree * M_PI / 180.0; }

inline double calculateDistance(const Coordinate &p1, const Coordinate &p2) {
  double delta_lat = toRadians(p1.lat - p2.lat);
  double delta_lon = toRadians(p1.lon - p2.lon);

  double lat1 = toRadians(p1.lat);
  double lat2 = toRadians(p2.lat);
  double lon1 = toRadians(p1.lon);
  double lon2 = toRadians(p2.lon);
  double a =
      std::pow(std::sin(delta_lat / 2.0), 2) +
      std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(delta_lon / 2.0), 2);
  double c = 2 * std::atan2(sqrt(a), sqrt(1.0 - a));
  return EarthRadiusNM * c;
}

} // namespace OFP

#endif // !GEOMETRY_H
#define GEOMETRY_H
