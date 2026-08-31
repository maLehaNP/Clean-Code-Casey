#include <iostream>
#include <ctime>

#include "shapes.h"


typedef float f32;
typedef unsigned int u32;


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


int main(int argc, char *argv[]) {
  printf("Number of command line parameters = %d\n", argc);
  printf("Parameters:\n");
  for (int counter = 0; counter < argc; counter++)
    printf("%s\n", argv[counter]);

  //if (argc > 1 && argv[1] == "TotalAreaVTBL4") {

  u32 ShapeCount = 1048576;  // 2^20, 0x100000
  u32 Repeats[] = { 1, 1000 };

  //f32 (*Functions[2])(u32, shape_base**) = { TotalAreaVTBL, TotalAreaVTBL4 };

  struct TotalAreaFunction {
    f32 (*function)(u32, shape_base**);
    const char* name;
  };

  TotalAreaFunction Functions[2] = {
    {TotalAreaVTBL, "TotalAreaVTBL"},
    {TotalAreaVTBL4, "TotalAreaVTBL4"}
  };

  shape_base** Shapes = new shape_base* [ShapeCount];
  for (int ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex) {
    switch (ShapeIndex % 4) {
      case 0: Shapes[ShapeIndex] = new square(1.0f); break;
      case 1: Shapes[ShapeIndex] = new rectangle(1.0f, 1.0f); break;
      case 2: Shapes[ShapeIndex] = new triangle(1.0f, 1.0f); break;
      case 3: Shapes[ShapeIndex] = new circle(1.0f); break;
      default: break;
    }
  }

  printf("\n");

  clock_t t;
  //f32 perf1, perf2;
  f32 PerfAccum;
  f32 perf[2];

  for (u32 r = 0; r < 2; ++r) {
    u32 RepeatCount = Repeats[r];
    printf("Repeat Count: %d\n", RepeatCount);
    printf("\n");

    for (u32 f = 0; f < 2; ++f) {
      //printf("         TotalAreaFun%d(%8d): ", f, ShapeCount);
      printf("        %14s(%8d): ", Functions[f].name, ShapeCount);
      fflush(stdout);
      PerfAccum = 0;
      for (int i = 0; i < RepeatCount; ++i) {
        t = clock();
        //Functions[f](ShapeCount, Shapes);
        Functions[f].function(ShapeCount, Shapes);
        PerfAccum += (f32)(clock() - t) / ShapeCount;
      }
      perf[f] = PerfAccum / RepeatCount;
      printf("%10.6f cycles/shape (%f - %f = %f)\n", perf[f], 0.0f, 0.0f, 0.0f);
    }
    printf("\n");

    //printf("         TotalAreaVTBL: %7.3fx\n", perf[0] / perf[0]);
    //printf("        TotalAreaVTBL4: %7.3fx\n", perf[0] / perf[1]);
    for (u32 f = 0; f < 2; ++f) {
      printf("        %14s: %7.3fx\n", Functions[f].name, perf[0] / perf[f]);
    }

    printf("\n");
  }

  //}
  //else
  //  return -1;

  return 0;
}

