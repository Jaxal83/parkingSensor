This README file describes how to use and implement the encryption class that was created for data
security across our mesh network. It provides data concealment of data and also data authorization
checks for each data packet being recieved.

The class can be found under the following files:
- duhCrypto.h
- duhCrypto.cpp

To create an object type:
duhCrypto() <object name>

There are many function that can be used on this object including a crypIt() function that encrypts
or decrypts a message depending on it's argument. It also either matches or adds the authorization
phrase at the beginning of each data packet.

For more details on the functions and how to implement the class, read the comments in the files.

An example of the class being implemented on a "server" and a "client" Arduino board can be found
under the following files:
- server.ino
- client.ino

Load the "serverCryp.ino" file on the server Arduino board and the "clientCryp.ino" on the client 
Arduino board. Both boards must have LoRa feather antennas attached. Run both boards and notice
how the encryption class is implemented.