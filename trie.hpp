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