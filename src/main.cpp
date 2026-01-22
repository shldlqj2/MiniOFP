#include "geometry.h"
#include <iostream>

int main() {
  OFP::Coordinate icn = {37.4602, 126.4407};
  OFP::Coordinate lax = {33.9416, -118.4085};

  double dist = OFP::calculateDistance(icn, lax);

  std::cout << "=== MiniOFP Distance Test ===" << std::endl;
  std::cout << "From: ICN (" << icn.lat << ", " << icn.lon << ")" << std::endl;
  std::cout << "To: LAX (" << lax.lat << ", " << lax.lon << ")" << std::endl;
  std::cout << "Distance: " << dist << "NM" << std::endl;

  return 0;
}
