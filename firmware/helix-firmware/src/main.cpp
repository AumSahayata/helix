#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <UI.h>
#include <Input.h>
#include <Music.h>
#include <Stopwatch.h>
#include <Pomodoro.h>

// =========================
// Display
// =========================
constexpr int TFT_MOSI = 11;
constexpr int TFT_SCLK = 12;
constexpr int TFT_CS = 6;
constexpr int TFT_DC = 5;
constexpr int TFT_RST = 4;

SPIClass displaySPI(FSPI);
Adafruit_ST7789 tft(&displaySPI, TFT_CS, TFT_DC, TFT_RST);

UI ui(tft);

// Create app objects
Stopwatch stopwatch(tft, ui);
Pomodoro pomodoro(tft, ui);
Music music(tft, ui);

// =========================
// InputEvents
// =========================
constexpr int BTN_UP = 35;
constexpr int BTN_DOWN = 36;
constexpr int BTN_SELECT = 37;
constexpr int BTN_BACK = 38;

Input input(
    BTN_UP,
    BTN_DOWN,
    BTN_SELECT,
    BTN_BACK);

// =========================
// Screens
// =========================
enum class Screen
{
  HOME,
  APP,
  MUSIC,
  POMODORO,
  STOPWATCH
};

Screen currentScreen = Screen::HOME;

// =========================
// Menu
// =========================
const char *menuItems[] = {
    "Music",
    "Calendar",
    "Pomodoro",
    "Stopwatch",
    "Password Key",
    "Settings"};

constexpr int MENU_COUNT =
    sizeof(menuItems) / sizeof(menuItems[0]);

int selectedItem = 0;

// =========================
// Draw Home Screen
// =========================
void drawMenuItem(int index, bool selected)
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
    tft.print(menuItems[index]);
  }
  else
  {
    tft.setTextColor(Theme::TEXT);
    tft.setCursor(25, y);
    tft.print("  ");
    tft.print(menuItems[index]);
  }
}

void drawMenu()
{
  ui.clear();
  ui.drawHeader("HOME");

  for (int i = 0; i < MENU_COUNT; i++)
  {
    drawMenuItem(i, i == selectedItem);
  }
}

// =========================
// Draw Temporary App
// =========================
void drawApp()
{
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);

  tft.setTextSize(3);
  tft.setCursor(20, 100);
  tft.println(menuItems[selectedItem]);

  tft.setTextSize(2);
  tft.setCursor(20, 160);
  tft.println("Coming soon");

  tft.setTextSize(1);
  tft.setCursor(20, 290);
  tft.println("BACK to return");
}

void goHome()
{
  currentScreen = Screen::HOME;
  drawMenu();
  Serial.println("Returning home");
}

// =========================
// Home Input
// =========================
void handleHomeInput(InputEvent event)
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
    Serial.println(menuItems[selectedItem]);
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
    Serial.println(menuItems[selectedItem]);
  }
  else if (event == InputEvent::SELECT_HOLD)
  {
    Serial.println("SELECT HOLD");
  }

  else if (event == InputEvent::SELECT_PRESS)
  {

    Serial.print("Opening: ");
    Serial.println(menuItems[selectedItem]);

    // Music is menu item 0
    if (selectedItem == 0)
    {
      currentScreen = Screen::MUSIC;

      music.drawScreen();
    }

    // Pomodoro is menu item 2
    else if (selectedItem == 2)
    {

      currentScreen = Screen::POMODORO;

      pomodoro.drawScreen();
    }

    // Stopwatch is menu item 3
    else if (selectedItem == 3)
    {

      currentScreen = Screen::STOPWATCH;

      stopwatch.drawScreen();
    }

    else
    {

      currentScreen = Screen::APP;

      drawApp();
    }
  }
}

// =========================
// App Input
// =========================
void handleAppInput(InputEvent event)
{

  if (event == InputEvent::BACK_PRESS)
  {
    goHome();
  }
}

// =========================
// Setup
// =========================
void setup()
{

  Serial.begin(115200);

  input.begin();

  // Display SPI
  displaySPI.begin(
      TFT_SCLK,
      -1,
      TFT_MOSI,
      TFT_CS);

  // Display
  tft.init(240, 320);
  tft.setRotation(0);

  stopwatch.begin();
  pomodoro.begin();
  music.begin();

  drawMenu();

  Serial.println("HELIX UI ready");
}

// =========================
// Main Loop
// =========================
void loop()
{
  InputEvent event = input.getEvent();

  // Handle input depending on current screen
  if (currentScreen == Screen::HOME)
  {
    handleHomeInput(event);
  }

  else if (currentScreen == Screen::MUSIC)
  {
    music.update();

    music.handleInput(event);

    if (event == InputEvent::BACK_PRESS)
    {
      if (music.isPlaying())
      {
        music.playPause();
      }

      goHome();
    }
  }

  else if (currentScreen == Screen::STOPWATCH)
  {
    // Let the Stopwatch handle its own timing/display.
    stopwatch.update();

    stopwatch.handleInput(event);

    if (event == InputEvent::BACK_PRESS)
    {
      if (stopwatch.isRunning())
      {
        stopwatch.startPause();
      }

      goHome();
    }
  }

  else if (currentScreen == Screen::POMODORO)
  {
    // Pomodoro handles timing, countdown,
    // drawing and Focus/Break transitions.
    pomodoro.update();

    pomodoro.handleInput(event);

    if (event == InputEvent::BACK_PRESS)
    {
      if (pomodoro.isRunning())
        pomodoro.startPause();

      goHome();
    }
  }

  else if (currentScreen == Screen::APP)
  {
    handleAppInput(event);
  }
}
