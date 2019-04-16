#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "uiposition.hpp"

TEST_CASE("UIPosition respects bounds","[uiposition]")
{
  using namespace gfx;
  const Frame mainFrame = {0,0, 230, 230};
  const Frame targetFrame = {120, 120, 230, 230};

  SECTION("frame within bounds returns true")
  {
    REQUIRE(mainFrame.isInBounds(targetFrame));
  }

    SECTION("frame within bounds returns true")
  {
    const Frame outsideFrame = {230,230, 10, 10};
    CHECK_FALSE(mainFrame.isInBounds(outsideFrame));
  }
}

TEST_CASE("Frame in frame translates bounds","[uiposition]")
{
  using namespace gfx;
  const Frame mainFrame = {0,0, 230, 230};
  const Frame targetFrame = {120, 120, 230, 230};

  SECTION("frame within bounds returns true")
  {
    REQUIRE(mainFrame.isInBounds(targetFrame));
  }

    SECTION("frame within bounds returns true")
  {
    const Frame outsideFrame = {230,230, 10, 10};
    CHECK_FALSE(mainFrame.isInBounds(outsideFrame));
  }
}

