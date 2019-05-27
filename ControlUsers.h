#pragma once
/*#include <message.h>
//#include <RFID.h>
#define MASTER "30 D7 52 73"

int add_del = 0;
int menu = 0;

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
			lcd.setCursor(0, 1);
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

void check_MASTER(bool& Users_trig, int& _menu_screen)
{
	if (Users_trig == true)
	{
		lcd.clear();
		lcd.print(" PLEASE INSERT ");
		lcd.setCursor(0, 1);
		lcd.print("  MASTER CARD  ");
		// Look for new cards
		if (!mfrc522.PICC_IsNewCardPresent()) return;
		// Select one of the cards
		if (!mfrc522.PICC_ReadCardSerial()) return;
		String card = RFID_read();
		if (card == MASTER)
		{
			add_del = 0;
			_menu_screen = 2;
			int Task = 0;
			control_users(card, Task, add_del, Users);
			Display(1, 0, _menu_screen, false, false, false, 0, 0, add_del, Task);
		}
	}
}*/
#define MASTER "30 D7 52 73"
#define NUM_OF_USERS 6
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
			lcd.print("  Access Deied");
			delay(2000);
			Display(true, 4, false, false, WF_status, MQTT_status);  //pointer=3
		}
	}
}

void controlUser(bool &allow_add_del, int pointer)
{
	if (allow_add_del&& mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
	{
		String uid = "";
		uid = RFID_read();
		for (int i=0; i<NUM_OF_USERS-1; i++)
			if (users[i] == uid && i!= pointer)
			{
				lcd.clear();
				lcd.print("   EXIST CARD");
				allow_add_del = false;
				delay(1500);
				Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
			}
			else if (users[i] == uid && i == pointer)
			{
				users[i] = "NULL";
				lcd.clear();
				lcd.print("DEL SUCCESSFULLY");
				allow_add_del = false;
				delay(1500);
				Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
			}
		if (allow_add_del && uid !=MASTER)
		{
			users[pointer] = uid;
			lcd.clear();
			lcd.print("ADD SUCCESSFULLY");
			allow_add_del = false;
			delay(1500);
			Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
		}
	}
}