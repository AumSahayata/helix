#include "Theme.h"

namespace
{
    constexpr Theme::Colors kSlate =
        {
            ST77XX_BLACK,
            0xE73C,
            0x4ACF,
            ST77XX_GREEN,
            ST77XX_YELLOW,
            0x8C71};

    constexpr Theme::Colors kForest =
        {
            ST77XX_BLACK,
            0xE73C,
            0x5B87,
            ST77XX_GREEN,
            ST77XX_YELLOW,
            0x8C50};

    constexpr Theme::Colors kSunset =
        {
            ST77XX_BLACK,
            0xE73C,
            0xA986,
            ST77XX_GREEN,
            ST77XX_YELLOW,
            0x9491};

    constexpr Theme::Colors kMono =
        {
            ST77XX_BLACK,
            0xE73C,
            0xC618,
            ST77XX_GREEN,
            ST77XX_YELLOW,
            0x8410};

    Theme::Type currentTheme = Theme::Type::Sunset;
}

namespace Theme
{
    void set(Type type)
    {
        currentTheme = type;
    }

    Type current()
    {
        return currentTheme;
    }

    const Colors &colors()
    {
        switch (currentTheme)
        {
        case Type::Slate:
            return kSlate;

        case Type::Forest:
            return kForest;

        case Type::Sunset:
            return kSunset;

        case Type::Mono:
            return kMono;
        }

        return kMono;
    }

    const char *name(Type type)
    {
        switch (type)
        {
        case Type::Slate:
            return "Slate";

        case Type::Forest:
            return "Forest";

        case Type::Sunset:
            return "Sunset";

        case Type::Mono:
            return "Mono";
        }

        return "Unknown";
    }
}