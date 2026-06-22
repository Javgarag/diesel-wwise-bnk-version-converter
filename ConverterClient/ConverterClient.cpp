#include <iostream>
#include <filesystem>
#include "soundbank.h"
#include "ak_types.h"

int main()
{
#ifdef NDEBUG
    std::filesystem::path path;
    std::cout << "Path to input soundbank: ";
    std::cin >> path;
    Wwise::Soundbank bank(path);

    int version;
    std::cout << "\nVersion to convert to (2015 == '113', 2022 = '145'): ";
    std::cin >> version;
    bank.Convert((Wwise::BankVersion)version, "output.bnk");
#else
    std::filesystem::path path = "D:\\GitHub\\diesel-wwise-bnk-version-converter\\x64\\Debug\\input_soundbank.bnk";
    Wwise::Soundbank bank(path);
    bank.Convert(Wwise::BankVersion::V2022, "D:\\GitHub\\diesel-wwise-bnk-version-converter\\x64\\Debug\\output_soundbank.bnk");
#endif
}