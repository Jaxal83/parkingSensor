// Arduino9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Arduino9x_TX

#include <ArduinoUniqueID.h> 
#include <SPI.h>
#include <RH_RF95.h>
#include <RTCZero.h>


/************************************ LORA RADIO STUFF ***********************/
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
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
 
// Blinky on receipt
//#define LED 13
int ID; 

void setup() 
{
  
  //spi.setPins(10, 8, 9); // MISO 10, MOSI 8, SCK  9
  pinMode(LED_BUILTIN, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  while (!Serial);
  Serial.begin(9600);

  // Get ID of Arduino
//  UniqueIDdump(Serial);
//  UniqueID8dump(Serial);
//  for (size_t i = 0; i< UniqueIDsize; i++){
//    Serial.println(UniqueID[i]);
//    
//  }
  // Unique ID Setup
  int first = UniqueID[0];
  if (first == 0x59){
    ID = 1;
  }else if (first == 0x15)
  {
    ID = 2;
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
  Serial.print("First ");
  Serial.println(first);
  Serial.print("This LoRa radio ID is ");
  Serial.println(ID);
  if (ID == 1){
    Serial.println("I am the Garage Sensor Master.");
  }
  else {
    Serial.println("I am a Garage Sensor Unit.");
  }
  



  
  delay(100);
  // LoRa Setup
  Serial.println("Arduino LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);


     // RTC SETUP
   if (!(ID == 1)){
    rtc.begin(); //Start RTC library, this is where the clock source is initialized

    rtc.setTime(hours, minutes, seconds); //set time
    rtc.setDate(day, month, year); //set date
  
    rtc.setAlarmTime(00, 00, 10); //set alarm time to go off in 1 minute
    
    //following two lines enable alarm, comment both out if you want to do external interrupt
    rtc.enableAlarm(rtc.MATCH_SS); //set alarm
    rtc.attachInterrupt(ISR); //creates an interrupt that wakes the SAMD21 which is triggered by a FTC alarm
    //comment out the below line if you are using RTC alarm for interrupt
   // extInterrupt(A1); //creates an interrupt source on external pin
    digitalWrite(LED_BUILTIN, LOW);
    //puts SAMD21 to sleep
    //rtc.standbyMode(); //library call
    
   }
}
 
void loop()
{
  // The GSM Program
  if (ID == 1){
    if (rf95.available())
    {
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      delay(500);
      if (rf95.recv(buf, &len))
      {
        digitalWrite(LED_BUILTIN, HIGH);
        RH_RF95::printBuffer("Received: ", buf, len);
        Serial.print("Got: ");
        Serial.println((char*)buf);
         Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
        
        // Send a reply
        uint8_t data[] = "Message Received";
        rf95.send(data, sizeof(data));
        rf95.waitPacketSent();
        Serial.println("Sent a reply");
        digitalWrite(LED_BUILTIN, LOW);
      }
      else
      {
        Serial.println("Receive failed");
      }
    }
   
  }
  // END OF GSM PROGRAM
  
   // The GSU Program
    else
    {
      static int16_t packetnum = 0;  // packet counter, we increment per xmission
      Serial.println("Sending to rf95_server");
      // Send a message to rf95_server
      
      char radiopacket[20] = "Hello World #      ";
      itoa(ID, radiopacket+13, 10);
      Serial.print("Sending "); Serial.println(radiopacket);
      radiopacket[19] = 0;
      
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
          Serial.print("Got reply: ");
          Serial.println((char*)buf);
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);    
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
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
      //digitalWrite(LED_BUILTIN, HIGH);
      //delay(500);
      //digitalWrite(LED_BUILTIN, LOW);
      rtc.standbyMode();
      //delay(5000);
    }
}



//interrupt service routine (ISR), called when interrupt is triggered 
//executes after MCU wakes up
void ISR()
{
  //while (!Serial);
  Serial.flush();
  digitalWrite(LED_BUILTIN, HIGH);
  uint8_t secs = rtc.getAlarmSeconds();
  secs = secs+5;
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
