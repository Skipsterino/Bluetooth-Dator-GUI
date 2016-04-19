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

int SerialPort::connect(std::string& port) {

	if (!connect(TEXT("COM5")))
	{
		std::cout << "Serial port connected!" << std::endl;
		connected = true;

		return true;
	}
	std::cout << "Error connecting to serial port!" << std::endl;
	return false;
}

int SerialPort::connect(wchar_t* device) {
	int error = 0;
	DCB dcb;
	COMMTIMEOUTS timeouts;

	timeouts.ReadIntervalTimeout = 200;
	timeouts.WriteTotalTimeoutConstant = 200;
	timeouts.ReadTotalTimeoutConstant = 200;
	timeouts.ReadTotalTimeoutMultiplier = 200;
	timeouts.WriteTotalTimeoutMultiplier = 200;

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
			connected = false;
		}
	}

	return((int)read_nbr);
}

void SerialPort::clear() {
	PurgeComm(serialPortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}
