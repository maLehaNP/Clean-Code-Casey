#pragma once
#include "common.h"
#include "shapes.h"

//#include <xmmintrin.h>  // __m128 functions
//#include <intrin.h>  // Такого нет
//#include <emmintrin.h>  // Double-precision functions
//#include <pmmintrin.h>  // Have _mm_hadd_ps
#include <immintrin.h>  // Include a lot of intrinsics headers


enum CornerArea_t {
  CornerArea_P2P,
  CornerArea_P,
};

struct CornerAreaFunction {
  CornerArea_t Type;
  f32 (*fun1)(u32, shape_base**);
  f32 (*fun2)(u32, shape_union*);
  const char* name;
};

f32 CornerAreaVTBL(u32 ShapeCount, shape_base **Shapes) {
  f32 Accum = 0.0f;
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex) {
    Accum += (1.0f / (1.0f + (f32)Shapes[ShapeIndex]->CornerCount())) * Shapes[ShapeIndex]->Area();
  }
  return Accum;
}

f32 CornerAreaVTBL4(u32 ShapeCount, shape_base **Shapes) {
  f32 Accum0 = 0.0f;
  f32 Accum1 = 0.0f;
  f32 Accum2 = 0.0f;
  f32 Accum3 = 0.0f;

  u32 Count = ShapeCount/4;

  while (Count--) {
    Accum0 += (1.0f / (1.0f + (f32)Shapes[0]->CornerCount())) * Shapes[0]->Area();
    Accum1 += (1.0f / (1.0f + (f32)Shapes[1]->CornerCount())) * Shapes[1]->Area();
    Accum2 += (1.0f / (1.0f + (f32)Shapes[2]->CornerCount())) * Shapes[2]->Area();
    Accum3 += (1.0f / (1.0f + (f32)Shapes[3]->CornerCount())) * Shapes[3]->Area();

    Shapes += 4;
  }

  f32 Result = (Accum0 + Accum1 + Accum2 + Accum3);
  return Result;
}



f32 CornerAreaSwitch(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum = 0.0f;
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex)
    Accum += (1.0f / (1.0f + (f32)GetCornerCountSwitch(Shapes[ShapeIndex].Type))) * GetAreaSwitch(Shapes[ShapeIndex]);
  return Accum;
}

f32 CornerAreaSwitch4(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum0 = 0.0f;
  f32 Accum1 = 0.0f;
  f32 Accum2 = 0.0f;
  f32 Accum3 = 0.0f;
  u32 Count = ShapeCount/4;
  while (Count--) {
    Accum0 += (1.0f / (1.0f + (f32)GetCornerCountSwitch(Shapes[0].Type))) * GetAreaSwitch(Shapes[0]);
    Accum1 += (1.0f / (1.0f + (f32)GetCornerCountSwitch(Shapes[1].Type))) * GetAreaSwitch(Shapes[1]);
    Accum2 += (1.0f / (1.0f + (f32)GetCornerCountSwitch(Shapes[2].Type))) * GetAreaSwitch(Shapes[2]);
    Accum3 += (1.0f / (1.0f + (f32)GetCornerCountSwitch(Shapes[3].Type))) * GetAreaSwitch(Shapes[3]);
    Shapes += 4;
  }
  f32 Result = (Accum0 + Accum1 + Accum2 + Accum3);
  return Result;
}


f32 CornerAreaTable(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum = 0.0f;
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex)
    Accum += GetCornerAreaUnion(Shapes[ShapeIndex]);
  return Accum;
}

f32 CornerAreaTable4(u32 ShapeCount, shape_union* Shapes) {
  f32 Accum0 = 0.0f;
  f32 Accum1 = 0.0f;
  f32 Accum2 = 0.0f;
  f32 Accum3 = 0.0f;
  u32 Count = ShapeCount/4;
  while (Count--) {
    Accum0 += GetCornerAreaUnion(Shapes[0]);
    Accum1 += GetCornerAreaUnion(Shapes[1]);
    Accum2 += GetCornerAreaUnion(Shapes[2]);
    Accum3 += GetCornerAreaUnion(Shapes[3]);
    Shapes += 4;
  }
  f32 Result = (Accum0 + Accum1 + Accum2 + Accum3);
  return Result;
}


// AVX routines

f32 CornerAreaSSE(u32 ShapeCount, shape_union* Shapes) {
  __m128 Accum = _mm_setzero_ps();

  //__m128 Area = _mm_setzero_ps();
  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount - 4; ShapeIndex += 4) {
    //Area = _mm_set_ps(
    //  GetCornerAreaUnion(Shapes[ShapeIndex]),
    //  GetCornerAreaUnion(Shapes[ShapeIndex + 1]),
    //  GetCornerAreaUnion(Shapes[ShapeIndex + 2]),
    //  GetCornerAreaUnion(Shapes[ShapeIndex + 3])
    //);
    //Accum = _mm_add_ps(Accum, Area);
    Accum = _mm_add_ps(Accum, _mm_set_ps(
      GetCornerAreaUnion(Shapes[ShapeIndex]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 1]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 2]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 3])
    ));
  }

  f32 vresult[4];
  _mm_storeu_ps(vresult, Accum);

  //f32 Result = 0.0f;
  //for (u32 i = 0; i < 4; i++) {
  //  Result += vresult[i];
  //}

  f32 Result = (vresult[0] + vresult[1] + vresult[2] + vresult[3]);
  return Result;
}

f32 CornerAreaSSE_2(u32 ShapeCount, shape_union* Shapes) {
  __m128 Accum = _mm_setzero_ps();

  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount - 4; ShapeIndex += 4) {
    __m128 r1 = _mm_set_ps(
      CTable_Corner[Shapes[ShapeIndex+0].Type],
      CTable_Corner[Shapes[ShapeIndex+1].Type],
      CTable_Corner[Shapes[ShapeIndex+2].Type],
      CTable_Corner[Shapes[ShapeIndex+3].Type]
    );
    __m128 r2 = _mm_set_ps(
      Shapes[ShapeIndex+0].Width,
      Shapes[ShapeIndex+1].Width,
      Shapes[ShapeIndex+2].Width,
      Shapes[ShapeIndex+3].Width
    );
    __m128 r3 = _mm_set_ps(
      Shapes[ShapeIndex+0].Height,
      Shapes[ShapeIndex+1].Height,
      Shapes[ShapeIndex+2].Height,
      Shapes[ShapeIndex+3].Height
    );

    Accum = _mm_add_ps(Accum, _mm_mul_ps(_mm_mul_ps(r1, r2), r3));
  }

  f32 vresult[4];
  _mm_storeu_ps(vresult, Accum);
  f32 Result = (vresult[0] + vresult[1] + vresult[2] + vresult[3]);
  return Result;
}

f32 CornerAreaSSE4(u32 ShapeCount, shape_union* Shapes) {
  __m128 Accum = _mm_setzero_ps();
  u32 Count = ShapeCount/4;
  while (Count--) {
    Accum = _mm_add_ps(Accum, _mm_set_ps(
      GetCornerAreaUnion(Shapes[0]),
      GetCornerAreaUnion(Shapes[1]),
      GetCornerAreaUnion(Shapes[2]),
      GetCornerAreaUnion(Shapes[3])
    ));
    Shapes += 4;
  }
  f32 vresult[4];
  _mm_storeu_ps(vresult, Accum);
  f32 Result = (vresult[0] + vresult[1] + vresult[2] + vresult[3]);
  return Result;
}

f32 CornerAreaSSE4_2(u32 ShapeCount, shape_union* Shapes) {
  __m128 Accum = _mm_setzero_ps();
  u32 Count = ShapeCount/4;
  while (Count--) {
    __m128 r1 = _mm_set_ps(
      CTable_Corner[Shapes[0].Type], CTable_Corner[Shapes[1].Type],
      CTable_Corner[Shapes[2].Type], CTable_Corner[Shapes[3].Type]
    );
    __m128 r2 = _mm_set_ps(
      Shapes[0].Width, Shapes[1].Width, Shapes[2].Width, Shapes[3].Width
    );
    __m128 r3 = _mm_set_ps(
      Shapes[0].Height, Shapes[1].Height, Shapes[2].Height, Shapes[3].Height
    );

    Accum = _mm_add_ps(Accum, _mm_mul_ps(_mm_mul_ps(r1, r2), r3));

    Shapes += 4;
  }
  f32 vresult[4];
  _mm_storeu_ps(vresult, Accum);
  f32 Result = (vresult[0] + vresult[1] + vresult[2] + vresult[3]);
  return Result;
}


f32 CornerAreaAVX(u32 ShapeCount, shape_union* Shapes) {
  __m512 Accum = _mm512_setzero_ps();

  for (u32 ShapeIndex = 0; ShapeIndex < ShapeCount - 16; ShapeIndex += 16) {
    Accum = _mm512_add_ps(Accum, _mm512_set_ps(
      GetCornerAreaUnion(Shapes[ShapeIndex + 0]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 1]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 2]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 3]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 4]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 5]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 6]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 7]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 8]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 9]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 10]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 11]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 12]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 13]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 14]),
      GetCornerAreaUnion(Shapes[ShapeIndex + 15])
    ));
  }

  return _mm512_reduce_add_ps(Accum);
}

f32 CornerAreaAVX4(u32 ShapeCount, shape_union* Shapes) {
  __m512 Accum = _mm512_setzero_ps();
  u32 Count = ShapeCount/16;
  while (Count--) {
    Accum = _mm512_add_ps(Accum, _mm512_set_ps(
      GetCornerAreaUnion(Shapes[0]),  GetCornerAreaUnion(Shapes[1]),
      GetCornerAreaUnion(Shapes[2]),  GetCornerAreaUnion(Shapes[3]),
      GetCornerAreaUnion(Shapes[4]),  GetCornerAreaUnion(Shapes[5]),
      GetCornerAreaUnion(Shapes[6]),  GetCornerAreaUnion(Shapes[7]),
      GetCornerAreaUnion(Shapes[8]),  GetCornerAreaUnion(Shapes[9]),
      GetCornerAreaUnion(Shapes[10]), GetCornerAreaUnion(Shapes[11]),
      GetCornerAreaUnion(Shapes[12]), GetCornerAreaUnion(Shapes[13]),
      GetCornerAreaUnion(Shapes[14]), GetCornerAreaUnion(Shapes[15])
    ));
    Shapes += 16;
  }
  return _mm512_reduce_add_ps(Accum);
}

enum shape_attr {
  ShapeAttr_Type,
  ShapeAttr_Width,
  ShapeAttr_Height
};

inline __m512 GetShapeAttrVec(shape_union* Shapes, shape_attr Attr) {
  f32 v[16];
  switch (Attr) {
    case ShapeAttr_Type: {
      for (int i = 0; i < 16; ++i)
        v[i] = CTable_Corner[Shapes[i].Type];
      return _mm512_loadu_ps(v);
      break;
    }
    case ShapeAttr_Width: {
      for (int i = 0; i < 16; ++i)
        v[i] = Shapes[i].Width;
      return _mm512_loadu_ps(v);
      break;
    }
    case ShapeAttr_Height: {
      for (int i = 0; i < 16; ++i)
        v[i] = Shapes[i].Height;
      return _mm512_loadu_ps(v);
      break;
    }
  }
  return _mm512_setzero_ps();
}

f32 CornerAreaAVX4_2(u32 ShapeCount, shape_union* Shapes) {
  __m512 Accum = _mm512_setzero_ps();
  u32 Count = ShapeCount/16;
  while (Count--) {
    __m512 r1 = GetShapeAttrVec(Shapes, ShapeAttr_Type);
    __m512 r2 = GetShapeAttrVec(Shapes, ShapeAttr_Width);
    __m512 r3 = GetShapeAttrVec(Shapes, ShapeAttr_Height);
    Accum = _mm512_add_ps(Accum, _mm512_mul_ps(_mm512_mul_ps(r1, r2), r3));
    Shapes += 16;
  }
  return _mm512_reduce_add_ps(Accum);
}

