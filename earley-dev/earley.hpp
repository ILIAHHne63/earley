#pragma once
#include "grammar.hpp"
#include "word.hpp"
#include <set>
#include <vector>
#include <iostream>

class Earley
{
public:
    Earley() : start_letter_('$') {}
    ~Earley() = default;
    bool Test(Grammar, const std::string &);
    void Preprocess();
    void Scan(uint64_t);
    void Predict(uint64_t);
    void Complete(uint64_t);
    Word Start(const char &);

private:
    Grammar grammar_;
    std::string word_;
    std::vector<std::set<Word>> sets_;
    char start_letter_;
};