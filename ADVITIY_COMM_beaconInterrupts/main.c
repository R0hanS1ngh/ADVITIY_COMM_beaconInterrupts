#define F_CPU 2000000UL
#include "beaconInterrupts.h"

int main(void)

{
	
	generateBeacon("SOS OSO"); //Input String

	PORTF_DIRSET=PIN6_bm;

	while (1) //Code running in parallel to the interrupt
	{ _delay_ms(1000);
		PORTF_OUTSET=PIN6_bm;
		_delay_ms(1000);
		PORTF_OUTCLR=PIN6_bm;
	}

}


