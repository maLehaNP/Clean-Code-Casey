#include <iostream>
#include <ctime>
#include <cstring>
#include "common.h"
#include "shapes.h"
#include "corner_area.h"


int main(int argc, char *argv[]) {
  const u32 ShapeCount = 1048576;  // 2^20, 0x100000
  const u32 Repeats[] = { 1, 1000 };

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
  shape_union* Shapes2 = new shape_union[ShapeCount];
  for (int ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex) {
    switch (ShapeIndex % 4) {
      case 0: Shapes2[ShapeIndex] = shape_union{ Shape_Square,    1.0f, 1.0f }; break;
      case 1: Shapes2[ShapeIndex] = shape_union{ Shape_Rectangle, 1.0f, 1.0f }; break;
      case 2: Shapes2[ShapeIndex] = shape_union{ Shape_Triangle,  1.0f, 1.0f }; break;
      case 3: Shapes2[ShapeIndex] = shape_union{ Shape_Circle,    1.0f, 1.0f }; break;
      default: break;
    }
  }

  u32 FunctionCount;
  CornerAreaFunction* Functions = new CornerAreaFunction[6];
  if (argc > 1) {
    if      (strcmp(argv[1], "CornerAreaVTBL4")   == 0) {
      FunctionCount = 2;
      Functions[0] = { CornerArea_P2P, CornerAreaVTBL,  nullptr, "CornerAreaVTBL"  };
      Functions[1] = { CornerArea_P2P, CornerAreaVTBL4, nullptr, "CornerAreaVTBL4" };
    }
    else if (strcmp(argv[1], "CornerAreaSwitch4") == 0) {
      FunctionCount = 4;
      Functions[0] = { CornerArea_P2P, CornerAreaVTBL,  nullptr, "CornerAreaVTBL"    };
      Functions[1] = { CornerArea_P2P, CornerAreaVTBL4, nullptr, "CornerAreaVTBL4"   };
      Functions[2] = { CornerArea_P, nullptr, CornerAreaSwitch,  "CornerAreaSwitch"  };
      Functions[3] = { CornerArea_P, nullptr, CornerAreaSwitch4, "CornerAreaSwitch4" };
    }
    else if (strcmp(argv[1], "CornerAreaTable4")  == 0) {
      FunctionCount = 6;
      Functions[0] = { CornerArea_P2P, CornerAreaVTBL,  nullptr, "CornerAreaVTBL"    };
      Functions[1] = { CornerArea_P2P, CornerAreaVTBL4, nullptr, "CornerAreaVTBL4"   };
      Functions[2] = { CornerArea_P, nullptr, CornerAreaSwitch,  "CornerAreaSwitch"  };
      Functions[3] = { CornerArea_P, nullptr, CornerAreaSwitch4, "CornerAreaSwitch4" };
      Functions[4] = { CornerArea_P, nullptr, CornerAreaTable,   "CornerAreaTable"   };
      Functions[5] = { CornerArea_P, nullptr, CornerAreaTable4,  "CornerAreaTable4"  };
    }
    else {
      printf("Wrong argument\n");
      return -1;
    }
  }

  printf("\n");

  clock_t t;
  f32 PerfAccum;
  f32 perf[4];

  for (u32 r = 0; r < 2; ++r) {
    u32 RepeatCount = Repeats[r];

    printf("Repeat Count: %d\n", RepeatCount);
    printf("\n");

    for (u32 f = 0; f < FunctionCount; ++f) {
      printf("%22s(%8d): ", Functions[f].name, ShapeCount);
      fflush(stdout);

      PerfAccum = 0;
      for (int i = 0; i < RepeatCount; ++i) {
        switch (Functions[f].Type) {
          case CornerArea_P2P:
            t = clock();
            Functions[f].fun1(ShapeCount, Shapes);
            PerfAccum += (f32)(clock() - t) / ShapeCount;
            break;
          case CornerArea_P:
            t = clock();
            Functions[f].fun2(ShapeCount, Shapes2);
            PerfAccum += (f32)(clock() - t) / ShapeCount;
            break;
        }
      }

      perf[f] = PerfAccum / RepeatCount;
      printf("%10.6f cycles/shape\n", perf[f]);
    }
    printf("\n");

    for (u32 f = 0; f < FunctionCount; ++f)
      printf("%22s: %7.3fx\n", Functions[f].name, perf[0] / perf[f]);
    printf("\n");
  }

  printf("Press Enter to exit...");
  getchar();

  return 0;
}

