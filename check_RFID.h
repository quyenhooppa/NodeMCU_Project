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
    int num = 0;
    if (uid == MASTER)
        access = true;
    else
        for (int i = 0; i < NUM_OF_USERS - 1; i++) if (uid == users[i])
        {
            access = true;
            num = i + 1;
        }
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
            lcd.print("  USER ");
            lcd.print(num);
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
    //Write_EEPPROM();
}
