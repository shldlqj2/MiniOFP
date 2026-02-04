// 테스트 simple 프레임 워크

#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <cmath>
#include <iostream>
#include <string>

static int failed_tests = 0;

#define ASSERT_NEAR(val1, val2, abs_error)                                     \
  if (std::abs((val1) - (val2)) > (abs_error)) {                               \
    std::cerr << "[FAIL] " << __FUNCTION__ << " line " << __LINE__ << ": "     \
              << (val1) << " != " << (val2) << std::endl;                      \
    failed_tests++;                                                            \
  } else {                                                                     \
    std::cout << "[PASS] " << __FUNCTION__ << std::endl;                       \
  }

#define ASSERT_TRUE(condition)                                                 \
  if (!(condition)) {                                                          \
    std::cerr << "[FAIL] " << __FUNCTION__ << "line" << __LINE__               \
              << ": Condition is false" << std::endl;                          \
    failed_tests++;                                                            \
  } else {                                                                     \
    std::cout << "[PASS] " << __FUNCTION__ << std::endl;                       \
  }

#endif // !SIMPLETEST_H
