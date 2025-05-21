# ♻️ Akıllı Çöp Kutusu Sistemi - I2C Tabanlı Çift Arduino Haberleşme Projesi

Bu proje, **I2C protokolü** kullanılarak **iki Arduino UNO** arasında haberleşen bir **akıllı çöp kutusu sistemi** geliştirmeyi amaçlamaktadır. Sistem, çöp kutusunun **doluluk oranını** ve **sıcaklık verilerini** sensörler aracılığıyla ölçer, bu verileri merkezi bir noktaya (master Arduino) iletir ve gerektiğinde **yangın önleme mekanizmasını** tetikler.

---

## 🚀 Proje Özeti

- **Slave Arduino**:
  - **HC-SR04** ile çöp kutusunun doluluk oranını ölçer.
  - **TMP36 sıcaklık sensörü** ile ortam sıcaklığını kontrol eder.
  - Ölçtüğü verileri I2C protokolü ile master Arduino'ya gönderir.
  - Belirlenen sıcaklık eşiği aşıldığında **yangın ihtimalini algılar** ve **alarmı** tetikler.
  - Ayrıca bir **servo motor**, su pompasını simüle eder ve yangın senaryosunda aktif hale gelir.

- **Master Arduino**:
  - Slave'den gelen verileri I2C ile alır.
  - Verileri **OLED ekran** üzerinden kullanıcıya sunar.

---

## 🔧 Kullanılan Donanım

- 2 × Arduino UNO  
- 1 × HC-SR04 Ultrasonik Mesafe Sensörü  
- 1 × **TMP36 Sıcaklık Sensörü**  
- 1 × OLED Ekran (I2C destekli)  
- 1 × Servo Motor (Yangın durumunda su pompası görevinde)  
- 1 × Buzzer (Alarm)  
- Jumper kablolar  
- Breadboard  

---

## 🔌 Bağlantı Şeması

### Slave Arduino:
- **HC-SR04**:
  - VCC → 5V  
  - GND → GND  
  - TRIG → D8  
  - ECHO → D9  

- **TMP36 Sıcaklık Sensörü**:
  - VCC → 5V  
  - GND → GND  
  - Vout (Analog çıkış) → A0  

- **Buzzer**:
  - Dijital pin → D10  

- **Servo Motor (su pompası)**:
  - Sinyal → D11  
  - VCC → 5V  
  - GND → GND  

### Master Arduino:
- **OLED Ekran (I2C destekli)**:
  - VCC → 5V  
  - GND → GND  
  - SDA → A4  
  - SCL → A5  

### I2C Haberleşme:
- Master → SDA: A4  
- Master → SCL: A5  
- Slave → SDA: A4  
- Slave → SCL: A5  
- **Ortak GND bağlantısı şarttır.**

---

## 💾 Kod Yapısı

- `slave.ino`  
  - HC-SR04 ile çöpün doluluk oranını ölçer.  
  - **TMP36 ile ortam sıcaklığını ölçer.**  
  - Doluluk ve sıcaklık verilerini I2C üzerinden gönderir.  
  - Eğer sıcaklık belirlenen eşik değerinin üstündeyse:  
    - Buzzer’ı çalıştırır (alarm)  
    - Servo motoru döndürerek yangınla mücadele sistemini (su pompası) devreye sokar.  

- `master.ino`  
  - I2C üzerinden slave’den gelen sıcaklık ve doluluk verilerini okur.  
  - Verileri OLED ekranda gösterir.

---

## 📈 Genişletilebilirlik

- **ESP32 ile Wi-Fi** bağlantısı kurularak verilerin Thingspeak, Firebase gibi platformlara gönderilmesi  
- Güneş paneli entegrasyonu ile dış mekan uygulamaları  
- LORA modülleri ile daha uzak mesafeden haberleşme  
- GSM modülü ile SMS bilgilendirme sistemi  
- Eşik değerlerinin mobil uygulama veya arayüz üzerinden dinamik olarak belirlenmesi

---

## 📷 Proje Görselleri

![micro_project_circuit](https://github.com/user-attachments/assets/e8ce201d-a2eb-40ba-8857-acec3be0a54b)  
![micro_project_](https://github.com/user-attachments/assets/018b7d2f-2ebb-4748-bdda-d1d43906bdd0)

---

## 🧠 Öğrenilenler

- **I2C protokolü** ile çoklu mikrodenetleyici haberleşmesi  
- **Sensör verilerinin işlenmesi** ve karar mekanizması kurulması  
- **Yangın senaryosu gibi kritik durumların** yazılımsal ve donanımsal yönetimi  
- **Gömülü sistemlerde modüler tasarım**  
- **Proje döngüsü yönetimi**, test ve entegrasyon süreçleri

---

## 👤 Geliştirici

**Kerem Yıldırım**  
📚 Kütahya Dumlupınar Üniversitesi – Bilgisayar Mühendisliği  
🔗 [GitHub Profilim](https://github.com/Krmyldrm-01)

---
