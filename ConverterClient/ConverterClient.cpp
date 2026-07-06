#include <fstream>
#include "soundbank.h"
#include "ak_types.h"

std::string find_option_argument(int argc, char* argv[], std::string option) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == option) {
            try {
                return argv[i + 1];
            }
            catch (...) {
                std::cout << "\tUsage: <.bnk input file> [-o output_file] [-v end_version]" << std::endl;
                std::cerr << "Malformed option: " << option << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
    }

    return "";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: <.bnk input file> [-o output_file] [-v end_version]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::filesystem::path path = argv[1];
    std::filebuf in_buff;
    std::istream bnk_in(&in_buff);

    if (!in_buff.open(path, std::ios::in | std::ios::binary)) {
        std::cerr << "ERROR: Failed to open file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    Wwise::Soundbank bank(bnk_in);
    in_buff.close();

    int version;
    if (find_option_argument(argc, argv, "-v") != "") {
        version = std::stoi(find_option_argument(argc, argv, "-v"));
    }
    else {
        std::cout << "\nVersion to convert to (2015 == '113', 2022 = '145'): ";
        std::cin >> version;
    }

    std::filesystem::path output_path;
    if (find_option_argument(argc, argv, "-o") != "") {
        output_path = find_option_argument(argc, argv, "-o");
    }
    else {
        std::cout << "\nOutput path (with extension): ";
        std::cin >> output_path;
    }

    std::filebuf out_buff;
    std::ostream bnk_out(&out_buff);

    if (!out_buff.open(output_path, std::ios::out | std::ios::binary)) {
        std::cerr << "ERROR: Failed to write new file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    bank.Convert((Wwise::BankVersion)version, bnk_out);
    bnk_out.flush();
    out_buff.close();
}