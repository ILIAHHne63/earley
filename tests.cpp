#include <gtest/gtest.h>
#include "earley.hpp"

TEST(Root, Root)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "SGB";
  std::string Term = "wab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->aGaB",
      "G->w",
      "B->",
      "S->",
      "S->a",
      "S->b"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(grammar.GetRoot() == 'S');
  EXPECT_FALSE(grammar.GetRoot() == 'B');
  EXPECT_FALSE(grammar.GetRoot() == 'G');
  EXPECT_FALSE(grammar.GetRoot() == 'a');
  EXPECT_FALSE(grammar.GetRoot() == 'o');
}

TEST(Letters, Letters)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "SGBUQO";
  std::string Term = "wpab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->aGaB",
      "G->w",
      "B->",
      "B->UiO",
      "Q->Op",
      "S->",
      "S->a",
      "S->b"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(grammar.IsNonTerminal('B'));
  EXPECT_TRUE(grammar.IsNonTerminal('G'));
  EXPECT_TRUE(grammar.IsNonTerminal('U'));
  EXPECT_TRUE(grammar.IsTerminal('p'));
  EXPECT_TRUE(grammar.IsTerminal('w'));
  EXPECT_TRUE(grammar.IsTerminal('b'));
  EXPECT_FALSE(grammar.IsTerminal('S'));
  EXPECT_FALSE(grammar.IsTerminal('B'));
  EXPECT_FALSE(grammar.IsTerminal('G'));
  EXPECT_FALSE(grammar.IsTerminal('U'));
  EXPECT_FALSE(grammar.IsNonTerminal('p'));
  EXPECT_FALSE(grammar.IsNonTerminal('w'));
  EXPECT_FALSE(grammar.IsNonTerminal('b'));
}

TEST(OneTerminal, OneTerminal)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "SG";
  std::string Term = "ab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->aSbS",
      "S->G",
      "S->a"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "aaba"));
  EXPECT_TRUE(earley.Test(grammar, "aabaaba"));
  EXPECT_FALSE(earley.Test(grammar, "baaa"));
  EXPECT_FALSE(earley.Test(grammar, ""));
  EXPECT_FALSE(earley.Test(grammar, "abab"));
}

TEST(BracketsFirst, BracketsFirst)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "S";
  std::string Term = "()";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->(S)S",
      "S->"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "(())()"));
  EXPECT_TRUE(earley.Test(grammar, "()()"));
  EXPECT_TRUE(earley.Test(grammar, "()"));
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_TRUE(earley.Test(grammar, "((())(()())())"));
  EXPECT_FALSE(earley.Test(grammar, "((())(()())()("));
  EXPECT_FALSE(earley.Test(grammar, "("));
  EXPECT_FALSE(earley.Test(grammar, "())"));
}

TEST(BracketsSecond, BracketsSecond)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "S";
  std::string Term = "()[]{}";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->(S)S",
      "S->[S]S",
      "S->{S}S",
      "S->"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "(){}{}{}[]"));
  EXPECT_TRUE(earley.Test(grammar, "()[]"));
  EXPECT_TRUE(earley.Test(grammar, "{[]}"));
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_TRUE(earley.Test(grammar, "{{{{}}}}[()]([])"));
  EXPECT_TRUE(earley.Test(grammar, "(((())))[{}][[]]"));
  EXPECT_FALSE(earley.Test(grammar, "{{{{{{))))))"));
  EXPECT_FALSE(earley.Test(grammar, "{{{{{{]]]]]]"));
  EXPECT_FALSE(earley.Test(grammar, "}[{}"));
}

TEST(EquationsFirst, EquationsFirst)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "ABCR";
  std::string Term = "12579";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->A+BC",
      "S->B+AC",
      "C->=R",
      "R->7",
      "S->",
      "A->+",
      "A->1",
      "A->2",
      "B->5",
      "B->9"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_TRUE(earley.Test(grammar, "2+5=7"));
  EXPECT_TRUE(earley.Test(grammar, "5+2=7"));
  EXPECT_FALSE(earley.Test(grammar, "9=7"));
}

TEST(MiddleTestFirst, MiddleTestFirst)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "TSF";
  std::string Term = "adbc";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->TS",
      "S->Tc",
      "T->FbT",
      "T->Fd",
      "F->",
      "F->a"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "dc"));
  EXPECT_TRUE(earley.Test(grammar, "bdc"));
  EXPECT_TRUE(earley.Test(grammar, "abbbdc"));
  EXPECT_FALSE(earley.Test(grammar, "abbbbb"));
  EXPECT_FALSE(earley.Test(grammar, "bbb"));
  EXPECT_FALSE(earley.Test(grammar, "abc"));
  EXPECT_FALSE(earley.Test(grammar, "abcde"));
  EXPECT_FALSE(earley.Test(grammar, "ac"));
}

TEST(EquationsSecond, EquationsSecond)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "S";
  std::string Term = "ab()*+-01";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->a",
      "S->b",
      "S->0",
      "S->1",
      "S->S+S",
      "S->(S-S)",
      "S->S-S",
      "S->(S*S)",
      "S->S*S",
      "S->(S+S)",
      "S->(SS)",
      "S->SS"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "a+b"));
  EXPECT_TRUE(earley.Test(grammar, "a-b"));
  EXPECT_TRUE(earley.Test(grammar, "a*a+b"));
  EXPECT_TRUE(earley.Test(grammar, "0*a+b"));
  EXPECT_TRUE(earley.Test(grammar, "(a+b)(a*ab)+1*b+0+(1*a)(0*b)-bb+ab+1"));
  EXPECT_TRUE(earley.Test(grammar, "(a+b)(a*ab)+1*b+0"));
  EXPECT_FALSE(earley.Test(grammar, "a++a"));
  EXPECT_FALSE(earley.Test(grammar, "a-+a"));
  EXPECT_FALSE(earley.Test(grammar, ""));
  EXPECT_FALSE(earley.Test(grammar, "A+a"));
}

TEST(MiddleTestScond, MiddleTestScond)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "S";
  std::string Term = "ab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->aSbS",
      "S->bSaS",
      "S->"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "ab"));
  EXPECT_TRUE(earley.Test(grammar, "ababbbaa"));
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_FALSE(earley.Test(grammar, "abbbbb"));
  EXPECT_FALSE(earley.Test(grammar, "a"));
  EXPECT_FALSE(earley.Test(grammar, "bbb"));
}

TEST(HardTestFirst, HardTestFirst)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "PBCD";
  std::string Term = "ab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "P->aB",
      "B->aB",
      "B->aBb",
      "B->a",
      "P->C",
      "P->D",
      "C->Cb",
      "C->aCb",
      "C->b",
      "D->aDb",
      "D->"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('P');
  EXPECT_TRUE(earley.Test(grammar, "ab"));
  EXPECT_TRUE(earley.Test(grammar, "aabb"));
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_TRUE(earley.Test(grammar, "abb"));
  EXPECT_FALSE(earley.Test(grammar, "aab"));
  EXPECT_FALSE(earley.Test(grammar, "a"));
}

TEST(MiddleTestThird, MiddleTestThird)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "S";
  std::string Term = "ab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->",
      "S->aSbSbS"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "abb"));
  EXPECT_TRUE(earley.Test(grammar, "aabbbb"));
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_TRUE(earley.Test(grammar, "ababbb"));
  EXPECT_FALSE(earley.Test(grammar, "b"));
  EXPECT_FALSE(earley.Test(grammar, "ab"));
  EXPECT_FALSE(earley.Test(grammar, "bba"));
}

TEST(MiddleTestFourth, MiddleTestFourth)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "SCG";
  std::string Term = "ab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->",
      "S->C",
      "S->CS",
      "C->Gc",
      "G->aGb",
      "G->"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "abcabc"));
  EXPECT_TRUE(earley.Test(grammar, "aabbcccaaabbbccc"));
  EXPECT_TRUE(earley.Test(grammar, "c"));
  EXPECT_TRUE(earley.Test(grammar, "ccccc"));
  EXPECT_TRUE(earley.Test(grammar, ""));
  EXPECT_FALSE(earley.Test(grammar, "aaaaaabbbbbcc"));
  EXPECT_FALSE(earley.Test(grammar, "aabb"));
}

TEST(Apricote, Apricote)
{
  Earley earley;
  Grammar grammar;
  std::string NonTerm = "S";
  std::string Term = "ab";
  grammar.UpdateLetters(Term, NonTerm);
  std::vector<std::string> rules = {
      "S->aSbS",
      "S->"};
  for (const std::string &format : rules)
  {
    grammar.Rule(format);
  }
  grammar.Root('S');
  EXPECT_TRUE(earley.Test(grammar, "aababb"));
  EXPECT_FALSE(earley.Test(grammar, "aabbba"));
}

int main(int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}