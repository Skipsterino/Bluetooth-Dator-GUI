/**
* @file SerialPort.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Wrapper class for the serial communication interface
*
* Class to wrap the interface of serial-port communication
* through simple name referencing
*/

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <Windows.h>
#include <string>

class SerialPort {
private:
	HANDLE serialPortHandle;
	bool connected{ false };

public:
	SerialPort();
	~SerialPort();

	int connect(std::string& port);
	int connect(wchar_t *device);
	void disconnect(void);

	bool isConnected() const;

	int sendArray(unsigned char *buffer, int len);
	int getArray(unsigned char *buffer, int len);

	void clear();
};

#endif

