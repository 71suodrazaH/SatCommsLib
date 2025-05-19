#include "SatCommsLib.h"
#include <string.h>

SatCommsLib::SatCommsLib() {
  // Constructor (initialization if needed)
}

uint32_t SatCommsLib::calculateCRC(const uint8_t* data, size_t size) {
  return crc32.crc32(data, size);
}

bool SatCommsLib::verifyMessage(const RobustMessage* msg) {
  uint32_t calculatedCRC = calculateCRC(msg->payload, PAYLOAD_SIZE);
  return calculatedCRC == msg->crc;
}

int SatCommsLib::createMessage(RobustMessage* msg, const char* data, uint8_t msgType) {
  int dataLen = strlen(data);
  if (dataLen > PAYLOAD_SIZE) {
    return -1; // Message too long
  }
  
  // Clear the entire struct
  memset(msg, 0, sizeof(RobustMessage));
  
  // Copy message data to payload area
  memcpy(msg->payload, data, dataLen);
  
  // Calculate CRC
  msg->crc = calculateCRC(msg->payload, PAYLOAD_SIZE);
  
  // Set metadata
  msg->length = dataLen;
  msg->header = msgType;
  
  return 0;
}

void SatCommsLib::extractText(const RobustMessage* msg, char* buffer, size_t bufferSize) {
  size_t textLen = min((int)msg->length, (int)bufferSize - 1);
  memcpy(buffer, msg->payload, textLen);
  buffer[textLen] = '\0'; // Ensure null termination
}

void SatCommsLib::printMessageInfo(const RobustMessage* msg, bool verified, const char* prefix) {
  char textBuf[PAYLOAD_SIZE + 1] = {0};
  extractText(msg, textBuf, sizeof(textBuf));
  
  Serial.printf("%s: \"%s\" (length=%d, header=0x%02X)\n", 
                prefix, textBuf, msg->length, msg->header);
  
  if (verified) {
    Serial.println("✓ CRC verified");
  }
}