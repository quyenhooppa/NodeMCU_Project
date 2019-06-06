#pragma once

#include <access_MQTT.h>
#include <DHTesp.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>
#include <Display_RFID.h>

#define SIM800_TX_PIN D0
#define SIM800_RX_PIN D1

SoftwareSerial mySerial(SIM800_TX_PIN, SIM800_RX_PIN);
DHTesp dht;

float temp;
float humid;
char message[6][16] = { "HELLO","WHAT R U DOING","EATEN RICE YET?","TEMPER:    ","HUMID:     " ,"QUIT MESSAGE" };

void change_to_char(char *temp_c, int pointer) //convert temp and humid value to char
{
	int TEMP;
	(pointer == 3) ? TEMP = temp * 100 : TEMP = humid * 100;
	for (int i = 0; i < 2; i++)
	{
		char c = TEMP % 10 + 48;
		TEMP = TEMP / 10;
		temp_c[4 - i] = c;
	}
	temp_c[2] = '.';
	for (int i = 0; i < 2; i++)
	{
		char c = TEMP % 10 + 48;
		TEMP = TEMP / 10;
		temp_c[1 - i] = c;
	}
}

void check_DHT()
{
	humid = dht.getHumidity();
	temp = dht.getTemperature();
}

void updateSerial()
{
	delay(500);
	while (Serial.available())
		mySerial.write(Serial.read());
	while (mySerial.available())
		Serial.write(mySerial.read());
}

void send_mess(int pointer, bool MQTT_status, bool & MS_trig)
{
	if (MQTT_status == false && MS_trig) //send to SMS when no wifi
	{
		MS_trig = false;
		mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
		updateSerial();
		mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
		delay(500);
		updateSerial();
		//mySerial.println("AT+CSCS=\"GSM\"\r");
		//delay(1000);
		mySerial.println("AT+CMGS=\"+84923528618\"");
		updateSerial();
		if (pointer == 4 || pointer == 3)
		{
			char temp_c[5] = " ";
			char Temp[16] = " ";
			strcpy(Temp, message[pointer]);
			change_to_char(temp_c, pointer);
			strcat(Temp, temp_c);
			char a[1] = "";
			strcat(Temp, a);
			mySerial.print(Temp); //text content
			updateSerial();
			mySerial.write((char)26);
			delay(2000);
			lcd.clear();
			lcd.print("Sent SMS!!!");
			lcd.setCursor(0, 1);
			lcd.print(Temp);
			delay(2000);
		}
		else {
			mySerial.print(message[pointer]);
			updateSerial();
			mySerial.write((char)26);
			delay(2000);
			lcd.clear();
			lcd.print("Sent SMS!!!");
			lcd.setCursor(0, 1);
			lcd.print(message[pointer]);
			delay(2000);
		}
		Display(true, pointer, true, false, false, false);
	}
	else if (MS_trig && MQTT_status) //send to server when wifi is connected
	{
		MS_trig = false;
		lcd.clear();
		if (pointer == 4 || pointer == 3)
		{
			char temp_c[5] = "";
			char Temp[16] = "";
			strcpy(Temp, message[pointer]);
			change_to_char(temp_c, pointer);
			strcat(Temp, temp_c);
			MQTT.publish("From RFID", Temp);
			lcd.print("Sent to server");
			lcd.setCursor(0, 1);
			lcd.print(Temp);
			delay(2000);
		}
		else {
			MQTT.publish("From RFID", message[pointer]);
			lcd.print("Sent to server");
			lcd.setCursor(0, 1);
			lcd.print(message[pointer]);
			delay(2000);
		}
		Display(true, pointer, true, false, false, false);
	}
}
