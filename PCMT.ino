#include <Wire.h>
#include <Ticker.h>
#include <RFID.h>
#include <check_RFID.h>
#include <ControlUsers.h>
#define button_PIN A0
    

bool access_allow=false;
int pointer=0;
int menu_screen=0;
bool access_DHT=false;
char IDWF[]="Blue Lagoon_Khach";
char PASSWF[]="24bluelagoon";


button _button(button_PIN);
Ticker State;
void check()
{
  _button.update_button(pointer,access_allow, menu_screen,temp, humid, WF_trig,WF_status,MQTT_status, MQTT_trig,MS_trig, Users_trig, allow_add_del);
}

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  mySerial.begin(115200);
  delay(1000);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC52
  Wire.begin(D1,D2);
  dht.setup(D3, DHTesp::DHT11);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  State.attach(0.1, check);
  Display(access_allow, pointer, menu_screen,false, false, false);
}


void loop() {
  check_DHT();
  access_WF(IDWF, PASSWF, pointer);
  access_MQTT(pointer);
  checkMaster(Users_trig,menu_screen, pointer);
  send_mess(pointer, MQTT_status,MS_trig);
  controlUser(allow_add_del, pointer);
  if (access_allow == false)
  {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) return; 
    checkAccess( access_allow);
    Display(access_allow, pointer, menu_screen, false, false,false);
  }
} 
