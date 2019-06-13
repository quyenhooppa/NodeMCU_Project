#pragma once

#include <Message.h>
#define NUM_OF_FEATURES 7
#define NUM_OF_MESS 6
#define NUM_OF_USERS 7

String Pin = "0123456789 X O";
String PassCode = "01234";
char checkCode[100] = "";

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
	//int day, hr, mins, second;
	float MQTT_status;
	float MQTT_trig;
	float WF_status;
	float WF_trig;
	bool MS_trig;
	bool Users_trig;
	bool Time_trig;
	bool allow_add_del;
	bool checkUsers;
	bool Access_trig;
	bool Pin_trig;
	int countPass = 0;
	int allow_access_pin;

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
	if (_access_allow == true)
	{
		if (button_pointer == HIGH && access_DHT == false && Users_trig==false && allow_add_del==false 
			&& WF_trig == false && MQTT_trig == false 
			&& Users_trig==false && allow_add_del==false && checkUser==false && Time_trig == false)
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
	else  
	{
		if (button_pointer == HIGH && access_DHT == false && Users_trig==false && allow_add_del==false && WF_trig == false && MQTT_trig == false 
			&& Users_trig==false && allow_add_del==false && checkUser==false && Pin_trig == false)
		{
			button_pointer = LOW;
			button_press = LOW;
			pointer = (pointer + 1) % 2;
			Display(_access_allow, pointer, _menu_screen,access_DHT,WF_status,MQTT_status, humid, temp);
		}
		if (button_press == HIGH || (Pin_trig == true))
		{
			if (pointer == 0 && Pin_trig == false)
			{			
				if (Access_trig == false)
				{
					lcd.clear();
					lcd.print("  PLEASE INSERT");
					lcd.setCursor(0,1);
					lcd.print("   YOUR CARD");
					Access_trig = true;
					button_press = LOW;	
					button_pointer = LOW;
				}
				else
				{
					Display(_access_allow, pointer, _menu_screen,access_DHT,WF_status,MQTT_status, humid, temp);
					Access_trig=false;
				}	
			}
			else if(pointer==1 || Pin_trig==true)
			{
				if (Pin_trig == false)
				{
						lcd.clear();
						lcd.print("0123456789 X O ");
						lcd.setCursor(0,1);
						lcd.print("|");
						lcd.setCursor(14,1);
						lcd.print("0");
						Pin_trig = true;
						countPass = 0;
						pointer=0;
						button_press = LOW;
				}
				else 
				{
					if (button_pointer == HIGH) 
					{
						pointer = (pointer + 1) % 14;
						lcd.clear();
						lcd.print("0123456789 X O");
						lcd.setCursor(pointer, 1);
						lcd.print("|");
						lcd.setCursor(14,1);
						lcd.print(countPass);
						button_pointer = LOW;
					}
					if (button_press == HIGH)
					{
						if (pointer == 13)
						{
							checkCode[countPass] = '\0';
							bool check = true;
							int i = 0;
							char c = PassCode.charAt(i);
							while (c != '\0')
							{
								if (checkCode[i++] != c)
								{
									check = false;
									break;
								}
									c = PassCode.charAt(i);
								}
								if (check == true && checkCode[i] != '\0')
									check = false;
								if (check == false)
								{
									countPass = 0;
									pointer = 1;
									allow_access_pin = 0;
									button_pointer = LOW;
									button_pointer = LOW;
								}
								else 
								{
									countPass = 0;
									pointer = 0;
									//_access_allow = true;
									allow_access_pin = 1;
									button_pointer = LOW;
									button_pointer = LOW;
								}
						}
						else if (pointer == 11)
						{
							checkCode[--countPass] = '\0';
							if (countPass < 0) 
								countPass = 0;
							lcd.setCursor(14,1);
							lcd.print(countPass);
						}
						else if (pointer >= 0 && pointer <= 9)
						{
							if (countPass >= 0) 
							{	
								checkCode[countPass++] = Pin[pointer];
								lcd.setCursor(14,1);
								lcd.print(countPass);
							}
						}
						button_press = LOW;
					}
				}					
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
		if (_menu_screen == 0)
		{
			if (Time_trig == false)
            {
				Time_trig = true;
			} 
			else if (Time_trig == true)
			{
				Time_trig = false;
				Display(_access_allow, pointer, _menu_screen, access_DHT, WF_status, MQTT_status, humid, temp);
			}
			button_press = LOW; button_press = LOW;
		}
		break;
	case 6:
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
		bool &_WF_trig, bool &_WF_status, bool &_MQTT_status, bool &_MQTT_trig, bool &_MS_trig, bool& _Users_trig, 
		bool &_allow_add_del, bool &_checkUser, bool& _Access_trig, bool& _Pin_trig, int &_allow_access_pin,
		bool& _Time_trig)
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
		Time_trig = _Time_trig;
		checkUsers = _checkUser;
		Access_trig = _Access_trig;
		Pin_trig = _Pin_trig;
		allow_access_pin = _allow_access_pin;
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
		_Time_trig = Time_trig;
		_allow_add_del = allow_add_del;
		_checkUser = checkUsers;
		_Access_trig = Access_trig;
		_Pin_trig = Pin_trig;
		_allow_access_pin = allow_access_pin;
	}
};
