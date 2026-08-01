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
        drawMenuItem(i, i == selectedItem);
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

        // Remove old highlight
        drawMenuItem(oldItem, false);

        // Draw new highlight
        drawMenuItem(selectedItem, true);

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

        drawMenuItem(oldItem, false);
        drawMenuItem(selectedItem, true);

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

void HomeMenu::drawMenuItem(
    int index,
    bool selected)
{

    int y = 75 + (index * 35);

    // Clear only this menu row
    tft.fillRect(
        10,
        y - 7,
        220,
        30,
        Theme::BACKGROUND);

    tft.setTextSize(2);

    if (selected)
    {
        tft.fillRoundRect(
            10,
            y - 7,
            220,
            30,
            5,
            Theme::PRIMARY);

        tft.setTextColor(Theme::BACKGROUND);
        tft.setCursor(25, y);
        tft.print("> ");
        tft.print(menuItems[index].title);
    }
    else
    {
        tft.setTextColor(Theme::TEXT);
        tft.setCursor(25, y);
        tft.print("  ");
        tft.print(menuItems[index].title);
    }
}