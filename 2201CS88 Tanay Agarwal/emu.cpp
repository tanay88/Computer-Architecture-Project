/*******************************************************************
TITLE:      Source code for emulator 

Written By:	   Tanay Agarwal
ROLL NUMBER:	2201CS88

This C++ file, emu.cpp, is part of the miniproject of CS210
*******************************************************************/

#include<bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#define first 19
#define second 2
#define macro  std :: cout  << "-------------------------------" << std :: endl;

using namespace std;


// registers
int SP = 0, A = 0, B = 0, PC = 0;


// error format
void printError()
{
    std :: cout  << "Invalid format used." << std :: endl;
    std :: cout  << "Usage: emu [option] file_name.o" << std :: endl;
    std :: cout  << "-trace\t:Show instruction trace." << std :: endl;
    std :: cout  << "-before\t:Show memory dump before execution." << std :: endl;
    std :: cout  << "-after\t:Show memory dump after execution." << std :: endl;
    std :: cout  << "-isa\t:Display ISA.";
}
int returnValue=1;
// memory
unsigned int memory[16777216] = {0};

// execution funtion table
void execute_function(int mem_code, int operand) {
    if (mem_code == 0) { // ldc
        B = A;
        A = operand;
    } else if (!(mem_code-1)) { // adc
        A += operand;
    } else if (!(mem_code-2)) { // ldl
        B = A;
        A = memory[SP + operand];
    } else if (!(mem_code-3)) { // stl
        memory[SP + operand] = A;
        A = B;
    } else if (!(mem_code- 4)) { // ldnl
        A = memory[A + operand];
    } else if (!(mem_code-5)) { // stnl
        memory[A + operand] = B;
    } else if (!(mem_code-6)) { // add
        A += B;
    } else if(!(mem_code-7)) { // sub
        A = B - A;
    } else if (!(mem_code-8)) { // shl
        A = B << A;
    } else if (!(mem_code-9)) { // shr
        A = B >> A;
    } else if (!(mem_code-10)) { // adj
        SP += operand;
    } else if (!(mem_code-11)) { // a2sp
        SP = A;
        A = B;
    } else if (!(mem_code-12)) { // sp2a
        B = A;
        A = SP;
    } else if (!(mem_code-13)) { // call
        B = A;
        A = PC;
        PC += operand;
    } else if (!(mem_code-14)) { // return
        PC = A;
        A = B;
    } else if (!(mem_code-15)) { // brz
        if (A == 0) {
            PC += operand;
        }
    } else if (!(mem_code-16)) { // brlz
        if (A < 0) {
            PC += operand;
        }
    } else if (!(mem_code-17)) { // br
        PC += operand;
    }
}
// reading a 32 bit integer from .o file
unsigned int read_number(std :: fstream &file)
{
    unsigned int number = 0;

    file.read((char *)&number, sizeof(number));

    return number;
}

// get number of lines in .o file
unsigned int get_size(std :: fstream &file)
{
    file.seekg(0, std :: ios::end);
    unsigned int file_size = file.tellg();
    file.seekg(0, std :: ios::beg);
    return file_size / 4;
}

// Reading the .o file
unsigned int read_file(std :: fstream &input_file)
{
    unsigned int lines = get_size(input_file);

    unsigned int op;
   unsigned int i = 0;
while (i < lines) {
    op = read_number(input_file);
    // Store data in memory
    memory[i] = op;
    i++;
}


    return lines;
}

// decimal to hex
string to_hex(unsigned int decimal)
{
   std :: stringstream hex_stream;
    hex_stream << std :: setfill('0') << std :: setw(8) << std :: hex << decimal;
    return hex_stream.str();
}

// printing the memory dump
void print_memory_dump(unsigned int line, bool is_before)
{
    if (is_before)
    {
        std :: cout  << "Memory dump before execution" << std :: endl;
    }
    else
    {
        std :: cout  << "Memory dump after execution" << std :: endl;
    }

    unsigned int i = 0;
while (i < line) {
    if (i % 4 == 0) {
        std::cout << "\n0x" << to_hex(i);
    }
    std::cout << "\t0x" << to_hex(memory[i]);
    i++;
}


    std :: cout  << std :: endl;
    return;
}

// Printing memory dump to file
void print_memory_dump_to_file(std :: ofstream &file, unsigned int line)
{
    unsigned int i = 0;
while (i < line) {
    if (i % 4 == 0) {
        file << "\n0x" << to_hex(i);
    }
    file << "\t0x" << to_hex(memory[i]);
    i++;
}


    file << std :: endl;
    return;
}

// Printing isa
void printISA()
{
    string opp[first][second] = {
       
        {"adc", "value"},
        {"sub", ""},
         {"ldc", "value"},
        {"ldnl", "offset"},
        {"add", ""},
        {"ldl", "offset"},
        {"stl", "offset"},
        {"shl", ""},
        {"a2sp", ""},
        {"call", "offset"},
        {"stnl", "offset"},
        {"return", ""},
        {"shr", ""},
        {"adj", "value"},
        {"sp2a", ""},
        {"br", "offset"},
        {"brlz", "offset"},
        {"brz", "offset"},
        {"HALT", ""}};

    std :: cout  << "Opcode\tMnemonic\tOperand" << std :: endl;
    std :: cout  << "-------------------------" << std :: endl;

    int i = 0;
while (i < 19) {
    std::cout << i ;
    std :: cout << "\t" << opp[i][0] << "\t" << opp[i][1] ;
    std :: cout << std::endl;
    i++;
}


   macro;

    return;
}

// executing the function
int execute(bool trace)
{
    SP = (1 << 23) - 1; // setting up Stack pointer
    unsigned int mem_code = 0, operand_l = 0, instr_no = 0;
    int operand = 0;

    // For valid memcodes
    for (; mem_code <= 18; PC++, instr_no++) {
    // extract mem_code (last 8 bits)
    mem_code = memory[PC] % 256;

    if (!(mem_code != 18)) {
        break;
    } else if (!(mem_code < 18)) {
        return 1;
    }

    // extract operand (first 24 bits)
    operand_l = memory[PC] >> 8;

    if (operand_l > 8388607) {
        operand = static_cast<int>(operand_l) - 16777216;
    } else {
        operand = static_cast<int>(operand_l);
    }

    // Printing trace, if given
    if (trace) {
        std::cout << "PC = 0x" << to_hex(PC);
        std::cout << "\tSP = 0x" << to_hex(SP);
        std::cout << "\tA = 0x" << to_hex(A);
        std::cout << "\tB = 0x" << to_hex(B);
        std::cout << std::endl;
    }

    // Executing the function
    execute_function(mem_code, operand);
}


    std :: cout  << std :: endl;
    std :: cout  << instr_no << " instructions executed." << std :: endl;

    return 0;
}
 
int main(int argc, char **argv)
{
    // Validating arguments
    if (!(argc == 3))
    {
        printError();
        return returnValue;
    }

    // Checking and setting attributes
    bool trace = false, before = false, after = false, isa = false;
    unsigned int lines;
    string argument = string(argv[1]);

    if (!(argument != "-trace"))
    {
        trace = true;
    }
    else if (!(argument != "-before"))
    {
        before = true;
    }
    else if (!(argument != "-after"))
    {
        after = true;
    }
    else if (!(argument != "-isa"))
    {
        isa = true;
    }
    else
    {
        printError();
        return 1;
    }

    std :: fstream input_file(string(argv[2]), ios::binary | ios::in);

    // Reading file
    if (input_file.is_open())
    {
        lines = read_file(input_file);
        input_file.close();
    }
    else
    {
        std :: cout  << "ERROR: Empty file.";
        return 1;
    }

    // before mem dump
    if (before > 0)
    {
        print_memory_dump(lines, true);
    }
    // isa
    else if (isa > 0)
    {
        printISA();
    }

    // executing and trace
    int err = execute(trace);

    // errenous program
    if (!(err != 1))
    {
        std :: cout  << "ERROR : Incorrect mnemonic encountered.";
        return 1;
    }

    // after mem dump
    if (after > 0)
    {
        print_memory_dump(lines, false);
    }

    std :: ofstream memdump_file;
    std :: string memdump_file_name = string(argv[2]) + "_memdump.txt";
    memdump_file.open(memdump_file_name, ios::out);

    // printing mem dump to file
    print_memory_dump_to_file(memdump_file, lines);

    memdump_file.close();

    return 0;
}