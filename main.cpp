#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <cmath>
#include <iomanip>
#include <sstream>

std::string get_config_home()
{
    if (const char *xdg = std::getenv("XDG_CONFIG_HOME"))
        return xdg;

    if (const char *home = std::getenv("HOME"))
        return std::string(home) + "/.config";

    return "";
}

std::filesystem::path state_path()
{
    std::string base = get_config_home();
    if (base.empty())
        return {};

    std::filesystem::path dir = std::filesystem::path(base) / "ledger";
    std::filesystem::create_directories(dir);

    return dir / "ledger.state";
}

bool load_state(int *rank, int *xp, int *focus)
{
    std::filesystem::path file = state_path();

    if (!std::filesystem::exists(file))
        return false;

    std::ifstream in(file);
    if (!in)
        return false;

    std::string line;

    *rank = -1;
    *xp = -1;
    *focus = -1;

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
    }

    return true;
}

void write_state(int rank, int xp, int focus)
{
    std::ofstream out(state_path());

    if (!out)
    {
        std::cerr << "Failed to write state file\n";
        return;
    }

    out << "Rank=" << rank << "\n";
    out << "XP=" << xp << "\n";
    out << "Focus=" << focus << "\n";
}

int xp_required(int rank)
{
    return 50 * rank * rank;
}

void init()
{
    std::cout << "Initialising ledger...\n";

    std::filesystem::path file = state_path();

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

    out << "Rank=1\nXP=0\nFocus=0\n";

    std::cout << "Ledger initialised successfully.\n";
}

void print_row(const std::string &text)
{
    const int width = 28;
    std::cout << "║ " << std::left << std::setw(width) << text << " ║\n";
}

void status()
{
    int rank, xp, focus;

    if (!load_state(&rank, &xp, &focus))
    {
        std::cout << "Ledger not initialised!\n";
        std::cout << "Run `ledger init` first.\n";
        return;
    }

    int xp_needed = xp_required(rank);
    float progress = (float)xp / xp_needed;

    if (progress < 0)
        progress = 0;
    if (progress > 1)
        progress = 1;

    const int bar_width = 16;
    int filled = progress * bar_width;

    std::string bar;

    for (int i = 0; i < bar_width; i++)
        bar += (i < filled ? '#' : '-');

    std::ostringstream progress_text;
    progress_text << "[" << bar << "] "
                  << int(progress * 100) << "%";

    std::ostringstream xp_text;
    xp_text << xp << " / " << xp_needed << " XP";

    std::cout << "\n";
    std::cout << "╔══════════════════════════════╗\n";
    print_row("LEDGER STATUS");
    std::cout << "╠══════════════════════════════╣\n";

    print_row("Rank   : " + std::to_string(rank));
    print_row("XP     : " + std::to_string(xp));
    print_row("Focus  : " + std::to_string(focus));
    print_row("");

    print_row("Level Progress");
    print_row(progress_text.str());
    print_row(xp_text.str());

    std::cout << "╚══════════════════════════════╝\n";
}

void log(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "ledger: missing log entry\n";
        std::cerr << "usage: ledger log \"activity 25m\"\n";
        return;
    }

    std::string entry = argv[2];

    size_t pos = entry.find(' ');

    if (pos == std::string::npos)
    {
        std::cerr << "Invalid log entry format\n";
        std::cerr << "usage: ledger log \"activity 25m\"\n";
        return;
    }

    std::string act = entry.substr(0, pos);

    std::string time = entry.substr(pos + 1);

    char unit = time.back();

    int value = std::stoi(time.substr(0, time.size() - 1));

    if (unit == 'm')
        value *= 60;
    else if (unit == 'h')
        value *= 3600;
    else if (unit != 's')
    {
        std::cerr << "Invalid time unit. Use s, m, or h.\n";
        return;
    }

    int rank, xp, focus;

    if (!load_state(&rank, &xp, &focus))
    {
        std::cerr << "Ledger not initialised!\n";
        std::cerr << "Run `ledger init` first.\n";
        return;
    }

    int gained_xp = value / 10; // scale XP
    int gained_focus = value / 60;

    xp += gained_xp;
    focus += gained_focus;

    while (xp >= xp_required(rank))
    {
        xp -= xp_required(rank);
        rank++;

        std::cout << "★ LEVEL UP! You reached Rank "
                  << rank << " ★\n";
    }

    write_state(rank, xp, focus);

    std::cout << "Logged: " << act
              << " for " << value << " seconds.\n";
}

void reset()
{
    write_state(1, 0, 0);
    std::cout << "Ledger reset successfully.\n";
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
    else if (cmd == "reset")
        reset();
    else if (cmd == "log")
        log(argc, argv);
    else
        std::cout << "ledger: unknown command `" << cmd << "`\n";

    return 0;
}