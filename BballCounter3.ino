#include <LiquidCrystal.h>
//#define lcd 3 //define lcd to pin 8 
//SoftwareSerial myLCD = SoftwareSerial(255,lcd);
LiquidCrystal lcd(8,9,10,11,12,13);
int countH, countG; //home and guest score count
boolean Home, Guest;


//third counter code with modes for home and guest and add 2 and 3, back button
//make default home and specific button to add to guest (-1 button)
void setup() {
  countH = 0;
  countG = 0;
  Home = false;
  Guest = false;

  pinMode(7,INPUT); //back button
  pinMode(6,INPUT); //home mode button
  pinMode(5,INPUT); //guest mode button
  pinMode(4,INPUT); //add 2 button
  pinMode(3,INPUT); //add 3 button
  pinMode(2,INPUT); //minus button
  pinMode(1,INPUT); //reset button

  lcd.begin(16,2); //initialize LCD
  lcd.print("Home: ");
  lcd.setCursor(0,1);
  lcd.print("Guest: ");
}

void loop() {

  dispScore(); //displays score
  
  if(digitalRead(6)==LOW){ //if home mode button pushed
    Home=true;
    lcd.setCursor(15,0);
    lcd.print("*");
  }
  if(digitalRead(5)==LOW){ //if guest mode button pushed
    Guest=true;
    lcd.setCursor(15,1);
    lcd.print("*");
  }
  if(digitalRead(1) == LOW){ //if reset home button is pushed
    reset();
  }
 
  while(Home==true){ //home mode
    if(digitalRead(4)==LOW){ //if add 2 button is pushed
      add2H();
      delay(250);
      dispScore();
    }
    if(digitalRead(3)==LOW){ //if add 3 button is pushed
      add3H();
      delay(250);
      dispScore();
    }
    if(digitalRead(2)==LOW){ //if minus button is pushed
      remH();
      delay(250);
      dispScore();
    }
    if(digitalRead(7)==LOW){ //if back button is pushed
      Home = false; 
      resetModeH();
    }
    if(digitalRead(1)==LOW){
      reset();
      dispScore();
      Home = false;
    }
  }//end home mode
  
  while(Guest==true){//guest mode
    if(digitalRead(4)==LOW){
      add2G();
      delay(250);
      dispScore();
    }
    if(digitalRead(3)==LOW){
      add3G();
      delay(250);
      dispScore();
    }
    if(digitalRead(2)==LOW){
      remG();
      delay(250);
      dispScore();
    }
    if(digitalRead(7)==LOW){ //if back button is pushed
      Guest = false; 
      resetModeG();
    }
    if(digitalRead(1)==LOW){
      reset();
      dispScore();
      Guest = false;
    }
  }//end guest mode
}

void dispScore(){
  lcd.setCursor(7,0);
  lcd.print(countH);
  lcd.setCursor(7,1);
  lcd.print(countG);
}

void add2H(){
  countH += 2;
}

void add3H(){
  countH += 3;
}

void add2G(){
  countG += 2;
}

void add3G(){
  countG += 3;
}

void remH(){
  countH--;
}

void remG(){
  countG--;
}

void reset(){
  countH = 0;
  countG = 0;
  lcd.setCursor(15,0);
  lcd.print(" ");
  lcd.setCursor(15,1);
  lcd.print(" ");
  lcd.setCursor(8,0);
  lcd.print(" ");
  lcd.setCursor(8,1);
  lcd.print(" ");
}

void resetModeH(){
  lcd.setCursor(15,0);
  lcd.print(" ");
}

void resetModeG(){
  lcd.setCursor(15,1);
  lcd.print(" ");
}

