/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/**
 * @file      ConnectionUSB.hpp
 * @since     Jun 19, 2018
 * @author    Patricio A. Rossi (MeduZa)
 * @copyright Copyright © 2018 Patricio A. Rossi (MeduZa)
 */

// For ints.
#include <cstdint>

// To handle usb devices.
#include <libusb.h>

// To handle dynamic arrays.
#include <vector>
using std::vector;

#include <chrono>
#include <thread>

#include "utility/Error.hpp"
#include "utility/Log.hpp"
#include "utility/Utility.hpp"

#ifndef CONNECTIONUSB_HPP_
#define CONNECTIONUSB_HPP_ 1

/// Default USB timeout
#define TIMEOUT 1500

/// the number of columns to display when dumping pins.
#define MAX_DUMP_COLUMNS 40

namespace LEDSpicer {

/**
 * LEDSpicer::ConnectionUSB
 * @note All USBLib functions return 0 when success.
 * Every LED is a single element, RGB elements uses three LEDs.
 * LEDs always have 255 levels of intensity even when they are on/off only.
 */
class ConnectionUSB {

public:

	ConnectionUSB(uint16_t requestType, uint16_t request, uint8_t elements);

	virtual ~ConnectionUSB();

	static void openSession();

	void initialize();

	static void terminate();

	/**
	 * This method will be called every time a transfer need to be done.
	 */
	virtual void transfer() = 0;

	/**
	 * Transfers miscellaneous data.
	 * @param data
	 */
	virtual void transferData(vector<uint8_t>& data);

	static void setInterval(uint8_t waitTime);

	static uint8_t getInterval();

protected:

	/// USB device handle.
	libusb_device_handle* handle = nullptr;

	/// App wide libusb session.
	static libusb_context *usbSession;

	/// Internal structure to keep initialization information
	struct ConnectionData {
		const char* name = nullptr;
		uint16_t
			/// Vendor ID
			vendor  = 0,
			/// Product ID
			product = 0,
			/// The value field for the setup packet.
			value   = 0;
		uint8_t
			/// Interface number
			interface = 0,
			/// Board ID
			boardId   = 0,
			/// Number of board LEDs
			LEDs      = 0;
	} board;

	vector<uint8_t> LEDs;

	uint8_t
		requestType = 0,
		request     = 0;

	static uint8_t waitTime;

	virtual void afterConnect() = 0;

	/**
	 * Connect to the USB board.
	 */
	void connect();

	/**
	 * Detach from kernel and claim the interface.
	 */
	void claimInterface();

	/**
	 * if anything needs to be initialized after claiming goes here.
	 */
	virtual void afterClaimInterface() = 0;
};

} /* namespace LEDSpicer */

#endif /* CONNECTIONUSB_HPP_ */
