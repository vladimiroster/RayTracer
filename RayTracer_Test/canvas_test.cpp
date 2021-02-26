#include "pch.h"
#include "../RayTracerLib/Canvas.h"
#include "../RayTracerLib/Tuple.h"

#include <iostream>

using namespace RayTracer;

TEST(TestCanvas, TestCanvasCreate) {
  Canvas c(10,20);
  EXPECT_EQ(10, c.width());
  EXPECT_EQ(20, c.height());
  for (int i = 0; i < c.width(); ++i) {
    for (int j = 0; j < c.height(); ++j) {
      EXPECT_EQ(Color(0.0f, 0.0f, 0.0f), c.pixel(i,j));
    }
  }
}

TEST(TestCanvas, TestPPMHeader) {
  Canvas c(5, 3);
  std::stringstream s;
  s << c;

  s.clear();
  s.seekg(0);

  std::string val;
  EXPECT_TRUE(std::getline(s, val, '\n'));
  EXPECT_EQ("P3", val);
  EXPECT_TRUE(std::getline(s, val, '\n'));
  EXPECT_EQ("5 3", val);
  EXPECT_TRUE(std::getline(s, val, '\n'));
  EXPECT_EQ("255", val);
}

TEST(TestCanvas, TestColorNeg) {
  EXPECT_EQ(0, toOutputColor(-4.0f));
}

TEST(TestCanvas, TestColorZero) {
  EXPECT_EQ(0, toOutputColor(0.0f));
}

TEST(TestCanvas, TestColorReg) {
  EXPECT_EQ(191, toOutputColor(0.75f));
}

TEST(TestCanvas, TestColorMax) {
  EXPECT_EQ(255, toOutputColor(1.0f));
}

TEST(TestCanvas, TestColorOver) {
  EXPECT_EQ(255, toOutputColor(4.0f));
}

TEST(TestCanvas, TestPixelOutput) {
  Canvas c(5,3);
  Color c1(1.5f, 0.0f, 0.0f), c2(0.0f, 0.5f, 0.0f), c3(-0.5f, 0.0f, 1.0f);

  c.pixel(0, 0) = c1;
  c.pixel(2, 1) = c2;
  c.pixel(4, 2) = c3;

  std::stringstream s;
  s << c;

  s.clear();
  s.seekg(11); // Skip the header

  std::string val;
  EXPECT_TRUE(std::getline(s, val, '\n'));
  EXPECT_EQ("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0", val);
  EXPECT_TRUE(std::getline(s, val, '\n'));
  EXPECT_EQ("0 0 0 0 0 0 0 128 0 0 0 0 0 0 0", val);
  EXPECT_TRUE(std::getline(s, val, '\n'));
  EXPECT_EQ("0 0 0 0 0 0 0 0 0 0 0 0 0 0 255", val);
}

// Not worth the time developing this feature
//TEST(TestCanvas, TestLongPixelOutput) {
//  Canvas c(10,2);
//  
//  for (size_t i = 0; i < c.width; ++i) {
//    for (size_t j = 0; j < c.height; ++j) {
//      c.pixel(i, j) = Color(1.0f, 0.8f, 0.6f);
//    }
//  }
//
//  std::stringstream s;
//  s << c;
//
//  s.clear();
//  s.seekg(12); // Skip the header
//
//  std::string val;
//  EXPECT_TRUE(std::getline(s, val, '\n'));
//  EXPECT_EQ("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 ", val);
//  EXPECT_TRUE(std::getline(s, val, '\n'));
//  EXPECT_EQ("153 255 204 153 255 204 153 255 204 153 255 204 153", val);
//  EXPECT_TRUE(std::getline(s, val, '\n'));
//  EXPECT_EQ("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 ", val);
//  EXPECT_TRUE(std::getline(s, val, '\n'));
//  EXPECT_EQ("153 255 204 153 255 204 153 255 204 153 255 204 153", val);
//}

TEST(TestCanvas, TestNLTerminator) {
  Canvas c(5, 3);
  std::stringstream s;
  s << c;
  s.clear();
  s.seekg(-1, std::ios_base::end);
  char val;
  s.get(val);
  EXPECT_EQ('\n', val);
}
