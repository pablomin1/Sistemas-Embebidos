#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// **Configuración WiFi**
const char* ssid = "UAM-ROBOTICA";
const char* password = "m4nt32024uat";

// **Configuración ThingSpeak**
unsigned long myChannelNumber = 2844413;   
const char* myWriteAPIKey = "WTK2M5W18SBS9LZR";  

// **Pines y configuración del DHT22**
#define DHTPIN 23    
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

// **Pines y configuración del Buzzer**
#define BUZZER_PIN 5   // Pin para el buzzer

// **Inicialización del BMP280**
Adafruit_BMP280 bmp;

// **Inicialización de la pantalla OLED**
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  
#define OLED_RESET    -1  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// **Cliente WiFi para ThingSpeak**
WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  // **Configurar Buzzer**
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  

  // **Inicializar pantalla OLED**
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
    Serial.println("No se encontró la pantalla OLED.");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando...");
  display.display();
  delay(2000);

  // **Conectar WiFi**
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi!");

  // **Inicializar ThingSpeak**
  ThingSpeak.begin(client);

  // **Inicializar DHT22**
  dht.begin();

  // **Inicializar BMP280**
  if (!bmp.begin(0x76)) {  
    Serial.println("No se encontró el sensor BMP280. Verifica las conexiones.");
    while (1);
  }
}

void loop() {
  // **Lectura de sensores**
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  float presion = bmp.readPressure() / 100.0; // Convertir de Pa a hPa

  // **Verificar datos válidos**
  if (isnan(temperatura) || isnan(humedad) || isnan(presion)) {
    Serial.println("Error al leer los datos de los sensores!");
    return;
  }

  // **Mostrar datos en OLED**
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: "); display.print(temperatura); display.println(" C");
  display.print("Humedad: "); display.print(humedad); display.println(" %");
  display.print("Presion: "); display.print(presion); display.println(" hPa");
  display.display();

  // **Activar buzzer si la humedad es >= 90%**
  if (humedad >= 90.0) {
    Serial.println("¡Humedad alta! Activando buzzer.");
    tone(BUZZER_PIN, 1000);  // Sonido a 1000 Hz
    delay(1000);             
    noTone(BUZZER_PIN);      
  }

  // **Enviar datos a ThingSpeak**
  ThingSpeak.setField(1, temperatura);  
  ThingSpeak.setField(2, humedad);      
  ThingSpeak.setField(3, presion);      

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Datos enviados a ThingSpeak correctamente.");
  } else {
    Serial.print("Error al enviar datos a ThingSpeak. Código HTTP: ");
    Serial.println(x);
  }

  delay(4000); // Espera 4 segundos antes de la siguiente lectura
}

