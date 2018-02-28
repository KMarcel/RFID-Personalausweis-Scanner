/****************************************************************************
  Titel:    Kaffee Dienstausweisscanner RFID
  Autor:    Marcel Kühl <Marcel.Kuehl@web.de>  http://github.com/KMarcel
  Datei:    RFID-Kaffee
  Software: Arduino IDE
  SoftwateID: Version 1.6 || vom 12.02.2018
*****************************************************************************/
//Biblioteken einbinden
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <TimerOne.h>
/******************************************************************************/
//Derektiven
#define SS_PIN 10
#define RST_PIN 9
#define Speicherplatze 7
#define Kosten_Tasse_Kaffee 0.30//0,30€
#define Version "Version 1.6 || vom 12.02.2018"
#define EEPROM_Accountzeiger 14
#define EEPROM_Lehrjahr 15
#define TasterFreischalten 2
/*****************************************************************************/
//Global Variablen definieren
// Eine Struktur eines Accountsanlegen
// Für mehr Informationen gucken Sie in dem Dokument.
struct index {
  byte ID[4];
  int AccountID;
  byte Kontostand;
  boolean Rechte;
};
struct index Account;
byte readCard[4]; // Die Eingelesene ID Nummer in 4 Blöcken
byte Accountzeiger;	// Wie viele Accounts schon vergeben wurden sind
int AccountID;
byte inAktive;
boolean Wiederkehr;
byte Jahresanzeige_Global;
/*****************************************************************************/
// Objekte Anlegen
LiquidCrystal_I2C lcd(0x20, 16, 2); // LC-Display Objekt erstellen und Konfigurieren 3F
MFRC522 mfrc522(SS_PIN, RST_PIN);
/***/
//Unterprogramme
void Admin_initialisieren() {
  if (EEPROM.read(1) != 125) {
    textausgabe("Neuer Admin 1", "suche...");
    while (!getID()) {}
    for (int a = 0; a < 4; a++) {
      EEPROM.write(a + 2, readCard[a]);
    }
    textausgabe("Neuer Admin 1", "Angelegt");
    delay(2000);
    textausgabe("Neuer Admin 2", "suche...");
    while (!getID()) {}
    for (int a = 0; a < 4; a++) {
      EEPROM.write(a + 6, readCard[a]);
    }
    textausgabe("Neuer Admin 2", "Angelegt");
    delay(2000);
    textausgabe("Neuer Admin 3", "suche...");
    while (!getID()) {}
    for (int a = 0; a < 4; a++) {
      EEPROM.write(a + 10, readCard[a]);
    }
    textausgabe("Neuer Admin 3", "Angelegt");
    delay(2000);
    EEPROM.write(1, 125);
  }
  Accountzeiger = EEPROM.read(EEPROM_Accountzeiger);
}
boolean getID() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  for (int i = 0; i < 4; i++)
  {
    readCard[i] = mfrc522.uid.uidByte[i];
  }
  inAktive = 0;
  return true;
}
boolean ActiveButtonRead(byte Pin) {
  if (digitalRead(Pin))
  {
    inAktive = 0;
  }
  return digitalRead(Pin);
}
boolean checkDatabase(byte readCardlocal[]){
  byte korrektnumber;
  for(int i = 16; i<1008;i += 7)
  {
    for(int a = 0; a<4; a++)
    {
      if(readCardlocal[a] == EEPROM.read(i+a))
      {
        korrektnumber++;
      }else{
        break;      
      }
    }
    AccountID = i;
    if(korrektnumber == 4)
    {
      return true;
    }
    korrektnumber = 0;
  }
  return false;
}
boolean checkTwo(byte readCardlocal[],byte readOldCardlocal[]){
  byte korrektnumber;
    for(int a = 0; a<4; a++)
    {
      if(readCardlocal[a] == readOldCardlocal[a])
      {
        korrektnumber++;
      }else{
        break;      
      }
    }
    if(korrektnumber == 4)
    {
      return true;
    }
  return false;
}
boolean isMaster(byte readCardlocal[]){
    byte korrektnumber = 0;
    for(int b = 0; b<9; b+= 4){
      for(int a = 0; a<4; a++){
        Serial.print("ReadCard: ");
        Serial.print(readCardlocal[a]);
        Serial.print(" = EEPROMCard");
        Serial.print(EEPROM.read(2+b+a));
        Serial.println();
        if(readCardlocal[a] == EEPROM.read(2+b+a))
        {
          korrektnumber++;
          
        }else{
          break;      
        }
      }
      Serial.println(korrektnumber);
      if(korrektnumber == 4)
      {
        return true;
      }
      korrektnumber = 0;
    }
    return false;
}
void inaktive() {
  if (inAktive == 11) {
    inAktive = 0;
  }
  inAktive++;
}
//LCD Ausgabe
void textausgabe(int Zeile1,String Zeile2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.setCursor(0, 1);
  lcd.print(Zeile2);
}
void textausgabe(int Zeile1,String Zeile2 ,int Zeile3) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.setCursor(0, 1);
  lcd.print(Zeile2);
  lcd.print(Zeile3);
}
void textausgabe(String Zeile1,String Zeile2 ,int Zeile3) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.setCursor(0, 1);
  lcd.print(Zeile2);
  lcd.print(Zeile3);
}
void textausgabe(String Zeile1,int Zeile2 ,String Zeile3) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.print(Zeile2);
  lcd.setCursor(0, 1);  
  lcd.print(Zeile3);
}
void textausgabe(String Zeile1,int Zeile2 ,String Zeile3 ,int Zeile4) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.print(Zeile2);
  lcd.setCursor(0, 1);
  lcd.print(Zeile3);
  lcd.print(Zeile4);
}
void textausgabe(String Zeile1,int Zeile2 ,double Zeile3 ,String Zeile4) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.print(Zeile2);
  lcd.setCursor(0, 1);
  lcd.print(Zeile3);
  lcd.print(Zeile4);
}
void textausgabe(String Zeile1, String Zeile2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Zeile1);
  lcd.setCursor(0, 1);
  lcd.print(Zeile2);
}
//Admin
void Menu_Admin(){
  int Menuzeiger = 1, Zustandsanderung = 2;
  bool Neuerstand, Alterstand;

  while (true)
  {
    Neuerstand = ActiveButtonRead(4);
    if (ActiveButtonRead(5))
    {
      switch (Menuzeiger)
      {
        case 1:
          AusbilderErstellen();
          break;
        case 2:
          AusbilderLoschen();
          break;
        case 3:
          Neuer_Nutzer();
          break;
        case 4:
          Lehrjahr_einstellen();
          break;
        case 5:
          Master_backwords();
          break;
        case 6:
          Wiederkehr = true;
          break;
      }
    }
    if (Neuerstand != Alterstand && (Alterstand = Neuerstand))
    {
      Menuzeiger++;
      if (Menuzeiger > 6)
      {
        Menuzeiger = 1;
      }
      delay(100);
    }
    if (Menuzeiger != Zustandsanderung)
    {
      Zustandsanderung = Menuzeiger;
      switch (Menuzeiger)
      {
        case 1:
          textausgabe("Menu", "-> AB Erstellen");
          break;
        case 2:
          textausgabe("   AB Erstellen", "-> AB Loeschen");
          break;
        case 3:
          textausgabe("   AB Loeschen", "->Nutzer Anlegen");
          break;
        case 4:
          textausgabe("  Nutzer Anlegen", "-> Lehrjahr");
          break;
        case 5:
          textausgabe("   Lehrjahr", "-> Master Del");
          break;
        case 6:
          textausgabe("   Master Del", "-> Abbrechen");
          break;
      }
    }
    if(Wiederkehr == true)
    {
      return;
    }
    if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        return;
    }
  }
}
void AusbilderErstellen() {
  textausgabe("Neue Ausbilder", "Karte suche...");
  while(true){
    while(!getID()){
      if(inAktive == 10){
        inAktive = 0;
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        textausgabe("Menu", "-> AB Erstellen");
        return;
      }
    }
    if(!checkDatabase(readCard))
    {
      textausgabe("Unbekannte", "Karte");
      delay(2000);
      textausgabe("kein Konto", "vorhanden");
      delay(2000);
      return;
    }
    if(EEPROM.read(AccountID+5)== 0){break;}
    }
    EEPROM.write(AccountID+5,1);
  textausgabe("Ausbilder", "Karte Erstellt");
  delay(4000);
  textausgabe("Menu", "-> AB Erstellen");
}
void AusbilderLoschen() {
  textausgabe("Ausbilder", "Karte Loeschen");
  while(true){
    while(!getID()){
      if(inAktive == 10){
        inAktive = 0;
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        textausgabe("   AB Erstellen", "-> AB Loeschen");
        return;
      }
    }
    if(!checkDatabase(readCard))
    {
      textausgabe("Unbekannte", "Karte");
      delay(2000);
      textausgabe("kein Konto", "vorhanden");
      delay(2000);
      return;
    }
    if(EEPROM.read(AccountID+5)== 0){break;}
    }
    EEPROM.write(AccountID+5,1);
  textausgabe("Ausbilder", "Karte Geloescht");
  delay(4000);
  textausgabe("   AB Erstellen", "-> AB Loeschen");
}
void Neuer_Nutzer(){
  bool localuser = true;
  bool Yeslocaluser = false;
  textausgabe("Neuer Dienst-", "ausweis gesucht...");
  while(true)
  {
    while(!getID()){
      if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        textausgabe("   AB Loeschen", "->Nutzer Anlegen");
        return;
      }
    }
    if(!checkDatabase(readCard)){
      break;
    }else
    {
      textausgabe("Nutzer bereits", "Bekannt");
      delay(1000);
      textausgabe("   AB Loeschen", "->Nutzer Anlegen");
      return;
    }
  }
  textausgabe("Nutzer Anlegen ?", "JA          NEIN");
  delay(1000);
  while (true)
  {
    if (ActiveButtonRead(4))
    {
      if(!Lehrjahr_Auswahlen()){
        
      }      
      textausgabe("Neuer Nutzer", "Angelegt");
      delay(2000);
      inAktive = 0;
      Wiederkehr = false;      
      Yeslocaluser = true;
      break;      
    }
    if (ActiveButtonRead(5))
    {
      textausgabe("Vorgang", "Abgebrochen");
      delay(2000);
      Wiederkehr = false;
      inAktive = 0;
      textausgabe("   AB Loeschen", "->Nutzer Anlegen");
      return;      
    }
    if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        textausgabe("   AB Loeschen", "->Nutzer Anlegen");
        return;
    }
 }
 if(Yeslocaluser)
 {
  new_Scanned_card(readCard);
  textausgabe("   AB Loeschen", "->Nutzer Anlegen");
 }
}
boolean new_Scanned_card(byte readCardlocal[]){
  for(int a = 0; a<4; a++)
  {
    EEPROM.write(Accountzeiger*7+a+16,readCardlocal[a]);
  }
  EEPROM.write(Accountzeiger*7+4+16, 0);
  EEPROM.write(Accountzeiger*7+5+16, 0);
  EEPROM.write(Accountzeiger*7+6+16, Jahresanzeige_Global);
  Accountzeiger++;
  EEPROM.write(14, Accountzeiger);
}
boolean Lehrjahr_Auswahlen(){
  int Menuzeiger1 = 1, Zustandsanderung1 = 2;
  bool Neuerstand1, Alterstand1;
  int Jahr = EEPROM.read(EEPROM_Lehrjahr)+2000;
  while (true)
  {
    Neuerstand1 = ActiveButtonRead(4);
    if (ActiveButtonRead(5))
    {
      switch (Menuzeiger1)
      {
        case 1:
          Jahresanzeige_Global = (Jahr-2000);
          return true;
        case 2:
          Jahresanzeige_Global = (Jahr-2001);
          return true;
        case 3:
          Jahresanzeige_Global = (Jahr-2002);
          return true;
        case 4:
          Jahresanzeige_Global = (Jahr-2003);
          return true;
        case 5:
          Jahresanzeige_Global = 255;
          return true;
      }
    }
    if (Neuerstand1 != Alterstand1 && (Alterstand1 = Neuerstand1))
    {
      Menuzeiger1++;
      if (Menuzeiger1 > 6)
      {
        Menuzeiger1 = 1;
      }
      delay(100);
    }
    if (Menuzeiger1 != Zustandsanderung1)
    {
      Zustandsanderung1 = Menuzeiger1;
      switch (Menuzeiger1)
      {
        case 1:
          textausgabe("Lehrjahr", "-> Lehrjahr ", Jahr);
          break;
        case 2:
          textausgabe("   Lehrjahr ",Jahr, "-> Lehrjahr ", (Jahr-1));
          break;
        case 3:
          textausgabe("   Lehrjahr ",(Jahr-1), "-> Lehrjahr ", (Jahr-2));
          break;
        case 4:
          textausgabe("   Lehrjahr ",(Jahr-2), "-> Lehrjahr ", (Jahr-3));
          break;
        case 5:
          textausgabe("   Lehrjahr ",(Jahr-3), "-> Ausbilder");
          break;
      }
    }
    if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        return false;
    }
  }
}
void Lehrjahr_einstellen(){
  int Lehrjahr = EEPROM.read(EEPROM_Lehrjahr)+2000;
  textausgabe("Waehlen Sie das","neuste Lehrjahr");
  delay(2000);
  readCard[0] = 0;
  readCard[1] = 0;
  readCard[2] = 0;
  readCard[3] = 0;
  while(true)
  {
    textausgabe(Lehrjahr-1,"-> ",Lehrjahr);
    if (ActiveButtonRead(4)) {
      Lehrjahr++;
    }
    if (ActiveButtonRead(5)) {
      Lehrjahr--;
    }
    getID();
    if(isMaster(readCard))
    {
      Serial.println("------------------------------");
      EEPROM.write(EEPROM_Lehrjahr,(Lehrjahr-2000));
      textausgabe("Lehrjahr", "geaendert");
      Lehrjahr_Update();
      delay(2000);
      textausgabe("  Nutzer Anlegen", "-> Lehrjahr");
      return;
    }
    if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        textausgabe("  Nutzer Anlegen", "-> Lehrjahr");
        return;
    }
  }
}
void Lehrjahr_Update(){
  for(int i = 16; i<Accountzeiger*7+16;i += 7)
  {
    if(EEPROM.read(i+6)<(EEPROM.read(EEPROM_Lehrjahr)-4))
    {
      for(byte o = 0; o<7; o++){
        EEPROM.write(i+o,0);
      }
        for(byte o = 0; o<7; o++){
          EEPROM.write(i+o,EEPROM.read(i+7+o));
          EEPROM.write(i+7+o,0);
        } 
      if(Accountzeiger > 0){
      Accountzeiger--;
      EEPROM.write(EEPROM_Accountzeiger,Accountzeiger);
      }
    }
  }
}
void Master_backwords(){
  delay(2000);
  textausgabe("Master erneuern ?", "JA          NEIN");
  while(true){
    if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        textausgabe("   Lehrjahr", "-> Master Del");
        return;
      }
    if(ActiveButtonRead(4))
    {
      textausgabe("Master", "Gelöscht");
      delay(2000);
      textausgabe("Das Geraet", "Ausschalten");
      break;
    }else if(ActiveButtonRead(5)){
      textausgabe("Vorgang", "Abgebrochen");
      delay(2000);
      textausgabe("   Lehrjahr", "-> Master Del");
      return; 
    }
  }  
  EEPROM.write(1,0);
  while(true){}
}
//Ausbilder
void Menu_Ausbilder() {
  textausgabe("Getr. Waehlen 10", "Kontoaufladen V");
  digitalWrite(TasterFreischalten, LOW);
  delay(1000);
  textausgabe("Getr. Waehlen ", "Kontoaufladen V");
  for (int i = 9; i >= 0; i--)
  {
    lcd.setCursor(14, 0);
    lcd.print(i);
    for(int a = 0; a<1000; a++){
      if(ActiveButtonRead(5))
      {
        Kontoaufladen();
        return;
      }
      delay(1);
    }
  }
  digitalWrite(TasterFreischalten, HIGH);
  Account.Kontostand--;
  EEPROM.write(Account.AccountID + 4, Account.Kontostand);
}
void Kontoaufladen(){
  byte Tassen = 1;
  double Geld = Kosten_Tasse_Kaffee;  
   
  textausgabe("Azubi Karte", "suche...");
  while(EEPROM.read(AccountID+5) == 1){
    while(!getID()){
      if(inAktive == 10){
          textausgabe("Abgebrochen", "wegen Zeit");
          delay(1000);
          return;
      }
    }
    if(!checkDatabase(readCard)){
      textausgabe("Unbekannte", "Karte");
      delay(2000);
      textausgabe("kein Konto", "vorhanden");
      delay(2000);
      return;
    }
  }
  int AccountID_Local = AccountID;
  textausgabe("Kaffee: ", Tassen, Geld, " Euro");
  while (true)
  {
    if (ActiveButtonRead(4)) {
      if(Tassen > 100){
      Tassen = 100;
      Geld =  Kosten_Tasse_Kaffee*100;
    }
    if(Tassen < 1){
      Tassen = 1;
      Geld =  Kosten_Tasse_Kaffee;
    }
      Tassen++;
      Geld= Geld+Kosten_Tasse_Kaffee;
      textausgabe("Kaffee: ", Tassen, Geld, " Euro");
      while(ActiveButtonRead(4)){delay(1);}
    }
    if (ActiveButtonRead(5)) {
      if(Tassen > 100){
      Tassen = 100;
      Geld =  Kosten_Tasse_Kaffee*100;
    }
    if(Tassen < 1){
      Tassen = 1;
      Geld =  Kosten_Tasse_Kaffee;
    }
      Tassen--;
      Geld= Geld-Kosten_Tasse_Kaffee;
      textausgabe("Kaffee: ", Tassen, Geld, " Euro");
      while(ActiveButtonRead(5)){delay(1);}
    }
    getID();
    if(checkTwo(readCard, Account.ID))
    {
      EEPROM.write(AccountID_Local+4,(EEPROM.read(AccountID_Local+4)+Tassen));
      textausgabe("Konto wurde", "Aufgeladen");
      delay(2000);
      return;
    }
    if(inAktive == 10){
        textausgabe("Abgebrochen", "wegen Zeit");
        delay(1000);
        return;
    }
  }
}

//Azubi
void Menu_Azubi() {
  if (Account.Kontostand == 0)
  {
    textausgabe("Nicht Genug", "Guthaben");
    delay(2000);
    textausgabe("Benachtigen Sie", "einen Ausbilder");
    delay(2000);
    return;
  }
  textausgabe("Getr. Waehlen 10", "Kontostand: ");
  lcd.setCursor(12, 1);
  lcd.print(Account.Kontostand);
  digitalWrite(TasterFreischalten, LOW);
  delay(1000);
  textausgabe("Getr. Waehlen ", "Kontostand: ");
  lcd.setCursor(12, 1);
  lcd.print(Account.Kontostand);
  for (int i = 9; i >= 0; i--)
  {
    lcd.setCursor(14, 0);
    lcd.print(i);
    delay(1000);
  }
  digitalWrite(TasterFreischalten, HIGH);
  Account.Kontostand--;
  EEPROM.write(Account.AccountID + 4, Account.Kontostand);
}
//Initialisieren
void setup() {
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(inaktive);
  Serial.begin(9600);
  lcd.begin();
  pinMode(TasterFreischalten, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  Admin_initialisieren();
  /*****************************************/
  // LCD Initialisieren auf dem Display ausgeben
  textausgabe("** RFID-Chip **", "*Initialisiert*");
  delay(3000);
  /*****************************************/
  digitalWrite(TasterFreischalten, HIGH);
}
//Endlosschleife
void loop() {
  textausgabe("Dienstausweis", "suche...");
  if(Accountzeiger == 125){
    while(true){
      textausgabe("* EEPROM ERROR *", "");
    }
  }
  while (!getID()) {Wiederkehr = false;}
  textausgabe("*Karte Erkannt*", "");
  if(!login()){
    return;
  }
  if(!isMaster(readCard))
  {
    if(Account.Rechte == 1){
      Menu_Ausbilder();
      return;
    }else if(Account.Rechte == 0){
      Menu_Azubi();
      return;
    }
  }else{
    Menu_Admin();
    return;
  }
}
boolean login(){
  if(!isMaster(readCard) && !checkDatabase(readCard))
  {
    textausgabe("Karte Unbekannt","Ausbilder");
    delay(3000);
    textausgabe("Ausbilder","Kontaktieren");
    delay(3000);
    return false;
  }else if(!isMaster(readCard)){
    for(byte i = 0; i<4; i++)
    {Account.ID[i] = readCard[i];}
    Account.AccountID = AccountID;
    Account.Kontostand = EEPROM.read(AccountID+4);
    Account.Rechte = EEPROM.read(AccountID+5);
    return true;
  }else if(isMaster(readCard)){
    return true;
  }
}

