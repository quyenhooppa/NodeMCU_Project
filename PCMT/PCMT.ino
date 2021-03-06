#include <Wire.h>
#include <Ticker.h>
#include <RFID.h>
#include <EPROM.h>
#include <check_RFID.h>
#include <ControlUsers.h>
#define button_PIN A0
#define LED_OUTPUT D8

bool access_allow=false;
int pointer=0;
int menu_screen=0;
int count = 0;
bool access_DHT=false;
char IDWF[]="AD";
char PASSWF[]="987654321";
button _button(button_PIN);
Ticker State;
void check()
{
  _button.update_button(pointer,access_allow, menu_screen,temp, humid, 
  WF_trig,WF_status,MQTT_status, MQTT_trig,MS_trig, Users_trig, allow_add_del, 
  checkUser, Access_trig, Pin_trig, allow_access_pin, Time_trig);//, day, hr, mins, second);
} 

void setup() 
{                    
  Serial.begin(9600);
  pinMode(LED_OUTPUT,OUTPUT);
  while(!Serial);
  mySerial.begin(9600);
  delay(2000);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC52
  Wire.begin(D2,D3);  
  //dht.setup(10, DHTesp::DHT11); // DHT pin SD3
  dht.begin();
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  State.attach(0.1, check);
  Time.begin();
  EEPROM.begin(512);
  //Write_EEPROM(false, 0, 0, users);
  Read_EEPROM(access_allow, menu_screen, count, users);
  Display(access_allow, pointer, menu_screen,false, false, false);
  //digitalWrite(LED_OUTPUT,HIGH);
}
//unsigned long int time1 = 0;

void loop() {
  check_DHT();
  access_WF(IDWF, PASSWF, pointer);
  access_MQTT(pointer);
  (WF_status)? digitalWrite(LED_OUTPUT,HIGH):digitalWrite(LED_OUTPUT,LOW);
  check_exsit_user(checkUser, menu_screen);
  checkMaster(Users_trig,menu_screen, pointer);
  send_mess(pointer, MQTT_status,MS_trig);
  controlUser(allow_add_del, pointer);
  if (Time_trig == true)
  {
    read_time(WF_status);
  }
  if (access_allow == false)
  {
    if (Access_trig)
    {
      Write_EEPROM(access_allow, menu_screen, count, users);
      if (!mfrc522.PICC_IsNewCardPresent()) return;
      if (!mfrc522.PICC_ReadCardSerial()) return; 
      checkAccess(access_allow, count, Access_trig);
      Write_EEPROM(access_allow, menu_screen, count, users);
    }
    else if (Pin_trig)
    {
      Write_EEPROM(access_allow, menu_screen, count, users);
      accessPin(access_allow, allow_access_pin, Pin_trig);
      Write_EEPROM(access_allow, menu_screen, count, users); 
    }
  }
  if (access_allow && count != 0) 
  {
    count = 0;
    Write_EEPROM(access_allow, menu_screen, count, users);
  }
  if (menu_screen != 2 || menu_screen != 0 || access_DHT == true)
    Write_EEPROM(access_allow, menu_screen, count, users); 
} 
