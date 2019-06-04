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

int address_begin = 0x0F + 10;

int users_save[5] = {5, 25, 45, 65, 85};
//bool allow_save = allow_access;


void Write_EEPROM(bool _allow_access, int _menu_screen)
{
    for (int i = 0; i < NUM_OF_USERS - 1; i++)
    {
        int j = 0;
        for (j = 0; j < users[i].length(); j++)
            EEPROM.put(address_begin + users_save[i] + j, users[i][j]);
        EEPROM.put(address_begin + users_save[i] + j, '\0');
        for (j = 0; j < 11; j++)
            EEPROM.put(address_begin + 2 * users_save[i] + j, ID_num[i][j]);
        EEPROM.put(address_begin + 2 * users_save[i] + j, '\0');
    }
    EEPROM.put(address_begin, _allow_access);
    int menu = _menu_screen;
    menu = (menu == 2) ? menu : 0;
    EEPROM.put(address_begin + 1, menu);
    EEPROM.commit();
    return;
}

void Read_EEPROM(bool& _allow_access, int& _menu_screen)
{
    for (int i = 0; i < NUM_OF_USERS - 1; i++)
    {
        int j = 0;
        for (j = 0; j < users[i].length(); j++)
            users[i][j] = EEPROM.read(address_begin + users_save[i] + j);
        users[i][j] = '\0';
        for (j = 0; j < 11; j++)
            ID_num[i][j] = ID_num[i][j] + (char)EEPROM.read(address_begin + 2 * users_save[i]);
        ID_num[i][j] = '\0';
    }
    _allow_access = EEPROM.read(address_begin);
    _menu_screen = EEPROM.read(address_begin + 1);
    return;
}

//#endif
/* EPROM_h */
