#pragma once
#include <iostream>
#include <vector>
#include "rules.hpp"

class Grammar
{
public:
    Grammar() = default;
    ~Grammar() = default;
    void UpdateLetters(const std::string &, const std::string &);
    void Rule(const std::string &);
    void Rule(const Rules &);
    void Root(const char &);
    char GetRoot() const;
    std::vector<Rules> &GetRules();
    bool IsNonTerminal(const char &) const;
    bool IsTerminal(const char &) const;

private:
    static std::unordered_map<char, bool> terminal_letters_;
    static std::unordered_map<char, bool> non_terminal_letters_;
    char root_;
    std::vector<Rules> rules_;
};