#include "../include/Aircraft.h"
#include "../include/NavDB.h"
#include "../include/geometry.h"
#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

TEST(GeometryTest, CalculateDistance) {

  using namespace OFP;

  Coordinate p1 = {0.0, 0.0};
  Coordinate p2 = {0.0, 1.0};

  double dist = calculateDistance(p1, p2);

  EXPECT_NEAR(dist, 60.0, 0.1);
}

TEST(AircraftTest, FuelCaculation) {
  using namespace OFP;
  Aircraft ac("TEST-JET", 1000, 5000, 2000);
  ac.setPerformance({100, 100}, {100, 100}, {100, 100});

  auto res = ac.calculateProfile(200.0, 0.0);

  ASSERT_TRUE(res.isPossible);
  EXPECT_NEAR(res.tripFuel, 200, 50.0);
}

TEST(NavDBTest, LoadFromStream) {
  using namespace OFP;
  std::stringstream mockCsv;
  mockCsv << "ICAO,NAME,LAT,LON\n";
  mockCsv << "TEST,Test Airport,10.0,20.0\n";

  std::ofstream out("temp_test.csv");
  out << "ICAO,NAME,LAT,LON\n";
  out << "TEST,Test Airport,10.0,20.0\n";
  out.close();

  NavDB db("temp_test.csv", "dummy.csv", "dummy.csv");

  const Airport *apt = db.findAirport("TEST");

  ASSERT_TRUE(apt != nullptr);
  EXPECT_EQ(apt->name, "Test Airport");
  EXPECT_DOUBLE_EQ(apt->loc.lat, 10.0);

  std::remove("temp_test.csv");
}
