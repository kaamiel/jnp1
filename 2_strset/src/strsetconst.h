#ifndef STRSETCONST_H
#define STRSETCONST_H

#ifdef __cplusplus
# include <iostream>
namespace jnp1 {
    extern "C" {
#endif

    // Returns ID of a set that cannot be modified and which contains exactly one element:
    // cstring "42". The set is created during on first call of this function and that is when
    // the ID is set.
    unsigned long strset42();

#ifdef __cplusplus
    }
}
#endif

#endif //STRSETCONST_H
