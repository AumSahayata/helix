#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <Input.h>
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

// Create app objects
Stopwatch stopwatch(tft);
Pomodoro pomodoro(tft);

// =========================
// Buttons
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
void drawMenu()
{
  tft.fillScreen(ST77XX_BLACK);

  // Header
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(15, 15);
  tft.print("HELIX");

  // Separator
  tft.drawFastHLine(10, 52, 220, ST77XX_WHITE);

  // Menu
  tft.setTextSize(2);

  for (int i = 0; i < MENU_COUNT; i++)
  {

    int y = 75 + (i * 35);

    if (i == selectedItem)
    {

      // Selected item background
      tft.fillRoundRect(
          10,
          y - 7,
          220,
          30,
          5,
          ST77XX_WHITE);

      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(25, y);
      tft.print("> ");
      tft.print(menuItems[i]);
    }
    else
    {

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(25, y);
      tft.print("  ");
      tft.print(menuItems[i]);
    }
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

// =========================
// Home Input
// =========================
void handleHomeInput(Button button)
{

  if (button == Button::UP)
  {

    selectedItem--;

    if (selectedItem < 0)
    {
      selectedItem = MENU_COUNT - 1;
    }

    drawMenu();

    Serial.print("Selected: ");
    Serial.println(menuItems[selectedItem]);
  }

  else if (button == Button::DOWN)
  {

    selectedItem++;

    if (selectedItem >= MENU_COUNT)
    {
      selectedItem = 0;
    }

    drawMenu();

    Serial.print("Selected: ");
    Serial.println(menuItems[selectedItem]);
  }

  else if (button == Button::SELECT)
  {

    Serial.print("Opening: ");
    Serial.println(menuItems[selectedItem]);

    // Pomodoro is menu item 2
    if (selectedItem == 2)
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
void handleAppInput(Button button)
{

  if (button == Button::BACK)
  {

    currentScreen = Screen::HOME;

    Serial.println("Returning home");

    drawMenu();
  }
}

// =========================
// Stopwatch Input
// =========================
void handleStopwatchInput(Button button)
{
  // START / PAUSE
  if (button == Button::SELECT)
  {
    stopwatch.startPause();
  }

  // RESET
  else if (button == Button::UP)
  {
    stopwatch.reset();
  }

  // BACK
  else if (button == Button::BACK)
  {
    // Pause before leaving if currently running.
    if (stopwatch.isRunning())
    {
      stopwatch.startPause();
    }

    currentScreen = Screen::HOME;

    drawMenu();

    Serial.println("Returning home");
  }
}

// =========================
// Pomodoro Input
// =========================
void handlePomodoroInput(Button button)
{
  // START / PAUSE
  if (button == Button::SELECT)
  {
    pomodoro.startPause();
  }

  // RESET
  else if (button == Button::UP)
  {
    pomodoro.reset();
  }

  // BACK
  else if (button == Button::BACK)
  {
    // Pause before leaving.
    if (pomodoro.isRunning())
    {
      pomodoro.startPause();
    }

    currentScreen = Screen::HOME;

    drawMenu();

    Serial.println("Returning home");
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

  drawMenu();

  Serial.println("HELIX UI ready");
}

// =========================
// Main Loop
// =========================
void loop()
{
  Button button = input.getButton();

  // Handle input depending on current screen
  if (currentScreen == Screen::HOME)
  {
    handleHomeInput(button);
  }

  else if (currentScreen == Screen::STOPWATCH)
  {
    // Let the Stopwatch handle its own timing/display.
    stopwatch.update();

    handleStopwatchInput(button);
  }

  else if (currentScreen == Screen::POMODORO)
  {
    // Pomodoro handles timing, countdown,
    // drawing and Focus/Break transitions.
    pomodoro.update();

    handlePomodoroInput(button);
  }

  else if (currentScreen == Screen::APP)
  {
    handleAppInput(button);
  }
}