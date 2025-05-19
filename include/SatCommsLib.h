#ifndef SAT_COMMS_LIB_H
#define SAT_COMMS_LIB_H

#include <Arduino.h>
#include <FastCRC.h>

// Message types
#define MSG_TYPE_DATA    0x01   // Regular data message
#define MSG_TYPE_ACK     0x02   // Acknowledgment 
#define MSG_TYPE_NACK    0x03   // Negative acknowledgment

// Common payload size
#define PAYLOAD_SIZE 60

// Unified message structure for all protocols
struct RobustMessage {
  uint8_t payload[PAYLOAD_SIZE]; // 60 bytes for payload data
  uint32_t crc;                  // 4 bytes for CRC32
  uint8_t length;                // 1 byte for message length
  uint8_t header;                // 1 byte for message type/header
};

class SatCommsLib {
private:
  FastCRC32 crc32;

public:
  SatCommsLib();
  
  // Core CRC functionality
  uint32_t calculateCRC(const uint8_t* data, size_t size);
  bool verifyMessage(const RobustMessage* msg);
  
  // Message creation (works for any protocol)
  int createMessage(RobustMessage* msg, const char* data, uint8_t msgType = MSG_TYPE_DATA);
  
  // Helper functions
  void extractText(const RobustMessage* msg, char* buffer, size_t bufferSize);
  void printMessageInfo(const RobustMessage* msg, bool verified = false, const char* prefix = "MSG");
};

#endif // SAT_COMMS_LIB_H