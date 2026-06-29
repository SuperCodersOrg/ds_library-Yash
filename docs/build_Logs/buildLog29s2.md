# Date: June 29 (Session 2)
Duration: 95 minutes

Goal: 
Implement a Command Parser (CLI/REPL) for `RedisLite` in separate files (`commandParser.h` and `commandParser.cpp`) to allow terminal-based interaction. The implementation must strictly align with the "build-from-scratch" philosophy of `SuperCodersCollectionsLibrary`, meaning we avoid built-in standard library helpers like `<sstream>` (`std::stringstream`) and `<algorithm>` (`std::transform`).

Problem Encountered: 
1. Splitting strings (tokenization) to extract commands, keys, and values is normally handled trivially by `std::stringstream`, so doing it manually required robust index tracking to handle varying amounts of whitespace.
2. Achieving case-insensitive command parsing without `std::transform` meant we needed a manual way to identify and convert characters to uppercase.

## What I Tried: 
1. **Created Class Structure**: Designed `CommandParser` with a `startREPL()` method for the interactive loop and a `parseAndExecute()` method to handle input logic.
2. **Manual Tokenization**: Built `while` loops that iterate character-by-character over the input string, manually identifying spaces (`' '`) and tabs (`'\t'`) to accurately extract the `command`, `key`, and `value` substrings.
3. **Manual Uppercase Conversion**: Utilized ASCII math (subtracting `32`) to detect and convert lowercase characters (`'a'` to `'z'`) into uppercase characters for the command string.
4. **Integration**: Stripped static tests from `main.cpp`, hooking it up to start the `CommandParser` REPL. Updated `CMakeLists.txt` to compile the new `.cpp` file.

## Outcome: 
Successfully built a robust, from-scratch CLI for `RedisLite`! Users can now launch the compiled binary and interactively run `SET`, `GET`, `DEL`, `EXIST`, `CLEAR`, `SIZE`, `EMPTY`, and `EXIT` commands via the terminal. The parser correctly processes variable-length inputs without relying on standard parsing libraries, proving the effectiveness of manual string manipulation algorithms in C++.
