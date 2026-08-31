#include <iostream>
#include <ctime>


typedef float f32;
typedef unsigned int u32;
const f32 Pi32 = 3.141592f;


class shape_base {
public:
  shape_base() {}
  virtual f32 Area() = 0;
};

class square : public shape_base {
public:
  square(f32 SideInit) : Side(SideInit) {}
  virtual f32 Area() { return Side * Side; }

private:
  f32 Side;
};

class rectangle : public shape_base {
public:
  rectangle(f32 WidthInit, f32 HeightInit)
    : Width(WidthInit), Height(HeightInit) {}
  virtual f32 Area() { return Width * Height; }

private:
  f32 Width, Height;
};

class triangle : public shape_base {
public:
  triangle(f32 BaseInit, f32 HeightInit)
    : Base(BaseInit), Height(HeightInit) {}
  virtual f32 Area() { return 0.5f * Base * Height; }

private:
  f32 Base, Height;
};

class circle : public shape_base {
public:
  circle(f32 RadiusInit)
    : Radius(RadiusInit) {}
  virtual f32 Area() { return Pi32 * Radius * Radius; }

private:
  f32 Radius;
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


int main(int argc, char *argv[]) {
  printf("Number of command line parameters = %d\n", argc);
  printf("Parameters:\n");
  for (int counter = 0; counter < argc; counter++)
    printf("%s\n", argv[counter]);

  //if (argc > 1 && argv[1] == "TotalAreaVTBL4") {

  u32 ShapeCount = 1048576;  // 2^20, 0x100000
  u32 RepeatCount = 1000;

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
  printf("Repeat Count: 1\n");
  printf("\n");

  clock_t t;
  f32 perf1, perf2;

  printf("         TotalAreaVTBL(%8d): ", ShapeCount);
  t = clock();
  TotalAreaVTBL(ShapeCount, Shapes);
  perf1 = (f32)(clock() - t) / ShapeCount;
  printf("%10.6f cycles/shape (%f - %f = %f)\n", perf1, 0.0f, 0.0f, 0.0f);

  printf("        TotalAreaVTBL4(%8d): ", ShapeCount);
  t = clock();
  TotalAreaVTBL4(ShapeCount, Shapes);
  perf2 = (f32)(clock() - t) / ShapeCount;
  printf("%10.6f cycles/shape (%f - %f = %f)\n", perf2, 0.0f, 0.0f, 0.0f);
  printf("\n");

  printf("         TotalAreaVTBL: %7.3fx\n", perf1 / perf1);
  printf("        TotalAreaVTBL4: %7.3fx\n", perf1 / perf2);
  printf("\n");

  printf("Repeat Count: %d\n", RepeatCount);
  printf("\n");

  f32 PerfAccum;

  printf("         TotalAreaVTBL(%8d): ", ShapeCount);
  PerfAccum = 0;
  for (int i = 0; i < RepeatCount; ++i) {
    t = clock();
    TotalAreaVTBL(ShapeCount, Shapes);
    PerfAccum += (f32)(clock() - t) / ShapeCount;
  }
  perf1 = PerfAccum / RepeatCount;
  printf("%10.6f cycles/shape (%f - %f = %f)\n", perf1, 0.0f, 0.0f, 0.0f);

  printf("        TotalAreaVTBL4(%8d): ", ShapeCount);
  PerfAccum = 0;
  for (int i = 0; i < RepeatCount; ++i) {
    t = clock();
    TotalAreaVTBL4(ShapeCount, Shapes);
    PerfAccum += (f32)(clock() - t) / ShapeCount;
  }
  perf2 = PerfAccum / RepeatCount;
  printf("%10.6f cycles/shape (%f - %f = %f)\n", perf2, 0.0f, 0.0f, 0.0f);
  printf("\n");

  printf("         TotalAreaVTBL: %7.3fx\n", perf1 / perf1);
  printf("        TotalAreaVTBL4: %7.3fx\n", perf1 / perf2);
  printf("\n");

  //}
  //else
  //  return -1;

  return 0;
}

