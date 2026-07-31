#pragma once

#include <vector>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

class Storage
{
public:
    Storage(SPIClass &spi, uint8_t csPin);

    bool begin();

    bool isMounted() const;

    bool listFiles(
        const char *path,
        std::vector<String> &files);

    bool listFilesRecursive(
        const char *path,
        std::vector<String> &files);

private:
    SPIClass &spi;
    uint8_t csPin;

    bool mounted = false;
};