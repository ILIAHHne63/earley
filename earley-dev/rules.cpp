#include "rules.hpp"
#include <stdexcept>
#include <iostream>

// -- definition of Rules methods
// -----------------------------------------
// -----------------------------------------

Rules::Rules(const std::string &rule_string)
{
    std::string jump_symbol = "->";
    uint64_t symbol_position = rule_string.find(jump_symbol);
    if (symbol_position == static_cast<uint64_t>(-1))
    {
        throw std::invalid_argument("ERROR\n");
    }
    uint64_t jump_symbol_size = jump_symbol.size();
    uint64_t jump_before = symbol_position + jump_symbol_size;
    Processing(rule_string.substr(0, symbol_position), rule_string.substr(jump_before, rule_string.size()));
}

Rules::Rules(const std::string &rule_string, const char &letter) : Rules(rule_string)
{
    jump_before_ = letter;
}

void Rules::SetLast(const std::string &jump_before)
{
    jump_before_ = jump_before[0];
}

void Rules::Processing(const std::string &jump_before, const std::string &jump_after)
{
    if (jump_before.empty() || jump_before.size() > 1)
    {
        throw std::invalid_argument("ERROR\n");
    }
    SetLast(jump_before);
    if (!isupper(jump_before_))
    {
        throw std::invalid_argument("ERROR\n");
    }
    for (auto iter = jump_after.begin(); iter != jump_after.end(); ++iter)
    {
        jump_after_.emplace_back(*iter);
    }
}

uint64_t Rules::Min(const Rules &otherwise) const
{
    return std::min(jump_after_.size(), otherwise.jump_after_.size());
}

bool Rules::operator==(const Rules &otherwise) const
{
    return (jump_before_ == otherwise.jump_before_) &&
           (jump_after_ == otherwise.jump_after_);
}

bool Rules::operator<(const Rules &otherwise) const
{
    if (jump_before_ != otherwise.jump_before_)
    {
        return jump_before_ < otherwise.jump_before_;
    }

    for (uint64_t index = 0; index < Min(otherwise); ++index)
    {
        if (jump_after_[index] < otherwise.jump_after_[index])
        {
            return true;
        }
    }
    return jump_after_.size() < otherwise.jump_after_.size();
}

bool Rules::operator>(const Rules &otherwise) const
{
    return !(*this < otherwise || *this == otherwise);
}

// -----------------------------------------
// -----------------------------------------
