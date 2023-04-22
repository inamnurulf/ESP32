#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



Adafruit_SSD1306 display(128, 32, &Wire, -1);

char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

const char* ssid = "Adhyasta";
const char* password = "juarasatu";
const char* ntpServer = "id.pool.ntp.org";
const long gmtOffset_sec = 7 * 3600; // GMT +7 (WIB)
const int daylightOffset_sec = 0;
WiFiUDP udp;

void setup() {
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
  while (WiFi.status() != WL_CONNECTED) {
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
  while (time(nullptr) < 1630654800) { // Tunggu sampai setidaknya 3 September 2021
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Waktu berhasil disinkronisasi");
}

void loop() {
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
  if (timeInfo.tm_min < 10) {
    display.print("0");
  }
  display.print(timeInfo.tm_min);
  display.print(":");
  if (timeInfo.tm_sec < 10) {
    display.print("0");
  }
  display.print(timeInfo.tm_sec);
  display.setCursor(0, 20);
  display.print("alarm : 00.00.00");
  display.display();

  delay(1000);
}
