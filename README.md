cpp_unit_test
=============

Quick and simple Unit Testing Module designed for
Object Oriented Programming written in C++

*****************************************************************
Definitions

     group :  groups using namespace multiple test suites together
              and a pool of sample tests used by the test suites

     suite :  each test suite group runs a sequence of sample tests

     run:     run method in suite object runs the pushed tests

              RUN_UNIT<_<option>>(<namespace>,<suiteclass>,<name>) macro will run the tests

	          RUN_UNIT:        the user is responsible for I/O buffer redirection
                               the user can freely choose how to redirect input/output buffers

              RUN_UNIT_PAIRED: the funtion takes care of file I/O buffer redirection
                               read from one input file under the test directory

     test  :  a sample test to be run (does not have to be used at all, and can be
              called by one or more test suites)

     check :  checks output file with the reference file

              RUN_UNIT:        user defined number of checks per test (0 or more)
                               the user is responsible for checking outputs & errors
                               the user is responsible for saving the passes/fails
              RUN_UNIT_PAIRED: one final I/O check per test (one-to-one pairing)
                               the function checks final outputs and errors
                               the function takes care of updating passes/fails


*****************************************************************
Designing the test suite (paired testing)

Create test subdirectories <test_1>,<test_2>,.. under the directory containing
executable file.

-----------------------------------------------------------------
Group Level (add.h)

    namespace AddUnitTesting {

        // generic test algorithm
        VOID add_test();

        // Create a function "test1" corresponding to test files
        // test1/test1.in (input)
        // test1/test1.out (output)
        // test1/test1.ref (reference for output)
        // test1/test1.err (error)
        // test1/test1.erf (error reference)

        VOID test1();
        // ...

        class TestingSuite: IS_UNITTEST {
        public:
            TestingSuite();
        };
    }

-----------------------------------------------------------------
Suite Level

    AddUnitTesting::TestingSuite::TestingSuite() : UnitTestSuite() {
        PUSH(test1);
        //...
    }

-----------------------------------------------------------------
Test Level

    VOID test1() {
        int x, y;
        while (cin >> x >> y)
            cout << add(x, y) << endl;
    }


*****************************************************************
Designing the test suite (User-defined/unpaired testing)

Suppose that one wishes to test an algorithm that controls movements of a robot.
-----------------------------------------------------------------
Group level (robot_test.h)

    #include <unittest.h>                   // add path to compiler unit test directory

    namespace robot {                       // robot namespace contains all tests/suites
        VOID talk();                        // test case that involves talking
        VOID move_left();                   // test case that involves moving left
        VOID move_right();                  // test case that involves moving right
        VOID move_up();                     // test case that involves moving up
        VOID move_down();                   // test case that involves moving down

        class RobotMovements: IS_UNITTEST {
            public:
            RobotMovements();               // push all relevant test cases into contructor
        };
    }
    -----------------------------------------------------------------
    Suite Level (robot_test.cc)

    robot::RobotMovements::RobotMovements() : UnitTestSuite() {
        PUSH(move_up);
        PUSH(move_right);
        //...
    }
-----------------------------------------------------------------
Test Level (robot_test.cc)

    VOID robot::move_up() {

        // redirect I/O buffers stdin and stdout
        UNIT::file_io_emulator fe("examples/move_up_instruction.input",
                                       "examples/move_up_instruction.output",
                                       "examples/move_up_instruction.ref");
        struct robot = Robot();
        robot.move();       // will read cin input to move up
        fe.checkoutput();   // check output file (location enties) from cout
        fe.release();       // restore stdin and stdout
    }
