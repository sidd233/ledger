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

bool load_state(int *rank, int *xp, int *focus, int *items)
{
    std::string base = get_config_home();
    if (base.empty())
        return false;

    std::filesystem::path dir = std::filesystem::path(base) / "ledger";
    std::filesystem::path file = dir / "ledger.state";

    if (!std::filesystem::exists(file))
        return false;

    std::ifstream in(file);
    if (!in)
        return false;

    std::string line;

    while (std::getline(in, line))
    {
        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        int value = std::stoi(line.substr(pos + 1));

        if (key == "Rank")
            *rank = value;
        else if (key == "XP")
            *xp = value;
        else if (key == "Focus")
            *focus = value;
        else if (key == "Items")
            *items = value;
    }

    return true;
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

    if (std::filesystem::exists(file))
    {
        std::cout << "Ledger already initialised.\n";
        return;
    }

    std::ofstream out(file);

    if (!out)
    {
        std::cerr << "Failed to create config file\n";
        return;
    }

    out << "Rank=1\nXP=0\nFocus=0\nItems=0\n";
    std::cout << "Ledger initialised successfully.\n";
}

void status(){
    int rank, xp, focus, items;

    if (!load_state(&rank, &xp, &focus, &items))
    {
        std::cout << "Ledger not initialised!\nTry `ledger init`";
        return;
    }

    std::cout << "---------------------------\n";
    std::cout << "        LEDGER STATUS      \n";
    std::cout << "---------------------------\n";
    std::cout << " Rank   : " << rank << "\n";
    std::cout << " XP     : " << xp << "\n";
    std::cout << " Focus  : " << focus << "\n";
    std::cout << " Items  : " << items << "\n";
    std::cout << "---------------------------\n";
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
        status();
    else
        std::cout << "ledger: unknown command `" << cmd << "`\n";

    return 0;
}
