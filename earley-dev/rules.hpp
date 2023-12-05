#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

class Rules
{
public:
    Rules() = default;
    ~Rules() = default;
    Rules(const std::string &);
    Rules(const std::string &, const char &);
    uint64_t Min(const Rules &) const;
    bool operator<(const Rules &) const;
    bool operator==(const Rules &) const;
    bool operator>(const Rules &) const;
    void SetLast(const std::string &);
    void Processing(const std::string &, const std::string &);
    char jump_before_;
    std::vector<char> jump_after_;
};
