#include <LiquidCrystalM.h>
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
LiquidCrystalM lcd(8,9,10,11,12,13);
int countH, countG, letter, c1, c2;
boolean Home, Guest, score, scoreSet;
char x;

void setup() {
  Serial.begin(9600);
  countH = 0;
  countG = 0;
  letter = 14;
  Home = false;
  Guest = false;
  score = false;
  scoreSet = false;
  c1 = 0; 
  c2 = 0;
  x = 'x';
  
  lcd.begin(16,2); //initialize LCD
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  dispScore(); //displays score
  char key = customKeypad.getKey(); //obtains the character of the key pressed
  
  if (key == 'A') { //if A is pushed, go to home mode
    Home=true;
//    lcd.setCursor(15,0); //print * to indicate at home mode
//    lcd.print("*");
    lcd.printM(15,0,"*");
  }
  if(key == 'B'){ //if B is pushed, go to guest mode
    Guest=true;
    lcd.setCursor(15,1); //print * to indicate at guest mode
    lcd.print("*");
  }
  if(key == '0'){ //if 0 is pushed, total reset
    reset();
    setup();
    loop();
  }
  while(Home==true){ //home mode
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add two 
      add2H();
      dispScore();
      if(scoreSet==true){
        checkWinH();
      }
    }
    if(key == '3'){ //if 3 is pushed, add three
      add3H();
      dispScore();
      if(scoreSet==true){
        checkWinH();
      }
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      remH();
      if (countH == 9){
        lcd.setCursor(8,0);
        lcd.print(" ");
      }
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
      if(scoreSet==true){
        checkWinG();
      }
    }
    if(key == '3'){ //if 3 is pushed, add three
      add3G();
      dispScore();
      if(scoreSet==true){
        checkWinG();
      }
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      remG();
      if(countG == 9) {
        lcd.setCursor(8,1);
        lcd.print(" ");
      }
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
  
  if(key == 'C'){ //if C is pushed, set the target score
    lcd.clear();
    lcd.print("Enter score:");
    score = true;
  }
  while(score==true){ //enter score mode
    char key = customKeypad.getKey();
    if(int(key) != 0 && letter > 12 && key != 'D'){
      scoreSet = true; //only true when custom score inputted
      if(letter == 13){
        lcd.setCursor(letter,0);
        lcd.print(x);
        lcd.setCursor(letter+1,0);
        lcd.print(key);
      } else {
        lcd.setCursor(letter,0);
        lcd.print(key);
      }
      if(letter==13){
        c1 = c2;
        c2 = key - '0';
      }
      if(letter==14){
        c2 = key - '0';
        x = key;
      }
      letter--;
    }
    if(key == 'D'){ //if D is pushed, go back to main menu
      lcd.clear();
      reset();
      score = false;
    }
  }//end score mode
}//end void loop

void dispScore(){ //displays scores
  lcd.setCursor(0,0);
  lcd.print("Home: ");
  lcd.setCursor(0,1);
  lcd.print("Guest: ");
  lcd.setCursor(7,0);
  lcd.print(countH);
  lcd.setCursor(7,1);
  lcd.print(countG);
}

void checkWinH(){
  if(c1 != 0){ //for double digit scores
    if((countH / 10) == c1){ //check first digit equal
        if((countH % 10) >= c2){ //check second digit lower than second digit of countH
          delay(1000);
          lcd.clear();
          lcd.print("Home Wins!");
          lcd.setCursor(0,1);
          lcd.print(countH);
          lcd.print(" - ");
          lcd.print(countG);
          char key = customKeypad.getKey();
          if(key == '0'){
            lcd.clear();
            reset();
            setup();
            loop();
          }
        }
    }
  } else {
      if(countH >= c2){ //for single digit scores
        delay(1000);
        lcd.clear();
        lcd.print("Home Wins!");
        lcd.setCursor(0,1);
        lcd.print(countH);
        lcd.print(" - ");
        lcd.print(countG);
        char key = customKeypad.getKey();
        if(key == '0'){
          reset();
          setup();
          loop();
        }
      }
  }
}

void checkWinG(){
  if(c1 != 0){ //for double digit scores
    if((countG / 10) == c1){ //check first digit equal
        if((countG % 10) >= c2){ //check second digit of countG greater than c2 
          delay(1000);
          lcd.clear();
          lcd.print("Guest Wins!");
          lcd.setCursor(0,1);
          lcd.print(countH);
          lcd.print(" - ");
          lcd.print(countG);
          char key = customKeypad.getKey();
          if(key == '0'){
            lcd.clear();
            reset();
            setup();
            loop();
          }
        }
    }
  } else {
      if(countG >= c2){ //for single digit scores
        delay(1000);
        lcd.clear();
        lcd.print("Guest Wins!");
        lcd.setCursor(0,1);
        lcd.print(countH);
        lcd.print(" - ");
        lcd.print(countG);
        char key = customKeypad.getKey();
        if(key == '0'){
          lcd.clear();
          reset();
          setup();
          loop();
        }
      }
  }
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
