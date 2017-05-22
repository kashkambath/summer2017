#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = { //define the symbols on the buttons of the keypads
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 2, A4, A5}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal lcd(8,9,10,11,12,13);
int countH, countG; //home and guest score count
boolean Home, Guest;

void setup() {
  Serial.begin(9600);
  countH = 0;
  countG = 0;
  Home = false;
  Guest = false;
  
  lcd.begin(16,2); //initialize LCD
  lcd.print("Home: ");
  lcd.setCursor(0,1);
  lcd.print("Guest: ");
}

void loop() {
  // put your main code here, to run repeatedly:
  dispScore(); //displays score
  char key = customKeypad.getKey(); //obtains the character of the key pressed
  
  if (key == 'A') { //if A is pushed, go to home mode
    Home=true;
    lcd.setCursor(15,0); //print * to indicate at home mode
    lcd.print("*");
  }
  if(key == 'B'){ //if B is pushed, go to guest mode
    Guest=true;
    lcd.setCursor(15,1); //print * to indicate at guest mode
    lcd.print("*");
  }
  if(key == '0'){ //if 0 is pushed, total reset
    reset();
  }
  while(Home==true){ //home mode
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add two 
      add2H();
      dispScore();
    }
    if(key == '3'){ //if 3 is pushed, add three
      add3H();
      dispScore();
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      remH();
      dispScore();
    }
    if(key == '*'){ //if * is pushed, exit mode
      Home = false; 
      resetModeH();
    }
    if(key == '0'){ //if 0 is pushed, total reset
      reset();
      dispScore();
      Home = false;
    }
    if(key == 'B'){ //change mode to guest
      Guest = true;
      resetModeH();
      lcd.setCursor(15,1);
      lcd.print("*");
      Home = false;
    }
  }//end home mode
  
  while(Guest==true){//guest mode
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add two
      add2G();
      dispScore();
    }
    if(key == '3'){ //if 3 is pushed, add three
      add3G();
      dispScore();
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      remG();
      dispScore();
    }
    if(key == '*'){ //if * is pushed, exit mode
      resetModeG();
      Guest = false; 
    }
    if(key == '0'){ //if 0 is pushed, total reset
      reset();
      dispScore();
      Guest = false;
    }
    if(key == 'A'){ //change mode to home
      Home = true;
      resetModeG();
      lcd.setCursor(15,0);
      lcd.print("*");
      Guest = false;
    }
  }//end guest mode
}

void dispScore(){ //displays scores
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

void reset(){ //hard reset, removes * and sets both scores to 0 
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

void resetModeH(){ //removes * in home row
  lcd.setCursor(15,0);
  lcd.print(" ");
}

void resetModeG(){ //removes * in guest row
  lcd.setCursor(15,1);
  lcd.print(" ");
}
