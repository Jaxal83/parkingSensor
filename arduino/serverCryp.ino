// Arduino9x_RX
    // -*- mode: C++ -*-
    // Example sketch showing how to create a simple messaging client (receiver)
    // with the RH_RF95 class. RH_RF95 class does not provide for addressing or
    // reliability, so you should only use RH_RF95 if you do not need the higher
    // level messaging abilities.
    // It is designed to work with the other example Arduino9x_TX
     
    #include <SPI.h>
    #include <RH_RF95.h>
    #include <duhCrypto.h>
     
    #define RFM95_CS 4
    #define RFM95_RST 2
    #define RFM95_INT 3
     
    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF95_FREQ 915.0
     
    // Singleton instance of the radio driver
    RH_RF95 rf95(RFM95_CS, RFM95_INT);
     
    // Blinky on receipt
    #define LED 13
     
    void setup() 
    {
      pinMode(LED, OUTPUT);     
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
     
      while (!Serial);
      Serial.begin(9600);
      delay(100);
     
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
     
      // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
     
      // The default transmitter power is 13dBm, using PA_BOOST.
      // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
      // you can set transmitter powers from 5 to 23 dBm:
      rf95.setTxPower(23, false);
    }

    int16_t packetnum = 0;

    uint8_t radiopacketReply[] = "And hello back to you";
    int replySize = sizeof(radiopacketReply) - 1;

    uint8_t* outMsg;
    int packSize;

    duhCrypto thisPacket;
     
    void loop()
    {
      if (rf95.available())
      {
        // Should be a message for us now   
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        
        if (rf95.recv(buf, &len))
        {
          packetnum++;
          Serial.print("\n<<<<<< packetnum = ");
          Serial.print(packetnum);
          Serial.print(" >>>>>> \n<<< IN >>>\n");
          
          digitalWrite(LED, HIGH);
          RH_RF95::printBuffer("Received: ", buf, len);
          Serial.print("Got: '");
          for(int i = 0; i < len; i++) {
            Serial.print((char)*(buf + i));
          }
          Serial.print("' (pack size = ");
          Serial.print(len);
          Serial.println(")");
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);

          thisPacket.crypIt("decrypt", buf, len);
          thisPacket.printMessage();
          thisPacket.printCrypMsg();
          delay(10);
          
          // Send a reply
          Serial.print("<<< OUT >>>\n");

          thisPacket.crypIt("encrypt", radiopacketReply, replySize);
          thisPacket.printMessage();
          thisPacket.printCrypMsg();

          packSize = thisPacket.getMessageLen();
          outMsg = thisPacket.getCrypMsg();
          delay(10);

          Serial.print("Sending '");
          for(int i = 0; i < packSize; i++) {
            Serial.print((char)*(outMsg + i));
          }
          Serial.print("' (pack size = ");
          Serial.print(packSize);
          Serial.println(")");
          
          rf95.send(outMsg, packSize);
          rf95.waitPacketSent();
          Serial.println("Sent a reply");
          digitalWrite(LED, LOW);
        }
        else
        {
          Serial.println("Receive failed");
        }
      }
    }
