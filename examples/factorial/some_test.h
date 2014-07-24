#include <unittest.h>

namespace Calculate {
    class Calculator {
    public:
        int add(int a, int b) {return a + b;}
        int sub(int a, int b) {return a - b;}
        int mult(int a, int b) {return a * b;}
        int div(int a, int b) {return a / b;}

    };

    class CalculatorTest: IS_UNITTEST {
        VOID factorial() {
            UNIT::file_io_emulator fe("input.txt","output.txt","ref.txt");
            Calculator calculator = Calculator();
            int factorial = 1;
            int i = 0;
            while (std::cin >> i) {
                while (i > 0) {
                    factorial *= i;
                    i -= 1;
                }
                std::cout << factorial << std::endl;
                factorial = 1;
            }
            fe.checkoutput();   // check output file (location enties) from cout
            fe.release();       // restore stdin and stdout
        }
    public:
        CalculatorTest(): UnitTestSuite() {
            PUSH(factorial);
        }
    };
}
