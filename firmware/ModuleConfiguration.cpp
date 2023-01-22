#include "ModuleConfiguration.h"

ModuleConfiguration::ModuleConfiguration(unsigned char* (*initialiser)(int&, unsigned int), bool (*validator)(unsigned int, unsigned char), unsigned int eepromAddress) {
  this->initialiser = initialiser;
  this->validator = validator;
  this->eepromAddress = eepromAddress;
}

void ModuleConfiguration::setup() {
  int size;

  this->initialiser(size, this->eepromAddress);
  this->size = size;
}

bool ModuleConfiguration::setByte(unsigned int index, unsigned char value) {
  if ((index < this->size) && (this->validator(index, value))) {
    this->configuration[index] = value;
    this->saveByte(index);
    return(true);
  }
  return(false);
}

unsigned char ModuleConfiguration::getByte(unsigned int index) {
  if (index < this->size) {
    return(this->configuration[index]);
  }
  return(0xff);
}

bool ModuleConfiguration::validateAddress(unsigned char index) {
  return(index < this->size);
}

bool ModuleConfiguration::processValue(unsigned char index, unsigned char value) {
  return(this->setByte(index, value));
}

void ModuleConfiguration::saveByte(unsigned int index) {
  EEPROM.update(this->eepromAddress + index, this->configuration[index]);
}

void ModuleConfiguration::save() {
  EEPROM.put(this->eepromAddress, this->configuration);
}

void ModuleConfiguration::load() {
  EEPROM.get(this->eepromAddress, this->configuration);
}

void ModuleConfiguration::erase() {
  for (unsigned int i = 0; i < this->size; i++) {
    EEPROM.update((this->eepromAddress + i), 0xff);
  }
}
