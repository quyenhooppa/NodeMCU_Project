<<<<<<< HEAD
#include <SPI.h>
#include <MFRC522.h>
#include <Display_RFID.h>
MFRC522 mfrc522(2, HIGH);   // Create MFRC522 instance.
#define MASTER "30 D7 52 73"
#define NUM_OF_USERS 3

String RFID_read()
{
	//===================Show UID on serial monitor=====================

	Serial.println();
	Serial.print(" UID tag :");
	String content = "";
	byte letter;
	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
		content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(mfrc522.uid.uidByte[i], HEX));
	}
	content.toUpperCase();
	return content.substring(1);
}

void checkAccess(String uid, bool &access_allow)
{
		//====================check id=======================================
		if (uid == MASTER) //change UID of the card that you want to give access // master
		{
			lcd.clear();
			//=====================cout welcome===============
			lcd.setCursor(3, 0);
			lcd.println(" Welcome       ");
			lcd.setCursor(0, 1);
			lcd.println("Mr.Khoa Handsome ");
			delay(2000);
			//====================cout have a nice day=======
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.println(" Have A NICE DAY            ");
			delay(1500);
			access_allow = true;
			//===================cout temp and humid=========
		}
		else {
			lcd.clear();
			Serial.println(" Access Denied ");
			lcd.println(" Access Denied    ");
			delay(2000);
		}
}


 //***********************Thuc Quyen******************************//
int check_card(String check, String* users)
{
  if (check == MASTER)
    return 1;
  for (int i = 0; i < NUM_OF_USERS; i++)
    if (users[i] == check)
      return 2;
  return 0;       
}

void control_users(String card, int& task, int &menu, String* users)
{
    int check = check_card(card, users);
    int i;
    if (task == 0)
    {
      if (check == 2 || check == 1)
      {
        menu = 2;
      }
      else if (check == 0)
      {
        for (i = 0; i < NUM_OF_USERS; i++)
        {
           if (users[i] == " ")
           {
             users[i] = card;
             lcd.clear();
             lcd.println(users[i]);
             delay(1000);
             break;
           }
        }
        if (i == 3)
        {
           lcd.clear();
           lcd.print("FULL USERS");
        }
        else
          menu = 4;   
      }
    }
    else if (task == 1)
    { 
      if (check == 1)     
      {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("MASTER!!!");
        lcd.setCursor(0 ,1);
        lcd.print(" CAN NOT REMOVE ");
        delay(800);
      }
      else if (check == 0)
      {
        menu = 3;
      }
      else if (check == 2)
      {
        for (i = 0; i < NUM_OF_USERS; i++)
        {
           if (users[i] == card)
           {
              lcd.clear();
              lcd.println(users[i]);
              delay(1000);
              users[i] = " ";
              break;
           }
        }
        menu = 4;
      }     
    }  
}

=======
#include <SPI.h>
#include <MFRC522.h>
#include <Display_RFID.h>
MFRC522 mfrc522(2, HIGH);   // Create MFRC522 instance.
#define MASTER "30 D7 52 73"
#define NUM_OF_USERS 6

String RFID_read()
{
	//===================Show UID on serial monitor=====================

	Serial.println();
	Serial.print(" UID tag :");
	String content = "";
	byte letter;
	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
		content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(mfrc522.uid.uidByte[i], HEX));
	}
	content.toUpperCase();
	return content.substring(1);
}

void checkAccess(bool &access_allow)
{
	String uid = "";
	uid = RFID_read();
	int access = false;
		//====================check id=======================================
	for (int i = 0; i < NUM_OF_USERS - 1; i++) if (uid == MASTER || uid == users[i]) access = true;
		if (access) //change UID of the card that you want to give access // master
		{
			lcd.clear();
			//=====================cout welcome===============
			lcd.setCursor(3, 0);
			lcd.println(" Welcome       ");
			lcd.setCursor(0, 1);
			if(uid==MASTER) lcd.println("Mr.Khoa Handsome ");
			else
			{
				lcd.setCursor(2,1);
				lcd.print(uid);
			}
			delay(2000);
			//====================cout have a nice day=======
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.println(" Have A NICE DAY            ");
			delay(1500);
			access_allow = true;
			//===================cout temp and humid=========
		}
		else {
			lcd.clear();
			Serial.println(" Access Denied ");
			lcd.println(" Access Denied    ");
			delay(2000);
		}
}
>>>>>>> 9cec58ecfe8608e0648100174ef025a210729733
