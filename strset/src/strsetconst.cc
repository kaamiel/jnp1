#include "strsetconst.h"
#include "strset.h"

namespace {

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif
    // Function used to print debug information in case -NDEBUG is not set.
    inline void printDebug(const char *s1) {
        if (debug) {
            std::cerr << s1 << '\n';
        }
    }

    // Function that creates the unmodifiable 42 Set if it has not been created yet
    // and returns its ID.
    unsigned long &init() {
        static unsigned long result;
        static bool initiated = false;
        if (!initiated) {
            initiated = true;
            printDebug("strsetconst init invoked");
            result = ::jnp1::strset_new();
            ::jnp1::strset_insert(result, "42");
            printDebug("strsetconst init finished");
        }
        return result;
    }
}

namespace jnp1 {

    unsigned long strset42() {
        return init();
    }

}