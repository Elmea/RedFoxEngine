#include "pch.h"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedFoxMaths.hpp"
using namespace RedFoxMaths;

#pragma region Float2Test

TEST(Float2, CrossProductTest) {
  float expected = glm_vec2_cross(new(vec2){2.f,3.f}, new(vec2){7.f,8.f});
  Float2 a = {2.f,3.f}, b = {7.f,8.f};
  float result = a.CrossProduct(b);
  EXPECT_EQ(result, expected);
  EXPECT_TRUE(true);
}

TEST(Float2, DotProductTest) {
  float expected = glm_vec2_dot(new(vec2){2.f,3.f}, new(vec2){7.f,8.f});
  Float2 a = {2.f,3.f}, b = {7.f,8.f};
  float result = a.DotProduct(b);
  EXPECT_EQ(result, expected);
  EXPECT_TRUE(true);
}

TEST(Float2, PlanRotationTest) {
  vec2s expected;
  glm_vec2_rotate(new(vec2){2.f,3.f}, PI/2, expected.raw);
  float angle = 90.f;
  Float2 result = {2.f,3.f};
  result.PlanRotation(angle);
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

TEST(Float2, MagnitudeTest) {
  float expected = glm_vec2_norm(new(vec2){2.f,3.f});
  Float2 a = {2.f,3.f};
  float result = a.Magnitude();
  EXPECT_EQ(result, expected);
  EXPECT_TRUE(true);
}

TEST(Float2, NormalizeTest) {
  vec2s expected = { 2.f,3.f };
  glm_vec2_normalize(expected.raw);
  Float2 a = {2.f,3.f};
  Float2 result = a.GetNormalized();
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

TEST(Float2, LerpTest) {
  vec2s expected = {0};
  glm_vec2_lerp(new(vec2){2.f,3.f}, new(vec2){4.f,5.f}, 0.5f, expected.raw);
  Float2 result = Float2::Lerp({2.f,3.f}, {4.f,5.f}, 0.5f);
  EXPECT_EQ(result.x, expected.x);
  EXPECT_EQ(result.y, expected.y);
  EXPECT_TRUE(true);
}

#pragma endregion