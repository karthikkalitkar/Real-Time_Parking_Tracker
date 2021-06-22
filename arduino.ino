 //Initialized variable to store recieved data
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String str="";
void setup() {
  //Serial Begin at 9600 Baud 
   lcd.begin(16, 2);
  Serial.begin(115200);
}

void loop() {
//  lcd.setCursor(0, 0);
  
  str="";
  while(Serial.available()){
    str += char(Serial.read());
  }
  lcd.clear();
  Serial.println(str);
  lcd.print(str);
  delay(1000);
  
}
