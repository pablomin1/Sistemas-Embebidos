//Amaya Montalvo Pablo Iván
//Vargas Guillen José Ricardo

#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 2      // Pin donde está conectado el sensor DHT22
#define DHTTYPE DHT22 // Tipo de sensor

// Crear instancias de las librerías
DHT dht(DHTPIN, DHTTYPE); // Inicializar el sensor DHT
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // Pines del LCD Keypad Shield

void setup() {
  lcd.begin(16, 2);  // Inicializa el LCD de 16 columnas y 2 filas
  lcd.clear();       // Limpiar la pantalla del LCD
  dht.begin();       // Inicializa el sensor DHT
}

void loop() {
  // Lee la humedad y la temperatura en grados Celsius
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Verifica si la lectura del sensor fue exitosa
  if (isnan(h) || isnan(t)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error al leer");
    lcd.setCursor(0, 1);
    lcd.print("el sensor DHT");
    return;
  }

  // Muestra los resultados en el LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(h);
  lcd.print(" %");
  
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" *C");

  delay(2000);  // Espera 2 segundos antes de actualizar las lecturas
}

