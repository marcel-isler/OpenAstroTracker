#include <EEPROM.h>
#include "EPROMStore.hpp"
#include "Utility.hpp"

// The global instance of the platform-independant EEPROM class
EPROMStore *EPROMStore::_eepromStore = NULL;

// Initialize the EEPROM storage in a platform-independent abstraction
void EPROMStore::initialize()
{
  LOGV2(DEBUG_VERBOSE, "EEPROM: Initialize. Instance is %s", _eepromStore == NULL ? "NULL" : "VALID");
  if (_eepromStore == NULL)
  {
    LOGV1(DEBUG_VERBOSE, "EEPROM: Creating single instance");
    _eepromStore = new EPROMStore();
  }
}

// Get the instance of the EEPROM storage
EPROMStore *EPROMStore::Storage()
{
  return _eepromStore;
}

#ifdef ESPBOARD

// Construct the EEPROM object for ESP boards, settign aside 32 bytes for storage
EPROMStore::EPROMStore()
{
  LOGV1(DEBUG_VERBOSE, "EEPROM[ESP]: Startup with 32 bytes");
  EEPROM.begin(32);
}

// Update the given location with the given value
void EPROMStore::update(int location, uint8_t value)
{
  LOGV3(DEBUG_VERBOSE, "EEPROM[ESP]: Writing %x to %d", value, location);
  EEPROM.write(location, value);
  LOGV1(DEBUG_VERBOSE, "EEPROM[ESP]: Committing");
  EEPROM.commit();
}

// Read the value at the given location
uint8_t EPROMStore::read(int location)
{
  uint8_t value;
  value = EEPROM.read(location);
  LOGV3(DEBUG_VERBOSE, "EEPROM[ESP]: Read %x from %d", value, location);
  return value;
}

#else

// Construct the EEPROM  object for non-ESP boards
EPROMStore::EPROMStore()
{
  LOGV1(DEBUG_VERBOSE, "EEPROM[UNO]: Startup ");
}

// Update the given location with the given value
void EPROMStore::update(int location, uint8_t value)
{
  LOGV3(DEBUG_VERBOSE, "EEPROM[UNO]: Writing8 %x to %d", value, location);
  EEPROM.write(location, value);
}

// Read the value at the given location
uint8_t EPROMStore::read(int location)
{
  uint8_t value = EEPROM.read(location);
  LOGV3(DEBUG_VERBOSE, "EEPROM[UNO]: Read8 %x from %d", value, location);
  return value;
}

#endif

void EPROMStore::updateInt16(int loByteAddr, int hiByteAddr, int16_t value)
{
  LOGV4(DEBUG_VERBOSE, "EEPROM: Writing16 %d to %d, %d", value, loByteAddr, hiByteAddr);
  update(loByteAddr, value & 0x00FF);
  update(hiByteAddr, (value >> 8) & 0x00FF);
}

int16_t EPROMStore::readInt16(int loByteAddr, int hiByteAddr)
{
  uint8_t valLo=EPROMStore::Storage()->read(loByteAddr);
  uint8_t valHi=EPROMStore::Storage()->read(hiByteAddr);
  uint16_t uValue = (uint16_t)valLo + (uint16_t)valHi * 256;
  int16_t value = static_cast<int16_t>(uValue);
  LOGV4(DEBUG_VERBOSE, "EEPROM: Read16 %d from %d, %d", value, loByteAddr, hiByteAddr);
  return value;
}
