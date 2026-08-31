#include <iostream>
#include <ctime>
#include <cstring>

#include "common.h"
#include "shapes.h"
#include "total_area.h"


int main(int argc, char *argv[]) {

  u32 ShapeCount = 1048576;  // 2^20, 0x100000
  shape_base** Shapes = new shape_base* [ShapeCount];

  u32 Repeats[] = { 1, 1000 };

  u32 FunctionCount;
  TotalAreaFunction Functions[4];
  //printf("%d %d\n", strlen(argv[1]), strlen("TotalAreaVTBL4"));
  //printf("%d\n", strcmp(argv[1], "TotalAreaVTBL4"));
  if (argc > 1) {
    if (strcmp(argv[1], "TotalAreaVTBL4") == 0) {
      FunctionCount = 2;
      Functions[0] = {TotalAreaVTBL, "TotalAreaVTBL"};
      Functions[1] = {TotalAreaVTBL4, "TotalAreaVTBL4"};
    }
    else
      return -1;
  }

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
  f32 PerfAccum;
  f32 perf[2];

  for (u32 r = 0; r < 2; ++r) {
    u32 RepeatCount = Repeats[r];
    printf("Repeat Count: %d\n", RepeatCount);
    printf("\n");

    for (u32 f = 0; f < FunctionCount; ++f) {
      printf("        %14s(%8d): ", Functions[f].name, ShapeCount);
      fflush(stdout);
      PerfAccum = 0;
      for (int i = 0; i < RepeatCount; ++i) {
        t = clock();
        Functions[f].function(ShapeCount, Shapes);
        PerfAccum += (f32)(clock() - t) / ShapeCount;
      }
      perf[f] = PerfAccum / RepeatCount;
      printf("%10.6f cycles/shape (%f - %f = %f)\n", perf[f], 0.0f, 0.0f, 0.0f);
    }
    printf("\n");

    for (u32 f = 0; f < FunctionCount; ++f)
      printf("        %14s: %7.3fx\n", Functions[f].name, perf[0] / perf[f]);

    printf("\n");
  }


  return 0;
}

