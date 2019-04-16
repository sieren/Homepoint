#pragma once

namespace gfx
{
  struct Point
  {
    int x;
    int y;
    int z;

    Point operator-(Point pos)
    {
      x -= pos.x;
      y -= pos.y;
      return *this;
    }

    Point operator+(Point pos)
    {
      x += pos.x;
      y += pos.y;
      return *this;
    }
  
    bool operator==(Point pos)
    {
      return x == pos.x && y == pos.y;
    }
  };
  
  struct Size
  {
    int width;
    int height;

    bool operator==(Size size)
    {
      return width == size.width && height == size.height;
    }
  };

  struct Frame
  {
    Point position = {0, 0, 0};
    Size size = {0, 0};
  
    bool operator==(Frame frame)
    {
      return size == frame.size && position == frame.position;
    }

    template<typename T>
    auto isInBounds(const T& oPos) const -> bool
    {
      // TODO Screen rotation from Touch Driver
      return (oPos.x >= position.x && oPos.x < position.x + size.width) &&
              (oPos.y >= position.y && oPos.y < position.y + size.height);
    }

    // TODO: Refactor to getCenterPointRelative()
    auto getCenterPoint() -> Point
    {
      const auto x = size.width / 2;
      const auto y = size.height / 2;
      return Point{x, y, 0};
    }

    auto frameInBounds(const Frame& frame) -> Frame
    {
      const auto x = position.x + frame.position.x;
      const auto y = position.y + frame.position.y;
      const auto z = position.z;
      return {{x,y,z}, frame.size};
    }
  };

}
