#include "earley.hpp"
#include <iostream>

std::unordered_map<char, bool> Grammar::terminal_letters_;
std::unordered_map<char, bool> Grammar::non_terminal_letters_;

// -- definition of Rules methods
// -----------------------------------------
// -----------------------------------------

Rules::Rules(const std::string &rule_string)
{
  std::string jump_symbol = "->";
  uint64_t symbol_position = rule_string.find(jump_symbol);
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
  SetLast(jump_before);
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

// -- definition of Grammar methods
// -----------------------------------------
// -----------------------------------------

void Grammar::UpdateLetters(const std::string &terminal_letters, const std::string &non_terminal_letters)
{
  for (auto iter = terminal_letters.begin(); iter != terminal_letters.end(); ++iter)
  {
    if (!terminal_letters_.contains(*iter))
    {
      terminal_letters_[*iter] = true;
    }
  }
  for (auto iter = non_terminal_letters.begin(); iter != non_terminal_letters.end(); ++iter)
  {
    if (!non_terminal_letters_.contains(*iter))
    {
      non_terminal_letters_[*iter] = true;
    }
  }
}

bool Grammar::IsNonTerminal(const char &letter) const
{
  return non_terminal_letters_.contains(letter);
}

bool Grammar::IsTerminal(const char &letter) const
{
  return terminal_letters_.contains(letter);
}

void Grammar::Rule(const std::string &rule_string) { rules_.emplace_back(rule_string); }

void Grammar::Rule(const Rules &rule) { rules_.push_back(rule); }

void Grammar::Root(const char &symbol) { root_ = symbol; }

char Grammar::GetRoot() const { return root_; }

std::vector<Rules> &Grammar::GetRules() { return rules_; };

// -----------------------------------------
// -----------------------------------------

// -- definition of Earley methods
// -----------------------------------------
// -----------------------------------------

bool Earley::Test(Grammar grammar, const std::string &word)
{
  grammar_ = grammar;
  word_ = word;
  Preprocess();
  for (uint64_t position = 0; position < (word.size() + 1); ++position)
  {
    Scan(position);
    uint64_t old_sent_list_size = 0;
    do
    {
      old_sent_list_size = sets_[position].size();
      Complete(position);
      Predict(position);
    } while (sets_[position].size() != old_sent_list_size);
  }
  std::set<Word> last_situation_list = sets_[word.size()];
  for (auto iter = last_situation_list.begin(); iter != last_situation_list.end(); ++iter)
  {
    if (*iter == Start(grammar_.GetRoot()).Shift())
    {
      return true;
    }
  }
  return false;
}

void Earley::Preprocess()
{
  sets_.clear();
  sets_.resize(word_.size() + 1);
  sets_[0].insert(Start(grammar_.GetRoot()));
}

Word Earley::Start(const char &root)
{
  std::string pre_process = std::string(1, root);
  pre_process += "->";
  pre_process += root;
  Rules pre_process_rule(pre_process, '$');
  grammar_.Rule(pre_process_rule);
  return Word(pre_process_rule, 0, 0);
}

void Earley::Scan(uint64_t position)
{
  if (position != 0)
  {
    uint64_t index = position - 1;
    for (auto iter = sets_[index].begin(); iter != sets_[index].end(); ++iter)
    {
      const Word &situation = *iter;
      if (situation.CheckNextLetter() && situation.GetNextLetter() == word_[index])
      {
        Word shift = situation.Shift();
        sets_[position].insert(shift);
      }
    }
  }
}

void Earley::Predict(uint64_t position)
{
  for (auto iter = sets_[position].begin(); iter != sets_[position].end(); ++iter)
  {
    Word sent = *iter;
    if (!sent.CheckRule() && !sent.CheckNextLetter())
    {
      for (auto rule_iter = grammar_.GetRules().begin(); rule_iter != grammar_.GetRules().end(); ++rule_iter)
      {
        const Rules &rule = *rule_iter;
        if (sent.GetNextLetter() == rule.jump_before_)
        {
          sets_[position].emplace(rule, position, 0);
        }
      }
    }
  }
}

void Earley::Complete(uint64_t position)
{
  for (auto iter = sets_[position].begin(); iter != sets_[position].end(); ++iter)
  {
    const Word &sent = *iter;
    if (sent.CheckRule())
    {
      for (auto inner_iter = sets_[sent.GetRootPoint()].begin(); inner_iter != sets_[sent.GetRootPoint()].end(); ++inner_iter)
      {
        const Word &upper_sent = *inner_iter;
        if (!upper_sent.CheckRule() && upper_sent.GetNextLetter() == sent.rule_.jump_before_)
        {
          sets_[position].insert(upper_sent.Shift());
        }
      }
    }
  }
}

// -----------------------------------------
// -----------------------------------------