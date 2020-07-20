#include "CliApp.hpp"

#include <algorithm>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <libgen.h>
#include <unistd.h>
#include <cmath>
#include <csignal>

using namespace std;

CliApp::~CliApp() {
    if (history_fn != NULL) {
        saveHistory();

        free(history_fn);
        history_fn = NULL;
    }
}

command_vec CliApp::init(const command_t* cmds, const char* pargs, int argc, char* const argv[]) {
    commands = cmds;

    command_vec argcommand;

    progName = basename(argv[0]);

    // parse as options only string before commands
    // as commands can include negative number (-1..), don't allow getopt
    // processing of command part

    int commandStart = argc;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            commandStart = i;

            for (; i < argc; i++) {
                argcommand.push_back(argv[i]);
            }

            break;
        }

        const char* a = strchr(pargs, argv[i][1]);

        if (a && a[1] == ':' && ((strlen(argv[i]) == 2) || (argv[i][1] == '-'))) {
            i++;
        }
    }

    int opt = -1;

    while ((opt = getopt(commandStart, argv, pargs)) != -1) {
        processArg(opt, optarg);
    }

    return argcommand;
}

void CliApp::printAppHelp() {
    cout << progName << " " << description << endl << endl;
    printUsage();
}

void CliApp::printHelp(const char* cmd) {
    command_vec possible;
    const command_t* c = findCommand(cmd, possible);

    if (c == NULL) {
        unknowCommand(cmd, possible);
        return;
    }

    printCommandHelp(c);
}

int CliApp::helpCommands(command_vec cmds) {
    if (cmds.empty()) {
        printGenericHelp();
        std::cout << "Commands:" << std::endl;
        printCommands();
        return 0;
    }

    for (auto cm : cmds) {
        if (cm == "all") {
            for (const command_t* c = commands; c->command != NULL; c++) {
                printCommandHelp(c);
            }

            return 0;
        }

        printHelp(cm.c_str());
    }

    return 0;
}

void CliApp::goInteractive(const char* prompt) {
    asprintf(&history_fn, "%s/.%s_history", getenv("HOME"), progName);

    using_history();
    int rr = read_history(history_fn);

    if (rr != ENOENT) {
        if (rr != 0) {
            std::cerr << "Error reading history " << history_fn << ":" << strerror(rr) << std::endl;
        }

        else if (verbose) {
            std::cout << "Read history from " << history_fn << std::endl;
        }
    }

    char* buf;

    signal(SIGINT, [](int) { exit(0); });

    while ((buf = readline(prompt)) != NULL) {
        if (strlen(buf) > 0) {
            add_history(buf);
        }

        processBuffer(buf);
    }

    if (verbose) {
        std::cerr << "Exiting" << std::endl;
    }
}

/**
 * Transform string to lower case.
 */
static inline std::string strlower(const std::string s) {
    std::string ret = s;
    std::transform(ret.begin(), ret.end(), ret.begin(), (int (*)(int))tolower);
    return ret;
}

/**
 * Transform string to upper case.
 */
static inline std::string strupper(const std::string s) {
    std::string ret = s;
    std::transform(ret.begin(), ret.end(), ret.begin(), (int (*)(int))toupper);
    return ret;
}

vector<string> tokenize(const string& input, const string& delim = std::string(" ")) {
    vector<string> vecTokens;

    // Do we have a delimited list?
    if (input.size() > 0) {
        string::size_type nStart = 0;
        string::size_type nStop = 0;
        string::size_type nDelimSize = delim.size();
        string szItem;

        // Repeat until we cannot find another delimitor.
        while ((nStop = input.find(delim, nStart)) != string::npos) {
            // Pull out the token.
            szItem = input.substr(nStart, nStop - nStart);
            vecTokens.push_back(szItem);
            // Set the new starting position after the delimitor.
            nStart = nStop + nDelimSize;
        }
        // Are there any chars left after the last delim?
        if (nStop == string::npos && nStart < input.size()) {
            // There are chars after the last delim - this is the last token.
            szItem = input.substr(nStart, input.size() - nStart);
            vecTokens.push_back(szItem);
        }
    }
    return vecTokens;
}

int CliApp::processBuffer(const char* buf) {
    command_vec cmds = tokenize(buf);

    if (cmds.empty()) {
        return -1;
    }

    return processCmdVector(cmds);
}

void CliApp::readCommands(const char* fn) {
    if (strcmp(fn, "-")) {
        std::ifstream ins(fn);

        if (!ins) {
            std::cerr << "Cannot open " << fn << ": " << strerror(errno) << std::endl;
            return;
        }

        readStreamCommands(ins);
    }

    else {
        readStreamCommands(std::cin);
    }
}

int CliApp::processCmdVector(command_vec cmds) {
    std::string cmd = strlower(cmds.front());

    command_vec matchedCmds;

    const command_t* c = findCommand(cmd, matchedCmds);

    if (c == NULL) {
        if (matchedCmds.empty()) {
            return processUnmached(cmds);
        }

        else {
            std::cerr << "multiple commands matching " << cmd << ":";

            for (auto c : matchedCmds) {
                std::cerr << " " << c;
            }

            std::cerr << std::endl;
        }

        return -1;
    }

    cmds.erase(cmds.begin());
    return processCommand(c, cmds);
}

void CliApp::saveHistory() {
    if (history_fn != NULL) {
        int wr = write_history(history_fn);

        switch (wr) {
            case 0:
                if (verbose) {
                    std::cout << "History saved to " << history_fn << std::endl;
                }

                break;

            default:
                std::cerr << "Unable to save history to " << history_fn << ":" << strerror(wr) << std::endl;
        }
    }
}

/**
 * Verify if passed arguments match argument map.
 *
 * Returns >= 0 as number of matching arguments. Print error message and returns
 * -1 if there isn't a match.
 */
int verifyArguments(const command_vec& cmds, const char* args) {
    auto verifyFloat = [](const char* d) -> int {
        try {
            std::stod(d);
            return true;
        } catch (...) {
            return false;
        }
    };

    auto verifyInteger = [](const char* i) -> int {
        try {
            std::stoi(i);
            return true;
        } catch (...) {
            return false;
        }
    };

    auto verifyBool = [](const char* b) -> int {
        std::string s = strupper(b);
        return (s == "TRUE" || s == "FALSE");
    };

    size_t an = 0;

    for (const char *a = args; *a; a++, an++) {
        if (an >= cmds.size()) {
            if (*a == 's' || *a == 'f' || *a == 'i' || *a == 'b' || *a == 'd' || *a == 'h' || *a == '?') {
                return an;
            }

            std::cerr << "Required arguments are missing, expected at least " << (an + 1) << ", got "
                      << cmds.size() << std::endl;
            return -1;
        }

        switch (*a) {
            case '?':
                return cmds.size();
            case 'F':
            case 'f':
                if (!verifyFloat(cmds[an].c_str())) {
                    std::cerr << "Expecting double number, received " << cmds[an].c_str() << std::endl;
                    return -1;
                }

                break;

            case 'I':
            case 'i':
                if (!verifyInteger(cmds[an].c_str())) {
                    std::cerr << "Expecting integer number, received " << cmds[an].c_str() << std::endl;
                    return -1;
                }

                break;

            case 'B':
            case 'b':
                if (!verifyBool(cmds[an].c_str())) {
                    std::cerr << "Expecting boolean (true/false), received " << cmds[an].c_str() << std::endl;
                    return -1;
                }

                break;

            case 'S':
            case 's':
                break;

            default:
                std::cerr << "Invalid formatting character " << *a << std::endl;
                return -1;
        }
    }

    return an;
}

int CliApp::processCommand(const command_t* cmd, const command_vec& args) {
    try {
        if (verifyArguments(args, cmd->args) >= 0) {
            return cmd->action(args);
        }
    }

    catch (std::exception& err) {
        cerr << "Processing " << cmd->command;

        for (auto ar : args) {
            cerr << " " << ar;
        }

        cerr << ": " << err.what() << endl;
    }

    return -1;
}

int CliApp::processUnmached(command_vec cmds) {
    std::cerr << "Unknown command: " << cmds.front() << std::endl;
    return -1;
}

void CliApp::printCommands() {
    for (const command_t* p = commands; p->command != NULL; p++) {
        std::cout << " " << p->command << std::endl;
    }
}

const command_t* CliApp::findCommand(std::string cmd, command_vec& matchedCmds) {
    const command_t* ret = NULL;

    for (const command_t* tc = commands; tc->command != NULL; tc++) {
        if (strncmp(cmd.c_str(), tc->command, cmd.length()) == 0) {
            matchedCmds.push_back(tc->command);
            ret = tc;
        }
    }

    if (matchedCmds.size() != 1) {
        return NULL;
    }

    return ret;
}

void CliApp::unknowCommand(std::string cmd, const command_vec matchedCmds) {
    if (matchedCmds.empty()) {
        std::cerr << "Unknow command " << cmd << ", please see help for allowed commands." << std::endl;
        return;
    }

    std::cerr << "Possible matches:";

    for (auto cm : matchedCmds) {
        std::cerr << " " << cm;
    }

    std::cerr << std::endl;
}

void CliApp::readStreamCommands(std::istream& ins) {
    while (!ins.eof()) {
        char cmdbuf[500];
        memset(cmdbuf, 0, sizeof(cmdbuf));
        ins.getline(cmdbuf, sizeof(cmdbuf));

        if (cmdbuf[0] == 0 and ins.eof()) {
            break;
        }

        if (cmdbuf[0] == '#' || cmdbuf[0] == '\0') {
            continue;
        }

        processBuffer(cmdbuf);
    }
}

void CliApp::printCommandHelp(const command_t* cmd) {
    std::cout << std::endl << " * " << strupper(cmd->command) << std::endl << std::endl;

    if (cmd->help_args) {
        std::cout << cmd->help_args << std::endl << std::endl;
    }

    std::cout << cmd->help << std::endl << std::endl;
}
