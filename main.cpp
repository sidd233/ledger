#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <filesystem>

std::string get_config_home()
{
    if (const char *xdg = std::getenv("XDG_CONFIG_HOME"))
        return xdg;

    if (const char *home = std::getenv("HOME"))
        return std::string(home) + "/.config";

    return "";
}

void init()
{
    std::cout << "Initialising ledger...\n";

    std::string base = get_config_home();
    if (base.empty())
    {
        std::cerr << "Cannot determine config directory\n";
        return;
    }

    std::filesystem::path dir = base + "/ledger";
    std::filesystem::create_directories(dir);

    std::filesystem::path file = dir / "ledger.state";
    std::ofstream out(file);

    if (!out)
    {
        std::cerr << "Failed to create config file\n";
        return;
    }

    out << "rank=1\nxp=0\nfocus=0";
}

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
        init();
    else if (cmd == "status")
        std::cout << "Ledger not initialized. Run `ledger init`.\n";
    else
        std::cout << "ledger: unknown command `" << cmd << "`\n";

    return 0;
}
