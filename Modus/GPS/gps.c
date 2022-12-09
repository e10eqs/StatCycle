/*
 * gps.c
 *
 *  Created on: Nov 21, 2022
 *      Author: ogage
 */
#include "gps.h"
#include "i2c.h"

#define PRINT_WRITE_MESSAGES 0
#define UART_ENABLED		 0

UINT bw;

struct ubxPacket packet;
struct PVTData pvt;

static uint32_t extractU4FromMsg(uint8_t * msg, uint8_t startIndex);
static uint16_t extractU2FromMsg(uint8_t * msg, uint8_t startIndex);

static uint32_t extractU4FromMsg(uint8_t * msg, uint8_t startIndex)
{
	uint32_t value = (uint32_t) extractU2FromMsg(msg, startIndex);
	value |= ((uint32_t) extractU2FromMsg(msg, startIndex + 2)) << 16;
	return value;
}

static uint16_t extractU2FromMsg(uint8_t * msg, uint8_t startIndex)
{
	uint16_t value = (uint16_t) msg[startIndex];
	value |= ((uint16_t) msg[startIndex + 1]) << 8;
	return value;
}

void calcChecksum(struct ubxPacket * msg)
{
	msg->checksumA = 0;
	msg->checksumB = 0;

	msg->checksumA += msg->class;
	msg->checksumB += msg->checksumA;

	msg->checksumA += msg->id;
	msg->checksumB += msg->checksumA;

	msg->checksumA += (msg->payload_len & 0xFF);
	msg->checksumB += msg->checksumA;

	msg->checksumA += (msg->payload_len >> 8);
	msg->checksumB += msg->checksumA;

	for (uint8_t i = 0; i < msg->payload_len; i++)
	{
		msg->checksumA += msg->payload[i];
		msg->checksumB += msg->checksumA;
	}
}

uint16_t getNumBytesAvailable()
{
	cy_rslt_t result;
	uint8_t write_buffer[1];
	uint8_t read_buffer[2];

	write_buffer[0] = 0xFD;
	result = cyhal_i2c_master_write(&i2c_master_obj, GPS_ADDR, write_buffer, 1, 0, 0);

	if( result != CY_RSLT_SUCCESS) {
		volatile uint8_t type = CY_RSLT_GET_TYPE(result);
		volatile uint16_t module_id = CY_RSLT_GET_MODULE(result);
		volatile uint16_t error_code = CY_RSLT_GET_CODE(result);
#if UART_ENABLED
		printf("Bytes available write error: type=%x, module=%x, code=%x\r\n", type, module_id, error_code);
#endif
		return 0xF0F0;
	}

	result = cyhal_i2c_master_read(&i2c_master_obj, GPS_ADDR, read_buffer, 2, 0, true);

	if( result != CY_RSLT_SUCCESS) {
#if UART_ENABLED
		uint8_t type = CY_RSLT_GET_TYPE(result);
		uint16_t module_id = CY_RSLT_GET_MODULE(result);
		uint16_t error_code = CY_RSLT_GET_CODE(result);
		printf("Bytes available read error: type=%x, module=%x, code=%x\r\n", type, module_id, error_code);
#endif
		return 0xF0F0;
	}

	read_buffer[0] &= 0x7F; // Clear MSB
	// read_buffer[0] &= 0xFD; // Clear LSB

	return ((uint16_t) read_buffer[0] << 8 | read_buffer[1]);

}

void readMessage()
{
	cy_rslt_t result;
	uint16_t bytes = getNumBytesAvailable();
	uint8_t read_buffer[bytes];

#if UART_ENABLED
	printf("Num Bytes: %d\n\r", bytes);
#endif

	if(bytes != 0) {
		result = cyhal_i2c_master_read(&i2c_master_obj, GPS_ADDR, read_buffer, bytes, 0, true);

		if( result != CY_RSLT_SUCCESS) {
#if UART_ENABLED
			uint8_t type = CY_RSLT_GET_TYPE(result);
			uint16_t module_id = CY_RSLT_GET_MODULE(result);
			uint16_t error_code = CY_RSLT_GET_CODE(result);
			printf("Read Message failure: type=%x, module=%x, code=%x\r\n", type, module_id, error_code);
#endif
			return;
		}
#if UART_ENABLED
		printf("%s\n\r", read_buffer);
		printf("\r\n\r\n");

#else
		// Write the read_buffer to the SD card
		if (fr == FR_OK) {
			f_write(&Fil, read_buffer, bytes, &bw);	/* Write data to the file */
		}

#endif
	}

}

bool writeMessage()
{
	calcChecksum(&packet);

	uint16_t payload_len = packet.payload_len;
	uint16_t message_len = payload_len + 8;
	uint8_t write_buffer[message_len];

	write_buffer[0] = SYNC_CHAR_1;
	write_buffer[1] = SYNC_CHAR_2;
	write_buffer[2] = packet.class;
	write_buffer[3] = packet.id;
	write_buffer[4] = ((uint8_t) (packet.payload_len & 0xFF)); // Length LSB
	write_buffer[5] = ((uint8_t) (packet.payload_len >> 8));   // Length MSB
	for(int i = 0; i < payload_len; i++) {
		write_buffer[6 + i] = packet.payload[i];
	}

	write_buffer[6 + payload_len] = packet.checksumA;
	write_buffer[7 + payload_len] = packet.checksumB;

#if PRINT_WRITE_MESSAGES
	printf("Message length: %d\r\n", message_len);
	for(int i = 0; i < message_len; i++) {
		printf("%x\r\n", write_buffer[i]);
	}
#endif


	cy_rslt_t result;
	result = cyhal_i2c_master_write(&i2c_master_obj, GPS_ADDR, write_buffer, message_len, 10000, true);

	if( result != CY_RSLT_SUCCESS ) {

		volatile uint8_t type = CY_RSLT_GET_TYPE(result);
		volatile uint16_t module_id = CY_RSLT_GET_MODULE(result);
		volatile uint16_t error_code = CY_RSLT_GET_CODE(result);
#if UART_ENABLED
		printf("Write message error\r\n");
		printf("type=%x, module=%x, code=%x\n", type, module_id, error_code);
#endif
		return false;
	}

#if UART_ENABLED
	printf("Write message success\r\n");
#endif

	return true;
}



bool setCommunicationToUbx()
{
	bool result;

	packet.class = CFG_CLASS;
	packet.id = CFG_PRT;

	packet.payload_len = 20;
	for(int i = 0; i < 20; i++) {
		packet.payload[i] = 0;
	}

	packet.payload[0] = 0x00;  // Port identifier number (equals 0 for I2C)
	packet.payload[4] = 0x84;  // Slave address (0x42 << 1)
	packet.payload[12] = 0x01; // A mask describing which input protocols are active
	packet.payload[14] = 0x01; // A mask describing which output protocols are active

	result = writeMessage();

	if( !result ) {
		return false;
	}

	result = waitForAck();

	if( result == true ) {
#if UART_ENABLED
		printf("Set communication to UBlox only\r\n");
#endif
		return true;
	}
	else {
#if UART_ENABLED
		printf("Failed to set communication to UBlox only\r\n");
#endif
		return false;
	}
}

bool saveConfig()
{
	bool result;

	packet.class = CFG_CLASS;
	packet.id = CFG_CFG;

	packet.payload_len = 12;
	for(int i = 0; i < 12; i++) {
		packet.payload[i] = 0;
	}

	packet.payload[4] = 0x1F;
	packet.payload[5] = 0x1F;

	result = writeMessage();

	if( !result ) {
		return false;
	}

	result = waitForAck();

	if( result == true ) {
#if UART_ENABLED
		printf("Save configuration\r\n");
#endif
		return true;
	}
	else {
#if UART_ENABLED
		printf("Failed to save configuration\r\n");
#endif
		return false;
	}
}

bool resetConfig() {
	bool result;

	packet.class = CFG_CLASS;
	packet.id = CFG_CFG;

	packet.payload_len = 12;
	for(int i = 0; i < 12; i++) {
		packet.payload[i] = 0;
	}

	packet.payload[0] = 0x1F;
	packet.payload[1] = 0x1F;

	packet.payload[8] = 0x1F;
	packet.payload[9] = 0x1F;

	result = writeMessage();

	if( !result ) {
		return false;
	}

	result = waitForAck();

	if( result == true ) {
#if UART_ENABLED
		printf("Reset configuration\r\n");
#endif
		return true;
	}
	else {
#if UART_ENABLED
		printf("Failed to reset configuration\r\n");
#endif
		return false;
	}

}

enum pvtState_e getPVT()
{
	packet.class = NAV_CLASS;
	packet.id = NAV_PVT;
	packet.payload_len = 0;

	bool result;

	result = writeMessage();

	if( result != true ) {
#if UART_ENABLED
		printf("\r\n");
		printf("***************************\r\n");
		printf("Failed to poll GPS receiver\r\n");
		printf("***************************\r\n");
		printf("\r\n");
#endif
		return POLL_ERROR;
	}

	uint8_t msgLength = 100;
	enum messageState_e message_state = waitForUbxMessage( &packet, msgLength );

	if( message_state != CORRECT_MESSAGE ) {
#if UART_ENABLED
		printf("\r\n");
		printf("**********************\r\n");
		printf("Failed to get PVT data\r\n");
		printf("**********************\r\n");
		printf("\r\n");
#else
		display_digit(2, message_state);
		cyhal_system_delay_ms(1000);
#endif
		return DATA_ERROR;
	}

	// Everything is plus 6
	pvt.year = extractU2FromMsg(packet.payload, 6 + 4);
    pvt.month = packet.payload[6 + 6];
	pvt.day = packet.payload[6 + 7];
    pvt.hour = (packet.payload[6 + 8] - 6) % 24;
    if(pvt.hour < 0) {
    	pvt.hour += 24;
    }
    pvt.min = packet.payload[6 + 9];
    pvt.sec = packet.payload[6 + 10];

    pvt.longitude = (int32_t) extractU4FromMsg(packet.payload, 6 + 24); //degrees
    pvt.latitude = (int32_t) extractU4FromMsg(packet.payload, 6 + 28); //degrees
    pvt.groundSpeed = (int32_t) extractU4FromMsg(packet.payload, 6 + 60); //mm/s

    pvt.validTimeFlag = packet.payload[6 + 11];
    pvt.fixType = packet.payload[6 + 20];
    pvt.fixStatusFlags = packet.payload[6 + 21];
    pvt.numberOfSatellites = packet.payload[6 + 23];

#if UART_ENABLED
    printf("GNSS fix Type: 0 - no fix, 1 - dead reckoning, 2 - 2D, 3 - 3D, 4 - GNSS, 5 - time\r\n");
    printf("GNSS fix: %d\r\n", pvt.fixType);
    printf("GNSS Fix OK: %x\r\n", (pvt.fixStatusFlags & 0x01) );
    printf("Valid Date: %x\r\n", (pvt.validTimeFlag & 0x01) );
    printf("Valid Time: %x\r\n", ((pvt.validTimeFlag & 0x02) >> 1) );
    printf("Fully resolved: %x\r\n", ((pvt.validTimeFlag & 0x04) >> 2) );
    printf("Number of satellites: %d\r\n", pvt.numberOfSatellites);
    printf("Hour: %d, Minute: %d, Second: %d\r\n", pvt.hour, pvt.min, pvt.sec);
    printf("Latitude: %ld\r\n", pvt.latitude);
    printf("Longitude: %ld\r\n", pvt.longitude);
    printf("SPEED: %ld\r\n", pvt.groundSpeed);
#else
	// Write PVT data to the SD card
	if (fr == FR_OK) {
		f_printf(&Fil, "GNSS fix Type: 0 - no fix, 1 - dead reckoning, 2 - 2D, 3 - 3D, 4 - GNSS, 5 - time\r\n");
		f_printf(&Fil, "GNSS fix: %d\r\n", pvt.fixType);
		f_printf(&Fil, "GNSS Fix OK: %x\r\n", (pvt.fixStatusFlags & 0x01) );
		f_printf(&Fil, "Valid Date: %x\r\n", (pvt.validTimeFlag & 0x01) );
		f_printf(&Fil, "Valid Time: %x\r\n", ((pvt.validTimeFlag & 0x02) >> 1) );
		f_printf(&Fil, "Fully resolved: %x\r\n", ((pvt.validTimeFlag & 0x04) >> 2) );
		f_printf(&Fil, "Number of satellites: %d\r\n", pvt.numberOfSatellites);
		f_printf(&Fil, "Hour: %d, Minute: %d, Second: %d\r\n", pvt.hour, pvt.min, pvt.sec);
		f_printf(&Fil, "Latitude: %ld\r\n", pvt.latitude);
		f_printf(&Fil, "Longitude: %ld\r\n", pvt.longitude);
		f_printf(&Fil, "SPEED: %ld\r\n", pvt.groundSpeed);
	}
#endif

	bool gnss_fix_ok = pvt.fixStatusFlags & 0x01;
	bool validTime = ((pvt.validTimeFlag & 0x02) >> 1);

	if( !gnss_fix_ok || !validTime ) {
#if UART_ENABLED
		printf("\r\n");
		printf("***********\r\n");
		printf("No GPS data\r\n");
		printf("***********\r\n");
		printf("\r\n");
#endif

		return NO_FIX;
	}

	return VALID_FIX;
}

bool waitForAck()
{
	uint8_t msgLength = 10; // 8(frame) + 2(payload)
	struct ubxPacket ackResponse;

	ackResponse.class = ACK_CLASS;
	ackResponse.id = ACK_ACK;

	enum messageState_e state = waitForUbxMessage( &ackResponse, msgLength );
	return state == CORRECT_MESSAGE;
}

enum messageState_e waitForUbxMessage(struct ubxPacket * msg, uint8_t msgLength)
{
	volatile uint8_t bytes = getNumBytesAvailable();
	int i = 0;

	while(bytes < msgLength) {

		if(i > 10) {
#if UART_ENABLED
			printf("TIMEOUT: Waiting for message\r\n");
#endif
			return TIMEOUT;
		}
		cyhal_system_delay_ms(100);
		bytes = getNumBytesAvailable();
		i++;
	}

	msg->payload_len = msgLength;

	cy_rslt_t result;
	result = cyhal_i2c_master_read(&i2c_master_obj, GPS_ADDR, msg->payload, msgLength, 0, true);

	if( result != CY_RSLT_SUCCESS ) {
#if UART_ENABLED
		printf("Read message error\r\n");
		uint8_t type = CY_RSLT_GET_TYPE(result);
		uint16_t module_id = CY_RSLT_GET_MODULE(result);
		uint16_t error_code = CY_RSLT_GET_CODE(result);
		printf("type=%x, module=%x, code=%x\n", type, module_id, error_code);
#endif
		return READ_ERROR;
	}

	if( msg->class != msg->payload[2] || msg->id != msg->payload[3] ) {
#if UART_ENABLED
		printf("Did not receive expected message\r\n");

		//printf("Class received: %x\r\n", msg->payload[2]);
		//printf("ID received: %x\r\n", msg->payload[3]);
		//printf("%s\n\r", msg->payload);
#endif
		return WRONG_MESSAGE;
	}

	return CORRECT_MESSAGE;
}
