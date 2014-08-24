/*
 * twislave.h
 *
 * Created:		31.05.2014 12:28:19
 * Author:		Scheik
 * Description:	Betrieb eines AVRs mit Hardware-TWI-Schnittstelle als Slave.
 *				Zu Beginn muss init_twi_slave mit der gewünschten Slave-Adresse als
 *				Parameter aufgerufen werden.
 *
 *				Der Datenaustausch mit dem Master erfolgt über den Buffer i2cdata,
 *				auf den von Master und Slave zugegriffen werden kann.
 *				Dies ist für den Slave eine globale Variable (Array aus uint8_t).

 *				Der Zugriff durch den Master erfolgt ähnlich wie bei einem
 *				normalen I2C-EEPROM.
 *				Man sendet zunächst die Bufferposition, an die man schreiben will,
 *				und dann die Daten.
 *				Die Bufferposition wird automatisch hochgezählt, so dass man mehrere
 *				Datenbytes hintereinander schreiben kann, ohne jedesmal die
 *				Bufferadresse zu schreiben.
 *
 *				Um vom Master aus zu lesen, überträgt man zunächst in einem
 *				Schreibzugriff die gewünschte Bufferposition und liest dann nach
 *				einem repeated start die Daten aus. Die Bufferposition wird
 *				automatisch hochgezählt, so dass man mehrere Datenbytes
 *				hintereinander lesen kann, ohne jedesmal die Bufferposition zu
 *				schreiben.
 *
 * Abgefangene Fehlbedienung durch den Master:
 *				- Lesen über die Grenze des txbuffers hinaus
 *				- Schreiben über die Grenzen des rxbuffers hinaus
 *				- Angabe einer ungültigen Schreib/Lese-Adresse
 *				- Lesezugriff, ohne vorher Leseadresse geschrieben zu haben
 */

#ifndef _TWISLAVE_H
#define _TWISLAVE_H

#include <util/twi.h> 														// enthält z.B. die Bezeichnungen für die Statuscodes in TWSR
#include <avr/interrupt.h>													// dient zur Behandlung der Interrupts

// Vom Benutzer konfigurierbare Einstellungen
#define i2c_buffer_size 16													// Grösse des Buffers in Byte (2..254)

//Je nach Statuscode in TWSR müssen verschiedene Bitmuster in TWCR geschreiben werden(siehe Tabellen im Datenblatt!).
//Makros für die verwendeten Bitmuster:

//ACK nach empfangenen Daten senden/ ACK nach gesendeten Daten erwarten
#define TWCR_ACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
//NACK nach empfangenen Daten senden/ NACK nach gesendeten Daten erwarten
#define TWCR_NACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
//switch to the non adressed slave mode...
#define TWCR_RESET TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);

// Globale Variablen, die vom Hauptprogramm genutzt werden
volatile uint8_t i2cdata[i2c_buffer_size];									// Der Buffer, in dem die Daten gespeichert werden. Aus Sicht des Masters läuft der Zugrif auf den Buffer genau wie bei einem I2C-EEPROm ab. Für den Slave ist es eine globale Variable
volatile uint8_t buffer_adr;												// "Adressregister" für den Buffer

// Function init_twi_slave
void init_twi_slave(uint8_t adr);											// Initaliserung des TWI-Inteface. Muss zu Beginn aufgerufen werden, sowie bei einem Wechsel der Slave Adresse. Parameter adr = gewünschte Slave-Adresse

// Fehlerbehandlung
// Bei zu alten AVR-GCC-Versionen werden die Interrupts anders genutzt, daher in diesem Fall mit einer Fehlermeldung abbrechen
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4.5 or later, update to newer AVR-GCC compiler !"
#endif

// Schutz vor unsinnigen Buffergrössen
#if (i2c_buffer_size > 254)
	#error Buffer zu gross gewaehlt! Maximal 254 Bytes erlaubt.
#endif
#if (i2c_buffer_size < 2)
	#error Buffer muss mindestens zwei Byte gross sein!
#endif


#endif //#ifdef _TWISLAVE_H
//// Ende von twislave.h ////
