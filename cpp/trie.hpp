#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <iterator>
#include <utility>

class TrieNode {
private:
    char c;
    std::unordered_set<char> next_set;
    bool complete;
public:
    TrieNode (char _c, char _next, bool _complete=false)
        : c(_c), complete(_complete) {
        
        next_set.insert(_next);
    }
    TrieNode (char _c, bool _complete=false)
        : c(_c), complete(_complete) {}

    bool operator== (const TrieNode& _other) const {
        return (c == _other.get_char());
    }

    // Getters.
    char get_char () const { return c; }
    bool is_complete () const { return complete; }
    // Setters.
    void set_char (char _c) { c = _c; }
    void set_complete (bool _complete) { complete = _complete; }

    bool in_next (char _next) const {
        bool not_in_next_set = (next_set.find(_next) == next_set.end());
        return !not_in_next_set;
    }
    bool add_to_next (char _next) {
        if (!in_next(_next)) {
            next_set.insert(_next);
        }
    }
};

namespace std {
template<>
struct hash<TrieNode> {
    std::size_t operator() (const TrieNode& _to_hash) const {
        return std::hash<char>()(_to_hash.get_char());
    }
};
}

/**
 * An iterator to iterate through a string in a pairwise manner.
 * Example: 'hello' : ('h', 'e'), ('e', 'l'), ('l', 'l'), ('l', 'o'), ('o', 0x00).
 */
class pairwise_iter {
private:
    std::string str;
public:
    class iterator : public std::iterator<std::random_access_iterator_tag,
                                          std::pair<char, char>,
                                          ptrdiff_t,
                                          std::pair<char, char>*,
                                          std::pair<char, char>> {

    private:
        std::string iterating;
        int first_char_index;
    public:
        explicit iterator (const std::string& _str)
                : iterating(_str), first_char_index(0) {}
        explicit iterator (const std::string& _str, int _first_char_index)
                : iterating(_str), first_char_index(_first_char_index) {}
        iterator& operator++ ()    { first_char_index++; return *this; }
        iterator  operator++ (int) { iterator ret = *this; ++(*this); return ret; }
        bool operator== (const iterator& _other) { return (iterating == _other.iterating) &&
                                                          (first_char_index == _other.first_char_index); }
        bool operator!= (const iterator& _other) { return !(*this == _other); }
        reference operator* () const {
            char first, second;
            if (first_char_index == iterating.size()-1) {
                first  = iterating[first_char_index];
                second = 0x00;
            } else {
                first  = iterating[first_char_index];
                second = iterating[first_char_index-1];
            }
            return std::make_pair(first, second);
        }
    };

    pairwise_iter (const std::string& _str)
            : str(_str) {}

    iterator begin () { return iterator(str); }
    iterator end   () { return iterator(str, str.size()); }
};

class Trie {
private:
    // A trie is a vector of unordered_sets of trie nodes.
    std::vector<std::unordered_set<TrieNode>> trie;

    std::unordered_set<TrieNode>::iterator get_letter_in_set (char, int);
    bool letter_in_set (char, int);
public:
    void insert (const std::string&);
    bool search (const std::string&) const;
};

std::unordered_set<TrieNode>::iterator Trie::get_letter_in_set (char _letter, int _trie_level) {
    if (_trie_level >= trie.size()) {
        return std::unordered_set<TrieNode>::iterator();
    }

    auto& set = trie[_trie_level];
    return set.find(TrieNode(_letter));
}

bool Trie::letter_in_set (char _letter, int _trie_level) {
    if (_trie_level >= trie.size()) {
        return false;
    }

    auto& set = trie[_trie_level];
    return !(set.find(TrieNode(_letter)) == set.end());
}

///////////////////////////////////////////////////////////////////////
////////////////////// Insert & Search ///////////////////////////////
/////////////////////////////////////////////////////////////////////

void Trie::insert (const std::string& _word) {
    int trie_level = 0;
    for (auto&& letter_pair : pairwise_iter(_word)) {
        char first  = letter_pair.first;
        char second = letter_pair.second;
        bool is_end_of_word = (second == 0x00);

        if (trie_level >= trie.size()) {
            trie.push_back(
                std::unordered_set<TrieNode>()
            );
        }

        if (letter_in_set(first, trie_level)) {
            // The letter is in this trie level.

            // Find the letter in the set.
            auto trie_node_iter = get_letter_in_set(first, trie_level);
            
            if (!(trie_node_iter->in_next(second))) {
                TrieNode tn = *trie_node_iter;
                tn.add_to_next(second);

                // Remove the trie node and add it again.
                trie[trie_level].erase(trie_node_iter);
                trie[trie_level].insert(tn);
            }

            // Iterator is invalidated, so get it again.
            trie_node_iter = get_letter_in_set(first, trie_level);

            if (is_end_of_word) {
                TrieNode tn = *trie_node_iter;
                tn.set_complete(true);

                trie[trie_level].erase(trie_node_iter);
                trie[trie_level].insert(tn);
            }
        
        } else {
            // The letter is not in this trie level.

            if (is_end_of_word) {
                TrieNode created(first, true);
                trie[trie_level].insert(created);
            } else {
                TrieNode created(first, second);
                trie[trie_level].insert(created);
            }
        }

        trie_level++;
    }
}