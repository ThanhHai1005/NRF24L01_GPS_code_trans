#include <SPI.h> // thư viện chuẩn SPI
#include <nRF24L01.h>
#include <RF24.h> // Thu viện RF24L01
#include <TinyGPS++.h>
#include <TinyGPS.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
TinyGPSPlus gps;

RF24 radio(8, 9); // CE, CSN

const byte address[][6] = {"12345", "00001"}; // địa chỉ 0, 1

char xyData[32] = "";
String xAxis, yAxis;
float datos[2];

void setup()

{
  Serial.begin(9600);
  radio.begin();

  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(1, address[1]);

  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);

  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("lat & lng");
  delay(1000);
  lcd.clear();
  lcd.print("Starting.....");
  delay(1000);
  lcd.clear();
}
void loop()
{
  radio.stopListening();  // chỉ gửi
  xAxis = analogRead(A1); // đọc giá trị analoag trục x của joystick
  yAxis = analogRead(A0); // đọc giá trị analoag trục y của joystick

  // gửi giá trị analog của trục x
  xAxis.toCharArray(xyData, 5);
  radio.write(&xyData, sizeof(xyData));
  // gửi giá trị analog của trục y
  yAxis.toCharArray(xyData, 5);
  radio.write(&xyData, sizeof(xyData));
  delay(10);

  radio.startListening();
  while (!radio.available())
    ; // bắt đầu nghe

  radio.read(datos, sizeof(datos));

  Serial.print("Latitud: ");
  Serial.print(datos[0]);
  Serial.print(" G° \n");
  Serial.print("Longitud: ");
  Serial.print(datos[1]);
  Serial.print(" G° \n");

  lcd.init();
  lcd.init();

  lcd.setCursor(0, 0);
  lcd.print("Latitud: ");
  lcd.print(datos[0]);
  lcd.print(" G \n");
  lcd.setCursor(0, 1);
  lcd.print("Longitud: ");
  lcd.print(datos[1]);
  lcd.print(" G° \n");

  delay(10);
}
