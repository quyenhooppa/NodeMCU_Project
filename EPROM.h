//
//  EPROM.h
//  
//
//  Created by Quyen Ho on 6/3/19.
//

#include <ESP_EEPROM.h>
#include <Display_RFID.h>
#include <string.h>
#define NUM_OF_USERS 6

int address_begin = 0x0F + 10;

int users_save[5] = {5, 25, 45, 65, 85};
int ID_save[5] = {110, 130, 150, 170, 190};

void Write_EEPROM(bool _allow_access, int _menu_screen, int _count, String* _users)
{
    if (_allow_access == true)
    {
    for (int i = 0; i < NUM_OF_USERS - 1; i++)
    {
        int j = 0;
        char c = _users[i].charAt(j);
        while (c != '\0')
        {
            c = _users[i].charAt(j);
            EEPROM.put(address_begin + users_save[i] + j, c);
            j++;
            c = _users[i].charAt(j);
        }
        EEPROM.put(address_begin + users_save[i] + j, '\0');
        for (j = 0; j < 11; j++)
            EEPROM.put(address_begin + ID_save[i] + j, ID_num[i][j]);
        EEPROM.put(address_begin + ID_save[i] + j, '\0');
    }
    }
    EEPROM.put(address_begin, _allow_access);
    int menu = _menu_screen;
    menu = (menu == 2) ? 2 : 0;
    EEPROM.put(address_begin + 1, menu);
    EEPROM.put(address_begin + 2, _count);
    EEPROM.commit();
    return; 
}

void Read_EEPROM(bool& _allow_access, int& _menu_screen, int& _count, String* _users)
{
    for (int i = 0; i < NUM_OF_USERS - 1; i++)
    {
        _users[i] = "";
        int j = 0;
        char c = EEPROM.read(address_begin + users_save[i] + j);
        while (c != '\0')
        {
            _users[i].concat(c);
            j++;
            c = EEPROM.read(address_begin + users_save[i] + j);
        }
        for (j = 0; j < 11; j++)
            ID_num[i][j] = EEPROM.read(address_begin + ID_save[i] + j);
        ID_num[i][j] = '\0';
    }
    _allow_access = EEPROM.read(address_begin);
    _menu_screen = EEPROM.read(address_begin + 1);
    _count = EEPROM.read(address_begin + 2);
    return;
}
