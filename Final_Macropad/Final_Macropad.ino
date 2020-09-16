//HID - project library contained within libraries below 
// WARNING: MOUSE GRAPHICS DRAWER HAS BEEN MODIFIED TO INCLUDE HID PROJECT
//#include "HID-Project.h"
#include "mouse_graphics_drawer.h"
#include <Keypad.h>

//master control
int Master = 7;

//Permanent buttons
int Perma1 = 0;
int Perma2 = 1;

//Page select button
int Select = 2;

//joystick controls
int x_axis = A8;
int y_axis = A9;

//rotary encoders
byte AInput[2] = {3,5};
byte BInput[2] = {4,6};

void setup() {
  
pinMode(Master, INPUT);
pinMode(Perma1, INPUT);
pinMode(Perma2, INPUT);
pinMode(Select, INPUT);

pinMode(x_axis, INPUT);
pinMode(y_axis, INPUT);

pinMode(AInput[0], INPUT);
pinMode(BInput[0], INPUT);
pinMode(AInput[1], INPUT);
pinMode(BInput[1], INPUT);

Serial.begin(9600);
}

//Keypad initialisation
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','4','8','F'},
  {'1','5','9','E'},
  {'2','6','A','D'},
  {'3','7','B','C'}
};
byte colPins[ROWS] = {14, 16, 10, 15}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


//@functions
void Unicode_Alt_Plus(String code);
void joystick(int num);
void scroll(int n, int s);
void rotary_encoder(int n, int f, int s);



//@main_variables
char page_num = '0';
char transKey = '0';

byte lastState[2] = {0};
int  steps[2] = {0};
int  cw[2] = {0};
byte AState[2] = {0};
byte BState[2] = {0};
byte State[2] = {0};

int key_state = 0;
int toggle_click = 0;

void loop() {

 if(digitalRead(Master) == HIGH)
 { 
   //Deal with permanent keys
   if(digitalRead(Perma1) == HIGH)
   {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('c');   
    Keyboard.releaseAll();   
    delay(200); 
   }
   if(digitalRead(Perma2) == HIGH)
   {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_TAB);    
    Keyboard.releaseAll();
    delay(200);
   }
   

   //get matrix keypad key numbers
   char customKey = customKeypad.getKey();   
   if(customKey)
   {
    //if availible, store in new storage
    //Serial.println(customKey);
    transKey = customKey;
   }   


   //page select
   if(digitalRead(Select) == HIGH)
   {    
    page_num = transKey;  
    Mouse.move(10,0,0);
    delay(5);
    Mouse.move(-10,0,0);

    //release all just in case
    Keyboard.releaseAll();
    Mouse.releaseAll();
   }

   //get state of keys
   key_state = customKeypad.getState();
   

   //joystick menu
   //1 - mouse
   //2 - wasd
   //3 - arrow
   //4 - 2D scroll
   //5 - Zoom and undo/redo
   //6 - Window arrows

   //rotary encoder menu
   //1 - vertical scroll
   //2 - horizontal scroll
   //3 - video forward/rewind-
   //4 - zoom in/out
   //5 - undo/redo
   //6 - mouse left right
   //7 - mouse up down 
   //8 - volume
   //9 - brightness
  
   
   if(digitalRead(Select) == LOW)
   {
     //@Browser
     if(page_num == '0')
     {     
      joystick(4);//scroll
      rotary_encoder(0,3,-1);//arrows/video forward/rewind
      rotary_encoder(1,4,-1);//zoom in out
      if(customKey == '0'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('d');delay(100);Keyboard.releaseAll();}//Win+D
      if(customKey == '1'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('w');delay(100);Keyboard.releaseAll();}//Ctrl+W
      if(customKey == '2'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('T');delay(100);Keyboard.releaseAll();}//Ctrl+Shift+T
      if(customKey == '3'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('t');delay(100);Keyboard.releaseAll();}//Ctrl+T
      if(customKey == '4'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('a');delay(100);Keyboard.releaseAll();}//Ctrl+A
      if(customKey == '5'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('c');delay(100);Keyboard.releaseAll();}//Ctrl+C
      if(customKey == '6'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('v');delay(100);Keyboard.releaseAll();}//Ctrl+V
      if(customKey == '7'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('f');delay(100);Keyboard.releaseAll();}//Ctrl+F
      if(customKey == '8'){Keyboard.press(KEY_LEFT_ALT);Keyboard.press(KEY_LEFT_SHIFT);delay(100);Keyboard.releaseAll();}//ALt+SHIFT change language
      if(customKey == '9'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('v');delay(100);Keyboard.releaseAll();}//Win+V
      if(customKey == 'A'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('S');delay(100);Keyboard.releaseAll();}//Win+Shift+S snipping tool
      if(customKey == 'B'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('h');delay(100);Keyboard.releaseAll();}//Ctrl+H History

      //foot pedals
      if(customKey == 'C'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('x');delay(100);Keyboard.releaseAll();}//cut
      if(customKey == 'D'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('c');delay(100);Keyboard.releaseAll();}//copy
      if(customKey == 'E'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('v');delay(100);Keyboard.releaseAll();}//paste
      if(customKey == 'F'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('f');delay(100);Keyboard.releaseAll();}//search
     }

     //@Create
     else if(page_num == '1')
     {
      joystick(4);//scroll
      rotary_encoder(0,5,-1);//redo undo
      rotary_encoder(1,4,-1);//zoom in out
      if(customKey == '0'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('s');delay(100);Keyboard.releaseAll();}//Ctrl+D
      if(customKey == '1'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press(KEY_LEFT_ARROW);delay(100);Keyboard.releaseAll();}//Ctrl+<- skip word back
      if(customKey == '2'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press(KEY_RIGHT_ARROW);delay(100);Keyboard.releaseAll();}//Ctrl+->skip word forward
      if(customKey == '3'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press(KEY_BACKSPACE);delay(100);Keyboard.releaseAll();}//Ctrl+bk delete word
      if(customKey == '4'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('a');delay(100);Keyboard.releaseAll();}//Ctrl+A
      if(customKey == '5'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('c');delay(100);Keyboard.releaseAll();}//Ctrl+C
      if(customKey == '6'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('v');delay(100);Keyboard.releaseAll();}//Ctrl+V
      if(customKey == '7'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('f');delay(100);Keyboard.releaseAll();}//Ctrl+F
      if(customKey == '8'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('x');delay(100);Keyboard.releaseAll();}//Ctrl+X cut
      if(customKey == '9'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('v');delay(100);Keyboard.releaseAll();}//Win+V
      if(customKey == 'A'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('S');delay(100);Keyboard.releaseAll();}//Win+Shift+S snipping tool
      if(customKey == 'B'){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('n');delay(100);Keyboard.releaseAll();}//ctrl+n new

     }

     //@multimedia
      else if(page_num == '2')
     {
      joystick(3);//arrows
      rotary_encoder(0,8,-1);//brightness
      rotary_encoder(1,9,-1);//volume      
      if(customKey == '0'){Consumer.write(MEDIA_VOLUME_DOWN);}//
      if(customKey == '1'){Consumer.write(MEDIA_VOLUME_UP);}//
      if(customKey == '2'){Consumer.write(CONSUMER_BRIGHTNESS_DOWN);}//
      if(customKey == '3'){Consumer.write(CONSUMER_BRIGHTNESS_UP);}//
      if(customKey == '4'){Consumer.write(MEDIA_PLAY_PAUSE);}//
      if(customKey == '5'){Consumer.write(MEDIA_VOLUME_MUTE);}//
      if(customKey == '6'){Consumer.write(MEDIA_PREVIOUS);}//
      if(customKey == '7'){Consumer.write(MEDIA_NEXT);}//
      if(customKey == '8'){Consumer.write(CONSUMER_CONTROL_CONFIGURATION);}//
      if(customKey == '9'){Consumer.write(CONSUMER_EMAIL_READER);}//
      if(customKey == 'A'){Consumer.write(CONSUMER_CALCULATOR);}//
      if(customKey == 'B'){Consumer.write(CONSUMER_EXPLORER);}//


      //foot pedals
      if(transKey == 'C' && (key_state == 1 || key_state == 2)){Consumer.write(MEDIA_VOLUME_DOWN);delay(80);}//
      if(customKey == 'D'){Consumer.write(MEDIA_PLAY_PAUSE);}//
      if(customKey == 'E'){Consumer.write(MEDIA_VOLUME_MUTE);}//
      if(transKey == 'F' && (key_state == 1 || key_state == 2)){Consumer.write(MEDIA_VOLUME_UP);delay(80);}//
     }
     
     //@mouse
     else if(page_num == '3')
     {
      joystick(1);//mouse
      rotary_encoder(0,1,1);//scroll vertical
      rotary_encoder(1,2,1);//scroll horizontal
      if(customKey == '0'){Mouse.click(MOUSE_LEFT);}//
      if(customKey == '1'){if(toggle_click > 1){toggle_click = 0;}  if(toggle_click == 0){Mouse.press(MOUSE_LEFT);} else{Mouse.releaseAll();} toggle_click++;}      
      if(transKey == '2' && (key_state == 1 || key_state == 2)){Mouse.move(0,-1,0);}//move up
      if(transKey == '3' && (key_state == 1 || key_state == 2)){Mouse.move(0,0,1);delay(100);}//scroll up
      if(customKey == '4'){Mouse.click(MOUSE_LEFT);delay(100);Mouse.click(MOUSE_LEFT);}//double click
      if(transKey == '5' && (key_state == 1 || key_state == 2)){Mouse.move(-1,0,0);}//move left
      if(customKey == '6'){Mouse.click(MOUSE_MIDDLE);}//
      if(transKey == '7' && (key_state == 1 || key_state == 2)){Mouse.move(1,0,0);}//move right
      if(customKey == '8'){Mouse.click(MOUSE_LEFT);delay(100);Mouse.click(MOUSE_LEFT);delay(100);
      Mouse.click(MOUSE_LEFT);}//triple click
      if(customKey == '9'){Mouse.click(MOUSE_RIGHT);}//
      if(transKey == 'A' && (key_state == 1 || key_state == 2)){Mouse.move(0,1,0);}//move down
      if(transKey == 'B' && (key_state == 1 || key_state == 2)){Mouse.move(0,0,-1);delay(100);}//scroll down
     }

     //@game1
     else if(page_num == '4')
     {
      joystick(2);//wasd
      rotary_encoder(0,1,20);//mouse scroll vertical
      rotary_encoder(1,6,20);//look around
      if(customKeypad.findInList('0') != -1){Keyboard.press(' ');delay(10);}else{Keyboard.release(' ');}
      if(customKeypad.findInList('1') != -1){Keyboard.press('w');delay(10);}else{Keyboard.release('w');}   
      if(customKeypad.findInList('2') != -1){Keyboard.press(KEY_LEFT_CTRL);Keyboard.press('w');delay(10);}else{Keyboard.release(KEY_LEFT_CTRL);Keyboard.release('w');}
      if(customKey == '3'){Keyboard.press('e');}    
      if(customKeypad.findInList('4') != -1){Keyboard.press('a');delay(10);}else{Keyboard.release('a');}
      if(customKeypad.findInList('5') != -1){Keyboard.press('s');delay(10);}else{Keyboard.release('s');}  
      if(customKeypad.findInList('6') != -1){Keyboard.press('d');delay(10);}else{Keyboard.release('d');}
      if(customKeypad.findInList('7') != -1){Mouse.press(MOUSE_RIGHT);delay(700);Mouse.releaseAll();delay(300);}  
      if(customKeypad.findInList('8') != -1){Keyboard.press('q');delay(10);}else{Keyboard.release('q');}
      if(customKey == '9'){Keyboard.press('f');}    
      if(customKeypad.findInList('A') != -1){Keyboard.press(KEY_LEFT_SHIFT);delay(10);}else{Keyboard.release(KEY_LEFT_SHIFT);}
      if(customKeypad.findInList('B') != -1){Mouse.click();delay(50);}         

      if(key_state == 0){Keyboard.releaseAll();}
        
     }

     //@OpenApps
     else if(page_num == '5')
     {
      if(customKey == '0'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("messenger");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '1'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("otakuassistant");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '2'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("minecraft");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '3'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("outlook");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '4'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("anki");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '5'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("word");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '6'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("arduino IDE");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '7'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '8'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == '9'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}
      if(customKey == 'A'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}    
      if(customKey == 'B'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('s');Keyboard.releaseAll();delay(50);Keyboard.print("");delay(50);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}    
    }
     //@ActionApps
     else if(page_num == '6')
     {
      joystick(6);//window arrows
      if(customKey == '0'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('a');Keyboard.releaseAll();}//Win+A action centre
      if(customKey == '1'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('n');Keyboard.releaseAll();}//Win+N notepad
      if(customKey == '2'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press(';');Keyboard.releaseAll();}//Win+; emojis
      if(customKey == '3'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('i');Keyboard.releaseAll();}//Win+I settings
      if(customKey == '4'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('k');Keyboard.releaseAll();}//Win+K connect
      if(customKey == '5'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('r');Keyboard.releaseAll();}//Win+R run
      if(customKey == '6'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('x');Keyboard.releaseAll();}//Win+X win menu
      if(customKey == '7'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('r');delay(200);Keyboard.releaseAll();Keyboard.print("chrome");delay(100);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();delay(1000);Keyboard.print("https://bb.imperial.ac.uk/");Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}//
      if(customKey == '8'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('r');delay(200);Keyboard.releaseAll();Keyboard.print("chrome");delay(100);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();delay(1000);Keyboard.print("www.netflix.com");Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}//
      if(customKey == '9'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('r');delay(200);Keyboard.releaseAll();Keyboard.print("chrome");delay(100);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();delay(1000);Keyboard.print("www.wikipedia.com");Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}// 
      if(customKey == 'A'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('r');delay(200);Keyboard.releaseAll();Keyboard.print("chrome");delay(100);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();delay(1000);Keyboard.print("www.facebook.com");Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}//
      if(customKey == 'B'){Keyboard.press(KEY_LEFT_GUI);Keyboard.press('r');delay(200);Keyboard.releaseAll();Keyboard.print("chrome");delay(100);Keyboard.press(KEY_ENTER);Keyboard.releaseAll();delay(1000);Keyboard.print("www.polyglotclub.com");Keyboard.press(KEY_ENTER);Keyboard.releaseAll();}//
      }


     //@Mathematics
     else if(page_num == '7')
     {
      if(BootKeyboard.getLeds() & LED_CAPS_LOCK)
      {
      if(customKey == '0'){Unicode_Alt_Plus("2261");}//≡
      if(customKey == '1'){Unicode_Alt_Plus("2248");}//≈
      if(customKey == '2'){Unicode_Alt_Plus("3C0");}//π
      if(customKey == '3'){Unicode_Alt_Plus("B0");}//°
      if(customKey == '4'){Unicode_Alt_Plus("2225");}//∥
      if(customKey == '5'){Unicode_Alt_Plus("27C2");}//⟂
      if(customKey == '6'){Unicode_Alt_Plus("2200");}//∀
      if(customKey == '7'){Unicode_Alt_Plus("2234");}//∴
      if(customKey == '8'){Unicode_Alt_Plus("394");}//Δ  
      if(customKey == '9'){Unicode_Alt_Plus("3B4");}//δ
      if(customKey == 'A'){Unicode_Alt_Plus("2220");}//∠  
      }
      else
      {
      if(customKey == '0'){Unicode_Alt_Plus("B1");}//±
      if(customKey == '1'){Unicode_Alt_Plus("221E");}//∞
      if(customKey == '2'){Unicode_Alt_Plus("221A");}//√  
      if(customKey == '3'){Unicode_Alt_Plus("221D");}//∝
      if(customKey == '4'){Unicode_Alt_Plus("2211");}//∑
      if(customKey == '5'){Unicode_Alt_Plus("222B");}//∫
      if(customKey == '6'){Unicode_Alt_Plus("222E");}//∮
      if(customKey == '7'){Unicode_Alt_Plus("2217");}//∗
      if(customKey == '8'){Unicode_Alt_Plus("2A2F");}//⨯
      if(customKey == '9'){Unicode_Alt_Plus("2207");}//∇
      if(customKey == 'A'){Unicode_Alt_Plus("2202");}//∂
      }
      if(customKey == 'B'){Keyboard.press(KEY_CAPS_LOCK);Keyboard.releaseAll();}
     }
     
     //@German
     else if(page_num == '8')
     {
      if(BootKeyboard.getLeds() && LED_CAPS_LOCK)
      {
      if(customKey == '0'){Unicode_Alt_Plus("C4");}//Ä
      if(customKey == '1'){Unicode_Alt_Plus("D6");}//Ö
      if(customKey == '2'){Unicode_Alt_Plus("DC");}//Ü
      if(customKey == '3'){Unicode_Alt_Plus("DF");}//ß      
      }
      else
      {
      if(customKey == '0'){Unicode_Alt_Plus("E4");}//ä
      if(customKey == '1'){Unicode_Alt_Plus("F6");}//ö
      if(customKey == '2'){Unicode_Alt_Plus("FC");}//ü
      if(customKey == '3'){Unicode_Alt_Plus("DF");}//ß
      if(customKey == '4'){Keyboard.print("der");}//der
      if(customKey == '5'){Keyboard.print("die");}//die 
      if(customKey == '6'){Keyboard.print("das");}//das
      if(customKey == '7'){Keyboard.print("den");}//den
      if(customKey == '8'){Keyboard.print("dem");}//dem
      if(customKey == '9'){Keyboard.print("des");}//des
      }
      if(customKey == 'B'){Keyboard.press(KEY_CAPS_LOCK);Keyboard.releaseAll();}      
     }
     
     //@Spanish
     else if(page_num == '9')
     {
      if(BootKeyboard.getLeds() && LED_CAPS_LOCK)
      {
      if(customKey == '0'){Unicode_Alt_Plus("D1");}//Ñ
      if(customKey == '1'){Unicode_Alt_Plus("BF");}//¿
      if(customKey == '2'){Unicode_Alt_Plus("A1");}//¡
      if(customKey == '3'){Unicode_Alt_Plus("C1");}//Á
      if(customKey == '4'){Unicode_Alt_Plus("C9");}//É
      if(customKey == '5'){Unicode_Alt_Plus("CD");}//Í
      if(customKey == '6'){Unicode_Alt_Plus("D3");}//Ó
      if(customKey == '7'){Unicode_Alt_Plus("DA");}//Ú
      if(customKey == '8'){Unicode_Alt_Plus("DC");}//Ü    
      }
      else
      {
      if(customKey == '0'){Unicode_Alt_Plus("F1");}//ñ
      if(customKey == '1'){Unicode_Alt_Plus("BF");}//¿
      if(customKey == '2'){Unicode_Alt_Plus("A1");}//¡   
      if(customKey == '3'){Unicode_Alt_Plus("E1");}//á
      if(customKey == '4'){Unicode_Alt_Plus("E9");}//é
      if(customKey == '5'){Unicode_Alt_Plus("ED");}//í
      if(customKey == '6'){Unicode_Alt_Plus("F3");}//ó
      if(customKey == '7'){Unicode_Alt_Plus("FA");}//ú
      if(customKey == '8'){Unicode_Alt_Plus("FC");}//ü
      if(customKey == '9'){Keyboard.print("el");}//el
      if(customKey == 'A'){Keyboard.print("la");}//la
      }
      if(customKey == 'B'){Keyboard.press(KEY_CAPS_LOCK);Keyboard.releaseAll();}
     }

      //@Polish
      else if(page_num == 'A')
      {
        if(BootKeyboard.getLeds() && LED_CAPS_LOCK)
      {
      if(customKey == '0'){Unicode_Alt_Plus("104");}//Ą
      if(customKey == '1'){Unicode_Alt_Plus("118");}//Ę
      if(customKey == '2'){Unicode_Alt_Plus("141");}//
      if(customKey == '3'){Unicode_Alt_Plus("143");}//Ń
      if(customKey == '4'){Unicode_Alt_Plus("17B");}//Ż
      if(customKey == '5'){Unicode_Alt_Plus("179");}//Ź
      if(customKey == '6'){Unicode_Alt_Plus("15A");}//Ś
      if(customKey == '7'){Unicode_Alt_Plus("106");}//Ć
      if(customKey == '8'){Unicode_Alt_Plus("D3");}//Ó
      }
      else
      {
      if(customKey == '0'){Unicode_Alt_Plus("105");}//ą
      if(customKey == '1'){Unicode_Alt_Plus("119");}//ę
      if(customKey == '2'){Unicode_Alt_Plus("142");}//ł
      if(customKey == '3'){Unicode_Alt_Plus("144");}//ń
      if(customKey == '4'){Unicode_Alt_Plus("17C");}//ż
      if(customKey == '5'){Unicode_Alt_Plus("17A");}//ź
      if(customKey == '6'){Unicode_Alt_Plus("15B");}//ś
      if(customKey == '7'){Unicode_Alt_Plus("107");}//ć
      if(customKey == '8'){Unicode_Alt_Plus("F3");}//ó
      }
      if(customKey == 'B'){Keyboard.press(KEY_CAPS_LOCK);Keyboard.releaseAll();}
     }

      //@French
      else if(page_num == 'B')
      {
      {
          if(BootKeyboard.getLeds() && LED_CAPS_LOCK)
      {      
      if(customKey == '0'){Unicode_Alt_Plus("C7");}//Ç
      if(customKey == '1'){Unicode_Alt_Plus("FF");}//ÿ
      if(customKey == '4'){Unicode_Alt_Plus("E8");}//è
      if(customKey == '5'){Unicode_Alt_Plus("EB");}//ë    
      }
      else
      {
      if(customKey == '0'){Unicode_Alt_Plus("E7");}//ç
      if(customKey == '1'){Unicode_Alt_Plus("153");}//œ
      if(customKey == '2'){Unicode_Alt_Plus("E0");}//à
      if(customKey == '3'){Unicode_Alt_Plus("E2");}//â
      if(customKey == '4'){Unicode_Alt_Plus("E9");}//é
      if(customKey == '5'){Unicode_Alt_Plus("EA");}//ê
      if(customKey == '6'){Unicode_Alt_Plus("EE");}//î
      if(customKey == '7'){Unicode_Alt_Plus("EF");}//ï
      if(customKey == '8'){Unicode_Alt_Plus("F9");}//ù 
      if(customKey == '9'){Unicode_Alt_Plus("FB");}//û   
      if(customKey == 'A'){Unicode_Alt_Plus("FC");}//ü  
      }
      if(customKey == 'B'){Keyboard.press(KEY_CAPS_LOCK);Keyboard.releaseAll();}
      }
      }

   }
  
    
   //mouse_draw_circle(20);

   

 }
 else
 {
  Serial.println("NOPE - Switch on Master");

  //mouse motion notifier
  delay(5000);
  Mouse.move(-30,0,0);
  delay(20);
  Mouse.move(30,0,0);
  delay(20);
 }

 //end of void loop
}


//@unicode
void Unicode_Alt_Plus(String code)
{
   //check num lock - if OFF then switch ON
   if (BootKeyboard.getLeds() & LED_NUM_LOCK)
   {
     Serial.println("ON");
     Keyboard.press(KEY_NUM_LOCK);
     Keyboard.releaseAll();
   }
  
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(HID_KEYPAD_ADD);
  Keyboard.press(KEY_BACKSPACE);
  delay(10);
  Keyboard.releaseAll();
  Keyboard.print(code);  
  Keyboard.press(KEY_RETURN);
  
  delay(100);
  Keyboard.releaseAll();
}

//@joystick
void joystick(int num)
{
    int x = 0;
    int y = 0;
    
    x = analogRead(x_axis);
    y = analogRead(y_axis);

    x -= 524;
    y -= 495;

    if( x < 70  && x > -70){x = 0;}
    if( y < 70  && y > -70){y = 0;}
    
  if(num == 1)
  {
   if(y > 0)
   {
   Mouse.move(0,-1,0);   
   }
   else if(y < 0)
   {
    Mouse.move(0,1,0);   
   }

   if(x > 0)
   {
   Mouse.move(-1,0,0);   
   }
   else if(x < 0)
   {
    Mouse.move(1,0,0);   
   }
   delay(1);
  }
  else if(num == 2)
  {
    if(y < -20)
    {
      Keyboard.press('s');
      delay(10);  
    }
    else if(y > 20)
    {
      Keyboard.press('w');  
      delay(10);  
    }
    else
    {
      Keyboard.releaseAll(); 
    }
     if(x < -20)
    {
      Keyboard.press('d');
      delay(10);    
    }
    else if(x > 20)
    {
      Keyboard.press('a');
      delay(10);     
    }
    else
    {
      Keyboard.releaseAll(); 
    }
  }   
  else if(num == 3)
  {
      if(y < 0)
    {
      Keyboard.press(KEY_DOWN_ARROW);
      delay(20);        
    }
    else if(y > 0)
    {
      Keyboard.press(KEY_UP_ARROW);
      delay(20);
    }
    else
    {
      Keyboard.releaseAll(); 
    }

     if(x < 0)
    {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(50);   
    }
    else if(x > 0)
    {
      Keyboard.press(KEY_LEFT_ARROW);
      delay(50);
    }
    else
    {
      Keyboard.releaseAll(); 
    }
  }
  else if(num == 4)
  {
    if(y < -30)
    {
      Mouse.move(0,0,-1);
      delay(50); 
    }
    else if(y > 30)
    {     
      Mouse.move(0,0,1);
      delay(50);      
    }

     if(x < -30)
    {
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Mouse.move(0,0,-1);
      delay(10); 
    }
    else if(x > 30)
    {
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Mouse.move(0,0,1);
      delay(10);           
    }
    else
    {
      Keyboard.releaseAll();  
    }
  }
  
  else if(num == 5)
  {
    if(x < -10)
    {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('z');
      delay(50); 
      Keyboard.releaseAll(); 
    }
    else if(x > 10)
    {     
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('y');
      delay(50);   
      Keyboard.releaseAll();    
    }

     if(y < -10)
    {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('+');
      delay(100); 
      Keyboard.releaseAll();   
    }
    else if(y > 10)
    {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('-');
      delay(100);
      Keyboard.releaseAll();  
    }
  }  
    else if(num == 6)
    
  {
    if(x < -10)
    {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_ARROW);
      delay(300); 
      Keyboard.releaseAll(); 
    }
    else if(x > 10)
    {        
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(300);   
      Keyboard.releaseAll();    
    }

     if(y < -10)
    {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_UP_ARROW);
      delay(300); 
      Keyboard.releaseAll();   
    }
    else if(y > 10)
    {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_DOWN_ARROW);
      delay(300);
      Keyboard.releaseAll();  
    }
  }     
}

//@scroll
void scroll(int n, int s)
{
  if(n == 1)
  {
    Mouse.move(0, 0, -1);
  }
  else if (n == -1)
  {
    Mouse.move(0, 0, 1);
  }
  if(n == 2)
  {
    Keyboard.press(KEY_LEFT_SHIFT);   
    delay(5); 
    Mouse.move(0, 0, 1);
    
  }
  else if (n == -2)
  {
    Keyboard.press(KEY_LEFT_SHIFT);  
    delay(5); 
    Mouse.move(0, 0, -1);
    
  }
   if(n == 3)
  {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  else if (n == -3)
  {
    Keyboard.press(KEY_RIGHT_ARROW);   
  }
   if(n == 4)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    delay(5);
    Mouse.move(0,0,-1);    
  }
  else if (n == -4)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    delay(5);
    Mouse.move(0,0,1);    
  }
     if(n == 5)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('z');
  }
  else if (n == -5)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('y');
  }
  if(n == 6)
  {
    Mouse.move(-1*s, 0, 0);
  }
  else if (n == -6)
  {
    Mouse.move(1*s, 0, 0);
  }
  if(n == 7)
  {
    Mouse.move(0, 1*s, 0);
  }
  else if (n == -7)
  {
    Mouse.move(0, -1*s, 0);
  }
   if(n == 8)
  {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
  else if (n == -8)
  {
    Consumer.write(MEDIA_VOLUME_UP);
  }
   if(n == 9)
  {
    Consumer.write(CONSUMER_BRIGHTNESS_DOWN);
  }
  else if (n == -9)
  {
    Consumer.write(CONSUMER_BRIGHTNESS_UP);
  }
}

//@rotary_encoder
void rotary_encoder(int n, int f, int s = 0)
{
  AState[n] = digitalRead(AInput[n]);
  BState[n] = digitalRead(BInput[n]) << 1;
  State[n] = AState[n] | BState[n];

  if (lastState[n] != State[n]){
    switch (State[n]) {
      case 0:
        if (lastState[n] == 2){
          steps[n]++;
          cw[n] = 1;
          scroll(f,s);
          
        }
        else if(lastState[n] == 1){
          steps[n]--;
          cw[n] = -1;
          scroll((-1)*f,s);
        }
        break;
      case 1:
        if (lastState[n] == 0){
          steps[n]++;
          cw[n] = 1;
          scroll(f,s);
        }
        else if(lastState[n] == 3){
          steps[n]--;
          cw[n] = -1;
          scroll((-1)*f,s);
        }
        break;
      case 2:
        if (lastState[n] == 3){
          steps[n]++;
          cw[n] = 1;            
           scroll(f,s);
        }
        else if(lastState[n] == 0){
          steps[n]--;
          cw[n] = -1;
          scroll((-1)*f,s);
        }
        break;
      case 3:
        if (lastState[n] == 1){
          steps[n]++;
          cw[n] = 1;
           scroll(f,s);
        }
        else if(lastState[n] == 2){
          steps[n]--;
          cw[n] = -1;
          scroll((-1)*f,s);
        }
        break;
    }
  }
  else
  {
     Keyboard.releaseAll();
  }

  lastState[n] = State[n];
}
