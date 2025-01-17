#include <ESP8266WiFi.h>       // Library untuk ESP8266
#include <ESP8266WebServer.h> // Library server web untuk ESP8266

// WiFi credentials
const char* ssid = "samping";        // Ganti dengan nama WiFi Anda
const char* password = "22223333";  // Ganti dengan password WiFi Anda

// Definisi pin untuk LED
const int led1 = 5;  // Pin GPIO untuk LED 1
const int led2 = 4;  // Pin GPIO untuk LED 2
const int led3 = 16; // Pin GPIO untuk LED 3
const int led4 = 2;  // Pin GPIO untuk LED 4

// Membuat instance server web
ESP8266WebServer server(80);

// Fungsi untuk menangani root (halaman utama)
void handleRoot() {
  String html = "<html><head><title>Kendali LED</title><style>";
  html += "body { font-family: Arial, sans-serif; }";
  html += "h1 { color: #4CAF50; }";
  html += "button { padding: 10px 20px; font-size: 16px; margin: 5px; cursor: pointer; border-radius: 5px; border: none; }";
  html += "button.on { background-color: #4CAF50; color: white; }";
  html += "button.off { background-color: #f44336; color: white; }";
  html += "</style></head><body>";
  html += "<h1>Kendali 4 LED</h1>";
  
  // Tombol untuk LED 1
  html += "<p><button class='on' onclick=\"window.location.href='/led1/on'\">LED 1 ON</button>";
  html += "<button class='off' onclick=\"window.location.href='/led1/off'\">LED 1 OFF</button></p>";

  // Tombol untuk LED 2
  html += "<p><button class='on' onclick=\"window.location.href='/led2/on'\">LED 2 ON</button>";
  html += "<button class='off' onclick=\"window.location.href='/led2/off'\">LED 2 OFF</button></p>";

  // Tombol untuk LED 3
  html += "<p><button class='on' onclick=\"window.location.href='/led3/on'\">LED 3 ON</button>";
  html += "<button class='off' onclick=\"window.location.href='/led3/off'\">LED 3 OFF</button></p>";

  // Tombol untuk LED 4
  html += "<p><button class='on' onclick=\"window.location.href='/led4/on'\">LED 4 ON</button>";
  html += "<button class='off' onclick=\"window.location.href='/led4/off'\">LED 4 OFF</button></p>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Fungsi untuk menangani kontrol LED
void controlLED(int pin, String state) {
  if (state == "on") {
    digitalWrite(pin, HIGH);
    Serial.println("LED di pin " + String(pin) + " menyala");
  } else if (state == "off") {
    digitalWrite(pin, LOW);
    Serial.println("LED di pin " + String(pin) + " mati");
  }
  server.send(200, "text/html", "<p>LED " + String(pin) + " turned " + state + "</p><a href=\"/\">Back</a>");
}

void setup() {
  // Inisialisasi serial monitor dengan baud rate 115200
  Serial.begin(115200);
  Serial.println("\nMemulai ESP8266...");

  // Konfigurasi pin sebagai output dan matikan LED saat awal
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);

  // Memulai koneksi WiFi
  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    retryCount++;
    if (retryCount > 30) { // Maksimal 30 detik mencoba
      Serial.println("\nGagal terhubung ke WiFi. Periksa SSID dan password.");
      return;
    }
  }
  
  Serial.println("\nTerhubung ke WiFi!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

  // Rute untuk halaman utama
  server.on("/", handleRoot);

  // Rute untuk kontrol LED
  server.on("/led1/on", []() { controlLED(led1, "on"); });
  server.on("/led1/off", []() { controlLED(led1, "off"); });
  server.on("/led2/on", []() { controlLED(led2, "on"); });
  server.on("/led2/off", []() { controlLED(led2, "off"); });
  server.on("/led3/on", []() { controlLED(led3, "on"); });
  server.on("/led3/off", []() { controlLED(led3, "off"); });
  server.on("/led4/on", []() { controlLED(led4, "on"); });
  server.on("/led4/off", []() { controlLED(led4, "off"); });

  // Memulai server web
  server.begin();
  Serial.println("Server web dimulai");
}

void loop() {
  server.handleClient(); // Menangani permintaan klien
}
