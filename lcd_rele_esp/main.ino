#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

String texto_serial;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(19,OUTPUT);
  lcd.init();
  Serial.begin(115200);
  digitalWrite(19,1);
}

void loop() {

  if (Serial.available() > 0){
       texto_serial = Serial.readStringUntil('/n');
  
        if (texto_serial == "ligar" || texto_serial == "LIGAR"){
          // Print a message to the LCD.
            lcd.backlight();
            digitalWrite(19,0);
            Serial.println("LCD LIGADO !");
            lcd.setCursor(0,0);
            lcd.print("TESTE, OK !!!");
        }
        if (texto_serial == "desligar" || texto_serial == "DESLIGAR"){
            digitalWrite(19,1);
            Serial.println("LCD DESLIGADO !");
        }

        if (texto_serial!= "desligar" && texto_serial !="ligar" && texto_serial !="DESLIGAR" && texto_serial !="LIGAR" ){
            Serial.println("ENVIE UM COMANDO VALIDO [LIGAR OU DESLIGAR]");
        }
  }

}