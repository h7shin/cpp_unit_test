#include "unittest.h"
//--------------------------------------------------
// EXAMPLES: HOW TO MAKE CUSTOMIZE TESTING SUITE
//--------------------------------------------------
// Note that you can create a separate
// testing file to run unit tests,
// and simply remove that file from
// the project once finished testing

// Example 1.
// example.h
// ifndef _EXAMPLE_H_
// define _EXAMPLE_H_
// ...
namespace NAMESPACEexample { // namespace is optional
    static void example_test_1() {
        std::cerr << "Example 1 Successful" << std::endl;
    }
    static void example_test_2() {
        std::cerr << "Example 2 Successful" << std::endl;
    }
    static void input_test_lines() {
        std::string s;
        while (std::getline(std::cin,s)) {
            std::cerr << s << std::endl;
        }
    }
    static void input_test_single() {
        std::string s;
        std::cin >> s;
        std::cerr << s << std::endl;
    }
    static void input_output_test_single() {
        std::string s;
        std::cin >> s;
        std::cerr << s << std::endl;
        std::cout << "testing-cout" << std::endl << "Successful" << std::endl;
    }
    static void example_test_3() {
        std::string s;
        NAMESPACE::io_emulator ce;
        ce.interrupt();

        ce << "Example 3 testing cin and end-line \n using emulator";

        input_test_lines();

        // You must use release and interrupt (or refresh) to
        // call consecutive << or >> operators (but you can get
        // cout buffer for every input without refreshing)

        ce.refresh();

        ce << "Successful";

        input_test_single();

        ce.refresh();

        ce << "Test";

        input_output_test_single();

        // stdout trapped in ce string buffer
        ce >> s;
        std::cerr << s << " ";

        ce >> s;
        std::cerr << s << std::endl;

        ce.refresh();

        // Reading from a file
        ce.fread("sample_file.txt");
        input_test_lines();

        // always call release when you are done using
        // the emulator (or you will get SIGSEV fault)
        ce.release();
    }
    //...
    class ExampleSuite: public  NAMESPACE::UnitTestSuite {
             public:
            ExampleSuite() : UnitTestSuite(){
                unittests.push_back(&example_test_1);
                unittests.push_back(&example_test_2);
                unittests.push_back(&example_test_3);
            }
    };
}
// endif
//--------------------------------------------------
// Example 2.
// example.h
// ifndef _EXAMPLE_H_
// define _EXAMPLE_H_
// ...
namespace NAMESPACEmacroexample { // namespace is optional
    VOID example_test_1() {
        std::cerr << "Example(Macro) 1 Successful" << std::endl;
    }
    VOID example_test_2() {
        std::cerr << "Example(Macro) 2 Successful" << std::endl;
    }
    //...
    class ExampleSuiteMacro: IS_UNITTEST {
            public:
            ExampleSuiteMacro(): UnitTestSuite(){
                PUSH(example_test_1);
                PUSH(example_test_2);
            }
    };
}
// endif
