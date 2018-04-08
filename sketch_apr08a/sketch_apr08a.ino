#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int X_MIDDLE = 560;
const int Y_MIDDLE = 543;
const int TOLERANCE = 200;
const int BUTTON_PRESSED = 0;
const int BUTTON_UNPRESSED = 1;

int m0 = 0;
int m1 = 0;
int s0 = 0;
int s1 = 0;
int cursor_position = 0;
int cursor_moved_x = 0;
int cursor_moved_y = 0;
int is_active = 0;

void print_time() {
  lcd.setCursor(0, 0);
  
  lcd.print(m0);
  lcd.print(m1);
  lcd.print(":");
  lcd.print(s0);
  lcd.print(s1); 

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

int get_current_digit() {
  switch (cursor_position) {
    case 0:
      return m0;
    case 1:
      return m1;
    case 3:
      return s0;
    case 4:
      return s1;
  }

  return -1;
}

void set_current_digit(int d) {
  switch (cursor_position) {
    case 0:
      m0 = d;
      break;
    case 1:
      m1 = d;
      break;
    case 3:
      s0 = d;
      break;
    case 4:
      s1 = d;
      break;
  }
}

void handle_change() {
  int y = analogRead(A1);

  if (cursor_moved_y == 0) {
    if (y > Y_MIDDLE + TOLERANCE && get_current_digit() > 0) {
      set_current_digit(get_current_digit() - 1);
      cursor_moved_y = 1;
      print_time();
    }
  
    if (y < Y_MIDDLE - TOLERANCE && get_current_digit() < 9) {
      set_current_digit(get_current_digit() + 1);
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

