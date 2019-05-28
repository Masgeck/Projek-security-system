//////////////////////////////////////1////////////////////
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Password.h>
#include <Keypad.h>
#include <Servo.h> 
BlynkTimer timer;
/////////////////////////////////////////2//////////////////////////////////////
Servo LOKER;// pemanggilan nama servo atau pemberia nama pada servo
int LED = D0; //pin wemos D0 (NOL), GPIO16, pemanggilan data LED
Password password = Password( "123" ); 
/////////////////////////
const byte ROWS = 4; //Jumlah baris pada keypad
const byte COLS = 3; //jumlah colom pada keypad
char keys[ROWS][COLS] = {

    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[ROWS] = {D4,D3,D2,D1}; //Nomor pin wemos D4,D3,D2,D1 GPIO 2,0,4,5
byte colPins[COLS] = {D8,D6,D5}; //nomor pin wemos D7,D6,D5 GPIO 13,12,14

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/////////////////////////////////3/////////////////////////

// Isi nomer tokendibawah.
// cek nom token di gmail you
char auth[] = "22fd64eb0bcb49c5910b7f8cbde5280e"; //isi tokennya, token didapat ketika membuat projek baru di bylink, caranya lihat google

// wifi mu
// paswwod mu
char ssid[] = "OPPO F9"; //nama wifi atau bisa juga hotspot teman kamu
char pass[] = "katasandi"; // passwordmu
int flag=0;// buat pemanggilan data notifikasi, 0 artinya salah, low, mati, biasa disbeut BOLEAN.

/////////////////////////////////4////////////////////////////

void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
    Serial.print("Masukkan Password: ");
    Serial.println(eKey);
    delay(10);
    Serial.write(254);
      
    switch (eKey){
      case '*': Ok(); delay(1); break;// untuk tombol OK.
      case '#': password.reset(); delay(1); break;// sebagai tombol reset paswoord
      default: password.append(eKey); delay(1);
    }
  }
}

//////////////////////////5///////////////////////////////

void Ok(){ 
  if (password.evaluate()){ //pasword meng screnning
    Serial.println("Akses Diterima!");
    LOKER.write(90);// posisi servo ketika memaskan paswoord yang benar yaitu 90 derajat
    delay(1);
    digitalWrite(LED, HIGH);// ketika paswod benar,LED hidup.
  }
  else{
    Serial.println("Akses Ditolak!");
    LOKER.write(0);// posisi servo ketika memaskan paswoord yang sala,0 derajat
    delay(1);
    digitalWrite(LED, LOW);//ketika paswod salah,LED mati
   }
}
///////////////////////6//////////////////////////////
void NotifLokerBuka()// 
{
  int NotifLokerBuka = digitalRead(D0);///Pin D0 ialah pin LED, Memakai logika pada LED untuk notifikasi.
  if (NotifLokerBuka==1 && flag==0) 
  {
    Serial.println("Ada yang masuk");
    
    //   1 notifikasi dengan jeda 15 detik
    Blynk.notify("Ada yang masuk");
    flag=0;
  }
  else if (NotifLokerBuka==0)
  {
    flag=0;
  }
}
///////////////////////7///////////////////////////
void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(16000L,NotifLokerBuka);//16000L= 15 DETIKAN
  pinMode(LED, OUTPUT);  //Indikator Terbuka dan paswod salah
  LOKER.attach(D7); //PIN WEMOS D7
  LOKER.write(0); // posisi 0 derajat ketika awal kali upload
  keypad.addEventListener(keypadEvent);
  
  //// Untuk pembacaan konektifitas wifi pada serial monitor
  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to...");
  //Serial.println(ssid);

  //WiFi.begin(ssid, pass);

  //while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("Wi-Fi connected successfully");
}

////////////////////////////////8/////////////////////////////
void loop(){
char key = keypad.getKey();
  Blynk.run();//
  timer.run(); // 
}
