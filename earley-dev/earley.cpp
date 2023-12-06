#include "earley.hpp"

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