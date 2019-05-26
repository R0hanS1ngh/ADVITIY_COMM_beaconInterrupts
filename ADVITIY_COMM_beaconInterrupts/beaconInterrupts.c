/*
 * beaconInterrupts.c
 *
 * Created: 26-05-2019 20:06:58
 *  Author: Rohan Singh
 */ 
#include <avr/interrupt.h>
#include <string.h>
#include "pmic_driver.h"
#include "beaconInterrupts.h"

unsigned char toGet[HMDATALEN]; //Array containing all the operations to be performed to get the given string as morse
volatile int order=0; //The order of operation to be performed from toGet
volatile int totalLength; //Size of the toGet array being used up


unsigned char morse[27][8]={{0,3,1,4,2,2,2,2},{1,3,0,3,0,3,0,4},{1,3,0,3,1,3,0,4},
{1,3,0,3,0,4,2,2},{0,4,2,2,2,2,2,2},{0,3,0,3,1,3,0,4},
{1,3,1,3,0,4,2,2},{0,3,0,3,0,3,0,4},{0,3,0,4,2,2,2,2},
{0,3,1,3,1,3,1,4},{1,3,0,3,1,4,2,2},{0,3,1,3,0,3,0,4},
{1,3,1,4,2,2,2,2},{1,3,0,4,2,2,2,2},{1,3,1,3,1,4,2,2},
{0,3,1,3,1,3,0,4},{1,3,1,3,0,3,1,4},{0,3,1,3,0,4,2,2},
{0,3,0,3,0,4,2,2},{1,4,2,2,2,2,2,2},{0,3,0,3,1,4,2,2},
{0,3,0,3,0,3,1,4},{0,3,1,3,1,4,2,2},{1,3,0,3,0,3,1,4},
{1,3,0,3,1,3,1,4},{1,3,1,3,0,3,0,4},{2,2,2,2,2,2,2,5}};
/*26 Alphabets + WORD space	*/

// 0: Dot
// 1: Dash
// 2: Nothing
// 3: Intra-Letter Gap
// 4: Inter-Letter Gap
// 5: WORD Gap


void generateBeacon(char input[])
{
	int length= strlen(input); //Length of string input

	int integerInput[length]; //Array containing integer values corresponding to each character from input string

	for(int j=0;j<length;j++)
	{
		if(((int)input[j])==32) //Condition for [space]
		{integerInput[j]= 26;}
		else                    //Condition for other characters
		{integerInput[j]= input[j]-'A';}
	}

	totalLength=8*length;
	for(int m=0; m<HMDATALEN; m++) //Convert each value in toGet array to '2'(action corresponding to do nothing) to avoid garbage values in the unused space
	{
		toGet[m]=2;
	}

	for (int k=0;k<length;k++)
	{for(int l=0;l<8;l++) //Concatenating each individual character operations to make the all-inclusive toGet array
		{
			toGet[(8*k)+l]=morse[integerInput[k]][l];
		}
	}


	PORTF_DIRSET=PIN1_bm;

	PMIC_EnableHighLevel();				//Enable interrupts : High level for timer
	TCC0.CTRLA = TC_CLKSEL_DIV64_gc;		//Set Prescaler 1(Same as CPU_PRESCALER)
	TCC0.CTRLB= TC_WGMODE_NORMAL_gc;    //Wave generation mode : Normal
	TCC0.INTCTRLA = TC_OVFINTLVL_HI_gc;	//Enable overflow interrupt

	TCC0.PER = 100;		    //Initialize Period
	TCC0.CNT=0;


	sei(); //Set interrupt
}

ISR(TCC0_OVF_vect) //Pin for interrupt: Port F- Pin 1
{
	if(toGet[order]==0)
	{   TCC0.PER = DOT;
		PORTF_OUTSET=PIN1_bm;
	}
	
	else if(toGet[order]==1)
	{   TCC0.PER = DASH;
		PORTF_OUTSET=PIN1_bm;
	}
	
	else if(toGet[order]==3)
	{   TCC0.PER = DOT;
		PORTF_OUTCLR=PIN1_bm;
	}
	
	else if(toGet[order]==4)
	{   TCC0.PER = DASH;
		PORTF_OUTCLR=PIN1_bm;
	}
	
	else if(toGet[order]==5)
	{	TCC0.PER = WORD;
		PORTF_OUTCLR=PIN1_bm;
	}
	
	else if(toGet[order]==2)
	{
		;
	}
	order++;
	
	if(order==totalLength) //close the ISR
	{   TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
		cli();
	}
}