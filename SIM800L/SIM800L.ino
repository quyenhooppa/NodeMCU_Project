#include <SoftwareSerial.h>
 long int Time = 0;
//SIM800 TX is connected to ESP D3 = 0, SD2 = 9, D1 = 5
#define SIM800_TX_PIN D1
 
//SIM800 RX is connected to ESP D8 = 15
#define SIM800_RX_PIN 15
 
//Create software serial object to communicate with SIM800
SoftwareSerial mySerial(SIM800_TX_PIN,SIM800_RX_PIN);

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication witj Arduino and SIM800
  mySerial.begin(9600);
  delay(1000);
  Serial.println("\n\nHello!");
      //count = count + 1; 
      mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
      delay(500);
      updateSerial();
      //mySerial.println("AT+CSCS=\"GSM\"\r");
      //delay(1000);
      mySerial.println("AT+CMGS=\"+84923528618\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();
      mySerial.print("Ho Le Thuc Quyen!!!"); //text content
      updateSerial();
      mySerial.write((char)26);
      delay(2000);
      Time = millis(); 
  Serial.println("Setup Complete!");
}

int count = 0;

void loop() {
  //Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  /*if(mySerial.available()){
    Serial.write(mySerial.read());
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){    
    mySerial.write(Serial.read());
  }*/
  if (millis() - Time > 2000 && count < 5)
  {
      Serial.println("\n\nHello!");
      count = count + 1; 
      mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
      delay(500);
      updateSerial();
      //mySerial.println("AT+CSCS=\"GSM\"\r");
      //delay(1000);
      mySerial.println("AT+CMGS=\"+84923528618\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();
      mySerial.print("Ho Le Thuc Quyen!!!"); //text content
      updateSerial();
      mySerial.write((char)26);
      delay(2000);
      Time = millis();
  }
    
  updateSerial();
}
