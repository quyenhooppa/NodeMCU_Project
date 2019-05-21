#include <Wire.h>
#include <check_RFID.h>

#define button_A0 A0

long int Time = 0;

bool state_pointer = false, state_press = false;
int button_pointer = LOW, button_press = LOW;
String uid = "";
int pointer = 0;
bool allow_access = false;
int add_del = 0;
int menu = 0;

String Users[NUM_OF_USERS] = {" ", " ", " "};  

void setup() 
{
  Serial.begin(115200);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC52
  pinMode(button_A0, INPUT);
  Wire.begin(5,4);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  Display(allow_access, menu, add_del);
}

void button_choose()
{
  int button_A0 = analogRead(A0);
  if (button_A0 > 520 && button_A0 < 560 && state_pointer == false)
  {
    button_pointer = HIGH;
    state_pointer = true;
  }
  else if (button_A0 > 740 && button_A0 < 780 && state_press == false)
  {
    button_press = HIGH;
    state_press = true;
  }
  else if ((button_A0 <= 520 || button_A0 >= 560) && state_pointer == true)
    state_pointer = false;
  else if ((button_A0 <= 740 || button_A0 >= 780) && state_press == true)
    state_press = false;
  return;
}

///****************************************************///

void Display(bool access_allow, int &menu, int add_del)
{
  if (access_allow == false)
  {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println(">:Control users         ");
      lcd.setCursor(0, 1);
      lcd.println(uid);
  }
  else
  {
    if (add_del == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println(">: Add user                 ");
      lcd.setCursor(0, 1);
      lcd.println(" : Remove users             ");
      menu = 0;
    }
    else if (add_del == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println(" : Add user                 ");
      lcd.setCursor(0, 1);
      lcd.println(">: Remove user              ");  
      menu = 1;  
    }
    else if (add_del == 2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("     Existed    ");      
    }
    else if (add_del == 3)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("     No card     ");      
    }
    else if (add_del == 4)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("  Successfully   ");           
    }
  }
}

///***************************************************///


void loop() {
  if (millis() - Time > 20)
  {
    button_choose();
    if (button_pointer == HIGH)
    {
      allow_access = false;
      button_pointer = LOW;
    }
    if (button_press == HIGH)
    {
      if (menu == 0)
        add_del = 1;
      else 
        add_del = 0;
      button_press = LOW;
    }
    Time = millis();
  }
  if (allow_access == false)
  {
    button_pointer = LOW;
    button_press = LOW;
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) return;
    uid = RFID_read();
    if (uid == MASTER)
    {
      allow_access = true;
      add_del = 0;
    }
  }
  else if (allow_access == true)
  {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) return;
    uid = RFID_read();
    control_users(uid, menu, add_del, Users);
    delay(1000);
    Display(allow_access, menu, add_del);
    delay(1000);
    add_del = menu;
  }
  Display(allow_access, menu, add_del);
}
