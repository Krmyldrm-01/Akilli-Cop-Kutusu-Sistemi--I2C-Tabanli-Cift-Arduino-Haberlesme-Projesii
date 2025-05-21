//MASTER 
#include <Wire.h>

//-----tanımlamalar 
#define trigPin 9
#define echoPin 10 
#define tempPin A0
#define slaveAdress 10

int distanceGlobal=0;//mesafe
float tempCGlobal=0.0;//sıcaklık

volatile int cnt =0;//timer için sayaç

bool slaveFlag=false;//slave için 
int slaveValue=1;


//-----setup
void setup()
{
  Wire.begin(); // wire içi boş ise master olmaktaydı 
  
  pinMode(trigPin, OUTPUT);  // ses dalgalarının yayılması için
  pinMode(echoPin, INPUT);   // süre hesaplaması için 
  
  
  cli(); //diğer int. kapatıldı                               

  TCCR1A = 0;                       

  TCCR1B = 0;                          
  TCCR1B |= B00000101; // pre değerimiz 1024 olarak ayarlandı                
                                       
                                       
  TIMSK1 |= B00000001; //timer türünü overflow seçtik                 
                                       

  sei(); //kapatılanlar açıldı
  //burada sıcaklık ve mesafe ölçümü için her 20 sn bir yapmasını istedik 
  //timer1 seçildiğinden değerimiz 65536 idi 
  //pre değerimiz 1024 olarak seçtik 
  //kesme süresi =65536* (1/16000000/bölme süresi(pre degğer ) )
  //pre değeri 1024 seçilirse kesme süresi= sn = 4.19
  //biz her 12 sn de bir ölçme yapmasını istiyoruz 
  //bu yüzden sayac değeri sayacı soruda cnt olarak ayarladık 
  //sayac = 12/4.19 = 2.8693 çıkmakta buna direkt olarak 3 diyebiliriz 
  
  
  Serial.begin(9600);
  Serial.println("Ben Master !-)");
}

//-------loop
void loop()
{
  if(cnt>=3)
  {  
  cnt=0;  
  Serial.println("İstenilen zaman dogrultusunda proje baslatiliyor..... ");
  Serial.println("Slave baglantisi basarili ise isleme devam edilecektir  : ");  
  slaveFlag=slaveControl();
    if(slaveFlag==true)
    {   
  		Serial.println("Proje Baslatiliyor------------------------->");
  		distanceGlobal = distanceMeasurement();
  		Serial.print("Olculen Mesafe: ");
  		Serial.print(distanceGlobal);
  		Serial.println(" cm");
  
 	    delay(1000); // bekle reis 
  
 	    tempCGlobal=readTemperature();
  		Serial.print("Olculen Sicaklik Degeri: ");
  		Serial.print(tempCGlobal);
  		Serial.println(" Derece ");
        
      
        Serial.print("Slave cihazına veri gönderimi");
      	Wire.beginTransmission(slaveAdress); // burada veri gönderimi için tekrardan bir iletişim başlatılması lazımdır  	
    
    	Wire.write(highByte(distanceGlobal)); //gönderilen veri h. byte
		Wire.write(lowByte(distanceGlobal));  //gönderilen veri l. byte
      
      	// tempCGlobal (float, 4 byte)
		byte* tempBytes = (byte*) &tempCGlobal; // float'ı byte dizisine eriştir
		// Bu satırda float veri türündeki tempCGlobal değişkeninin bellek adresi,
		// byte tipinde bir pointer'a (işaretçiye) dönüştürülmektedir.

		// Amaç: float değişkenin bellek üzerindeki 4 byte'lık ham temsilini elde etmektir.
		// (byte*) ifadesi bir "type cast"tir — yani veri türünü zorla başka türe çevirme.
		// Bu sayede, float değişkenin her bir byte'ına ayrı ayrı ulaşabiliriz.
        for (int i = 0; i < 4; i++) 
  			Wire.write(tempBytes[i]); // Float'ı 4 byte olarak tek tek gönder
      
   		
        Wire.endTransmission();
    }
    else
       Serial.println("Slave bunulanmadi");  
  
  }
}


//-------metod 
int distanceMeasurement() // mesafe ölçümü için gerekli 
{
  unsigned long elapsedTime=0;  // ses dalgasının çarpıp gelme süresini tutan değişken
  int distanceLocal=0;
  
  // ultrasonik ses dalgaları göndermesi için trig ayarlanıyor 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //echo pininden pulse süresini ölç
  elapsedTime = pulseIn(echoPin, HIGH); 
  
  // sesin havadaki hızı = 343 m/s veya 0.034 cm/us (mikrosaniye)
  distanceLocal = (elapsedTime * 0.034) / 2.0; // çift gidip gelme süresi olduğundan 2 ye bölüm 
  
  return distanceLocal;  
}

//sıcaklık ölçümü
float readTemperature()
{
  int val = analogRead(tempPin); // ADC'den 0-1023 değer arası okuma
  float voltage = val * (5.0 / 1023.0); // ADC değeri voltaja çevrildi
  float tempC = (voltage - 0.5) * 100.0; // TMP36 dönüşüm formülü
  return tempC;
}

//timer metod 
ISR(TIMER1_OVF_vect)
{  
  cnt++;
}

//slave var mı yok mu ?
bool slaveControl()
{
  	
	Wire.beginTransmission(slaveAdress); //slave 10 ile iletişimi başlat 
    slaveValue = Wire.endTransmission(); //dönen sayı 0 ise başarıdır
    if(slaveValue==0)
      return true;
    else 
      return false;
}


