#include "word.hpp"

// -- definition of Word methods
// -----------------------------------------
// -----------------------------------------

bool Word::CheckNextLetter() const
{
    if (CheckRule())
    {
        return false;
    }
    return !isupper(rule_.jump_after_[point_]);
}

uint64_t Word::GetRootPoint() const
{
    return root_point_;
}
uint64_t Word::GetPoint() const
{
    return point_;
}

char Word::GetNextLetter() const
{
    return rule_.jump_after_[point_];
}

bool Word::CheckRule() const
{
    return point_ == rule_.jump_after_.size();
}

Word Word::Shift() const
{
    return Word(rule_, root_point_, point_ + 1);
}

bool Word::operator==(const Word &otherwise) const
{
    return (rule_ == otherwise.rule_ &&
            root_point_ == otherwise.root_point_ &&
            point_ == otherwise.point_);
}

bool Word::operator<(const Word &otherwise) const
{
    return std::make_tuple(rule_, root_point_, point_) <
           std::make_tuple(otherwise.rule_, otherwise.root_point_, otherwise.point_);
}

bool Word::operator>(const Word &otherwise) const
{
    return !(*this < otherwise || *this == otherwise);
}

// -----------------------------------------
// -----------------------------------------