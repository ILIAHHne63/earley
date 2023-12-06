#include "grammar.hpp"

std::unordered_map<char, bool> Grammar::terminal_letters_;
std::unordered_map<char, bool> Grammar::non_terminal_letters_;

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
