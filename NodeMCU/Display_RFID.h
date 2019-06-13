#pragma once

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <WiFiUdp.h>
#include <NTPClient.h>

const long UTC = 25200;
WiFiUDP udp;
NTPClient Time(udp, "pool.ntp.org", UTC);

int hr;
int mins;
int second;
int day;
unsigned long int time1 = 0;

String users[5] = {"", "", "", "", "" };
char ID_num[5][11] = {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"};
char Days[7][12] = {"SUN", "MON", "TUES", "WED", "THURS", "FRI", "SAT"};

void read_time(bool _WF_status)
{
    if  (_WF_status)
    {
        Time.update();
	    day = Time.getDay();
  	    hr = Time.getHours();
        mins = Time.getMinutes();
        second = Time.getSeconds();
        lcd.clear();
        lcd.print("Current Day-Time");
        lcd.setCursor(1,1);
        lcd.print(Days[day]);
        if (hr < 10)
                {
                    lcd.setCursor(7,1);
                    lcd.print("0");
                    lcd.print(hr);
                }
                else
                {
                    lcd.setCursor(7,1);
                    lcd.print(hr);
                }
                lcd.print(":");
                if (mins < 10)
                {
                    lcd.setCursor(10,1);
                    lcd.print("0");
                    lcd.print(mins);
                }
                else
                {
                    lcd.setCursor(10,1);
                    lcd.print(mins);
                }
                lcd.print(":");
                if (second < 10)
                {
                    lcd.setCursor(13,1);
                    lcd.print("0");
                    lcd.print(second);
                }
                else
                {
                    lcd.setCursor(13,1);
                    lcd.print(second);
                }
                delay(1000);
            }	
    else 
    {
        lcd.clear();
        lcd.print("NOT CONNECT WIFI");
        lcd.setCursor(0,1);
        lcd.print("CAN NOT GET TIME");
        delay(1000);
    }       
}

void Display(bool access_allow, int pointer, int menu_screen, bool access_DHT, 
            bool _WF_status, bool _MQTT_status, float humid = 0, float temp = 0) 
{
    if (access_allow == false && access_DHT == false)
    {
        if (pointer == 0)
        {
            lcd.clear();
            lcd.print(">:ACCESS BY CARD");
            lcd.setCursor(0, 1);
            lcd.print(" :ACCESS BY PIN");
        }
        else 
        {
            lcd.clear();
            lcd.print(" :ACCESS BY CARD");
            lcd.setCursor(0, 1);
            lcd.print(">:ACCESS BY PIN");
        }
    }
    else if (access_allow == true && access_DHT == false)
    {
        if (menu_screen == 0 ) //main menu
        {
            switch (pointer)
            {
                case 0:
                    lcd.clear();
                    lcd.print(">:SEND MESSAGE         ");
                    lcd.setCursor(0, 1);
                    lcd.print(" :TEMP AND HUMID     ");
                    break;
                case 1:
                    lcd.clear();
                    lcd.print(" :SEND MESSAGE      ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:TEMP AND HUMID     ");
                    break;
                case 2:
                    lcd.clear();
                    lcd.print(" :TEMP AND HUMID     ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:CONNECT SV:           ");
                    lcd.setCursor(13,1);
                    (_MQTT_status == true) ? lcd.print("ON") : lcd.print("OFF");
                    break;
                case 3:
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print(" :CONNECT SV:            ");
                    lcd.setCursor(13, 0);
                    (_MQTT_status == true) ? lcd.print("ON") : lcd.print("OFF");
                    lcd.setCursor(0, 1);
                    lcd.print(">:CONNECT WF:          ");
                    lcd.setCursor(13, 1);
                    (_WF_status == true)?  lcd.print("ON") :lcd.print("OFF");
                    break;
                case 4:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :CONNECT WF:          ");
                    lcd.setCursor(0,1);
                    lcd.print(">:CONTROL USERS         ");
                    lcd.setCursor(13, 0);
                    (_WF_status == true) ? lcd.print("ON") : lcd.print("OFF");
                    break;
                case 5:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :CONTROL USERS          ");
                    lcd.setCursor(0,1);
                    lcd.print(">:DAY & TIME: ");
                    lcd.setCursor(13, 1);
                    (_WF_status == true) ? lcd.print("ON") : lcd.print("OFF");
                    break;
                default:
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print(" :DAY & TIME: ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:QUIT     ");
                    lcd.setCursor(13, 0);
                    (_WF_status == true) ? lcd.print("ON") : lcd.print("OFF");                    
                    break;
            }
        }
        else if (menu_screen == 1) //message mode
        {
            switch (pointer)
            {
                case 0:
                    lcd.clear();
                    lcd.print(">:HELLO         ");
                    lcd.setCursor(0, 1);
                    lcd.print(" :WHAT R U DOING?     ");
                    break;
                case 1:
                    lcd.clear();
                    lcd.print(" :HELLO      ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:WHAT R U DOING?      ");
                    break;
                case 2:
                    lcd.clear();
                    lcd.print(" :WHAT R U DOING        ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:EATEN RICE YET     ");
                    break;
                case 3:
                    lcd.clear();
                    lcd.print(" :EATEN RICE YET        ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:TEMPERATURE     ");
                    break;
                case 4:
                    lcd.clear();
                    lcd.print(" :TEMPERATURE        ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:HUMID     ");
                    break;
                default:
                    lcd.clear();
                    lcd.print(" :HUMID       ");
                    lcd.setCursor(0, 1);
                    lcd.print(">:QUIT MESSAGE    ");
                    break;
            }
        }
        else if (menu_screen == 2) //control users
        {
            switch (pointer)
            {
				case 0:
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print(">:CHECK USERS ");
					lcd.setCursor(0, 1);
					lcd.print(" :U1 ");
					lcd.print(ID_num[0]);
					break;
				case 1:
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print(" :CHECK USERS ");
					lcd.setCursor(0, 1);
					lcd.print(">:U1 ");
					lcd.print(ID_num[0]);
					break;
                case 2:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :U1 ");
                    lcd.print(ID_num[0]);
                    lcd.setCursor(0,1);
                    lcd.print(">:U2 ");
                    lcd.print(ID_num[1]);
                    break;
                case 3:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :U2 ");
                    lcd.print(ID_num[1]);
                    lcd.setCursor(0,1);
                    lcd.print(">:U3 ");
                    lcd.print(ID_num[2]);
                    break;
                case 4:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :U3 ");
                    lcd.print(ID_num[2]);
                    lcd.setCursor(0,1);
                    lcd.print(">:U4 ");
                    lcd.print(ID_num[3]);
                    break;
                case 5:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :U4 ");
                    lcd.print(ID_num[3]);
                    lcd.setCursor(0,1);
                    lcd.print(">:U5 ");
                    lcd.print(ID_num[4]);
                    break;
                case 6:
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(" :U5 ");
                    lcd.print(ID_num[4]);
                    lcd.setCursor(0,1);
                    lcd.print(">:Quit CU        ");
                    break;
            }
        }
        else if (menu_screen == 3)
        {
            unsigned long int time_millis = millis();
            if (time_millis - time1 > 1000)
            {
                read_time(_WF_status);
                delay(1000);
                time1 = millis();
            }
        }
    }
    else //if (access_allow == true && access_DHT == true)//show temp and humid
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("TEMPERATURE: ");
        lcd.setCursor(12, 0);
        lcd.println(temp, 1);
        lcd.setCursor(0, 1);
		lcd.println("HUMID      :        ");
        lcd.setCursor(12, 1);
        lcd.println(humid, 1);
    }
}
