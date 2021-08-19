
# ifdef testmode

# include "main.hpp"
# include "scanner.hpp"
# include "token.hpp"
# include "expr.hpp"
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
  std::string counting {"12345"};
  Scanner scanner {counting};
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, NUMBER);
  ASSERT_EQ(repr(tokens[0].literal), counting);
}

TEST(ScannerTest, ScannerParsesStrings){
  Scanner scanner {std::string {"\"String With Spaces\""} };
  std::vector<Token> tokens{scanner.scanTokens()};
  ASSERT_EQ(tokens.size(), 2);
  ASSERT_EQ(tokens[0].type, STRING);
}

TEST(ASTPrinterTest, ASTPrintsSimpleTree){
  Scanner scanner {std::string {"123+456"} };
  std::vector<Token> tk{scanner.scanTokens()};
  Expr a, b;
  a = Literal { &tk[0].literal};
  b = Literal { &tk[2].literal};
  ASSERT_EQ(
      repr(*(std::get<Literal>(a).value)),
      "123"
  );

  ASSERT_EQ(
      ASTPrinter().visit(
        Binary {
          &a, &tk[1], &b
        }
      ),
      std::string {"(+ 123 456)"}
  );

  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
# endif
