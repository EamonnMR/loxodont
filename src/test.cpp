# include "main.cpp"
# include "main.hpp"

# include "main.hpp"
# include "gtest/gtest.h"

#ifdef testmode
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
