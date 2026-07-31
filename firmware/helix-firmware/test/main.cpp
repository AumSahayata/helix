#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <Storage.h>
#include <UI.h>
#include <HomeMenu.h>
#include <Input.h>
#include <Music.h>
#include <Stopwatch.h>
#include <Pomodoro.h>
#include <AudioPlayer.h>

// =========================
// Display
// =========================
constexpr int TFT_MOSI = 11;
constexpr int TFT_SCLK = 12;
constexpr int TFT_CS = 6;
constexpr int TFT_DC = 5;
constexpr int TFT_RST = 4;
constexpr int SD_MISO = 13;

SPIClass spi(FSPI);
Storage storage(spi, 10);
Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

UI ui(tft);
HomeMenu homeMenu(tft, ui);

// Create app objects
Stopwatch stopwatch(tft, ui);
Pomodoro pomodoro(tft, ui);
Music music(tft, ui);
AudioPlayer audioPlayer;

// =========================
// InputEvents
// =========================
constexpr int BTN_UP = 1;
constexpr int BTN_DOWN = 2;
constexpr int BTN_SELECT = 15;
constexpr int BTN_BACK = 21;

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
// Draw Temporary App
// =========================
void drawApp()
{
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);

  tft.setTextSize(3);
  tft.setCursor(20, 100);
  tft.println("Coming Soon");

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
  homeMenu.draw();
  Serial.println("Returning home");
}

void openMusic()
{
  currentScreen = Screen::MUSIC;
  music.drawScreen();
}

void openPomodoro()
{
  currentScreen = Screen::POMODORO;
  pomodoro.drawScreen();
}

void openStopwatch()
{
  currentScreen = Screen::STOPWATCH;
  stopwatch.drawScreen();
}

void openPlaceholder()
{
  currentScreen = Screen::APP;
  drawApp();
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

  spi.begin(
      TFT_SCLK,
      SD_MISO,
      TFT_MOSI);

  // Display
  tft.init(240, 320);
  tft.setRotation(0);

  Serial.println("Before SD");
  if (storage.begin())
  {
    Serial.println("SD Card mounted");
  }
  else
  {
    Serial.println("Failed to mount SD Card");
  }

  stopwatch.begin();
  pomodoro.begin();
  music.begin();
  audioPlayer.begin();

  homeMenu.draw();

  Serial.println("HELIX UI ready");

  std::vector<String> files;

  if (storage.listFilesRecursive("/Music", files))
  {
    for (auto &file : files)
    {
      Serial.println(file);
    }
    music.setPlaylist(files);
    Serial.println("Playlist received");
    Serial.println(files.size());
  }
}

// =========================
// Main Loop
// =========================
void loop()
{
  InputEvent event = input.getEvent();

  switch (currentScreen)
  {
  case Screen::HOME:
  {
    HomeAction action = homeMenu.handleInput(event);

    switch (action)
    {
    case HomeAction::OPEN_MUSIC:
      openMusic();
      break;

    case HomeAction::OPEN_POMODORO:
      openPomodoro();
      break;

    case HomeAction::OPEN_STOPWATCH:
      openStopwatch();
      break;

    case HomeAction::OPEN_PLACEHOLDER:
      openPlaceholder();
      break;

    case HomeAction::NONE:
    default:
      break;
    }

    break;
  }

  case Screen::MUSIC:
    music.update();
    music.handleInput(event);

    if (event == InputEvent::BACK_PRESS)
    {
      if (music.isPlaying())
        music.playPause();

      goHome();
    }
    break;

  case Screen::STOPWATCH:
    stopwatch.update();
    stopwatch.handleInput(event);

    if (event == InputEvent::BACK_PRESS)
    {
      if (stopwatch.isRunning())
        stopwatch.startPause();

      goHome();
    }
    break;

  case Screen::POMODORO:
    pomodoro.update();
    pomodoro.handleInput(event);

    if (event == InputEvent::BACK_PRESS)
    {
      if (pomodoro.isRunning())
        pomodoro.startPause();

      goHome();
    }
    break;

  case Screen::APP:
    handleAppInput(event);
    break;
  }
}