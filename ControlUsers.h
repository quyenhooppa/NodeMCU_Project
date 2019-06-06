#pragma once
#define MASTER "30 D7 52 73"
#define NUM_OF_USERS 5
#include <stdlib.h>
#include <string.h>
#include <ctime>

void change_Char(char* number, int a);
void check_exsit_user(bool &checkUser,int menu_screen)
{
	if (checkUser && mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
	{
		String uid = "";
		uid = RFID_read();
		for (int i = 0; i < NUM_OF_USERS; i++)
			if (uid == users[i])
			{
				lcd.clear();
				lcd.print("  EXIST USERS");
				lcd.setCursor(0, 1);
				char arr[16] = "     IN U   ";
					arr[9] = (i+1) % 10 + 48;
				lcd.print(arr);
				checkUser = false;
				delay(2000);
				Display(true, 0, menu_screen, false, WF_status, MQTT_status);  //pointer=3
				return;
			}
		lcd.clear();
		lcd.print("THERE IS NO USER");
		lcd.setCursor(0, 1);
		lcd.print("  ALLOW TO ADD  ");
		checkUser = false;
		delay(2000);
		Display(true, 0, menu_screen, false, WF_status, MQTT_status);  //pointer=3
		return;
	}
}
void checkMaster(bool &Users_trig, int &menu_screen, int &pointer)
{
    
    if (Users_trig && mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() )
    {
        String uid = "";
        uid = RFID_read();
        if (uid == MASTER)
        {
            Users_trig = false;
            lcd.clear();
            lcd.print(" WELCOME MASTER");
            menu_screen = 2;
            pointer = 0;
            delay(2000);
            Display(true, pointer, menu_screen, false, WF_status, MQTT_status);  //pointer=3
        }
        else if (uid != MASTER && uid != "")
        {
            Users_trig = false;
            lcd.clear();
            lcd.print("  Access Denied");
            delay(2000);
            Display(true, 4, false, false, WF_status, MQTT_status);  //pointer=3
        }
    }
}

void change_Char(char* number, int a)
{
    for (int i = 0; i < 3; i++)
    {
        char c = a % 10 + 48;
        a = a / 10;
        number[2 - i] = c;
    }
    return;
}

void controlUser(bool &allow_add_del, int pointer)
{
    srand(time(0));
    if (allow_add_del&& mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        String uid = "";
        uid = RFID_read();
        if (uid == MASTER && users[pointer - 1] == "")
        {
            lcd.clear();
            lcd.print("  MASTER CARD ");
            lcd.setCursor(0,1);
            lcd.print("CANT ADD OR DEL");
            allow_add_del = false;
            delay(1500);
            Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer - 1=3
        }
		else if (users[pointer - 1] == uid || uid == MASTER)
		{
			users[pointer - 1] = "";
			strcpy(ID_num[pointer - 1], "EMPTY");
			lcd.clear();
			lcd.print("DEL SUCCESSFULLY");
			allow_add_del = false;
			delay(1500);
			Write_EEPROM(true, 2, users);
			Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer - 1=3
		}
		else
		{
			for (int i = 0; i < NUM_OF_USERS; i++)
				if (users[pointer - 1] != "" && i != pointer - 1)
				{
					lcd.clear();
					lcd.print("  NOT THE SAME  ");
					lcd.setCursor(0, 1);
					lcd.print("   CARD USER");
					allow_add_del = false;
					delay(1500);
					Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer - 1=3
					break;
				}
				else if (users[i] == uid && users[pointer - 1] == "")
				{
					lcd.clear();
					lcd.print("EXISTED CARD MEM");
					lcd.setCursor(0, 1);
					lcd.print("  CAN NOT ADD ");
					allow_add_del = false;
					delay(1500);
					Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer - 1=3
					break;
				}
		}
        if (allow_add_del && uid != MASTER)
        {
            users[pointer - 1] = uid;
            
            //creat ID number for each user
            strcpy(ID_num[pointer - 1],"");
            char temp[3] = " ";
            int random = rand() % 26 + 65;
            temp[0] = random;
            random = rand() % 26 + 65;
            temp[1] = random;
            strcat(ID_num[pointer - 1], temp);
            strcat(ID_num[pointer - 1], "2019");
            int temp_i = rand() % 1000;
            char temp_c[3] = " ";
            change_Char(temp_c, temp_i);
            strcat(ID_num[pointer - 1], temp_c);
            char temp1[2] = " ";
            random = rand() % 26 + 65;
            temp1[0] = random;
            strcat(ID_num[pointer - 1], temp1);
            
            lcd.clear();
            lcd.print("ADD SUCCESSFULLY");
            allow_add_del = false;
            delay(1500);
            Write_EEPROM(true, 2, users);
            Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
        }
    }
}
