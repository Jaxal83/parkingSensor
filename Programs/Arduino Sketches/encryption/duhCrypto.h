/*duhCrypto.h
* Joel Berain
* This header file is use to implement a symmetric encryption method for a char array.
*/

#ifndef duhCrypto_h
#define duhCrypto_h

#include "Arduino.h"

class duhCrypto {

private:
	int key1;
	int key2;

	int* pattern;
	
	int* id;
	int idSize;

	uint8_t* message;
	int messageLen;

	String crypMode;
	uint8_t* crypMsg;
	int crypLen;

public:
	duhCrypto();

	int crypIt(String mode, uint8_t msg[], int size);

	uint8_t* getCrypMsg();

	uint8_t* getMessage();

	int getMessageLen();

	int getCrypLen();

	void printCrypMsg();

	void printMessage();
};
#endif
