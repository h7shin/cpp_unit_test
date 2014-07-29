#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_


/****************************************************************
         _     _ __   _ _____ _______
         |     | | \  |   |      |
         |_____| |  \_| __|__    | (FOR C++)

 UNIT TESTING MODULE FOR OBJECT ORIENTED SOFTWaRE
 AUTHOR hyunwookjamesshin at gmail dot com
 FreeBSD License

*****************************************************************
About

    *this file is included in main
     and where you create a customized unit test suite
    *add unittest.cc that goes with this file
    *each test is a function that takes no
    input and returns void. One
    can call another function inside it

*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>  //io_emulator using stringstream
#include <fstream>  //io_emulator using fstream
///-------------------------------------------------
///  @note (unittest.h) Change the namespace for Unit Testing Suite (optional)
///-------------------------------------------------

#define UNIT my_name_space     // use this macro

// For backward support
#define NAMESPACE UNIT

///--------------------------------------------------
///  @note (unittest.h) OPTIONAL MACROS (see examples for usage)
///--------------------------------------------------
#define VOID            static void
#define IS_UNITTEST     public  UNIT::UnitTestSuite
#define PUSH(x)         unittests.push_back(UNIT::UNITTEST(#x,&x));

//-------------------------------------------------
// Do Not Change This Section (This is the short activation macro)
// You may change the names 'RUN_UNIT' or 'RUN_UNIT_S'
//-------------------------------------------------

# define RUN_UNIT(nspace,suitename,name) nspace::suitename name = nspace::suitename();\
name.execute_all();

# define RUN_UNIT_S(suitename,name) suitename name = suitename();\
name.execute_all();

# define RUN_UNIT_PAIRED(nspace,suitename,name) nspace::suitename name = nspace::suitename();\
name.execute_all_paired();

# define RUN_UNIT_PAIRED_VERBOSE(nspace,suitename,name) nspace::suitename name = nspace::suitename();\
name.execute_all_paired(1);

# define RUN_UNIT_PAIRED_WRITE_ONLY(nspace,suitename,name) nspace::suitename name = nspace::suitename();\
name.execute_all_paired_write_only();

///------------------------------------------------
///  @note (unittest.h) HOW TO RUN THE UNIT TESTS IN MAIN
///-------------------------------------------------
/*
// Option 1. Without Macro

#include "unittest.h"

int main () {
    UNITexample::ExampleSuite suite = UNITexample::ExampleSuite();
    suite.execute_all();
}

// Option 2. With Macro

int main () {
    RUN_UNIT(UNITexample,ExampleSuite,suite1);
}

// Option 3. No namespace

int main () {
    RUN_UNIT_S(ExampleSuite,suite1);
}

// Option 4. Paired Tests
int main () {
    RUN_UNIT_PAIRED(UNITexample,ExampleSuite,suite1);
}

*/
///--------------------------------------------------
/// @note (unittest.h) TRACK NUMBER OF FAILS
///--------------------------------------------------
namespace UNIT {
    class Track{
        static int num_failed;
        static int total;
    public:
        Track();
        void add(bool passed);
        void print_status();
    };
}
//--------------------------------------------------
// DO NOT CHANGE BELOW
//--------------------------------------------------
///--------------------------------------------------
///  @note (unittest.h) Genetic UnitTestSuite Class
///--------------------------------------------------

namespace UNIT {

    typedef struct UnitTest {
        std::string id; // use to pick the right files
        void (*test)();
        UnitTest(std::string id, void (*test)()) : test(test), id(id) {}
    } UNITTEST; // backward support


    class UnitTestSuite {
        protected:
            std::vector<UNITTEST> unittests;
        public:
            void add_unit(UNITTEST);

            // Unpaired testing (open)

            // run each test as is (simply run the method as implemented)
            // I/O redirects is the responsibility of the client module
            // see examples for details

            // this is useful for interactive testing / debugging

            // Use RUN_UNIT macro for this method
            void execute_all();

            // Paired testing (one-to-one)

            // each test case is mapped to a specific input/output files
            // execute all tests with corresponding input/output/files.
            // Input,output,reference files must be named <testname>.in
            // <testname>.out, <testname>.ref, under <testname> subdirectory.
            // This is for batch (regression) testing

            // All file I/O buffer redirections (including restorations of
            // cin/cout)
            // are handled and abstracted
            // away from the client module

            // Use RUN_UNIT_PAIRED macro for this method
            // verbose option if 1 prints the content of the output/error
            // to the terminal
            void execute_all_paired() {execute_all_paired(0);}

            void execute_all_paired(bool verbose);

            // execute_all_paired_write_only execute all tests from
            // an input file, and writes to the reference (for both
            // output and error) files.
            // Warning: this function will OVERWRITE all existing
            // reference files, before running this functions make
            // sure to have a backup copy.

            // Use RUN_UNIT_PAIRED_WRITE_ONLY macro for this method
            void execute_all_paired_write_only();

    };
}

///--------------------------------------------------
///  @note (unittest.h) IOSTREAM TESTING - USING STRINGSTREAM (DO NOT CHANGE)
///--------------------------------------------------
// This is for testing programs with stdin and stdout

// io_emulator injects string buffer into stdin (cin)
// and gets output as stringstream
// stream operator "<<" is used to inject/get stream
// To use cin_emulator, call interupt();
// and then call release(), to disable cin_emulator
// see examples below for details
// You can only use io_emulator for non-paired testing
// (see above)
namespace UNIT {
    class io_emulator {
        std::stringstream is;
        std::stringstream os;
        std::streambuf* previous_i_buffer;
        std::streambuf* previous_o_buffer;
    public:
        io_emulator() {}
        void interrupt();
        void release();
        void refresh();
        void fread(std::string filename);
        friend io_emulator & operator<< (io_emulator &ce, std::string s);
        friend io_emulator & operator>> (io_emulator &ce, std::string &s);
    };
}
///--------------------------------------------------
/// @note (unittest.h) IOSTREAM TESTING - USING FILES (DO NOT CHANGE)
///--------------------------------------------------

namespace UNIT {

    class file_io_emulator {

        std::ifstream* inf;
        std::ofstream* outf;
        std::ofstream* errf;
        std::string inputfile;
        std::string outputfile;

        std::string errorfile;           //stderror file (optional)
        std::string reffile;             //reference file (optional)
        std::string errorreffile;        //reference file (optional)

        std::stringstream *consolestream;

        std::streambuf* previous_i_buffer;
        std::streambuf* previous_o_buffer;
        std::streambuf* previous_e_buffer;

        bool verbose;               // option for printintg out contents of files

        void interrupt();           // activate redirection
        Track t;                    // correctness checks tracker

        void checkfiles(std::string, std::string);

        void flushconsole();

    public:

        friend file_io_emulator &operator<<(file_io_emulator&, std::string s);

        file_io_emulator();

        file_io_emulator(bool verbose);

        // for checking I/O with reference file
        file_io_emulator(std::string inputfile,std::string outputfile,std::string reffile);

        //  for checking I/O with reference file (with stderror redirection)
        file_io_emulator(std::string inputfile,std::string outputfile,
                         std::string reffile,std::string errorfile,std::string errorreffile);

        // for simply writing to the output file
        file_io_emulator(std::string inputfile,std::string outputfile);

        // reset
        void reset(std::string inputfile,std::string outputfile,std::string reffile);

        // with standard error
        void reset(std::string inputfile,std::string outputfile,std::string reffile,
                   std::string errorfile,std::string errorreffile);

        // check file output with the reference file
        void checkoutput();

        // check file output with the reference error file
        void checkerror();

        ~file_io_emulator();
        void release();           // restore cin/cout/cerr (must do)

    };
}
#endif
