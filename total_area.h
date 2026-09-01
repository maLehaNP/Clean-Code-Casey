#pragma once
#include "common.h"
#include "shapes.h"


/*
struct TotalAreaP2P_s {
  f32 (*function)(u32, shape_base**);
  const char* name;
};

struct TotalAreaP_s {
  f32 (*function)(u32, shape_union*);
  const char* name;
};
*/

enum TotalArea_t {
  TotalArea_P2P,
  TotalArea_P,
};

struct TotalAreaFunction {
  TotalArea_t Type;
  f32 (*fun1)(u32, shape_base**);
  f32 (*fun2)(u32, shape_union*);
  const char* name;
};

f32 TotalAreaVTBL(u32 ShapeCount, shape_base **Shapes) {
  f32 Accum = 0.0f;
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex) {
    Accum += Shapes[ShapeIndex]->Area();
  }
  return Accum;
}

f32 TotalAreaVTBL4(u32 ShapeCount, shape_base **Shapes) {
  f32 Accum0 = 0.0f;
  f32 Accum1 = 0.0f;
  f32 Accum2 = 0.0f;
  f32 Accum3 = 0.0f;

  u32 Count = ShapeCount/4;

  while (Count--) {
    Accum0 += Shapes[0]->Area();
    Accum1 += Shapes[1]->Area();
    Accum2 += Shapes[2]->Area();
    Accum3 += Shapes[3]->Area();

    Shapes += 4;
  }

  f32 Result = (Accum0 + Accum1 + Accum2 + Accum3);
  return Result;
}



f32 TotalAreaSwitch(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum = 0.0f;
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex)
    Accum += GetAreaSwitch(Shapes[ShapeIndex]);
  return Accum;
}

f32 TotalAreaSwitch4(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum0 = 0.0f;
  f32 Accum1 = 0.0f;
  f32 Accum2 = 0.0f;
  f32 Accum3 = 0.0f;
  u32 Count = ShapeCount/4;
  while (Count--) {
    Accum0 += GetAreaSwitch(Shapes[0]);
    Accum1 += GetAreaSwitch(Shapes[1]);
    Accum2 += GetAreaSwitch(Shapes[2]);
    Accum3 += GetAreaSwitch(Shapes[3]);
    Shapes += 4;
  }
  f32 Result = (Accum0 + Accum1 + Accum2 + Accum3);
  return Result;
}


f32 TotalAreaTable(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum = 0.0f;
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex)
    Accum += GetAreaUnion(Shapes[ShapeIndex]);
  return Accum;
}

f32 TotalAreaTable4(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum0 = 0.0f;
  f32 Accum1 = 0.0f;
  f32 Accum2 = 0.0f;
  f32 Accum3 = 0.0f;
  u32 Count = ShapeCount/4;
  while (Count--) {
    Accum0 += GetAreaUnion(Shapes[0]);
    Accum1 += GetAreaUnion(Shapes[1]);
    Accum2 += GetAreaUnion(Shapes[2]);
    Accum3 += GetAreaUnion(Shapes[3]);
    Shapes += 4;
  }
  f32 Result = (Accum0 + Accum1 + Accum2 + Accum3);
  return Result;
}

