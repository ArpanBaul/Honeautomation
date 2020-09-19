#include<ESP8266WiFi.h>
#include<DNSServer.h>
#include<ESP8266WebServer.h>
#include<WiFiManager.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "OzWBzypec4fCVR9lEVaGKCo8J-bdBeOF";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "RealMe";
//char pass[] = "apu12345";
//char server[] = "xxx.xxx.xxx.xxx";  // IP for your Local Server
char server[] = "blynk-cloud.com";
int port = 8080;

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

int out1=16; //pin for relay 1 (D0 on nodemcu)
int state1; //state of relay 1
int out2=5;  //pin for relay 2 (D1 on nodemcu)
int state2; //state of relay 2
int out3=4;  //pin for relay 3 (D2 on nodemcu)
int state3; //state of relay 3
int out4=0;  //pin for relay 4 (D3 on nodemcu)
int state4; //state of relay 4
int vcc=12;
int gnd=14;
int stateall; // state of the master switch

const uint16_t RECV_PIN = 13; //Pin for the IR Receiver data pin (14=D5 for Node mcu)
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup(){
   Serial.begin(115200);
   
  irrecv.enableIRIn(); // start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(RECV_PIN);
  WiFiManager wifiManager;
  Serial.println("Connecting.....");
  wifiManager.autoConnect("apu","apu123");
  Serial.println("Connected");
  state1=1; //Relay1 is turned off
  pinMode(out1,OUTPUT);
  state2=1; //Relay2 is turned off
  pinMode(out2,OUTPUT);
  state3=1; //Relay3 is turned off
  pinMode(out3,OUTPUT);
  state4=1; //Relay4 is turned off
  pinMode(out4,OUTPUT);
  pinMode(vcc,OUTPUT);
  pinMode(gnd,OUTPUT);
  stateall=1; //Master Switch is off at startup

  digitalWrite(out1, HIGH); //Making the pins high to keep the relay board off (for active low relays)
  digitalWrite(out2, HIGH);// make it LOW for active high relay boards.
  digitalWrite(out3, HIGH);
  digitalWrite(out4, HIGH);
  digitalWrite(vcc,HIGH);
  digitalWrite(gnd,LOW);
 //Blynk.begin(auth, ssid, pass, server, port);    
 Blynk.config(auth,server,port);
}

BLYNK_WRITE(V10) //Button Widget is writing to pin V10 that is used for master button in app
{
  int pinData = param.asInt(); 
  if (pinData==0){    //Code to turn everything ON/OFF : MASTER SWITCH
      digitalWrite(out1,LOW); //turning on the relay1 with master button
      state1=0;
      Blynk.virtualWrite(V1, 0); //telling the app to display the relay1 button on
      
      digitalWrite(out2,LOW); //turning on the relay2 with master button
      state2=0;
      Blynk.virtualWrite(V2, 0); //telling the app to display the relay2 button on
      
      digitalWrite(out3,LOW); //turning on the relay3 with master button
      state3=0;
      Blynk.virtualWrite(V3, 0); //telling the app to display the relay3 button on
            
      digitalWrite(out4,LOW); //turning on the relay4 with master button
      state4=0;
      Blynk.virtualWrite(V4, 0); //telling the app to display the relay4 button on
      stateall=0; //switched the state of master button status to on
     }
    if (pinData==1){ 
      digitalWrite(out1,HIGH);    //this is the opposite of the process 
      state1=1;
      Blynk.virtualWrite(V1, 1); //to turn off the relays and the button states changed to off
      
      digitalWrite(out2,HIGH);
      state2=1;
      Blynk.virtualWrite(V2, 1);
      
      digitalWrite(out3,HIGH);
      state3=1;
      Blynk.virtualWrite(V3, 1);
      
      digitalWrite(out4,HIGH);
      state4=1;
      Blynk.virtualWrite(V4, 1);
      stateall=1; //master button status changed to off
     }}

BLYNK_WRITE(V1)    // Virtual pin 1 to contlor relay 1  
{
  int pinData = param.asInt(); 
  if (pinData==0){ 
      digitalWrite(out1,LOW); //relay1 is now turned on
      state1=0; 
     }
    if (pinData==1){ 
      digitalWrite(out1,HIGH); //relay1 is now turned off
      state1=1; 
      }}


BLYNK_WRITE(V2) // Virtual pin 2 to contlor relay 2 
{
  int pinData = param.asInt(); 
  if (pinData==0){ 
      digitalWrite(out2,LOW);
      state2=0; 
     }
    if (pinData==1){ 
      digitalWrite(out2,HIGH);
      state2=1; 
      }}
      
BLYNK_WRITE(V3) // Virtual pin 3 to contlor relay 3 
{
  int pinData = param.asInt(); 
  if (pinData==0){ 
      digitalWrite(out3,LOW);
      state3=0; 
     }
    if (pinData==1){ 
      digitalWrite(out3,HIGH);
      state3=1; 
      }}

BLYNK_WRITE(V4) // Virtual pin 4 to contlor relay 4 
{
  int pinData = param.asInt(); 
  if (pinData==0){ 
      digitalWrite(out4,LOW);
      state4=0; 
     }
    if (pinData==1){ 
      digitalWrite(out4,HIGH);
      state4=1; 
      }}
      
void loop() {
  if (irrecv.decode(&results)) {    
serialPrintUint64(results.value, HEX);
    Serial.println("");
//here comes the remote part. the IR decode the hex value and compare

  if (results.value==0x80BF916E){ //replace the hex code with yours keeping the prefix 0x
    if(state1==1){ // if relay1 state was turned off, then we turn it on 
      digitalWrite(out1,LOW);
      state1=0; //relay1 is now turned on
      Blynk.virtualWrite(V1, 0); //sync the data from hardware to app
    }
    else{
      digitalWrite(out1,HIGH); 
      state1=1; 
      Blynk.virtualWrite(V1, 1);
    }}


  if (results.value==0x80BF9B64){ 
    if(state2==1){ 
      digitalWrite(out2,LOW);
      state2=0; 
      Blynk.virtualWrite(V2, 0);
    }
    else{
      digitalWrite(out2,HIGH); 
      state2=1;
      Blynk.virtualWrite(V2, 1);
    }}
  if (results.value==0x80BF6996){ 
    if(state3==1){ 
      digitalWrite(out3,LOW);
      state3=0; 
      Blynk.virtualWrite(V3, 0);
    }
    else{
      digitalWrite(out3,HIGH); 
      state3=1;
      Blynk.virtualWrite(V3, 1);
    }}
    if (results.value==0xFFCF30){ 
    if(state4==1){ 
      digitalWrite(out4,LOW);
      state4=0; 
      Blynk.virtualWrite(V4, 0);
    }
    else{
      digitalWrite(out4,HIGH); 
      state4=1;
      Blynk.virtualWrite(V4, 1);
    }}
//the master switch for remote
 
  if (results.value==0x80BF21DE){ // hex Code to turn everything ON/OFF
    if(stateall==1){ // if everything were turned off, then we turn them on 
      digitalWrite(out1,LOW);
      state1=0;
      Blynk.virtualWrite(V1, 0);
      
      digitalWrite(out2,LOW);
      state2=0;
      Blynk.virtualWrite(V2, 0);
      
      digitalWrite(out3,LOW);
      state3=0;
      Blynk.virtualWrite(V3, 0);
      
      digitalWrite(out4,LOW);
      state4=0;
      Blynk.virtualWrite(V4, 0);
      stateall=0;
      Blynk.virtualWrite(V10, 0); //sync the master button to the app
    }
    else{  
      digitalWrite(out1,HIGH); // if everything were turned off, then we turn them on 
      state1=1;
      Blynk.virtualWrite(V1, 1);
      
      digitalWrite(out2,HIGH);
      state2=1;
      Blynk.virtualWrite(V2, 1);
      
      digitalWrite(out3,HIGH);
      state3=1;
      Blynk.virtualWrite(V3, 1);
      
      digitalWrite(out4,HIGH);
      state4=1;
      Blynk.virtualWrite(V4, 1);
      
      stateall=1;
      Blynk.virtualWrite(V10, 1);
    }}


irrecv.resume();
  }
  delay(100);
 Blynk.run();
}
