#pragma once

#include "data.h"

void log(const Instruction& s);

void print_motion(const Motion& motion);

void print_instruction(const Instruction& s);
void print_todo(const Instruction& s);

void print_display(const Message& display);

class Unit;
void log_io(Unit* unit);

extern int log_winks;
