#include "pch.h"

#include "../RayTracerLib/Profiler.h"

#include <sstream>
#include <chrono>
#include <thread>

using namespace RayTracer;

TEST(TestProfiler, TestProfileEnabled) {
  std::stringstream ss;
  Profiler p(true, ss);

  {
    auto profile = p.profile("test");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::string label;
  int time;

  ss.seekg(0, std::ios::beg);
  ss >> label;
  ss >> time;
  EXPECT_EQ("test:", label);
  EXPECT_GE(100, time);
}

TEST(TestProfiler, TestProfileDisabled) {
  std::stringstream ss;
  Profiler p(false, ss);

  {
    auto profile = p.profile("test");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  EXPECT_EQ("", ss.str());
}