#include "word.cpp"
#include "grammar.cpp"
#include "earley.cpp"
#include "rules.cpp"
#include <iostream>

int main()
{
  uint64_t num_rules;
  uint64_t num_words;
  uint64_t num_non_term_letters;
  uint64_t num_term_letters;
  char root;
  std::string main_rule;
  std::string term_letters;
  std::string non_term_letters;
  std::string word;
  Earley earley;
  Grammar grammar;

  // std::cout << "Введите число нетерминальных символов : ";
  std::cin >> num_non_term_letters;

  // std::cout << "Введите число терминальных символов : ";
  std::cin >> num_term_letters;

  // std::cout << "Введите число правил : ";
  std::cin >> num_rules;

  // std::cout << "Введите нетерминальные символы : \n";
  std::cin >> non_term_letters;

  // std::cout << "Введите терминальные символы : \n";
  std::cin >> term_letters;

  grammar.UpdateLetters(term_letters, non_term_letters);

  // std::cout << "Введите правила : \n";
  for (uint64_t i = 0; i < num_rules; ++i)
  {
    std::cin >> main_rule;
    grammar.Rule(main_rule);
  }

  // std::cout << "Введите корень: ";
  std::cin >> root;
  grammar.Root(root);

  // std::cout << "Введите количество слов : ";
  std::cin >> num_words;

  for (uint64_t i = 0; i < num_words; ++i)
  {
    // std::cout << "Введите слова : ";
    std::cin >> word;
    earley.Test(grammar, word) ? std::cout << "Да\n" : std::cout << "Нет\n";
  }
}
