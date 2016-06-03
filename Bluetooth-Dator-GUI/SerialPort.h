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
public:

	/**
	* @brief Class constructor
	*/
	SerialPort();
	~SerialPort();

	/**
	* @brief Connects to a serial port using a string ref.
	*
	* @param port The port to connect to
	*/
	int connect(std::string& port);
	
	/**
	* @brief Connects to a serial port using a wchar_t-array.
	*
	* @param device The port to connect to
	*/
	int connect(wchar_t *device);
	
	/**
	* @brief Disconnects the serial port.
	*
	*/
	void disconnect(void);

	/**
	* @brief Return connection status
	*
	*/
	bool isConnected() const;

	/**
	* @brief Sends an array of bytes over the serial port.
	*
	* @param buffer Pointer to the first byte of the buffer to send. 
	* @param len Number of bytes to send.
	*/
	int sendArray(unsigned char *buffer, int len);
	
	/**
	* @brief Recieves an array of bytes over the serial port.
	*
	* @param buffer Pointer to the first byte of the buffer to store incoming data.
	* @param len Number of bytes to recieve.
	*/
	int getArray(unsigned char *buffer, int len);

	/**
	* @brief Flushes the internal storage.
	*
	*/
	void clear();
	
private:
	HANDLE serialPortHandle; /**< Handle to the windows managed serial port */
	bool connected{ false }; /**< Connection status */
};

#endif

