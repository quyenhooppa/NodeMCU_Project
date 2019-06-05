#pragma once
#define MASTER "30 D7 52 73"
#define NUM_OF_USERS 6
#include <stdlib.h>
#include <string.h>
#include <ctime>

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
        if (uid == MASTER)
        {
            lcd.clear();
            lcd.print("  MASTER CARD ");
            lcd.setCursor(0,1);
            lcd.print("CANT ADD OR DEL");
            allow_add_del = false;
            delay(1500);
            Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
        }
        else
        {
            for (int i = 0; i < NUM_OF_USERS - 1; i++)
                /*if (ID_num[i] != "EMPTY" && i != pointer)
                {
                    lcd.clear();
                    lcd.print("  NOT THE SAME  ");
                    lcd.setCursor(0,1);
                    lcd.print("   CARD USER");
                    allow_add_del = false;
                    delay(1500);
                    Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
                    break;
                }*/
                if (users[i] == uid && i != pointer)
                {
                    lcd.clear();
                    lcd.print("  EXISTED USER  ");
                    allow_add_del = false;
                    delay(1500);
                    Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
                    break;
                }
                else if (users[i] == uid && i == pointer)
                {
                    users[i] = "";
                    strcpy(ID_num[i], "EMPTY");
                    lcd.clear();
                    lcd.print("DEL SUCCESSFULLY");
                    allow_add_del = false;
                    delay(1500);
                    Write_EEPROM(true, 2, users);
                    Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
                }
        }
        if (allow_add_del && uid != MASTER)
        {
            users[pointer] = uid;
            
            //creat ID number for each user
            strcpy(ID_num[pointer],"");
            char temp[3] = " ";
            int random = rand() % 26 + 65;
            temp[0] = random;
            random = rand() % 26 + 65;
            temp[1] = random;
            strcat(ID_num[pointer], temp);
            strcat(ID_num[pointer], "2019");
            int temp_i = rand() % 1000;
            char temp_c[3] = " ";
            change_Char(temp_c, temp_i);
            strcat(ID_num[pointer], temp_c);
            char temp1[2] = " ";
            random = rand() % 26 + 65;
            temp1[0] = random;
            strcat(ID_num[pointer], temp1);
            
            lcd.clear();
            lcd.print("ADD SUCCESSFULLY");
            allow_add_del = false;
            delay(1500);
            Write_EEPROM(true, 2, users);
            Display(true, pointer, 2, false, WF_status, MQTT_status);  //pointer=3
        }
    }
}
