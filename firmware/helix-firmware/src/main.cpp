#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <Startup.h>
#include <Storage.h>
#include <UI.h>
#include <HomeMenu.h>
#include <Input.h>
#include <Music.h>
#include <MusicLibrary.h>
#include <Stopwatch.h>
#include <Pomodoro.h>
#include <AudioPlayer.h>
#include <Settings.h>
#include <SettingsApp.h>
#include <Logo.h>

// =========================
// Display
// =========================
constexpr int TFT_MOSI = 11;
constexpr int TFT_SCLK = 12;
constexpr int TFT_CS = 6;
constexpr int TFT_DC = 5;
constexpr int TFT_RST = 4;
constexpr int SD_MISO = 13;

constexpr uint8_t STARTUP_TASKS = 4;

SPIClass spi(FSPI);
Storage storage(spi, 10);
Settings settings;
Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

Startup startup(tft);
UI ui(tft);
HomeMenu homeMenu(tft, ui);

// Create app objects
Stopwatch stopwatch(tft, ui);
Pomodoro pomodoro(tft, ui);
AudioPlayer audioPlayer;
MusicLibrary musicLibrary(storage);
Music music(tft, ui, audioPlayer, musicLibrary);
SettingsApp settingsApp(tft, ui, settings, audioPlayer);

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

App *currentApp = nullptr;

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
  currentApp = nullptr;
  homeMenu.draw();

  Serial.println("Returning home");
}

void openApp(App &app)
{
  currentApp = &app;
  currentApp->onEnter();
  currentApp->draw();
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

  // Display
  tft.init(240, 320);
  tft.setRotation(0);

  startup.begin(STARTUP_TASKS);

  input.begin();

  spi.begin(
      TFT_SCLK,
      SD_MISO,
      TFT_MOSI);

  startup.startTask("Storage");
  storage.begin();
  startup.finishTask();
  startup.tick();

  startup.startTask("Settings");
  settings.load();
  Theme::set(settings.getTheme());
  startup.finishTask();
  startup.tick();

  startup.startTask("Audio");
  audioPlayer.begin();
  audioPlayer.setVolume(settings.getVolume());
  startup.finishTask();
  startup.tick();

  startup.startTask("Applications");
  stopwatch.begin();
  pomodoro.begin();
  music.begin();
  settingsApp.begin();
  randomSeed(micros());

  startup.finishTask();
  startup.tick();

  startup.complete();
  while (!startup.isFinished())
  {
    startup.tick();
    delay(1);
  }

  homeMenu.draw();

  Serial.println("HELIX UI ready");
}

// =========================
// Main Loop
// =========================
void loop()
{
  InputEvent event = input.getEvent();

  audioPlayer.update();

  if (currentApp == nullptr)
  {
    HomeAction action = homeMenu.handleInput(event);

    switch (action)
    {
    case HomeAction::OPEN_MUSIC:
      openApp(music);
      break;

    case HomeAction::OPEN_POMODORO:
      openApp(pomodoro);
      break;

    case HomeAction::OPEN_STOPWATCH:
      openApp(stopwatch);
      break;

    case HomeAction::OPEN_PLACEHOLDER:
      openApp(settingsApp);
      break;

    case HomeAction::NONE:
    default:
      break;
    }

    return;
  }

  currentApp->update();
  bool handled = currentApp->handleInput(event);

  if (!handled && event == InputEvent::BACK_PRESS)
  {
    // Temporary app-specific cleanup

    if (currentApp == &music)
    {
      if (music.isPlaying())
      {
        music.playPause();
      }
    }

    if (currentApp == &stopwatch)
    {
      if (stopwatch.isRunning())
      {
        stopwatch.startPause();
      }
    }

    if (currentApp == &pomodoro)
    {
      if (pomodoro.isRunning())
      {
        pomodoro.startPause();
      }
    }

    goHome();
  }
}