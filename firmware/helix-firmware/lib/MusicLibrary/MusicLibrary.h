#pragma once

#include <vector>
#include <Arduino.h>
#include <Storage.h>

class MusicLibrary
{
public:
    explicit MusicLibrary(Storage &storage);

    bool load();

    const std::vector<String> &tracks() const;

    bool isLoaded() const;

private:
    Storage &storage;

    std::vector<String> tracks_;

    bool loaded_ = false;
};