#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 32, &Wire, -1);

char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

const char *ssid = "@Rumah Watik";
const char *password = "123teko100";
const char *ntpServer = "id.pool.ntp.org";
const long gmtOffset_sec = 7 * 3600; // GMT +7 (WIB)
const int daylightOffset_sec = 0;
WiFiUDP udp;

// Define the keypad
#define DELAY 125
#define WAITBLINK 2000
const int ROWS = 4;
const int COLS = 4;
int hourss = 0, minutess = 0, secondss = 0;
bool blink = true;

String keys[ROWS][COLS] = {
    {"1", "2", "3", "hour"},
    {"4", "5", "6", "minute"},
    {"7", "8", "9", "second"},
    {"*", "0", "#", "ENTER"}};

int numberNow = 0;

unsigned long onclickTime = 0;
unsigned long blinkTime = 0;
String currentKey = "";

int rowPins[ROWS] = {13, 12, 14, 27};
int colPins[COLS] = {26, 25, 33, 32};

void setup()
{
  Serial.begin(115200);

  // Inisialisasi OLED

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Menghubungkan ke WiFi...");
  display.display();

  // Hubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Terhubung ke WiFi");

  // Sinkronkan waktu menggunakan NTP
  udp.begin(2390);
  setenv("TZ", "WIB-7", 1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sedang sinkronisasi waktu...");
  while (time(nullptr) < 1630654800)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Waktu berhasil disinkronisasi");
}

void loop()
{
  for (int col = 0; col < COLS; col++)
  {
    pinMode(colPins[col], OUTPUT);
    digitalWrite(colPins[col], LOW);

    for (int row = 0; row < ROWS; row++)
    {
      pinMode(rowPins[row], INPUT_PULLUP);
      if ((millis() - onclickTime) > DELAY)
      {
        if (digitalRead(rowPins[row]) == LOW)
        {
          currentKey = keys[row][col];
          Serial.print(currentKey);
          onclickTime = millis();
          if (col == 3)
          {
            numberNow = row;
          }
        }
      }
    }

    for (int row = 0; row < ROWS; row++)
    {
      pinMode(rowPins[row], INPUT_PULLUP);
      digitalWrite(rowPins[row], HIGH);
    }

    // Reset the current column to INPUT
    pinMode(colPins[col], INPUT_PULLUP);
  }

  // Dapatkan waktu sekarang
  time_t now = time(nullptr);
  tm timeInfo;
  localtime_r(&now, &timeInfo);

  // Tampilkan waktu di OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(daysOfTheWeek[timeInfo.tm_wday]);
  display.print(", ");
  display.print(timeInfo.tm_mday);
  display.print("/");
  display.print(timeInfo.tm_mon + 1);
  display.print("/");
  display.print(timeInfo.tm_year + 1900);
  display.setCursor(0, 10);
  display.print(timeInfo.tm_hour);
  display.print(":");
  if (timeInfo.tm_min < 10)
  {
    display.print("0");
  }
  display.print(timeInfo.tm_min);
  display.print(":");
  if (timeInfo.tm_sec < 10)
  {
    display.print("0");
  }
  display.print(timeInfo.tm_sec);
  display.setCursor(0, 20);
  display.print("alarm : ");
  if (numberNow == 0)
  {
    if ((millis() - blinkTime) > WAITBLINK)
    {
      if (blink == true)
      {
        if (hourss < 10)
        {
          display.print("0");
        }
        display.print(hourss);
      }
      blink != blink;
      blinkTime = millis();
    }
    }
    else
    {
      if (hourss < 10)
      {
        display.print("0");
      }
      display.print(hourss);
  }

  display.print(":");
  if (numberNow == 1)
  {
    if ((millis() - blinkTime) > WAITBLINK)
    {
      if (blink == true)
      {
        if (minutess < 10)
        {
          display.print("0");
        }
        display.print(minutess);
      }
      blink != blink;
      blinkTime = millis();
    }
    }
    else
    {
      if (minutess < 10)
      {
        display.print("0");
      }
      display.print(minutess);
  }

  display.print(":");
  if (numberNow == 2)
  {
    if ((millis() - blinkTime) > WAITBLINK)
    {
      if (blink == true)
      {
        if (secondss < 10)
        {
          display.print("0");
        }
        display.print(secondss);
      }

      blink != blink;
      blinkTime = millis();
    }
    }
    else
    {
      if (secondss < 10)
      {
        display.print("0");
      }
      display.print(secondss);
  }

  display.display();

  delay(1000);
}
