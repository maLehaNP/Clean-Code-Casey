#pragma once
#include "common.h"
const f32 Pi32 = 3.141592f;


class shape_base {
public:
  shape_base() {}
  virtual f32 Area() = 0;
  virtual u32 CornerCount() = 0;
};


class square : public shape_base {
public:
  square(f32 SideInit) : Side(SideInit) {}
  virtual f32 Area() { return Side * Side; }
  virtual u32 CornerCount() { return 4; }

private:
  f32 Side;
};

class rectangle : public shape_base {
public:
  rectangle(f32 WidthInit, f32 HeightInit)
    : Width(WidthInit), Height(HeightInit) {}
  virtual f32 Area() { return Width * Height; }
  virtual u32 CornerCount() { return 4; }

private:
  f32 Width, Height;
};

class triangle : public shape_base {
public:
  triangle(f32 BaseInit, f32 HeightInit)
    : Base(BaseInit), Height(HeightInit) {}
  virtual f32 Area() { return 0.5f * Base * Height; }
  virtual u32 CornerCount() { return 3; }

private:
  f32 Base, Height;
};

class circle : public shape_base {
public:
  circle(f32 RadiusInit)
    : Radius(RadiusInit) {}
  virtual f32 Area() { return Pi32 * Radius * Radius; }
  virtual u32 CornerCount() { return 0; }

private:
  f32 Radius;
};


enum shape_type : u32 {
  Shape_Square,
  Shape_Rectangle,
  Shape_Triangle,
  Shape_Circle,

  Shape_Count,
};

struct shape_union {
  shape_type Type;
  f32 Width;
  f32 Height;
};


f32 GetAreaSwitch(shape_union Shape) {
  f32 Result = 0.0f;

  switch (Shape.Type) {
    case Shape_Square: { Result = Shape.Width * Shape.Width; } break;
    case Shape_Rectangle: { Result = Shape.Width * Shape.Height; } break;
    case Shape_Triangle: { Result = 0.5f * Shape.Width * Shape.Height; } break;
    case Shape_Circle: { Result = Pi32 * Shape.Width * Shape.Width; } break;

    case Shape_Count: {} break;
  }

  return Result;
}

u32 GetCornerCountSwitch(shape_type Type) {
  u32 Result = 0;

  switch (Type) {
    case Shape_Square:    { Result = 4; } break;
    case Shape_Rectangle: { Result = 4; } break;
    case Shape_Triangle:  { Result = 3; } break;
    case Shape_Circle:    { Result = 0; } break;

    case Shape_Count: {} break;
  }

  return Result;
}


#define Shape_Count 4

f32 const CTable[Shape_Count] = { 1.0f, 1.0f, 0.5f, Pi32 };

f32 GetAreaUnion(shape_union Shape) {
  f32 Result = CTable[Shape.Type] * Shape.Width * Shape.Height;
  return Result;
}


f32 const CTable_Corner[Shape_Count] = { 1.0f / (1.0f + 4.0f), 1.0f / (1.0f + 4.0f), 0.5f / (1.0f + 3.0f), Pi32 };

inline f32 GetCornerAreaUnion(shape_union Shape) {
  f32 Result = CTable_Corner[Shape.Type] * Shape.Width * Shape.Height;
  return Result;
}

