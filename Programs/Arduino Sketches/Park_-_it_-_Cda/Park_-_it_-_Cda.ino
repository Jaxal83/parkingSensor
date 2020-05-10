/*
 * This is the sketch Zane used for the finished GSU in his garage.
 * It uses ZSS.ino as a starting point
 * The IDs 2 and 4 were used for the final demo
 * 
 * 
 * 
 */

#include <ArduinoUniqueID.h> 
#include <SPI.h>
#include <RH_RF95.h>
#include <RTCZero.h>
#include <NewPing.h>
#include <FastLED.h>

/*********************************** For controlling the LEds *****************/
#define DATA_PIN    5                         // for setting the LED
#define LED_TYPE    WS2811                    // one way, fire and forget LED strip
#define COLOR_ORDER GRB
#define NUM_LEDS      1
#define BRIGHTNESS    155

CRGB leds[NUM_LEDS];                         // create an array of LEDs

/************************************* Ultrasonic Sensor ***********************/
#define TRIGGER_PIN  A5                      // for sending the ping
#define ECHO_PIN     A4                      // for receiving the ping back
#define MAX_DISTANCE 200                     // max distance that the sensor will read

NewPing sonar(TRIGGER_PIN, ECHO_PIN);        // creating the sonar


#include "RTClib.h"
/************************************ LORA RADIO STUFF ***********************/
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 900.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
/****** END OF LORA RADIO STUFF **********************************************/

/*************************** RTC *********************************************/
/* Create an rtc object */
RTCZero rtc;
/* Change these values to set the current initial time */
const byte seconds = 00;
const byte minutes = 00;
const byte hours = 13;

/* Change these values to set the current initial date */
const byte day = 29;
const byte month = 2;
const byte year = 20;

/****************************** END OF RTC **********************************/
RTC_DS3231 ds3231;

unsigned const int GSG = 99;

int ID; 
int occupied = 0;

static boolean ack = false;

void setup() 
{
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);         // populate the array with the individual LEDs
  FastLED.setBrightness(BRIGHTNESS);
  
  //spi.setPins(10, 8, 9); // MISO 10, MOSI 8, SCK  9
  pinMode(LED_BUILTIN, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  
  // Start DS3231 connection
  if (! ds3231.begin()) {
    //Serial.println("Couldn't find RTC");
    while (1);
  }
  // Get ID of Arduino
//  UniqueIDdump(Serial);
//  UniqueID8dump(Serial);
//  for (size_t i = 0; i< UniqueIDsize; i++){
//    Serial.println(UniqueID[i]);
//    
//  }
  // Unique ID Setup
  int first = UniqueID[0];
  if (first != 0x59){
    //while (!Serial);
    //Serial.begin(9600);
  }
  if (first == 0x59){
    ID = 1;
    //while (!Serial);
    //Serial.begin(9600);
  }
  else if (first == 0x15)
  {
    ID = 2;
    while (!Serial);
    Serial.begin(9600);
  }
  else if (first == 0x8D)
  {
    ID = 3;
  }
  else if (first == 0x2F)
  {
    ID = 4;
  }
  else if (first == 0x54){
    ID = 5;
  }
  else if (first == 0x5F){
    ID = 6;
  }
  else if (first == 0xD9){
    ID = 7;
  }
  else {
    ID = 0;
  }
  
  if (ID == 1){
//    Serial.print("First ");
//    Serial.println(first);
//    Serial.print("This LoRa radio ID is ");
//    Serial.println(ID);
//    Serial.println("I am the Garage Sensor Master.");
//    Serial.println("I will forward on all messages received to the Dragino GSG.");
  }
  else {
    Serial.println("I am a Garage Sensor Unit.");
    Serial.print("This LoRa radio ID is ");
    Serial.println(ID);
  }
  



  
  delay(100);
  // LoRa Setup
  //Serial.println("Arduino LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    //Serial.println("LoRa radio init failed");
    while (1);
  }
  //Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    //Serial.println("setFrequency failed");
    while (1);
  }
  //Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  if (!rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512)){
        //Serial.println("Invalid setModemConfig() option");
     }

     //Bw125Cr45Sf128    Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range.
    //Bw500Cr45Sf128    Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range.
    //Bw31_25Cr48Sf512  Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range.
    //Bw125Cr48Sf4096   Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range.
      // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);


     // RTC SETUP
   if (!(ID == 1)){
    rtc.begin(); //Start RTC library, this is where the clock source is initialized

    rtc.setTime(hours, minutes, seconds); //set time
    rtc.setDate(day, month, year); //set date
  
    rtc.setAlarmTime(00, 00, 20); //set alarm time to go off in 1 minute
    
    //following two lines enable alarm, comment both out if you want to do external interrupt
    rtc.enableAlarm(rtc.MATCH_SS); //set alarm
    rtc.attachInterrupt(ISR); //creates an interrupt that wakes the SAMD21 which is triggered by a FTC alarm
    //comment out the below line if you are using RTC alarm for interrupt
   // extInterrupt(A1); //creates an interrupt source on external pin
    digitalWrite(LED_BUILTIN, LOW);
    //puts SAMD21 to sleep
    //rtc.standbyMode(); //library call
    
   }
   else {
    rtc.begin(); //Start RTC library, this is where the clock source is initialized

    rtc.setTime(hours, minutes, seconds); //set time
    rtc.setDate(day, month, year); //set date
  
    rtc.setAlarmTime(00, 00, 30); //set alarm time to go off in 1 minute
    
    //following two lines enable alarm, comment both out if you want to do external interrupt
    rtc.enableAlarm(rtc.MATCH_SS); //set alarm
    rtc.attachInterrupt(ISR); //creates an interrupt that wakes the SAMD21 which is triggered by a FTC alarm
   }
}
 
void loop()
{
  /**************************************** checking distance with ultrasonic sensor ********************/
  checkForCars();                               // check and update stall status before sending it to gateway

  
  
  //RtcDateTime now = rtc.GetDateTime();
  // The GSM Program
  if (ID == 6){
    digitalWrite(LED_BUILTIN, HIGH);
    if (rf95.waitAvailableTimeout(10000))
    {
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      delay(500);
      if (rf95.recv(buf, &len))
      {
        // first character is 6
        if (buf[0] == 0x36){
          //digitalWrite(LED_BUILTIN, HIGH);
          RH_RF95::printBuffer("Received: ", buf, len);
//          Serial.print("Got: ");
//          Serial.println((char*)buf);
//          Serial.print("RSSI: ");
//          Serial.println(rf95.lastRssi(), DEC);
          
          // Send a reply to the GSU
//          char data[2];
//          //uint8_t data[2];
//          data[0] = buf[1];
//          data[1] = '1'; 
//          rf95.send((uint8_t *)data, sizeof(data));
//          rf95.waitPacketSent();
////          Serial.print("Sent a reply ");
////          Serial.println(data);
//          digitalWrite(LED_BUILTIN, LOW);
//          delay(500);
          //rf95.setTxPower(5, false);
//          DateTime now = ds3231.now();
//          Serial.println(now.unixtime());
//          Serial.println("");
          // Forward the message onto the GSG Dragino
          char gatewayMessage[26];
          gatewayMessage[0] = '<';
          gatewayMessage[1] = '1';
          gatewayMessage[2] = '>';
          itoa (GSG, gatewayMessage+3, 10);
          for (int i = 0; i < 26; i++){
            gatewayMessage[i+5] = buf[i];
          }
          //itoa (buf, gatewayMessage+1, 10);
          //Serial.print("Sending "); Serial.print(gatewayMessage); 
          //Serial.println(" to the GSG.");
          rf95.send((uint8_t *)gatewayMessage, 26);
          rf95.waitPacketSent();
          //rf95.setTxPower(23, false);
        }
      }
      else
      {
        //Serial.println("Receive failed");
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
  // END OF GSM PROGRAM

  // NIKOLAI'S RECEIVER
  // WILL RECEIVE A MESSAGE THEN FORWARD ONTO MASTER AT ZANE'S HOUSE
  else if (ID == 1){
    digitalWrite(LED_BUILTIN, HIGH);
    //Serial.println("Listening...");
    if (rf95.waitAvailableTimeout(5000))
    {
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      //delay(500);
      if (rf95.recv(buf, &len))
      {
        // first character is 6
        //if (buf[0] == 0x36){
          //digitalWrite(LED_BUILTIN, HIGH);
          //RH_RF95::printBuffer("Received: ", buf, len);
//          Serial.print("Got: ");
//          Serial.println((char*)buf);
//          Serial.print("RSSI: ");
//          Serial.println(rf95.lastRssi(), DEC);
          
          // Forward onto the GSM
          digitalWrite(LED_BUILTIN, LOW);
//          DateTime now = ds3231.now();
          //Serial.println(now.unixtime());
          //Serial.println("");
          // Forward the message onto the GSG Dragino
          char gatewayMessage[26];
          //itoa (gatewayMessage, 10);
          for (int i = 0; i < 26; i++){
            gatewayMessage[i] = buf[i];
          }
          //itoa (buf, gatewayMessage+1, 10);
          //Serial.print("Sending "); Serial.print(gatewayMessage); 
          //Serial.println(" to the GSG.");
          rf95.send((uint8_t *)gatewayMessage, 26);
          rf95.waitPacketSent();
        //}
      }
      else
      {
       // Serial.println("Receive failed");
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
    
  }

// passive receiver
  else if (ID == 3){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Listening...");
    if (rf95.waitAvailableTimeout(5000))
    {
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      //delay(500);
      if (rf95.recv(buf, &len))
      {
        // first character is 6
        //if (buf[0] == 0x36){
          //digitalWrite(LED_BUILTIN, HIGH);
          //RH_RF95::printBuffer("Received: ", buf, len);
          Serial.print("Got: ");
          Serial.println((char*)buf);
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);
          
          // Forward onto the GSM
          digitalWrite(LED_BUILTIN, LOW);
//          DateTime now = ds3231.now();
          //Serial.println(now.unixtime());
          //Serial.println("");
          // Forward the message onto the GSG Dragino
//          char gatewayMessage[26];
//          //itoa (gatewayMessage, 10);
//          for (int i = 0; i < 26; i++){
//            gatewayMessage[i] = buf[i];
//          }
//          //itoa (buf, gatewayMessage+1, 10);
//          //Serial.print("Sending "); Serial.print(gatewayMessage); 
//          //Serial.println(" to the GSG.");
//          rf95.send((uint8_t *)gatewayMessage, 26);
//          rf95.waitPacketSent();
        //}
      }
      else
      {
       Serial.println("Receive failed");
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
  
}
  // THE GSU
  else if (ID == 4 || ID ==2){
      digitalWrite(LED_BUILTIN, HIGH);
      ack = false;
      // Send a message to rf95_server
      DateTime now = ds3231.now();

      char radiopacket[24];

      // SEND DIRECTLY TO THE GSG HAS HEADER
      //char gatewayMessage[26];
      radiopacket[0] = '<';
      radiopacket[1] = '1';
      radiopacket[2] = '>';
      itoa(6, radiopacket+3, 10);
      itoa(ID, radiopacket+4, 10);
      itoa(occupied, radiopacket + 5, 10);
      itoa(now.unixtime(), radiopacket+6, 10);
      radiopacket[23] = 0;

      // SEND TO GSM SHORTER PACKET
      /*
      itoa(6, radiopacket, 10);
      itoa(ID, radiopacket+1, 10);
      itoa(occupied, radiopacket + 2, 10);
      itoa(now.unixtime(), radiopacket+3, 10);
      radiopacket[23] = 0;*/
      
      //Serial.print("Sending "); Serial.println(radiopacket);
      
    //while(!ack){
      //static int16_t packetnum = 0;  // packet counter, we increment per xmission
      Serial.println("Sending to rf95_server");
      Serial.print("Sending "); Serial.println(radiopacket);
      
      
      Serial.println("Sending..."); delay(10);
      rf95.send((uint8_t *)radiopacket, 20);
     
      Serial.println("Waiting for packet to complete..."); delay(10);
      rf95.waitPacketSent();
      
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);
    //}
  }
  
   // The GSU Program SENDS A MESSAGE
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      ack = false;
      // Send a message to rf95_server
        DateTime now = ds3231.now();
        
        char radiopacket[24];
        itoa(6, radiopacket, 10);
        itoa(ID, radiopacket+1, 10);
        itoa(occupied, radiopacket + 2, 10);
        itoa(now.unixtime(), radiopacket+3, 10);
        //Serial.print("Sending "); Serial.println(radiopacket);
        radiopacket[23] = 0;
      while(!ack){
        static int16_t packetnum = 0;  // packet counter, we increment per xmission
        Serial.println("Sending to rf95_server");
        Serial.print("Sending "); Serial.println(radiopacket);
        
        
        Serial.println("Sending..."); delay(10);
        rf95.send((uint8_t *)radiopacket, 20);
       
        Serial.println("Waiting for packet to complete..."); delay(10);
        rf95.waitPacketSent();
        // Now wait for a reply
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
       
        Serial.println("Waiting for reply..."); delay(10);
        if (rf95.waitAvailableTimeout(5000))
        { 
          // Should be a reply message for us now   
          if (rf95.recv(buf, &len))
         {
            //char id = (char)ID;
            //if (buf[0] == id){
            //id =
            Serial.println((char*)buf[0]);
              Serial.print("Got reply: ");
              Serial.println((char*)buf);
              Serial.print("RSSI: ");
              Serial.println(rf95.lastRssi(), DEC);    
//              digitalWrite(LED_BUILTIN, HIGH);
//              delay(50);
//              digitalWrite(LED_BUILTIN, LOW);
//              delay(50);
//              digitalWrite(LED_BUILTIN, HIGH);
//              delay(50);
//              digitalWrite(LED_BUILTIN, LOW);
              ack = true;
            //}
          }
          else
          { 
            Serial.println("Receive failed");
          }
        }
      
        else
        {
          Serial.println("No reply, is there a listener around?");
        }
      }
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      //rtc.standbyMode();
      delay(5000);
      //__WFI();
    }
}



//interrupt service routine (ISR), called when interrupt is triggered 
//executes after MCU wakes up
void ISR()
{
  //while (!Serial);
  //Serial.flush();
  digitalWrite(LED_BUILTIN, HIGH);
  uint8_t secs = rtc.getAlarmSeconds();
  if (ID == 1){
    secs = secs + 30;
  }
  else
  {
    secs = secs + 10;
  }
  secs = secs%60;
  //uint8_t mins = rtc.getAlarmMinutes();
  //mins = mins%60;
  //mins = mins + 1;
  
  //rtc.setAlarmMinutes(mins); //set alarm time to go off in 1 minute
  //delay(2000);
  //digitalWrite(LED_BUILTIN, LOW);
  rtc.setAlarmSeconds(secs);
  //rtc.enableAlarm(rtc.MATCH_SS); //set alarm
  //rtc.standbyMode();
}


//function that sets up external interrupt
void extInterrupt(int interruptPin) {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin, ISR, LOW);
}

//function to show how to put the 
void samSleep()
{
  // Set the sleep mode to standby
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  // SAMD sleep
  __WFI();
}

void checkForCars()
{
  int distanceCheck1 = sonar.ping_cm();           // send out 3 pings with a 10ms delay in between 
  delay(10);

  int distanceCheck2 = sonar.ping_cm();           // sensor receives the ping, and calculates the distance
  delay(10);

  int distanceCheck3 = sonar.ping_cm();           // 3 checks to avoid outlyers or erroneous reads
  delay(10);
  
  if (distanceCheck1 > 100 && distanceCheck2 > 100 && distanceCheck3 > 100)           // all 3 pings need to be within a consistent range
  {
    leds[0] = CRGB(0, 255, 0);                                                        // if they are, change the LED color
    occupied = 0;                                                                     // change the status of the parking stall for tx packet
    FastLED.show();                                                                   // update the LEDs new color
    delay(50);                                                                        // give the LED time to update to avoid getting stuck in a random hue
  }

  else if (distanceCheck1 < 100 && distanceCheck2 < 100 && distanceCheck3 < 100)
  {
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
    occupied = 1;
    delay(50);
  }
}
