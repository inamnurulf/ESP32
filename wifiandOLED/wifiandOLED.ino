#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 32, &Wire, -1);

char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

const char *ssid = "Dhimz_Net";
const char *password = "halahsaksakee";
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
bool keepBlinking = false;

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

// led pin
int ledPin = 2;

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
  pinMode(ledPin, OUTPUT);
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
            if (row == 3)
            {
              keepBlinking = false;
            }
          }
          if (currentKey == "hour")
          {
            hourss += 1;
            if (hourss > 23)
              hourss = 0; // Jika melebihi 23, kembali ke 0
          }
          else if (currentKey == "minute")
          {
            minutess += 1;
            if (minutess > 59)
              minutess = 0; // Jika melebihi 59, kembali ke 0
          }
          else if (currentKey == "second")
          {
            secondss += 1;
            if (secondss > 59)
              secondss = 0; // Jika melebihi 59, kembali ke 0
          }
          if (currentKey == "1")
          {
            hourss += 10;
            if (hourss > 23)
              hourss = 0; // Jika melebihi 23, kembali ke 0
          }
          else if (currentKey == "4")
          {
            minutess += 10;
            if (minutess > 59)
              minutess = 0; // Jika melebihi 59, kembali ke 0
          }
          else if (currentKey == "7")
          {
            secondss += 10;
            if (secondss > 59)
              secondss = 0; // Jika melebihi 59, kembali ke 0
          }
          if (currentKey == "2")
          {
            hourss += 5;
            if (hourss > 23)
              hourss = 0; // Jika melebihi 23, kembali ke 0
          }
          else if (currentKey == "5")
          {
            minutess += 5;
            if (minutess > 59)
              minutess = 0; // Jika melebihi 59, kembali ke 0
          }
          else if (currentKey == "8")
          {
            secondss += 5;
            if (secondss > 59)
              secondss = 0; // Jika melebihi 59, kembali ke 0
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
  if (keepBlinking == true)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("ALARM HIT");
    display.display();
    delay(500);
    digitalWrite(ledPin, HIGH);

    // Wait for 1 second
    delay(500);

    // Turn the LED off
    digitalWrite(ledPin, LOW);
  }
  else
  {

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

    if (hourss < 10)
    {
      display.print("0");
    }
    display.print(hourss);
    display.print(":");

    if (minutess < 10)
    {
      display.print("0");
    }
    display.print(minutess);
    display.print(":");

    if (secondss < 10)
    {
      display.print("0");
    }
    display.print(secondss);
    display.display();
    delay(1000);
  }

  if (timeInfo.tm_hour == hourss && timeInfo.tm_min == minutess && timeInfo.tm_sec == secondss)
  {
    keepBlinking = true;
  }
}
