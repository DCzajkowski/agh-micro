#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int X_MIDDLE = 560;
const int Y_MIDDLE = 543;
const int TOLERANCE = 300;
const int BUTTON_PRESSED = 0;
const int BUTTON_UNPRESSED = 1;
const int MAX_DURATION = 5999; // 99 minutes and 59 seconds
const int MIN_DURATION = 0;

int duration = 0;
int cursor_position = 0;
int cursor_moved_x = 0;
int cursor_moved_y = 0;
int is_active = 0;

void print_time() {
  lcd.setCursor(0, 0);

  int seconds = duration % 60;
  int minutes = (duration - seconds) / 60;
  
  if (minutes < 10) {
    lcd.print("0");  
  }
  lcd.print(minutes);
  
  lcd.print(":");
  
  if (seconds < 10) {
    lcd.print("0");  
  }
  lcd.print(seconds);

  lcd.setCursor(cursor_position, 0);
}

void print_cursor() {
  lcd.setCursor(cursor_position, 0);
  lcd.cursor();
}

void handle_movement() { 
  int x = analogRead(A0);

  if (cursor_moved_x == 0) {
    if (x > X_MIDDLE + TOLERANCE && cursor_position > 0) {
      cursor_position -= 1;
      cursor_moved_x = 1;

      if (cursor_position == 2) {
        cursor_position -= 1;  
      }
      
      print_cursor();
    }
  
    if (x < X_MIDDLE - TOLERANCE && cursor_position < 4) {
      cursor_position += 1;
      cursor_moved_x = 1;

      if (cursor_position == 2) {
        cursor_position += 1;  
      }
      
      print_cursor();
    }  
  }

  if (cursor_moved_x == 1) {
    if (x < X_MIDDLE + TOLERANCE && x > X_MIDDLE - TOLERANCE) {
      cursor_moved_x = 0;
      print_cursor();
    }
  }
}


/** @param int flag: 1 to add, -1 to subtract */
void change_time(int flag) {
  int newDuration = duration;
  
  switch (cursor_position) {
    case 0:
      newDuration += flag * 600;
      break;
    case 1:
      newDuration += flag * 60;
      break;
    case 3:
      newDuration += flag * 10;
      break;
    case 4:
      newDuration += flag * 1;
      break;
  }

  if (newDuration >= 0 && newDuration <= MAX_DURATION) {
    duration = newDuration;
  }
}

void handle_change() {
  int y = analogRead(A1);

  if (cursor_moved_y == 0) {
    if (y > Y_MIDDLE + TOLERANCE) {
      change_time(-1);
      cursor_moved_y = 1;
      print_time();
    }
  
    if (y < Y_MIDDLE - TOLERANCE) {
      change_time(1);
      cursor_moved_y = 1;
      print_time();
    }  
  }

  if (cursor_moved_y == 1) {
    if (y < Y_MIDDLE + TOLERANCE && y > Y_MIDDLE - TOLERANCE) {
      cursor_moved_y = 0;
      print_time();
    }
  }
}

void activate() {
  if (digitalRead(A2) == BUTTON_PRESSED) {
    is_active = 1; 
    print_time();
    print_cursor();
  }
}

void setup() {
  lcd.begin(8, 2); //Deklaracja typu

  pinMode(A2, INPUT);//ustawiamy pin A3 jako wejście pod przycisk i włączamy podciąganie  
  digitalWrite(A2, HIGH);  
  pinMode(A1, INPUT); // y
  pinMode(A0, INPUT); // x 
}
 
void loop() {
  if (is_active == 1) {
    handle_movement(); 
    handle_change();
  } else {
    activate();
  }
}

