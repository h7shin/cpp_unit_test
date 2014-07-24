#include "example.h"

using namespace std;

NAMESPACEexample::ExampleSuite::ExampleSuite() : UnitTestSuite(){
    PUSH(example_test_1);
    PUSH(example_test_2);
    PUSH(example_test_3);
}

static void NAMESPACEexample::example_test_1() {
    std::cerr << "Example 1 Successful" << std::endl;
}
static void NAMESPACEexample::example_test_2() {
    std::cerr << "Example 2 Successful" << std::endl;
}
static void NAMESPACEexample::input_test_lines() {
    std::string s;
    while (std::getline(std::cin,s)) {
        std::cerr << s << std::endl;
    }
}
static void NAMESPACEexample::input_test_single() {
    std::string s;
    std::cin >> s;
    std::cerr << s << std::endl;
}
static void NAMESPACEexample::input_output_test_single() {
    std::string s;
    std::cin >> s;
    std::cerr << s << std::endl;
    std::cout << "testing-cout" << std::endl << "Successful" << std::endl;
}
static void NAMESPACEexample::example_test_3() {
    std::string s;
    UNIT::io_emulator ce;
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


NAMESPACEmacroexample::ExampleSuiteMacro::ExampleSuiteMacro(): UnitTestSuite(){
    PUSH(example_test_1);
    PUSH(example_test_2);
}

VOID NAMESPACEmacroexample::example_test_1() {
    std::cerr << "Example(Macro) 1 Successful" << std::endl;
}
VOID NAMESPACEmacroexample::example_test_2() {
    std::cerr << "Example(Macro) 2 Successful" << std::endl;
}

VOID NAMESPACEfileexample::example_test_1() {
    int x, y;
    NAMESPACE::file_io_emulator fe("examples/basic/example1.input",
                                   "examples/basic/example1.output",
                                   "examples/basic/example1.ref");
    while(std::cin >> x >> y) {
        std::cout <<add(x, y) << std::endl;
    }
    //std::cout <<add(1,0) << std::endl;
    fe.checkoutput();
    fe.release();
}
VOID NAMESPACEfileexample::example_test_2() {
    int x, y;
    NAMESPACE::file_io_emulator fe("examples/basic/example2.input",
                                   "examples/basic/example2.output",
                                   "examples/basic/example2.ref");
    while(std::cin >> x >> y) {
        std::cout <<add(x, y) << std::endl;
    }
    //std::cout <<add(1,0) << std::endl;
    fe.checkoutput();
    fe.release();
}
int add(int x, int y) {
    return x + y;
}

NAMESPACEfileexample::ExampleSuite::ExampleSuite() : UnitTestSuite() {
    PUSH(example_test_1);
    PUSH(example_test_2);
}
