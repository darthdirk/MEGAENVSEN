#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <dht.h>
dht DHT;
Adafruit_BMP280 bmp; // I2C

#define TFT_DC   8
#define TFT_RST  9
#define TFT_CS   10 
#define TFT_MOSI 11
#define TFT_SCLK 13
#define METER_TO_FEET 3.25084f
#define DHT11_PIN     5

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
float p = 3.1415926;

static void dht_init(void)
{
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }

}

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test\n"));
  unsigned status;
  status = bmp.begin();

  delay(10);
  tft.init(135, 240);
  dht_init();
  
   /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
          
  Serial.println(F("Initialized\n"));

   // large block of text
  tft.fillScreen(ST77XX_BLACK);
  delay(500);

}

void loop() 
{
  float temp_c = bmp.readTemperature();
  float temp_f = (temp_c * 9.0 / 5.0) + 32.0;
  float pressure_Pa = bmp.readPressure();
  float pressure_hPa = pressure_Pa / 100.0;
  float altitude_m = bmp.readAltitude(1013.25);
  float altitude_f = (altitude_m * METER_TO_FEET);
  float humidity = DHT.read11(DHT11_PIN);

tft.fillScreen(ST77XX_BLACK);
delay(100);
tft.setCursor(0, 0);
tft.setTextColor(ST77XX_RED);
tft.setTextSize(2);
tft.println("Temp:");
tft.print(temp_f);
tft.println("*F");
tft.setTextColor(ST77XX_GREEN);
tft.setTextSize(2);
tft.println("Pressure:");
tft.print(pressure_hPa);
tft.println("hPa");
tft.setTextColor(ST77XX_YELLOW);
tft.setTextSize(2);
tft.println("Humidity");
tft.print(DHT.temperature);
tft.println("%");
tft.setTextColor(ST77XX_BLUE);
tft.setTextSize(2);
tft.println("Altitude:");
tft.print(altitude_f);
tft.println("ft");
delay(5000); 
}
