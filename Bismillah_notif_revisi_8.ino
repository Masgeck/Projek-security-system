#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Password.h>
#include <Keypad.h>
#include <Servo.h> 
BlynkTimer timer;
/////////////////////////////////////////1////////
Servo servoLamers;
int BUZER = D0; //pin wemos D0 (NOL), GPIO16
Password password = Password( "2912" ); 
/////////////////////////
const byte ROWS = 4; //Jumlah baris pada keypad
const byte COLS = 3; //jumlah colom pada keypad
char keys[ROWS][COLS] = {

  {'#', '0', '*'},
  {'9', '8', '7'},
  {'6', '5', '4'},
  {'3', '2', '1'}
};

byte rowPins[ROWS] = {D4,D3,D2,D1}; //Nomor pin wemos D4,D3,D2,D1 GPIO 2,0,4,5
byte colPins[COLS] = {D7,D6,D5}; //nomor pin wemos D7,D6,D5 GPIO 13,12,14

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
/////////////////////////
// Isi nomer tokendibawah.
// cek nom token di gmail you
char auth[] = "8399e80255d240789c34c05a69c4df80"; //isi tokennya caranya lihat google

// wifi mu
// paswwod mu
char ssid[] = "Lg magna"; //nama wifi
char pass[] = "@Mas_Geck"; // passwordmu
int flag=0;//
//////////////////////
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
    Serial.print("Masukkan Password: ");
    Serial.println(eKey);
    delay(10);
    Serial.write(254);
      
    switch (eKey){
      case '*': Login(); delay(1); break;
      case '#': password.reset(); delay(1); break;
      default: password.append(eKey); delay(1);
    }
  }
}
///////////////////
void Login(){ 
  if (password.evaluate()){ 
    Serial.println("Akses Diterima!");
    servoLamers.write(0);
    delay(1);
    digitalWrite(BUZER, LOW);
  }
  else{
    Serial.println("Akses Ditolak!");
    servoLamers.write(180);
    delay(1);
    digitalWrite(BUZER, HIGH);
   }
}
///////////////////
////
void AWAS()
{
  int AWAS = digitalRead(D0);///Buzzer ke pin WEMOS D0
  if (AWAS==1 && flag==0) {
    Serial.println("Segera WA Group keluarga dan tetangga");
    
    //   We allow 1 notification per 15 seconds for now.
    Blynk.notify("Segera WA Group keluarga dan tetangga");
    flag=1;
  }
  else if (AWAS==0)
  {
    flag=0;
  }
}
//////////////////
void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(16000L,AWAS);
  pinMode(BUZER, OUTPUT);  //Indikator Terbuka dan paswod salah
  servoLamers.attach(D8); //PIN WEMOS D8
  servoLamers.write(180); 
  keypad.addEventListener(keypadEvent);
}
////////////////////
void loop(){
  
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
