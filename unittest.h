#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

/**********************************************
         _     _ __   _ _____ _______
         |     | | \  |   |      |
         |_____| |  \_| __|__    | (FOR C++)

 UNIT TESTING MODULE FOR OBJECT ORIENTED SOFTWaRE
 AUTHOR hyunwookjamesshin at gmail dot com
 FreeBSD License 

***********************************************
About

    *this file is included in main
     and where you create a customized unit test suite
    *add unittest.cc that goes with this file
    *each test is a function that takes no
    input and returns void. One
    can call another function inside it

**********************************************/
//Levels of hiearchy
//     group :  groups using namespace multiple test suites together
//              and it contains all sample tests to be used by the test suites
//     suite :  each class within the group runs a sequence of sample tests
//     test  :  a sample test to be run (does not have to run at all, or can run
//              in two or more test suites)

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

//-------------------------------------------------
// Change the namespace for Unit Testing Suite (optional)
//-------------------------------------------------

#define NAMESPACE my_name_space

//--------------------------------------------------
// OPTIONAL MACROS (see examples below for usage)
//--------------------------------------------------
#define VOID        static void
#define IS_UNITTEST public  NAMESPACE::UnitTestSuite
#define PUSH(x)     unittests.push_back(&x);

//-------------------------------------------------
// Do Not Change This Section (This is the short activation macro)
// You may change the names 'RUN_UNIT' or 'RUN_UNIT_S'
//-------------------------------------------------

# define RUN_UNIT(nspace,suitename,name) nspace::suitename name = nspace::suitename();\
name.execute_all();

# define RUN_UNIT_S(suitename,name) suitename name = suitename();\
name.execute_all();

//------------------------------------------------
// HOW TO RUN THE UNIT TESTS IN MAIN
//-------------------------------------------------
/*
// Option 1. Without Macro

#include "unittest.h"

int main () {
    NAMESPACEexample::ExampleSuite suite = NAMESPACEexample::ExampleSuite();
    suite.execute_all();
}

// Option 2. With Macro

int main () {
    RUN_UNIT(NAMESPACEexample,ExampleSuite,suite1);
}

// Option 3. No namespace

int main () {
    RUN_UNIT(ExampleSuite,suite1);
}

*/

//--------------------------------------------------
// DO NOT CHANGE BELOW
//--------------------------------------------------
typedef void (*UNITTEST)();

namespace NAMESPACE {
    class UnitTestSuite {
        protected:
            std::vector<UNITTEST> unittests;
        public:
            void add_unit(UNITTEST);
            void execute_all();
    };
}
//--------------------------------------------------
// IOSTREAM TESTING (DO NOT CHANGE)
//--------------------------------------------------
// This is for testing programs with stdin and stdout

// io_emulator injects its buffer into stdin/stdout
// you can input buffer into io_emulator using output
// stream operator <<
// you can also redirect cout bugger to io_emulator
// which you can use it later using operator >> 
// To use io_emulator, call interupt();
// and then CALL release(), to disable io_emulator
// see examples below for details
namespace NAMESPACE {
    class io_emulator {
        std::stringstream is;
        std::stringstream os;
        std::streambuf* previous_i_buffer;
        std::streambuf* previous_o_buffer;
    public:
        void interrupt();
        void release();
        void refresh();
        void fread(std::string filename);
        friend io_emulator & operator<< (io_emulator &ce, std::string s);
        friend io_emulator &operator>> (io_emulator &ce, std::string &s);
    };
}
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
#endif
