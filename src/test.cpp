
# ifdef testmode

# include "main.hpp"
# include "gtest/gtest.h"
# include "string"

TEST(ScannerTest, ScannerAddsEOFToken){
  Scanner scanner {std::string {""} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens.size(), 1);
  ASSERT_EQ(tokens.back().type, KW_EOF);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
# endif
