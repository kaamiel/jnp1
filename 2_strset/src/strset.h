#ifndef STRSET_H
#define STRSET_H

#ifndef __cplusplus
#include <stddef.h>
#endif

#ifdef __cplusplus
# include <iostream>

namespace jnp1 {
    extern "C" {
#endif

    // Creates a new set and returns its ID.
    unsigned long strset_new();

    // If there exists a set having ID @id, deletes it. Otherwise does nothing.
    void strset_delete(unsigned long id);

    // If there exists a set having ID @id, returns quantity of its elements. Otherwise returns 0.
    size_t strset_size(unsigned long id);

    // If there exists a set having ID @id and it does not contain element @value,
    // element @value is added to it. Otherwise does nothing.
    void strset_insert(unsigned long id, const char *value);

    // If there exists a set having ID @id and it contains element @value,
    // deletes element @value from the set. Otherwise does nothing.
    void strset_remove(unsigned long id, const char *value);

    // If there exists a set having ID @id and it contains element @value returns 1.
    // Otherwise returns 0.
    int strset_test(unsigned long id, const char *value);

    // If there exists a set having ID @id, deletes all its elements.
    // Otherwise does nothing.
    void strset_clear(unsigned long id);

    // Compares sets having IDs @id1 and @id2.
    int strset_comp(unsigned long id1, unsigned long id2);

#ifdef __cplusplus
    }
}
#endif

#endif //STRSET_H
