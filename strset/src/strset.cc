#include <unordered_map>
#include <set>
#include "strset.h"
#include "strsetconst.h"
#include <cassert>
#include <limits>

namespace {

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif

    using StrSet = std::set<std::string>;
    using Map = std::unordered_map<unsigned long, StrSet>;

    // During the execution @nextId stores value of ID assigned to next set created by strset_new().
    unsigned long nextId = 0;

    // Function that returns static map, which maps IDs of sets of strings with afromentioned sets.
    Map &map() {
        static auto *res = new Map();
        return *res;
    }

    // Following functions are used to print debug information in case -NDEBUG is not set.
    inline void printDebug(const char *s1) {
        if (debug) std::cerr << s1 << '\n';
    }

    inline void printDebug(const char *s1, unsigned long id, const char *s2) {
        if (debug) std::cerr << s1 << id << s2 << '\n';
    }

    inline void
    printDebug(const char *s1, unsigned long id, const char *s2, size_t n, const char *s3) {
        if (debug) std::cerr << s1 << id << s2 << n << s3 << '\n';
    }

    inline void printDebug(const char *s1, unsigned long id, const char *s2, const char *value,
                           const char *s3 = "") {
        if (debug) {
            // Conditional operator to process @value of NULL properly.
            std::string val = value == nullptr ? "NULL" : "\"" + std::string(value) + "\"";
            std::cerr << s1 << id << s2 << val << s3 << '\n';
        }
    }
}

namespace jnp1 {

    unsigned long strset_new() {
        printDebug("strset_new()");
        // In case of @nextId overflow:
        assert (nextId != std::numeric_limits<unsigned long>::max());

        StrSet set;
        map().insert(make_pair(nextId, set));

        printDebug("strset_new: set ", nextId, " created");
        return nextId++;
    }

    void strset_delete(unsigned long id) {
        printDebug("strset_delete(", id, ")");

        if (id == strset42()) {
            printDebug("strset_delete: attempt to remove the 42 Set");
            return;
        }

        auto it = map().find(id);
        if (it == map().end()) {
            printDebug("strset_delete: set ", id, " does not exist");
        } else {
            map().erase(it);
            printDebug("strset_delete: set ", id, " deleted");
        }
    }

    size_t strset_size(unsigned long id) {
        printDebug("strset_size(", id, ")");

        if (id == strset42()) {
            printDebug("strset_size: the 42 Set contains 1 element(s)");
            return 1;
        }

        auto it = map().find(id);
        if (it == map().end()) {
            printDebug("strset_size: set ", id, " does not exist");
            return 0;
        } else {
            size_t result = it->second.size();
            printDebug("strset_size: set ", id, " contains ", result, " element(s)");
            return result;
        }
    }

    void strset_insert(unsigned long id, const char *value) {
        printDebug("strset_insert(", id, ", ", value, ")");

        if (value == nullptr) {
            printDebug("strset_insert: invalid value (NULL)");
            return;
        }

        auto it = map().find(id);
        // Whitelisting first insertion of "42" into the 42 Set.
        if (id == strset42() && !(it->second.empty())) {
            printDebug("strset_insert: attempt to insert into the 42 Set");
            return;
        }

        if (it == map().end()) {
            printDebug("strset_insert: set ", id, " does not exist");
        } else {
            if (it->second.find(value) == it->second.end()) {
                it->second.insert(value);
                printDebug("strset_insert: set ", id, ", element ", value, " inserted");
            } else {
                printDebug("strset_insert: set ", id, ", element ", value, " was already present");
            }
        }
    }

    void strset_remove(unsigned long id, const char *value) {
        printDebug("strset_remove(", id, ", ", value, ")");

        if (value == nullptr) {
            printDebug("strset_remove: invalid value (NULL)");
        }

        if (id == strset42()) {
            printDebug("strset_remove: attempt to remove from the 42 Set");
            return;
        }

        auto it = map().find(id);
        if (it == map().end()) {
            printDebug("strset_remove: set ", id, " does not exist");
        } else {
            if (it->second.find(value) == it->second.end()) {
                printDebug("strset_remove: set ", id, " does not contain the element ", value);
            } else {
                it->second.erase(value);
                printDebug("strset_remove: set ", id, ", element ", value, " removed");
            }
        }
    }

    int strset_test(unsigned long id, const char *value) {
        printDebug("strset_test(", id, ", ", value, ")");

        if (value == nullptr) {
            printDebug("strset_test: invalid value (NULL)");
            return 0;
        }

        if (id == strset42()) {
            if (std::string("42") == value) {
                printDebug("strset_test: the 42 Set contains the element \"42\"");
                return 1;
            } else {
                printDebug("strset_test: the ", 42, " Set does not contain the element ", value);
                return 0;
            }
        }

        auto it = map().find(id);
        if (it == map().end()) {
            printDebug("strset_test: set ", id, " does not exist");
            return 0;
        } else {
            if (it->second.find(value) == it->second.end()) {
                printDebug("strset_test: set ", id, " does not contain the element ", value);
                return 0;
            } else {
                printDebug("strset_test: set ", id, " contains the element ", value);
                return 1;
            }
        }
    }

    void strset_clear(unsigned long id) {
        printDebug("strset_clear(", id, ")");

        if (id == strset42()) {
            printDebug("strset_clear: attempt to clear the 42 Set");
            return;
        }

        auto it = map().find(id);
        if (it == map().end()) {
            printDebug("strset_clear: set ", id, " does not exist");
        } else {
            it->second.clear();
            printDebug("strset_clear: set ", id, " cleared");
        }
    }

    int strset_comp(unsigned long id1, unsigned long id2) {
        printDebug("strset_comp(", id1, ", ", id2, ")");

        auto it1 = map().find(id1), it2 = map().find(id2), end = map().end();
        if (it1 == end) {
            if (it2 == end) {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 0");
                printDebug("strset_comp: set ", id1, " does not exist");
                printDebug("strset_comp: set ", id2, " does not exist");
                return 0;
            } else if (it2->second.empty()) {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 0");
                printDebug("strset_comp: set ", id1, " does not exist");
                return 0;
            } else {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is -1");
                printDebug("strset_comp: set ", id1, " does not exist");
                return -1;
            }
        } else if (it2 == end) {
            if (it1->second.empty()) {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 0");
                printDebug("strset_comp: set ", id2, " does not exist");
                return 0;
            } else {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 1");
                printDebug("strset_comp: set ", id2, " does not exist");
                return 1;
            }
        }

        auto s1 = it1->second.begin(), s2 = it2->second.begin(),
                end1 = it1->second.end(), end2 = it2->second.end();
        for (; s1 != end1 && s2 != end2; ++s1, ++s2) {
            int cmp = (*s1).compare(*s2);
            if (cmp < 0) {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is -1");
                return -1;
            } else if (cmp > 0) {
                printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 1");
                return 1;
            }
        }
        if (s1 == end1 && s2 == end2) {
            printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 0");
            return 0;
        } else if (s1 == end1) {
            printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is -1");
            return -1;
        } else {
            printDebug("strset_comp: result of comparing set ", id1, " to set ", id2, " is 1");
            return 1;
        }
    }
}
