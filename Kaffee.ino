/**************************************************************/
/**************************************************************/
// Einbinden von wichtigen Biblioteken
#include "SPI.h"   //Schaltet MOSI, MISO, SS, SDA für die Kommunikation frei
#include "RFID.h"  //Beinhaltet die erkennung des RFID Chips
/**************************************************************/
//Wire Biblioteken
#include "Wire.h"         //Schaltet SDA, SCL I²C frei für die Kommunikation
#include "Display_I2C.h"  //Funktionen für die Kommunikation zum LC Display
/**************************************************************/
RFID Modul; // Erstellen eines RFID Objektes Namens Modul
Kaffee Espresso;// Kaffee funktionen einbinden Objekt erstellen
/**************************************************************/
//Inisialiserungs Funktion wird ausgerufen
void setup()
{
  Modul.begin();

}

/**************************************************************/
// loopscheife wir ausgeführt
void loop()
{

}

/**************************************************************/
// user funktion für den Normalen Benutzer
void user()
{

}

/**************************************************************/
// Master funktion für den Administrator
void Master()
{

}
