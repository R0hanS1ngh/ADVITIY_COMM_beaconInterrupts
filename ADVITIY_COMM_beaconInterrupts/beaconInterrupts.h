/*
 * beaconInterrupts.h
 *
 * Created: 26-05-2019 20:07:43
 *  Author: Rohan Singh
 */ 


#ifndef BEACONINTERRUPTS_H_
#define BEACONINTERRUPTS_H_
#include "avr_compiler.h"
#define DOT 3000
#define DASH 9000
#define WORD 12000
// The length of a dot is 1 time unit:DOT
// A dash is 3 time units:DASH
// The space between symbols (dots and dashes) of the same letter is 1 time unit:DOT
// The space between letters is 3 time units:DASH
// The space between words is 7 time units:DASH+WORD
#define HMDATALEN 800 //Maximum number of HM Data operations required

void generateBeacon(char input[]);

#endif /* BEACONINTERRUPTS_H_ */