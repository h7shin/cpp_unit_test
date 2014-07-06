#include "unittest.h"
#include <fstream>

/**********************************************
         _     _ __   _ _____ _______
         |     | | \  |   |      |
         |_____| |  \_| __|__    |

 UNIT TESTING MODULE
 AUTHOR hyunwookjamesshin at gmail dot com
 This is a free software under
 FreeBSD Licence See Details in LICENCE FILE
 
***********************************************/
//--------------------------------------------------
// DO NOT CHANGE BELOW
//--------------------------------------------------
using namespace std;

void NAMESPACE::UnitTestSuite::add_unit(UNITTEST unittest)
{
    unittests.push_back(unittest);
}
void NAMESPACE::UnitTestSuite::execute_all()
{
    int counter = 0;
    for (vector<UNITTEST>::iterator it = unittests.begin();
    it != unittests.end(); ++it) {
        std::cerr << "___Test " << counter << endl;
        (*it)();
        counter++;
    }
}
namespace NAMESPACE {
    io_emulator & operator<< (io_emulator &ce, std::string s) {
        ce.is << s;
        return ce;
    }
    void io_emulator::interrupt() {
        previous_o_buffer = std::cout.rdbuf();
        previous_i_buffer = std::cin.rdbuf();
        std::cin.rdbuf(is.rdbuf());
        std::cout.rdbuf(os.rdbuf());
    }
    void io_emulator::release() {
        is.str("");
        os.str("");
        std::cin.rdbuf( previous_i_buffer );
        std::cout.rdbuf( previous_o_buffer );
    }
    void io_emulator::refresh() {
        release();
        interrupt();
    }
    io_emulator & operator>> (io_emulator &co, std::string &s) {
        co.os >> s;
        return co;
    }
    void io_emulator::fread(std::string filename) {
        std::ifstream file(filename.c_str());
        std::string line;
        if (file.is_open()) {
            while(getline(file, line)) {
                is << line << " \n";
            }
        }
    }
}
