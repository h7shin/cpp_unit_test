#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include "unittest.h"

int add(int x, int y);

//--------------------------------------------------
// @note (example.h) EXAMPLES: HOW TO MAKE CUSTOMIZE TESTING SUITE
//--------------------------------------------------
// Note that you can create a separate
// testing file to run unit tests,
// and simply remove that file from
// the project once finished testing

// @note  (example.h) Example 1.
// example.h
// ifndef _EXAMPLE_H_
// define _EXAMPLE_H_
// ...
namespace NAMESPACEexample { // namespace is optional
    static void example_test_1();
    static void example_test_2();
    static void input_test_lines();
    static void input_test_single();
    static void input_output_test_single();
    static void example_test_3() ;
    //...
    class ExampleSuite: public  NAMESPACE::UnitTestSuite {
        public:
            ExampleSuite();
    };
}
// endif
//--------------------------------------------------
// @note (example.h) Example 2.
// example.h
// ifndef _EXAMPLE_H_
// define _EXAMPLE_H_
// ...
namespace NAMESPACEmacroexample { // namespace is optional
    VOID example_test_1();
    VOID example_test_2();
    //...
    class ExampleSuiteMacro: IS_UNITTEST {
            public:
            ExampleSuiteMacro();
    };
}
// endif
//--------------------------------------------------
// @note (example.h) Example 3.

// file input/output redirection and output checking

namespace NAMESPACEfileexample {
    VOID example_test_1();
    VOID example_test_2();
    class ExampleSuite: IS_UNITTEST {
        public:
        ExampleSuite();
    };
}

#endif
