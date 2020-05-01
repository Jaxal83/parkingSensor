/*duhCrypto.cpp
* Joel Berain
This is the .cpp file for duhCrypto.h.
*/

#include "Arduino.h"
#include "duhCrypto.h"

//Macros
#define KEY1 700//key to use for cryption
#define KEY2 900//key to use for cryption
#define PATTERN_LEN 7 //length of pattern used for cryption

#define ASCII_MIN 32
#define ASCII_MAX 126

/*duhCrypto
This is the constructor method.
*/
duhCrypto::duhCrypto() {
	int pat[] = { KEY1, KEY1, KEY1, KEY2, KEY1, KEY2, KEY1 }; //pattern to use
	pattern = (int*)malloc(PATTERN_LEN * sizeof(int)); 
	for (int i = 0; i < PATTERN_LEN; i++) {
		pattern[i] = *(pat + i);
	}

	uint8_t identify[] = { "parkItCda" };
	idSize = sizeof(identify) - 1;
	id = (int*)malloc(idSize * sizeof(int));
	for (int i = 0; i < idSize; i++) {
		id[i] = int(identify[i]);
	}	

	message = (uint8_t*)malloc(30 * sizeof(uint8_t));
	crypMsg = (uint8_t*)malloc(30 * sizeof(uint8_t));
}

/*crypIt
This function either encrypts or decrypts the input message.
*/
int duhCrypto::crypIt(String mode, uint8_t msg[], int size) {
	crypMode = mode; //"encrypt" or "decrypt"

	int key; //current key to use
	int character; //current character of input message
	int crypChar;
	int patternVar = 0; //place holder for key pattern

	if (crypMode == "encrypt") {
		messageLen = size; //input message length
		crypLen = size + idSize;

		for (int i = 0; i < messageLen; i++) {
			message[i] = *(msg + i);
		}

		for (int j = 0; j < idSize; j++) {
			crypMsg[j] = (uint8_t)id[j];
		}

		int ii = 0;
		for (int i = idSize; i < crypLen; i++) {
			key = *(pattern + patternVar); //get next key

			//increment key place holder
			patternVar++;
			if (patternVar == PATTERN_LEN) {
				patternVar = 0;
			}
			character = int(*(message + ii)); //get next character		

			crypChar = character - key;

			int dif;
			if (crypChar < ASCII_MIN) {
				dif = ASCII_MIN - crypChar - 1;
				if (dif > (ASCII_MAX - ASCII_MIN)) {
					crypChar = ASCII_MAX - dif % (ASCII_MAX - ASCII_MIN);
				}
				else {
					crypChar = ASCII_MAX - dif;
				}
			}
			else {
				crypChar = character - key; //calculate crypted character
			}
			crypMsg[i] = (uint8_t)crypChar; //add to crypted array
			ii++;
		}
	}

	if (crypMode == "decrypt") {
		messageLen = size; //input message length
		crypLen = size - idSize;

		for (int i = 0; i < messageLen; i++) {
			message[i] = *(msg + i);
		}

		for (int j = 0; j < idSize; j++) {
			if ((int)*(msg + j) != *(id + j)) {
				Serial.println("!!UNAUTHORIZED SENDER!!");
				uint8_t noMsg[] = "!!UNAUTHORIZED SENDER!!";
				crypLen = sizeof(noMsg) - 1;
				for (int jj = 0; jj < crypLen; jj++) {
					crypMsg[jj] = noMsg[jj];
				}
				return 0;
			}
		}
		Serial.println("++Authorized Sender++");

		int ii = idSize;
		for (int i = 0; i < crypLen; i++) {

			key = *(pattern + patternVar); //get next key

			//increment key place holder
			patternVar++;
			if (patternVar >= PATTERN_LEN) {
				patternVar = 0;
			}

			character = int(*(message + ii)); //get next character

			crypChar = character + key;

			int dif;
			if (crypChar > ASCII_MAX) {
				dif = crypChar - ASCII_MAX - 1;
				if (dif > (ASCII_MAX - ASCII_MIN)) {
					crypChar = ASCII_MIN + dif % (ASCII_MAX - ASCII_MIN);
				}
				else {
					crypChar = ASCII_MIN + dif;
				}
			}
			else {
				crypChar = character + key; //calculate crypted character
			}
			crypMsg[i] = (uint8_t)crypChar; //add to crypted array
			ii++;
		}
	}
	return 1;
}

/*getCrypMsg
This function returns the crypted message.
*/
uint8_t* duhCrypto::getCrypMsg() {
	return crypMsg;
}

uint8_t* duhCrypto::getMessage() {
	return message;
}

/*getMessageLen
This function returns the message length.
*/
int duhCrypto::getMessageLen() {
	return messageLen;
}

int duhCrypto::getCrypLen() {
	return crypLen;
}

/*printCrypMsg
This function prints the crypted message.
*/
void duhCrypto::printCrypMsg() {
	Serial.print(crypMode);
	Serial.print("ed message = '");
	for (int i = 0; i < crypLen; i++) {
		Serial.print((char)crypMsg[i]);
	}
	Serial.println("'");
}

/*printMessage
This function prints the original message.
*/
void duhCrypto::printMessage() {
	Serial.print("original message = '");
	for (int i = 0; i < messageLen; i++) {
		Serial.print((char)message[i]);
	}
	Serial.println("'");
}