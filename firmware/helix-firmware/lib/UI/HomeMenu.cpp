#include "HomeMenu.h"

HomeMenu::HomeMenu(
    Adafruit_ST7789 &display,
    UI &ui)
    : tft(display),
        ui(ui)
{
}

void HomeMenu::draw()
{
    ui.clear();
    ui.drawHeader("HOME");

    for (int i = 0; i < MENU_COUNT; i++)
    {
        ui.drawMenuItem(
            i,
            menuItems[i].title,
            i == selectedItem);
    }
}

HomeAction HomeMenu::handleInput(InputEvent event)
{
    if (event == InputEvent::UP_PRESS)
    {
        int oldItem = selectedItem;

        selectedItem--;

        if (selectedItem < 0)
        {
            selectedItem = MENU_COUNT - 1;
        }

        ui.drawMenuItem(
            oldItem,
            menuItems[oldItem].title,
            false);

        ui.drawMenuItem(
            selectedItem,
            menuItems[selectedItem].title,
            true);

        Serial.print("Selected: ");
        Serial.println(menuItems[selectedItem].title);
    }

    else if (event == InputEvent::DOWN_PRESS)
    {
        int oldItem = selectedItem;
        selectedItem++;

        if (selectedItem >= MENU_COUNT)
        {
            selectedItem = 0;
        }

        ui.drawMenuItem(
            oldItem,
            menuItems[oldItem].title,
            false);
        ui.drawMenuItem(
            selectedItem,
            menuItems[selectedItem].title,
            true);

        Serial.print("Selected: ");
        Serial.println(menuItems[selectedItem].title);
    }
    else if (event == InputEvent::SELECT_HOLD)
    {
        Serial.println("SELECT HOLD");
    }

    else if (event == InputEvent::SELECT_PRESS)
    {
        Serial.print("Opening: ");
        Serial.println(menuItems[selectedItem].title);

        return menuItems[selectedItem].action;
    }
    return HomeAction::NONE;
}