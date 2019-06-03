//
//  EPROM.h
//  
//
//  Created by Quyen Ho on 6/3/19.
//

//#ifndef EPROM_h
//#define EPROM_h

#include <ESP_EEPROM.h>
#include <Display_RFID.h>
//#include <check_RFID.h>
#define NUM_OF_USERS 6

int address_begin = 0x0F;

int users_save[5] = {5, 6, 7, 8, 9};
//bool allow_save = allow_access;


void Write_EEPROM(bool allow_access, int menu_screen)
{
    for (int i = 0; i < NUM_OF_USERS - 1; i++)
    {
        for (int j = 0; j < users[i].length(); j++)
            EEPROM.write(address_begin + users_save[i], users[i][j]);
        for (int j = 0; j < 11; j++)
            EEPROM.write(address_begin + 2 * users_save[i], ID_num[i][j]);
    }
    EEPROM.write(address_begin, allow_access);
    EEPROM.write(address_begin + 1, menu_screen);
    EEPROM.commit();
    return;
}

void Read_EEPROM(bool& allow_access, int& menu_screen)
{
    for (int i = 0; i < NUM_OF_USERS - 1; i++)
    {
        for (int j = 0; j < users[i].length(); j++)
            users[i][j] = users[i][j] + (char)EEPROM.read(address_begin + users_save[i]);
        for (int j = 0; j < 11; j++)
            ID_num[i][j] = ID_num[i][j] + (char)EEPROM.read(address_begin + 2 * users_save[i]);
    }
    allow_access = EEPROM.read(address_begin);
    menu_screen = EEPROM.read(address_begin + 1);
    return;
}

//#endif
/* EPROM_h */
