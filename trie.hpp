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

class Trie {
private:
    // A trie is a vector of unordered_sets of trie nodes.
    std::vector<std::unordered_set<TrieNode>> trie;

    std::unordered_set<TrieNode>::iterator get_letter_in_set (char, int);
    bool letter_in_set (char, int);
public:
    void insert (const std::string&);
    bool search (const std::string&);
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