//SLAVE
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //lcd ekran için kütüphane 
#include <Servo.h>


//tanımlalar
#define buzzerPin 9
#define waterServoPin 8

unsigned long distance=0;// gelen mesafe değerini ltutucak 
float temp=0.0; //gelen sıcaklık değerini tutucak  

volatile byte dataArray[6];
//veriler byte byte geldiğinden array içerisinde tutulucak 
volatile bool isData=false;

LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd ekran için

Servo waterServo;//servo motor için 

//------setup
void setup()
{
  Serial.begin(9600);
  Wire.begin(10); // Slave adresi 10
  Wire.onReceive(masterData); // Gelen veri olunca çağrılacak fonksiyon
  
  lcd.init();
  lcd.backlight();
  waterServo.attach(waterServoPin);
  waterServo.write(0);   // Su sistemi pasif
 
}



//------loop
void loop()
{
  if (isData) {
    isData = false;
	delay(1000);
    // 2 byte int → mesafe
    distance = (dataArray[0] << 8) | dataArray[1]; //ilk olarak h. byte gönderildiği için 8 bit kaydırma işlemi 

    // 4 byte float → sıcaklık
    byte floatBytes[4];
    for (int i = 0; i < 4; i++) {
      floatBytes[i] = dataArray[i + 2];
    }
    memcpy(&temp, floatBytes, 4); // float olarak gelen veriyi dizi üzerinden byte byte çekip birleştirme işlemi 
    // Şimdi floatBytes dizisinde 4 byte’lık sıcaklık bilgisi yer almakta.
	// Ancak bu veri hala byte (ham) formatındadır, yani doğrudan float olarak kullanamayız.

	// "memcpy" (memory copy) fonksiyonu, bir bellek adresinden başka bir bellek adresine
	// belirli sayıda byte'ı doğrudan kopyalamak için kullanılır.

	// Burada amaç: float tipindeki bir değişken olan "temp" içerisine, 
	// 4 byte'lık sıcaklık verisini ham bellekteki haliyle yerleştirmektir.

	// "&temp" → sıcaklık değerinin tutulacağı float değişkenin adresi
	// "floatBytes" → kopyalanacak veri kaynağı (4 byte)
	// "4" → kopyalanacak byte sayısı (float türü 4 byte olduğundan)

	// Bu işlem sonunda "temp" artık gerçek bir float sıcaklık değeri olarak kullanılabilir.
   
    /* debug için 
    delay(1000);
    Serial.print("Mesafe: ");
    Serial.print(distance);
    Serial.print(" cm  -  Sicaklik: ");
    Serial.println(temp);
    */
    buzzerServoFunc();
  	// LCD'ye yazdır
  	lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("D:");
  	lcd.print(distance);
  	lcd.print(" cm");

  	lcd.setCursor(0, 1);
  	lcd.print("T:");
  	lcd.print(temp);
    lcd.print(" C");
  }
}

//------metod

// Master'dan veri geldiğinde çağrılır
void masterData(int dataByte) {
  isData = true;
  for (int i = 0; i < dataByte && i < sizeof(dataArray); i++) {
    dataArray[i] = Wire.read();
  }
}



void buzzerServoFunc()
{
	// Sıcaklık alarmı
  if (temp > 90.0) {
    digitalWrite(buzzerPin, HIGH);
    waterServo.write(90);
  } else {
    digitalWrite(buzzerPin, LOW);
    waterServo.write(0);
  }
}

