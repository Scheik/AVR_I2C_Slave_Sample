/*
Testprogramm für den Slave
Der Buffer wird mit Werten gefüllt. Dann wird er fortlaufend über die serielle Schnittstelle ausgegeben.
Nun kann man dort sehen, wenn der Master einen Wert ändert
*/
#include <util/twi.h> 	    //enthält z.B. die Bezeichnungen für die Statuscodes in TWSR
#include <avr/interrupt.h>  //dient zur Behandlung der Interrupts
#include "global.h"
#include "twislave.h"

#define SLAVE_ADRESSE 0x50 //Die Slave-Adresse

int main (void)
{
	init_twi_slave(SLAVE_ADRESSE);					//TWI als Slave mit Adresse slaveadr starten
	sei();

	while(1)
	{

	} //end.while
} //end.main
