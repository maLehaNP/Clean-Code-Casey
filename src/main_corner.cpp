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

  #ifdef __AVX512F__
  u32 FunctionCount = 13;
  #else
  u32 FunctionCount = 10;
  #endif // __AVX512F__

  CornerAreaFunction* Functions = new CornerAreaFunction[FunctionCount];
  Functions[0]  = { CornerArea_P2P, CornerAreaVTBL,  nullptr, "CornerAreaVTBL"    };
  Functions[1]  = { CornerArea_P2P, CornerAreaVTBL4, nullptr, "CornerAreaVTBL4"   };
  Functions[2]  = { CornerArea_P, nullptr, CornerAreaSwitch,  "CornerAreaSwitch"  };
  Functions[3]  = { CornerArea_P, nullptr, CornerAreaSwitch4, "CornerAreaSwitch4" };
  Functions[4]  = { CornerArea_P, nullptr, CornerAreaTable,   "CornerAreaTable"   };
  Functions[5]  = { CornerArea_P, nullptr, CornerAreaTable4,  "CornerAreaTable4"  };
  Functions[6]  = { CornerArea_P, nullptr, CornerAreaSSE,     "CornerAreaSSE"     };
  Functions[7]  = { CornerArea_P, nullptr, CornerAreaSSE4,    "CornerAreaSSE4"    };
  Functions[8]  = { CornerArea_P, nullptr, CornerAreaSSE_2,   "CornerAreaSSE_2"   };
  Functions[9]  = { CornerArea_P, nullptr, CornerAreaSSE4_2,  "CornerAreaSSE4_2"  };
  #ifdef __AVX512F__
  Functions[10] = { CornerArea_P, nullptr, CornerAreaAVX,     "CornerAreaAVX"     };
  Functions[11] = { CornerArea_P, nullptr, CornerAreaAVX4,    "CornerAreaAVX4"    };
  Functions[12] = { CornerArea_P, nullptr, CornerAreaAVX4_2,  "CornerAreaAVX4_2"  };
  #endif // __AVX512F__

  if (argc == 2) {
    if      (strcmp(argv[1], "CornerAreaVTBL4")   == 0) FunctionCount = 2;
    else if (strcmp(argv[1], "CornerAreaSwitch4") == 0) FunctionCount = 4;
    else if (strcmp(argv[1], "CornerAreaTable4")  == 0) FunctionCount = 6;
    else {
      printf("Wrong argument\n");
      return -1;
    }
  }
  else if (argc > 2) {
    printf("Too many arguments\n");
    return -1;
  }

  printf("\n");

  clock_t t;
  f32 PerfAccum;
  f32 perf[FunctionCount];

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

  for (int ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex)
    delete Shapes[ShapeIndex];
  delete[] Shapes;
  delete[] Shapes2;
  delete[] Functions;

  printf("Press Enter to exit...");
  getchar();

  return 0;
}

