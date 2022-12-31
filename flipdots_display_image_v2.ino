// A flipdots experiment by Caustic and FineQuasar17
// For use with hanover flipdot displays

String command;
String arg;

int Del = 2;
int FlipDelay = 1;
int blankDelay = 5000;

// dot counts
#define dots_per_row 19
#define dots_per_column 32

int row_idx = 0;
int col_idx = 0;

//below are the pins wired directly into the hanover display board
int EnablePin = 7; //this enables the board, if using just 1 board always keep high
int CoilPin = 2; //this sends a short pulse to actually flip the pixel, if the set pin is low itll make it black on this pulse etc
int SetPin = 4; //this pin sets either black or yellow, high is yellow, low is black
int RowPin = 3; //this advances the row 1-32 (even though 17-32 is not visible)
int ColumnPin = 6; //this advances the column 1-32
int ResetPin = 5; //this resets the row and column back to zero

//these are the control buttons for the display board
//19,20,21,15,14,13,12,11

int ResetButton = 14; //this button wipes the whole etch a sketch
int LeftButton = 12; //left movement
int RightButton = 16; //right movement
int UpButton = 10; //up movement
int DownButton = 11; //down movement
int WriteButtonYellow = 9; //write yellow pixel
int WriteButtonBlack = 8; //write black pixel
int UnspecifiedButton = 15; //its wired but no use its on the up stroke of the switch


int RightButtonLatch = 0;
int LeftButtonLatch = 0;
int UpButtonLatch = 0;
int DownButtonLatch = 0;

int ResetButtonState = 0;
int LeftButtonState = 0;
int RightButtonState = 0;
int UpButtonState = 0;
int DownButtonState = 0;
int WriteButtonStateYellow = 0;
int WriteButtonStateBlack = 0;
int WriteButtonStateUnspecifiedButton = 0;

//lnnc
bool display_buffer[dots_per_column][dots_per_row] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:

  Serial.println("Type a command!");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(CoilPin, OUTPUT);
  pinMode(SetPin, OUTPUT);
  pinMode(RowPin, OUTPUT);
  pinMode(ColumnPin, OUTPUT);
  pinMode(ResetPin, OUTPUT);
  pinMode(EnablePin, OUTPUT);
  pinMode(ResetButton, INPUT_PULLUP);
  pinMode(LeftButton, INPUT_PULLUP);
  pinMode(RightButton, INPUT_PULLUP);
  pinMode(UpButton, INPUT_PULLUP);
  pinMode(DownButton, INPUT_PULLUP);
  pinMode(WriteButtonYellow, INPUT_PULLUP);
  pinMode(WriteButtonBlack, INPUT_PULLUP);
  pinMode(UnspecifiedButton, INPUT_PULLUP);

  //boots the display with a boot logo
  initialize();
  show_display_buffer();
  delay(500);
  AllBlack();
}

void loop() {

  //Serial command console

  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');

    if (command.startsWith("send")) {
      arg = command.substring(4);
      arg.trim();
      send_message(arg);
    }
    if (command.startsWith("set_dot_idx")) {
      arg = command.substring(11);
      arg.trim();
      // Copy the value of the string to a char_array
      // Length (with one extra character for the null terminator)
      int arg_len = arg.length() + 1;
      // Prepare the character array (the buffer)
      char char_array[arg_len];
      // Copy it over
      arg.toCharArray(char_array, arg_len);
      char* arg_array[1]; // lets save the first 2 args
      //tokenize the arguments, split by delim
      char* token = strtok(char_array, " ");
      const char s[2] = ",";
      /* get the first token */
      token = strtok(char_array, s);
      arg_array[0] = token; //save arg0 in idx 0
      token = strtok(NULL, s);
      arg_array[1] = token; //save arg1 in idx 1
      set_dot_idx(atoi(arg_array[0]), atoi(arg_array[1])); //converts a string representation of an int, into an int type
    }
    else if (command.startsWith("reset_dot_idx")) {
      reset_dot_idx();
    }
    else if (command.startsWith("display_dot_idx")) {
      display_dot_idx();
    }
    else if (command.startsWith("show_display_buffer")) {
      show_display_buffer();
    }
    else if (command.startsWith("init")) {
      initialize();
    }
    else if (command.startsWith("DotYellow")) {
      DotYellow();
    }
    else if (command.startsWith("DotBlack")) {
      DotBlack();
    }
    else if (command.startsWith("AllYellow")) {
      AllYellow();
    }
    else if (command.startsWith("AllBlack")) {
      AllBlack();
    }
    else if (command.startsWith("RowYellow")) {
      RowYellow();
    }
    else if (command.startsWith("RowBlack")) {
      RowBlack();
    }
    else if (command.startsWith("RowAdvance")) {
      RowAdvance();
    }
    else if (command.startsWith("RowRetreat")) {
      RowRetreat();
    }
    else if (command.startsWith("ColumnAdvance")) {
      ColumnAdvance();
    }
    else if (command.startsWith("ColumnRetreat")) {
      ColumnRetreat();
    }
    else {
      Serial.println("Invalid command");
    }
  }
  // Cool Buttons
  //wipe the screen
  ResetButtonState = digitalRead(ResetButton);
  if (ResetButtonState == LOW)
  {
    Serial.println("ResetButton");
    digitalWrite(ResetPin, HIGH);
    delay(2);
    digitalWrite(ResetPin, LOW);
//    initialize();
    AllBlack();
  }

  //write display_buffer pixel
  WriteButtonStateUnspecifiedButton = digitalRead(UnspecifiedButton);
  if (WriteButtonStateUnspecifiedButton == LOW) {
    show_display_buffer();
    Serial.println("UnspecifiedButton");
  }

  //write a yellow pixel
  WriteButtonStateYellow = digitalRead(WriteButtonYellow);
  if (WriteButtonStateYellow == LOW) {
    DotYellow();
    Serial.println("YelloButton");
  }
  //write a black pixel
  WriteButtonStateBlack = digitalRead(WriteButtonBlack);
  if (WriteButtonStateBlack == LOW) {
    DotBlack();
  }

  //advance column right
  RightButtonState = digitalRead(RightButton);
  if ((RightButtonState == LOW) && (RightButtonLatch == 0)) {
    ColumnAdvance();
    RightButtonLatch = 1;
  }
  RightButtonState = digitalRead(RightButton);
  if ((RightButtonState == HIGH) && (RightButtonLatch == 1)) {
    RightButtonLatch = 0;
    delay(5);
  }

  //retreat column right
  LeftButtonState = digitalRead(LeftButton);
  if ((LeftButtonState == LOW) && (LeftButtonLatch == 0)) {
    ColumnRetreat();
    LeftButtonLatch = 1;
  }
  LeftButtonState = digitalRead(LeftButton);
  if ((LeftButtonState == HIGH) && (LeftButtonLatch == 1)) {
    LeftButtonLatch = 0;
  }

  //advance row up
  UpButtonState = digitalRead(UpButton);
  if ((UpButtonState == LOW) && (UpButtonLatch == 0)) {
    RowAdvance();
    UpButtonLatch = 1;
  }
  UpButtonState = digitalRead(UpButton);
  if ((UpButtonState == HIGH) && (UpButtonLatch == 1)) {
    UpButtonLatch = 0;
    delay(5);
  }

  //retreat row down
  DownButtonState = digitalRead(DownButton);
  if ((DownButtonState == LOW) && (DownButtonLatch == 0)) {
    RowRetreat();
    DownButtonLatch = 1;
  }
  DownButtonState = digitalRead(DownButton);
  if ((DownButtonState == HIGH) && (DownButtonLatch == 1)) {
    DownButtonLatch = 0;
  }

}


void show_display_buffer() {
  for (int i = 0; i < dots_per_column; i++) //loop across the columns.
  {
    for (int j = 0; j < dots_per_row; j++) //loop across the row.
    {
      if (display_buffer[i][j])
      {
        DotYellow();
        RowAdvance();
      }
      else {
        DotBlack();
        RowAdvance();
      }
    }
    Serial.println(); // carriage return after the last label
    ColumnAdvance();
  }
}

void set_dot_idx(int row, int col) {
  col_idx = col;
  row_idx = row;
  Serial.println();
  Serial.println("---------------------");
  Serial.print("row_idx: ");
  Serial.println(row_idx);
  Serial.print("col_idx: ");
  Serial.println(col_idx);
  Serial.println("---------------------");
}

void reset_dot_idx() {
  row_idx = 0;
  col_idx = 0;
}

void display_dot_idx() {
  Serial.println();
  Serial.println("---------------------");
  Serial.print("row_idx: ");
  Serial.println(row_idx);
  Serial.print("col_idx: ");
  Serial.println(col_idx);
  Serial.println("---------------------");
}


void draw_buffer(bool image_buffer[dots_per_row][dots_per_column]) {

  for (int i = 0; i < dots_per_row; i++) //loop across all rows.
  {
    for (int j = 0; j < dots_per_column; j++) //loop across length of dot array.
    {
      if (image_buffer[i][j])
      {
        DotYellow();
      }
      else {
        DotBlack();
      }
      delay(Del);
    }
    Serial.println(); // carriage return after the last label
  }
  Serial.println(); // carriage return after the last label
}

//This Draws a dot Yellow
void DotYellow() {
  Serial.print("░");
  digitalWrite(CoilPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(SetPin, HIGH);   // turn the LED off by making the voltage LOW
  delay(FlipDelay);             // wait for a second
  digitalWrite(CoilPin, LOW);  // turn the LED on (HIGH is the voltage level)
}

//this draws a dot black
void DotBlack() {
  Serial.print("█");
  digitalWrite(CoilPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(SetPin, LOW);   // turn the LED off by making the voltage LOW
  delay(FlipDelay);            // wait for a second
  digitalWrite(CoilPin, LOW);  // turn the LED on (HIGH is the voltage level)
}

//this makes the whole screen yellow
void AllYellow() {
  for (int i = 0; i < dots_per_column; i++)
  {
    RowYellow();
  }
}

//this makes the whole screen black
void AllBlack() {
  for (int i = 0; i < dots_per_column; i++)
  {
    RowBlack();
  }
}

//This makes a single row yellow
void RowYellow() {
  DotYellow();
  delay(10);
  RowAdvance();

  for (int i = 0; i < (dots_per_row - 2); i++)
  {
    DotYellow();
    delay(Del);
    RowAdvance();
  }

  DotYellow();
  delay(Del);
  ColumnAdvance();
}

//This makes a single row black
void RowBlack() {
  DotBlack();
  delay(10);
  RowAdvance();

  for (int i = 0; i < (dots_per_row - 2); i++)
  {
    DotBlack();
    delay(Del);
    RowAdvance();
  }

  DotBlack();
  delay(Del);
  ColumnAdvance();
}

//this advances the row
void RowAdvance() {
  digitalWrite(RowPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1);                      // wait for a second
  digitalWrite(RowPin, LOW);   // turn the LED off by making the voltage LOW
  if (row_idx < dots_per_row) {
    row_idx++;
  }
  else if (row_idx == dots_per_row) { // if you advance, and are at the end of the row, go back to idx 0
    row_idx = 0;
  }
  //Serial.println();
}

//this makes the row go back by going forward minus 1, remember the hanover display counts 32 (dots_per_column) even though there are less pixels
void RowRetreat() {
  for (int i = 0; i < 31; i++)
  {
    digitalWrite(RowPin, HIGH);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(RowPin, LOW);   // turn the LED off by making the voltage LOW
  }
  if (row_idx > 0) {
    row_idx--;
  }
  else if (row_idx == 0) { // if you advance, and are at the beginning of the row, go back to idx [dots_per_row]
    row_idx = dots_per_row;
  }
}

//this makes the column go forward
void ColumnAdvance() {
  digitalWrite(ColumnPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1);                      // wait for a second
  digitalWrite(ColumnPin, LOW);   // turn the LED off by making the voltage LOW

  if (col_idx < dots_per_column) {
    col_idx++;
  }
  else if (col_idx == dots_per_column) { // if you advance, and are at the beginning of the column, go back to idx [dots_per_column]
    col_idx = 0;
  }

  Serial.println();
}

//this makes the column go back by going foward all of the columns minus 1
void ColumnRetreat()  {
  for (int i = 0; i < 31; i++)
  {
    digitalWrite(ColumnPin, HIGH);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(ColumnPin, LOW);   // turn the LED off by making the voltage LOW
  }
  if (col_idx > 0) {
    col_idx--;
  }
  else if (col_idx == 0) { // if you advance, and are at the beginning of the column, go back to idx [dots_per_column]
    col_idx = dots_per_column;
  }
}

void initialize() {
  digitalWrite(EnablePin, HIGH);
  digitalWrite(ResetPin, HIGH);
  delay(2);
  digitalWrite(ResetPin, LOW); //this resets the row and column back to zero
  col_idx = 0; // reset column index
  row_idx = 0; // reset row index
  AllYellow();
  AllBlack();
}

void send_message(String arg) {
  Serial.println("Message Sent: " + arg);
}
