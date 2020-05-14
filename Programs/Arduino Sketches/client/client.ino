    // LoRa 9x_TX
    // -*- mode: C++ -*-
    // Example sketch showing how to create a simple messaging client (transmitter)
    // with the RH_RF95 class. RH_RF95 class does not provide for addressing or
    // reliability, so you should only use RH_RF95 if you do not need the higher
    // level messaging abilities.
    // It is designed to work with the other example LoRa9x_RX
     
    #include <SPI.h>
    #include <RH_RF95.h>
    // These pins are changed from the default so that it works on the Feather M0 
    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 3
    // Use these pins instead if using an Arduino Uno with the LoRa module  
	//#define RFM95_CS 4
    //#define RFM95_RST 2
    //#define RFM95_INT 3
	
     
    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF95_FREQ 900.0
     
    // Singleton instance of the radio driver
    RH_RF95 rf95(RFM95_CS, RFM95_INT);
    
    // Blinky on receipt
    #define LED 13 
    
    void setup() 
    {
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
      pinMode(LED, OUTPUT);     
     
      while (!Serial);
      Serial.begin(9600);
      delay(100);
     
      Serial.println("Arduino LoRa TX Test!");
     
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
	  // Long range modem config, comment this out to use the default
      if (!rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512)){
            Serial.println("Invalid setModemConfig() option");
            while (1);
      }

      //Bw125Cr45Sf128    Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range.
      //Bw500Cr45Sf128    Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range.
      //Bw31_25Cr48Sf512  Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range.
      //Bw125Cr48Sf4096   Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range.
      Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
      
      // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
     
      // The default transmitter power is 13dBm, using PA_BOOST.
      // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
      // you can set transmitter powers from 5 to 23 dBm:
      rf95.setTxPower(23, false);
    }
     
    int16_t packetnum = 0;  // packet counter, we increment per xmission
     
    void loop()
    {
      Serial.println("Sending to rf95_server");
      // Send a message to rf95_server
      
      char radiopacket[20] = "Hello World #      ";
      itoa(packetnum++, radiopacket+13, 10);
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
          digitalWrite(LED, LOW);
          delay(50);
          digitalWrite(LED, HIGH);
          delay(50);
          Serial.print("Got reply: ");
          Serial.println((char*)buf);
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);    
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
      delay(5000);
    }
