#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "ledger: missing command\n";
        std::cout << "Try `ledger init` or `ledger status`.\n";
        return 0;
    }

    std::string cmd = argv[1];

    if (cmd == "init")
        std::cout << "Initializing ledger...\n";
    else if (cmd == "status")
        std::cout << "Ledger not initialized. Run `ledger init`.\n";
    else
        std::cout << "ledger: unknown command `" << cmd << "`\n";

    return 0;
}
