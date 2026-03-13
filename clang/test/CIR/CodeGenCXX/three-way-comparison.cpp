// RUN: %clang_cc1 -std=c++20 -triple x86_64-unknown-linux-gnu -fclangir -emit-cir %s -o %t.cir
// RUN: FileCheck --input-file=%t.cir %s

#include "Inputs/std-compare.h"

int test_int_spaceship(int a, int b) {
  auto result = a <=> b;
  // CHECK: cir.cmp3way(%{{.*}} : !s32i, %{{.*}}, #cir.cmp3way_strong_info<strong, lt = -1, eq = 0, gt = 1>) : !s32i
  return (result < 0) ? -1 : (result > 0) ? 1 : 0;
}

unsigned int test_uint_spaceship(unsigned int a, unsigned int b) {
  auto result = a <=> b;
  // CHECK: cir.cmp3way(%{{.*}} : !u32i, %{{.*}}, #cir.cmp3way_strong_info<strong, lt = -1, eq = 0, gt = 1>) : !s32i
  return (result < 0) ? 0 : (result > 0) ? 2 : 1;
}

float test_float_spaceship(float a, float b) {
  auto result = a <=> b;
  // CHECK: cir.cmp3way(%{{.*}} : !cir.float, %{{.*}}, #cir.cmp3way_partial_info<partial, lt = -1, eq = 0, gt = 1, unordered = -127>)
  return (result < 0) ? -1.0f : (result > 0) ? 1.0f : 0.0f;
}