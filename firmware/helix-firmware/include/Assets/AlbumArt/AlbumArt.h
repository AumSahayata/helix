#pragma once

#include "blue.h"
#include "cyber.h"
#include "white.h"
#include "green.h"
#include "red.h"
#include "orange.h"
#include "purple.h"

namespace Assets
{
    constexpr uint8_t kAlbumArtCount = 7;

    const uint16_t* const albumArt[kAlbumArtCount] =
    {
        blue,
        cyber,
        white,
        green,
        red,
        orange,
        purple
    };
}