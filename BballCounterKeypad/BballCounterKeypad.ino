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
int countH, countG, letter, c1, c2;
boolean Home, Guest, score, scoreSet, oneTwo, Home1,Guest1;
char x;

void setup() {
  Serial.begin(9600);
  countH = 0; countG = 0;
  letter = 14;
  Home = false; Guest = false; Home1 = false; Guest1 = false;
  score = false; scoreSet = false; oneTwo = false;
  c1 = 0; c2 = 0; x = 'x';

  lcd.begin(16,2); //initialize LCD
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  dispScore(); //displays score
  char key = customKeypad.getKey(); //obtains the character of the key pressed
  
  if (key == 'A') { //if A is pushed, go to home mode (2's and 3's)
    Home=true;
    printN(15,0,"*");
  }
  if(key == 'B'){ //if B is pushed, go to guest mode (2's and 3's)
    Guest=true;
    printN(15,1,"*");
  }
  if(key == '0'){ //if 0 is pushed, total reset
    setup();
  }
  if(key == 'C'){ //if C is pushed, set the target score
    lcd.clear();
    lcd.print("Enter score:");
    score = true;
  }
  if(key=='D'){ //if D is pushed, enter one's and two's mode
    lcd.clear();
    lcd.print("One's and Two's");
    delay(3000);
    lcd.clear();
    oneTwo = true;
  }
  while(Home==true){ //home mode (2's and 3's)
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add two 
      if(countH<0){
        printN(8,0," ");
      }
      countH+=2;
      dispScore();
      if(scoreSet==true){ //if final score is inputted, check if home wins
        checkWinH();
      }
    }
    if(key == '3'){ //if 3 is pushed, add three
      if(countH<0){
        printN(8,0," ");
      }
      countH+=3;
      dispScore();
      if(scoreSet==true){ //if final score is inputted, check if home wins
        checkWinH();
      }
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      countH--;
      if (countH == 9){
        printN(8,0," ");
      }
      dispScore();
    }
    if(key == '*'){ //if * is pushed, exit mode
      Home = false; 
      printN(15,0," ");
    }
    if(key == '0'){ //if 0 is pushed, total reset
      reset();
      dispScore();
      Home = false;
    }
    if(key == 'B'){ //change mode to guest (2's and 3's)
      Guest = true;
      printN(15,0," ");
      printN(15,1,"*");
      Home = false;
    }
  }//end home mode (2's and 3's)
  
  while(Guest==true){//guest mode
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add two
      if(countG<0){
        printN(8,1," ");
      }
      countG+=2;
      dispScore();
      if(scoreSet==true){ //if final score is inputted, check if guest wins
        checkWinG();
      }
    }
    if(key == '3'){ //if 3 is pushed, add three
      if(countG<0){
        printN(8,1," ");
      }
      countG+=3;
      dispScore();
      if(scoreSet==true){ //if final score is inputted, check if guest wins
        checkWinG();
      }
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      countG--;
      if(countG == 9) {
        printN(8,1," ");
      }
      dispScore();
    }
    if(key == '*'){ //if * is pushed, exit mode
      printN(15,1," ");
      Guest = false; 
    }
    if(key == '0'){ //if 0 is pushed, total reset
      reset();
      dispScore();
      Guest = false;
    }
    if(key == 'A'){ //change mode to home (2's and 3's)
      Home = true;
      printN(15,1," ");
      printN(15,0,"*");
      Guest = false;
    }
  }//end guest mode (2's and 3's)

  while(score==true){ //enter score mode (2's and 3's)
    char key = customKeypad.getKey();
    if(int(key) != 0 && letter > 12 && key != 'D'){
      scoreSet = true; //only true when custom score inputted
      if(letter == 13){
        printN(letter,0,String(x));
        printN(letter+1,0,String(key));
      } else {
        printN(letter,0,String(key));
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
    if(key == 'D'){ //if D is pushed, exit score mode  
      lcd.clear();
      reset();
      score = false;
    }
  }//end score mode (2's and 3's)
  
  while(oneTwo==true){ //start one's and two's mode
    dispScore(); //displays score
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if (key == 'A') { //if A is pushed, go to home mode (1's and 2's)
      Home1=true;
      printN(15,0,"#");
    }
    if(key == 'B'){ //if B is pushed, go to guest mode (1's and 2's)
      Guest1=true;
      printN(15,1,"#");
    }
    if(key == '0'){ //if 0 is pushed, total reset
      setup();
    }
    if(key == 'C'){ //if C is pushed, set the target score
      lcd.clear();
      lcd.print("Enter score:");
      score = true;
    }
   while(Home1==true){ //home mode (1's and 2's)
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add one 
      if(countH<0){
        printN(8,0," ");
      }
      countH++;
      dispScore();
      if(scoreSet==true){ //if final score inputted, check if home wins
        checkWinH();
      }
    }
    if(key == '3'){ //if 3 is pushed, add two
      if(countH<0){
        printN(8,0," ");
      }
      countH+=2;
      dispScore();
      if(scoreSet==true){ //if final score inputted, check if home wins
        checkWinH();
      }
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      countH--;
      if (countH == 9){
        printN(8,0," ");
      }
      dispScore();
    }
    if(key == '#'){ //if # is pushed, exit mode
      Home1 = false; 
      printN(15,0," ");;
    }
    if(key == '0'){ //if 0 is pushed, total reset
      reset();
      dispScore();
      Home1 = false;
    }
    if(key == 'B'){ //change mode to guest (1's and 2's)
      Guest1 = true;
      printN(15,0," ");
      printN(15,1,"#");
      Home1 = false;
    }
  }//end home mode (1's and 2's)
  
  while(Guest1==true){//guest mode (1's and 2's)
    char key = customKeypad.getKey(); //obtains the character of the key pressed
    if(key == '2'){ //if 2 is pushed, add one
      if(countG<0){
        printN(8,1," ");
      }
      countG++;
      dispScore();
      if(scoreSet==true){ //if final score inputted, check if guest wins
        checkWinG();
      }
    }
    if(key == '3'){ //if 3 is pushed, add two
      if(countG<0){
        printN(8,1," ");
      }
      countG+=2;
      dispScore();
      if(scoreSet==true){ //if final score inputted, check if guest wins
        checkWinG();
      }
    }
    if(key == '1'){ //if 1 is pushed, subtract one
      countG--;
      if(countG == 9) {
        printN(8,1," ");
      }
      dispScore();
    }
    if(key == '#'){ //if # is pushed, exit mode
      printN(15,1," ");;
      Guest1 = false; 
    }
    if(key == '0'){ //if 0 is pushed, total reset
      reset();
      dispScore();
      Guest1 = false;
    }
    if(key == 'A'){ //change mode to home (1's and 2's)
      Home1 = true;
      printN(15,1," ");
      printN(15,0,"#");
      Guest1 = false;
    }
  }//end guest mode (1's and 2's)

  while(score==true){ //enter score mode (1's and 2's)
    char key = customKeypad.getKey();
    if(int(key) != 0 && letter > 12 && key != 'D'){
      scoreSet = true; //only true when custom score inputted
      if(letter == 13){
        printN(letter,0,String(x));
        printN(letter+1,0,String(key));
      } else {
        printN(letter,0,String(key));
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
  }//end score mode (1's and 2's)
  }//end one's and two's mode
}//end void loop

void dispScore(){ //displays scores
  printN(0,0,"Home: ");
  printN(0,1,"Guest: ");
  printN(7,0,String(countH));
  printN(7,1,String(countG));
  printN(6,0," ");
  printN(9,0," ");
  printN(10,0," ");
}

void checkWinH(){
  if(c1 != 0){ //for double digit scores
    if((countH / 10) == c1){ //check first digit equal
        if((countH % 10) >= c2){ //check second digit lower than second digit of countH
          delay(1000);
          lcd.clear();
          lcd.print("Home Wins!");
          printN(0,1,String(countH));
          lcd.print(" - ");
          lcd.print(countG);
        }
    }
  } else {
      if(countH >= c2){ //for single digit scores
        delay(1000);
        lcd.clear();
        lcd.print("Home Wins!");
        printN(0,1,String(countH));
        lcd.print(" - ");
        lcd.print(countG);
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
          printN(0,1,String(countH));
          lcd.print(" - ");
          lcd.print(countG);
        }
    }
  } else {
      if(countG >= c2){ //for single digit scores
        delay(1000);
        lcd.clear();
        lcd.print("Guest Wins!");
        printN(0,1,String(countH));
        lcd.print(" - ");
        lcd.print(countG);
      }
  }
}

void reset(){ //hard reset, removes * and sets both scores to 0 
  countH = 0;
  countG = 0;
  printN(15,0," ");
  printN(15,1," ");
  printN(8,0," ");
  printN(8,1," ");
}

void printN(int x, int y, String n){ //method to make printing easier and more compact
  lcd.setCursor(x,y);
  lcd.print(n);
}
