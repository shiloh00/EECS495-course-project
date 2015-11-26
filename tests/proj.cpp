#include "proj/proj.hpp"
#include <UnitTest++/UnitTest++.h>

namespace proj
{

  TEST(True)
  {
    CHECK(true);
  }

}  // namespace proj

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
