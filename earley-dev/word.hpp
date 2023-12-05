#pragma once
#include "rules.hpp"

class Word
{
public:
    Rules rule_;
    Word(const Rules &rule, const uint64_t &root_point, const uint64_t &point) : rule_(rule), root_point_(root_point),
                                                                                 point_(point) {}
    uint64_t GetRootPoint() const;
    uint64_t GetPoint() const;
    char GetNextLetter() const;
    Word Shift() const;
    bool CheckNextLetter() const;
    bool CheckRule() const;
    bool operator==(const Word &) const;
    bool operator<(const Word &) const;
    bool operator>(const Word &) const;

private:
    uint64_t root_point_;
    uint64_t point_;
};