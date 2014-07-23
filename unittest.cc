#include "unittest.h"
#include <fstream>

/**********************************************
         _     _ __   _ _____ _______
         |     | | \  |   |      |
         |_____| |  \_| __|__    |

 UNIT TESTING MODULE
 AUTHOR hyunwookjamesshin at gmail dot com
 CONTACT THE AUTHOR BEFORE USE OR MODIFICATION

***********************************************/
//--------------------------------------------------
// DO NOT CHANGE BELOW
//--------------------------------------------------
using namespace std;

int NAMESPACE::Track::num_failed = 0;
int NAMESPACE::Track::total = 0;

NAMESPACE::Track::Track() {
    num_failed = 0;
    total = 0;
}
void NAMESPACE::Track::add(bool passed) {
    num_failed = (passed) ? num_failed : num_failed + 1;
    total++;
}
void NAMESPACE::Track::print_status() {
    cerr << "------------"<< endl;
    cerr << "TOTAL CHECKS: " << total << " FAILED: " << num_failed << endl;
}
void NAMESPACE::UnitTestSuite::add_unit(UNITTEST unittest) {
    unittests.push_back(unittest);
}
void NAMESPACE::UnitTestSuite::execute_all() {
    int counter = 0;
    Track t = Track();
    for (vector<UNITTEST>::iterator it = unittests.begin();
    it != unittests.end(); ++it) {
        std::cerr << "------------"<< std::endl << "TEST " << counter << endl;
        (*it)();
        counter++;
    }
    t.print_status();
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
    file_io_emulator::~file_io_emulator() {
        std::cin.rdbuf( previous_i_buffer );
        std::cout.rdbuf( previous_o_buffer );
        delete inf;
        delete outf;
    }
    void file_io_emulator::release() {
        std::cin.rdbuf( previous_i_buffer );
        std::cout.rdbuf( previous_o_buffer );
    }
    void file_io_emulator::interrupt() {
        previous_o_buffer = std::cout.rdbuf();
        previous_i_buffer = std::cin.rdbuf();
        inf = new ifstream(inputfile.c_str());
        outf = new ofstream(outputfile.c_str());

        if (inf->is_open()) {
            std::cin.rdbuf(inf->rdbuf());
        } else {
            cerr << "Cannot open file" << inputfile << endl;
        }

        std::cout.rdbuf(outf->rdbuf());
    }
    file_io_emulator::file_io_emulator(std::string inputfile,
                     std::string outputfile) :
                    inputfile(inputfile),outputfile(outputfile) {
        interrupt();
    }
    file_io_emulator::file_io_emulator(std::string inputfile,
                     std::string outputfile,
                     std::string reffile) :
                    inputfile(inputfile),outputfile(outputfile),reffile(reffile) {
        interrupt();
    }
    void file_io_emulator::checkoutput() {
        ifstream outasinf(outputfile.c_str());
        ifstream refasinf(reffile.c_str());
        std::string line_out;
        std::string line_ref;
        Track t = Track();
        int counter = 1;
        if (outasinf.is_open() && refasinf.is_open()) {
            if (outasinf.good() && refasinf.good()) {
                while (getline(outasinf,line_out) && getline(refasinf,line_ref)) {
                    if (line_out != line_ref) {
                        cerr << "DIFFERENCE FOUND AT LINE: "
                         << counter  << endl;
                        cerr << "Reference " << line_ref << endl;
                        cerr << "Output    " << line_out << endl;
                        t.add(0);
                        return;
                    }
                    counter++;
                }
                if (getline(outasinf,line_out)) {
                    cerr << "DIFFERENCE FOUND FOR OUTPUT FILE ADDITIONAL LINE AT:"
                    << counter << " "<< line_out << endl;
                    t.add(0);
                    return;
                } else if (getline(refasinf,line_ref)) {
                    cerr << "DIFFERENCE FOUND FOR REFERENCE FILE ADDITIONAL LINE AT:"
                    << counter << " "<<line_ref << endl;
                    t.add(0);
                    return;
                }
            }
        }
        cerr << "Check passed!" << endl;
    }
}

