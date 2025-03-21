#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2-es LCD, I2C cím: 0x27

byte upArrow[8] = {
  B00100,  //   *
  B01110,  //  ***
  B11111,  // *****
  B00100,  //   *
  B00100,  //   *
  B00100,  //   *
  B00100,  //   *
  B00100   //   *
};

byte downArrow[8] = {
  B00100,  //   *
  B00100,  //   *
  B00100,  //   *
  B00100,  //   *
  B00100,  //   *
  B11111,  // *****
  B01110,  //  ***
  B00100   //   *
};

#define BUTTON_DOWN 3
#define BUTTON_UP 2

const char* menuItems[] = {"1.Fast&Furious", "2.Fast2Furious", "3.TokyoDrift"};
int menuItemCount = 3;
int selectedMenuIndex = 0;
int lastRenderedMenuItem = 0;
int buttonUpState = 0;
int buttonDownState = 0;
bool isStartedAtNow = true;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Starting up...");
  
  Serial.begin(9600);
	delay(700);			
  lcd.clear();
  delay(200);	
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  // Egyéni karakterek betöltése a CGRAM-be
  lcd.createChar(0, upArrow);   // 0-as indexű karakterként betöltjük a felfelé nyilat
  lcd.createChar(1, downArrow); // 1-es indexű karakterként betöltjük a lefelé nyilat
  updateMenuOption(selectedMenuIndex);
}

void loop() {
  buttonUpState = digitalRead(BUTTON_UP);
  buttonDownState = digitalRead(BUTTON_DOWN);
  

  if (buttonUpState == HIGH) {
    if (selectedMenuIndex >= menuItemCount - 1) {
      return;
    } else {
      selectedMenuIndex++;
    }
  } else if (buttonDownState == HIGH) {
    if (selectedMenuIndex <= 0) {
      return;
    } else {
      selectedMenuIndex--;
    }
  }
  delay(250);
  updateMenuOption(selectedMenuIndex);
}

void updateMenuOption(int selectedIndex) {
  if(lastRenderedMenuItem == selectedMenuIndex && !isStartedAtNow) {
    return;
  }
  if(isStartedAtNow) {
    isStartedAtNow=false;
  }
  lcd.clear();
  lcd.print(menuItems[selectedIndex]);
  bool possibleUp = true;
  bool possibleDown = true;
  if (selectedMenuIndex == 0) {
    possibleUp = false;
  }
  if (selectedMenuIndex == menuItemCount - 1) {
    possibleDown = false;
  }
  printArrows(possibleUp, possibleDown);
  lastRenderedMenuItem = selectedIndex;
}

void printArrows(bool possibleUp, bool possibleDown) {
  if (possibleUp) {
    lcd.setCursor(15, 0);
    lcd.write(0);    
  }    
  if (possibleDown) {
    lcd.setCursor(15, 1); 
    lcd.write(1);  
  }
} 
