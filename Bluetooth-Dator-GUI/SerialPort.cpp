/**
* @file SerialPort.cpp
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Wrapper class for the serial communication interface
*
* Class to wrap the interface of serial-port communication
* through simple name referencing
*/

#include "SerialPort.h"
#include <iostream>
#include <string>

SerialPort::SerialPort() {
	serialPortHandle = INVALID_HANDLE_VALUE;
	}

SerialPort::~SerialPort() {
	if (serialPortHandle != INVALID_HANDLE_VALUE)
		CloseHandle(serialPortHandle);

	serialPortHandle = INVALID_HANDLE_VALUE;
}

#pragma warning(push)
#pragma warning( disable : 4996)

int SerialPort::connect(std::string& port) {

	const size_t cSize = strlen(port.c_str()) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, port.c_str(), cSize);

	if (!connect(wc))
	{
		std::cout << "Serial port connected!" << std::endl;
		connected = true;

		return true;
	}
	std::cout << "Error connecting to serial port!" << std::endl;
	return false;
}

#pragma warning( pop )

int SerialPort::connect(wchar_t* device) {
	int error = 0;
	DCB dcb;
	COMMTIMEOUTS timeouts = { 0, 0, 300, 0, 0}; //read constant 300, rest = 0 <=> not used.

	memset(&dcb, 0, sizeof(dcb));

	dcb.DCBlength = sizeof(dcb);

	dcb.BaudRate = 115200;
	dcb.Parity = NOPARITY;
	dcb.fParity = 0;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;

	serialPortHandle = CreateFile(device, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);

	if (serialPortHandle != INVALID_HANDLE_VALUE) {
		if (!SetCommState(serialPortHandle, &dcb))
			error = 2;
		else if (!SetCommTimeouts(serialPortHandle, &timeouts))
			error = 3;
	}
	else {
		error = 1;
	}

	if (error != 0) {
		disconnect();
	}
	else {
		clear();
	}

	return error;
}

void SerialPort::disconnect(void) {
	CloseHandle(serialPortHandle);
	serialPortHandle = INVALID_HANDLE_VALUE;
	connected = false;
	std::cout << "Serial port disconnected" << std::endl;
	//printf("Port 1 has been CLOSED and %d is the file descriptionn", fileDescriptor);
}

bool SerialPort::isConnected() const
{
	return connected;
}

int SerialPort::sendArray(unsigned char *buffer, int len) {
	unsigned long result{ 0 };

	if (serialPortHandle != INVALID_HANDLE_VALUE)
		WriteFile(serialPortHandle, buffer, len, &result, NULL);

	return result;
}

int SerialPort::getArray(unsigned char *buffer, int len) {
	unsigned long read_nbr{ 0 };

	if (serialPortHandle != INVALID_HANDLE_VALUE)
	{
		ReadFile(serialPortHandle, buffer, len, &read_nbr, NULL);
		DWORD error = GetLastError();
		if (error != ERROR_SUCCESS) {
			disconnect();
		}
	}

	return((int)read_nbr);
}

void SerialPort::clear() {
	PurgeComm(serialPortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}
