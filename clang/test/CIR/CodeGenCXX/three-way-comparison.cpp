// RUN: %clang_cc1 -std=c++20 -triple x86_64-unknown-linux-gnu -fclangir -emit-cir %s -o %t.cir
// RUN: FileCheck --input-file=%t.cir %s -check-prefix=CIR
// RUN: %clang_cc1 -std=c++20 -triple x86_64-unknown-linux-gnu -fclangir -emit-llvm %s -o %t.ll
// RUN: FileCheck --input-file=%t.ll %s -check-prefix=LLVM
// RUN: %clang_cc1 -std=c++20 -triple x86_64-unknown-linux-gnu -emit-llvm %s -o %t.ll
// RUN: FileCheck --input-file=%t.ll %s -check-prefix=OGCG

#include "../../CodeGenCXX/Inputs/std-compare.h"

int test_int_spaceship(int a, int b) {
  auto result = a <=> b;
  return (result < 0) ? -1 : (result > 0) ? 1 : 0;
  // CIR: cir.cmp3way(%{{.*}} : !s32i, %{{.*}}, #cir.cmp3way_strong_info<strong, lt = -1, eq = 0, gt = 1>) : !s32i
  // LLVM: %[[CMP_LT:.*]] = icmp slt i32 %{{.*}}, %{{.*}}
  // LLVM: %[[CMP_EQ:.*]] = icmp eq i32 %{{.*}}, %{{.*}}
  // LLVM: %[[SEL1:.*]] = select i1 %[[CMP_EQ]], i32 0, i32 1
  // LLVM: %[[SEL2:.*]] = select i1 %[[CMP_LT]], i32 -1, i32 %[[SEL1]]
  // OGCG: %[[CMP_LT:.*]] = icmp slt i32 %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_LT:.*]] = select i1 %[[CMP_LT]], i8 -1, i8 1
  // OGCG: %[[CMP_EQ:.*]] = icmp eq i32 %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_EQ:.*]] = select i1 %[[CMP_EQ]], i8 0, i8 %[[SEL_LT]]
}

unsigned int test_uint_spaceship(unsigned int a, unsigned int b) {
  auto result = a <=> b;
  return (result < 0) ? 0 : (result > 0) ? 2 : 1;
  // CIR: cir.cmp3way(%{{.*}} : !u32i, %{{.*}}, #cir.cmp3way_strong_info<strong, lt = -1, eq = 0, gt = 1>) : !s32i
  // LLVM: %[[CMP_LT:.*]] = icmp ult i32 %{{.*}}, %{{.*}}
  // LLVM: %[[CMP_EQ:.*]] = icmp eq i32 %{{.*}}, %{{.*}}
  // LLVM: %[[SEL1:.*]] = select i1 %[[CMP_EQ]], i32 0, i32 1
  // LLVM: %[[SEL2:.*]] = select i1 %[[CMP_LT]], i32 -1, i32 %[[SEL1]]
  // OGCG: %[[CMP_LT:.*]] = icmp ult i32 %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_LT:.*]] = select i1 %[[CMP_LT]], i8 -1, i8 1
  // OGCG: %[[CMP_EQ:.*]] = icmp eq i32 %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_EQ:.*]] = select i1 %[[CMP_EQ]], i8 0, i8 %[[SEL_LT]]
}

float test_float_spaceship(float a, float b) {
  auto result = a <=> b;
  return (result < 0) ? -1.0f : (result > 0) ? 1.0f : 0.0f;
  // CIR: cir.cmp3way(%{{.*}} : !cir.float, %{{.*}}, #cir.cmp3way_partial_info<partial, lt = -1, eq = 0, gt = 1, unordered = -127>)
  // LLVM: %[[CMP_LT:.*]] = fcmp olt float %{{.*}}, %{{.*}}
  // LLVM: %[[CMP_EQ:.*]] = fcmp oeq float %{{.*}}, %{{.*}}
  // LLVM: %[[SEL1:.*]] = select i1 %[[CMP_EQ]], i32 0, i32 1
  // LLVM: %[[SEL2:.*]] = select i1 %[[CMP_LT]], i32 -1, i32 %[[SEL1]]
  // LLVM: %[[CMP_UNO:.*]] = fcmp uno float %{{.*}}, %{{.*}}
  // LLVM: %[[SEL3:.*]] = select i1 %[[CMP_UNO]], i32 -127, i32 %[[SEL2]]
  // OGCG: %[[CMP_EQ:.*]] = fcmp oeq float %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_EQ:.*]] = select i1 %[[CMP_EQ]], i8 0, i8 -127
  // OGCG: %[[CMP_GT:.*]] = fcmp ogt float %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_GT:.*]] = select i1 %[[CMP_GT]], i8 1, i8 %[[SEL_EQ]]
  // OGCG: %[[CMP_LT:.*]] = fcmp olt float %{{.*}}, %{{.*}}
  // OGCG: %[[SEL_LT:.*]] = select i1 %[[CMP_LT]], i8 -1, i8 %[[SEL_GT]]
}
