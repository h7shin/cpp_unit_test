#include <unittest.h>


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
    cerr << "------------"<< "\n";
    cerr << "TOTAL CHECKS: " << total << " FAILED: " << num_failed << "\n";
}
void NAMESPACE::UnitTestSuite::add_unit(UNITTEST unittest) {
    unittests.push_back(unittest);
}
void NAMESPACE::UnitTestSuite::execute_all() {
    int counter = 0;
    Track t = Track();
    for (vector<UNITTEST>::iterator it = unittests.begin();
    it != unittests.end(); ++it) {
        std::cerr << "------------"<< "\n" << "TEST " << counter << "\n";
        (it->test)();
        counter++;
    }
    t.print_status();
}

void NAMESPACE::UnitTestSuite::execute_all_paired(bool verbose) {
    int counter = 0;
    Track t = Track();
    file_io_emulator fe = file_io_emulator(verbose);
    fe << "UNIT TESTING RESULTS " << "\n"; //overloaded operator
    fe << "------------"<< "\n";
    for (vector<UNITTEST>::iterator it = unittests.begin();
    it != unittests.end(); ++it) {
       fe << it->id << "\n";
       fe.reset(it->id+"/"+it->id+".in",
                 it->id+"/"+it->id+".out",
                 it->id+"/"+it->id+".ref",
                 it->id+"/"+it->id+".err",
                 it->id+"/"+it->id+".erf"
                 );
        (it->test)();
        fe << "Output Check: ";
        fe.checkoutput();
        fe << "Error Check : ";
        fe.checkerror();
        fe << "------------"<< "\n";
        counter++;
    }
    fe.release();
    t.print_status();
}
void NAMESPACE::UnitTestSuite::execute_all_paired_write_only() {
    int counter = 0;
    Track t = Track();
    file_io_emulator fe = file_io_emulator();
    for (vector<UNITTEST>::iterator it = unittests.begin();
    it != unittests.end(); ++it) {
        fe << it->id << "\n";;
        fe.reset(it->id+"/"+it->id+".in",
                 it->id+"/"+it->id+".ref",
                 it->id+"/"+it->id+".ref",
                 it->id+"/"+it->id+".erf",
                 it->id+"/"+it->id+".erf"
                 );
        (it->test)();
        counter++;
    }
    fe.release();
    t.print_status();
}

//---------------------------------------------------------------------

namespace NAMESPACE {

    file_io_emulator &operator<<(file_io_emulator &fe, std::string s) {
        (*(fe.consolestream)) << s;
        return fe;
    }

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

    // ----------------------------------------------------------------------
    void file_io_emulator::flushconsole() {
        std::string s;

        std::cout<< consolestream->str() << std::endl;
        (*consolestream).clear();
        (*consolestream).str("");
    }
    file_io_emulator::~file_io_emulator() {
        std::cin.rdbuf( previous_i_buffer );
        std::cout.rdbuf( previous_o_buffer );
        std::cerr.rdbuf( previous_e_buffer );
        flushconsole();
        delete inf;
        delete outf;
        if (errorfile != "")
            delete errf;
    }
    void file_io_emulator::release() {
        std::cin.rdbuf( previous_i_buffer );
        std::cout.rdbuf( previous_o_buffer );
        std::cerr.rdbuf( previous_e_buffer );
        flushconsole();
    }
    void file_io_emulator::interrupt() {
        previous_o_buffer = std::cout.rdbuf();
        previous_i_buffer = std::cin.rdbuf();
        previous_e_buffer = std::cerr.rdbuf();
        inf = new ifstream(inputfile.c_str());
        outf = new ofstream(outputfile.c_str());

        if (errorfile != "") {
            errf = new ofstream(errorfile.c_str());
            std::cerr.rdbuf(errf->rdbuf());
        }

        if (inf->is_open()) {
            std::cin.rdbuf(inf->rdbuf());
        } else if (inputfile  != "") {
            cerr << "Cannot open file '" << inputfile << "' "<< "\n";
        }

        std::cout.rdbuf(outf->rdbuf());

    }
    file_io_emulator::file_io_emulator() : inputfile(""),outputfile(""),reffile(""),errorfile(""),errorreffile("")  {
        t = Track();
        consolestream = new stringstream("");
        interrupt();
    }
    file_io_emulator::file_io_emulator(bool verbose) : verbose(verbose),inputfile(""),outputfile(""),reffile(""),errorfile(""),errorreffile("")  {
        t = Track();
        consolestream = new stringstream("");
        interrupt();
    }
    file_io_emulator::file_io_emulator(std::string inputfile,
                     std::string outputfile) :
                    inputfile(inputfile),outputfile(outputfile),errorfile(""),errorreffile("") {
        t = Track();
        consolestream = new stringstream("");
        interrupt();
    }
    file_io_emulator::file_io_emulator(std::string inputfile,
                     std::string outputfile,
                     std::string reffile) :
                    inputfile(inputfile),outputfile(outputfile),reffile(reffile),errorfile(""),errorreffile("") {
        t = Track();
        consolestream = new stringstream("");
        interrupt();
    }
    file_io_emulator::file_io_emulator(std::string inputfile,
                     std::string outputfile,
                     std::string reffile, std::string errorfile,
                     std::string errorreffile) :
                    inputfile(inputfile),outputfile(outputfile),reffile(reffile),
                    errorfile(errorfile),errorreffile(errorreffile) {
        t = Track();
        consolestream = new stringstream("");
        interrupt();
    }
    void file_io_emulator::reset(std::string inputfile,
                     std::string outputfile,
                     std::string reffile)
                   {
        this->inputfile = inputfile;
        this->outputfile = outputfile;
        this->reffile = reffile;
        release();
        flushconsole();
        interrupt();
    }
    void file_io_emulator::reset(std::string inputfile,
                     std::string outputfile,
                     std::string reffile,std::string errorfile,
                     std::string errorreffile)
                   {
        this->inputfile = inputfile;
        this->outputfile = outputfile;
        this->reffile = reffile;
        this->errorfile = errorfile;
        this->errorreffile = errorreffile;
        release();
        interrupt();
    }

    void file_io_emulator::checkoutput() {
        checkfiles(outputfile,reffile);
    }
    void file_io_emulator::checkerror() {
        checkfiles(errorfile,errorreffile);
    }
    void file_io_emulator::checkfiles(std::string check, std::string ref) {
        ifstream outasinf(check.c_str());
        ifstream refasinf(ref.c_str());
        std::string line_out;
        std::string line_ref;
        bool out_avail;
        bool ref_avail;
        int counter = 1;
        if (outasinf.is_open() && refasinf.is_open()) {
            if (outasinf.good() && refasinf.good()) {
                if (verbose) (*consolestream) << endl;
                out_avail = getline(outasinf,line_out);
                ref_avail = getline(refasinf,line_ref);

                while (out_avail && ref_avail) {
                    if (verbose) (*consolestream) << counter << " :" << line_out << endl;
                    if (line_out != line_ref) {
                        (*consolestream) << "DIFFERENCE FOUND AT LINE: "
                        << counter  << "\n";
                        (*consolestream) << "Reference " << line_ref << "\n";
                         (*consolestream) << "Output    " << line_out << "\n";
                        t.add(0);
                        return;
                    }
                    counter++;
                    out_avail = getline(outasinf,line_out);
                    ref_avail = getline(refasinf,line_ref);
                }
                if (out_avail) {
                     (*consolestream) << "DIFFERENCE FOUND FOR OUTPUT FILE ADDITIONAL LINE AT: "
                    << counter << "\n"<< line_out << "\n";
                    t.add(0);
                    return;
                } else if (ref_avail) {
                    (*consolestream)  << "DIFFERENCE FOUND FOR REFERENCE FILE ADDITIONAL LINE AT: "
                    << counter << "\n"<<line_ref << "\n";
                    t.add(0);
                    return;
                }
            t.add(1);
            (*consolestream) << "Check passed!" << "\n";
            return;
            }

        }
        t.add(0);
        (*consolestream) << "Couldn't fetch files! " << check << " or " << ref << "\n";
    }
}

