#include <unittest.h>
#include <iostream>

using namespace std;

int add(int x, int y) {
    return x + y;
}

namespace AddUnitTesting {

    // generic test algorithm
    VOID add_test() {
        int x, y;
        while (cin >> x >> y)
            cout << add(x, y) << endl;
    }

    // each function maps to specific test case
    // test1/test1.in
    // test1/test1.ref
    VOID test1() {
        add_test();
    }

    // test1/test2.in
    // test1/test2.ref
    VOID test2() {
        add_test();
    }

    // test1/test3.in
    // test1/test3.ref
    VOID test3() {
        add_test();
    }


    class TestingSuite: IS_UNITTEST {
    public:
        TestingSuite() : UnitTestSuite() {
            PUSH(test1);
            PUSH(test2);
            PUSH(test3);
        }
    };
}

int main() {
    //RUN_UNIT_PAIRED_WRITE_ONLY(AddUnitTesting,TestingSuite,suite0);

    // Paired
    RUN_UNIT_PAIRED(AddUnitTesting,TestingSuite,suite1);
}
