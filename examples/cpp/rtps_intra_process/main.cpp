#include <iostream>
#include <thread>

#include "fastdds/rtps/RTPSDomain.hpp"

#include "IntraWriter.h"
#include "IntraReader.h"
#include "./settings.hpp"

using eprosima::fastdds::rtps::RTPSDomain;


void showUsage(const std::string& name) {
    std::cerr << "Usage: " << name << " <options>\n"
              << "Options:\n"
              << "  -h --help\t\tShow this help message\n"
              << "  -m --mode <num:2>\tIntraProcess mode: 0-OFF, 1-USER_DATA_ONLY, 2-FULL\n"
              << "  -s --samples <num:0>\tSpecify the number of samples to send (0 for infinite)\n"
              << "  -i --interval <ms:500>  Specify the time interval between samples in milliseconds\n"
              << std::endl;
}

void parseCommandLine(int argc, char** argv, int& samples, int& interval, int &mode) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-s" || arg == "--samples") && i + 1 < argc) {
            samples = std::stoi(argv[++i]);
        } else if ((arg == "-i" || arg == "--interval") && i + 1 < argc) {
            interval = std::stoi(argv[++i]);
        } else if ((arg == "-m" || arg == "--mode") && i + 1 < argc) {
            mode = std::stoi(argv[++i]);
        } else if (arg == "-h" || arg == "--help") {
            showUsage(argv[0]);
            exit(0);
        }
    }
}

int main(
        int argc,
        char** argv)
{
    int mode = 2; // IntraProcess mode. Default: 2 (FULL) 
    int samples = 0; // Default: 0 (infinite)
    int interval = 500; // Default: 500 milliseconds

    parseCommandLine(argc, argv, samples, interval, mode);

    std::cout << "Starting Intra-Process Example" << std::endl;

    set_intra_process_mode(mode);
    // std::cout << "IntraProcess mode: " << XMLProfileManager::library_settings().intraprocess_delivery << std::endl;

    // run reader and writer
    {
        IntraReader TR;
        if (TR.init() && TR.reg())
        {
            // TR.run();
        }

        IntraWriter TW;
        if (TW.init() && TW.reg())
        {
            TW.run(samples, interval);
        }
    }

    RTPSDomain::stopAll();
    std::cout << "EVERYTHING STOPPED FINE" << std::endl;

    return 0;
}
