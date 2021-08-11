
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

TEST(ScannerTest, ScannerRecognizesSingleTokens){
  Scanner scanner {std::string {"()/."} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens[0].type, LEFT_PAREN);
  ASSERT_EQ(tokens[1].type, RIGHT_PAREN);
  ASSERT_EQ(tokens[2].type, SLASH);
}

TEST(ScannerTest, ScannerDiscardsComments){
  Scanner scanner {std::string {"abcde//56789"} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens.size(), 2);
}

TEST(ScannerTest, ScannerScansIdentifiers){
  Scanner scanner {std::string {"ident 1 2 3"} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens[0].type, IDENTIFIER);
}

TEST(ScannerTest, ScannerParsesDigits){
  Scanner scanner {std::string {"12345"} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, NUMBER);
}

TEST(ScannerTest, ScannerParsesStrings){
  Scanner scanner {std::string {"\"String With Spaces\""} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, STRING);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
# endif
