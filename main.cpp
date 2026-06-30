#include <iostream>
#include "commandParser.h"

int main() {
    CommandParser cli;
    cli.startREPL();
    cli.~CommandParser();
    return 0;
}
