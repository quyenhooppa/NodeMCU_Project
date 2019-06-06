#pragma once

#include <Message.h>
#define NUM_OF_FEATURES 6
#define NUM_OF_MESS 6
#define NUM_OF_USERS 7
class button
{
private:
	bool state_pointer = false;
	bool state_press = false;
	int button_pointer = LOW;
	int button_press = LOW;
	int pointer=0;
	int A0;
	bool access_DHT=false;
	float humid;
	float temp;
	float MQTT_status;
	float MQTT_trig;
	float WF_status;
	float WF_trig;
	bool MS_trig;
	bool Users_trig;
	bool allow_add_del;
	bool checkUsers;

void button_choose()
{
	int button_A0 = analogRead(A0);
	if (button_A0 > 500 && button_A0 < 600 && state_pointer == false)
	{
		button_pointer = HIGH;
		state_pointer = true;
	}
	else if (button_A0 > 700 && button_A0 < 800 && state_press == false)
	{
		button_press = HIGH;
		state_press = true;
	}
	else if ((button_A0 <= 520 || button_A0 >= 560) && state_pointer == true)
		state_pointer = false;
	else if ((button_A0 <= 740 || button_A0 >= 780) && state_press == true)
		state_press = false;
}

void change_state(bool &_access_allow,	int &_menu_screen)
{
	if (_access_allow==true)
	{
		if (button_pointer == HIGH && access_DHT == false && Users_trig==false && allow_add_del==false && WF_trig == false && MQTT_trig == false 
			&& Users_trig==false && allow_add_del==false && checkUser==false)
		{
			button_pointer = LOW;
			button_press = LOW;
			switch (_menu_screen)
			{
			case 0: pointer = (pointer + 1) % NUM_OF_FEATURES; break;
			case 1: pointer = (pointer + 1) % NUM_OF_MESS; break;
			case 2: pointer = (pointer + 1) % NUM_OF_USERS; break;
			}
			Display(_access_allow, pointer, _menu_screen,access_DHT,WF_status,MQTT_status, humid, temp);
		}
		if (button_press == HIGH)
		{
			switch (_menu_screen)
			{
			case 0: control_menu_screen(_access_allow, _menu_screen); break;
			case 1: control_message(_menu_screen);  break;
			case 2: control_users(_menu_screen); break;
			}
		}
	}
}

void control_users(int &_menu_screen)
{
	if (pointer == 6)
	{
		_menu_screen = 0;
		pointer = 4;
		button_press = LOW;
		button_pointer = LOW;
		Display(1, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp);
	}
	else
	{
		if (pointer == 0)
		{
			if (checkUsers == false)
			{
				checkUsers = true;
				lcd.clear();
				lcd.print("  INSERT YOUR");
				lcd.setCursor(0, 1);
				lcd.print("      CARD");
			}
			else if (checkUsers == true)
			{
				checkUsers = false;
				Display(1, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp);
			}
		}
		else
		{
			if (allow_add_del == false)
			{
				allow_add_del = true;
				lcd.clear();
				lcd.print("  INSERT YOUR");
				lcd.setCursor(0, 1);
				lcd.print("      CARD");
			}
			else if (allow_add_del)
			{
				allow_add_del = false;
				Display(1, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp);
			}
		}
		button_press = LOW;
		button_pointer = LOW;
	}
}
void control_message(int &_menu_screen)
{
	if (pointer==5) 
	{
		_menu_screen = 0;
		pointer = 0;
		button_press = LOW;
		button_pointer = LOW;
		Display(1, pointer, _menu_screen,access_DHT,WF_status,MQTT_status, humid, temp);
	}
	else
	{
		MS_trig = true;
		button_press = LOW;
		button_pointer = LOW;
	}
}

void control_menu_screen(bool &_access_allow, int &_menu_screen)
{
	switch (pointer)
	{
	case 0:
		_menu_screen = 1; pointer = 0;  button_press = LOW; button_pointer = LOW; 
		Display(_access_allow, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp); 
		break;
	case 1:
		access_DHT = !access_DHT; button_press = LOW; button_pointer = LOW; 
		Display(_access_allow, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp); 
		break;
	case 2:
		if (WF_status) MQTT_trig = true; button_press = LOW; button_pointer = LOW; break;
	case 3:
		WF_trig = true;  button_press = LOW; button_press = LOW; button_pointer = LOW; break;
	case 4:
		if (Users_trig == false)
		{
			Users_trig = true;
			lcd.clear();
			lcd.print("  INSERT YOUR");
			lcd.setCursor(0, 1);
			lcd.print("  MASTER CARD");
		}
		else if (Users_trig == true)
		{
			Users_trig = false;
			Display(_access_allow, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp);
		}
		button_press = LOW; button_press = LOW;
		break;
	case 5:
		_access_allow = false; pointer = 0; button_press = LOW; button_pointer = LOW; 
		Display(_access_allow, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp); 
		break;
	}
}
public:
	button(int _A0)
	{
			A0 = _A0;
	}
	void update_button(int &_pointer, bool &_access_allow, int &_menu_screen, float _temp, float _humid, 
		bool &_WF_trig, bool &_WF_status, bool &_MQTT_status, bool &_MQTT_trig, bool &_MS_trig, bool& _Users_trig, bool &_allow_add_del, bool &_checkUser)
	{
		button_choose();
		allow_add_del = _allow_add_del;
		pointer = _pointer;
		MS_trig = _MS_trig;
		MQTT_status = _MQTT_status;
		MQTT_trig = _MQTT_trig;
		WF_status = _WF_status;
		WF_trig = _WF_trig;
		Users_trig = _Users_trig;
		checkUsers = _checkUser;
		temp = _temp;
		humid = _humid;
		change_state(_access_allow,  _menu_screen);
		_MQTT_status= MQTT_status ;
		_MQTT_trig = MQTT_trig;
		_WF_status = WF_status;
		_WF_trig = WF_trig;
		_MS_trig = MS_trig;
		_pointer = pointer;
		_Users_trig = Users_trig;
		_allow_add_del = allow_add_del;
		_checkUser = checkUsers;
	}
};
