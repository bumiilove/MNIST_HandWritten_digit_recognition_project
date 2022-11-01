#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include <TFT_eSPI.h>               // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();          // Invoke custom library

#define SENSIBILITY 400
#define MINPRESSURE 10
#define MAXPRESSURE 1000

//arduino
//#define YP A3  // must be an analog pin, use "An" notation! LCD_CS
//#define XM A2  // must be an analog pin, use "An" notation! LCD_RS
//#define YM 9   // can be a digital pin
//#define XP 8   // can be a digital pin
#define YP 14  // must be an analog pin, use "An" notation!
#define XM 12  // must be an analog pin, use "An" notation!
#define YM 22   // can be a digital pin
#define XP 21   // can be a digital pin

//// Calibrate values
#define TS_MINX 115
#define TS_MINY 55
#define TS_MAXX 955
#define TS_MAXY 875
//#define TS_MINX 90
//#define TS_MINY 140
//#define TS_MAXX 905
//#define TS_MAXY 965

TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);
////
//#define LCD_CS A3
//#define LCD_CD A2
//#define LCD_WR A1
//#define LCD_RD A0

#define LCD_CS 27
#define LCD_CD 12
#define LCD_WR 14
#define LCD_RD 13

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E3
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


// optional
#define LCD_RESET A4
#define BOXSIZE 224
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int flag =0;
uint16_t map_[112][112] = {0,};

uint16_t minx= 4087;
uint16_t maxx= 0;
uint16_t miny= 0;
uint16_t maxy= -3072;
int xmincnt =0;


void setup() {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  //tft.reset();
  tft.setRotation(0);
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(8, 270, 80, 30, GREEN);
  tft.fillRect(150, 270, 80, 30, YELLOW);
  
  //pinMode(13, OUTPUT);
  
  tft.setCursor (10, 12);
  tft.setTextSize (2);
  tft.setTextColor(RED);
//  tft.println("SPFD5408");
  tft.println("Hand written digit");
  
  waitOneTouch();
    tft.setCursor (16, 12);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
   //digitalWrite(13, LOW);
   

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
//  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
//  pinMode(YM, OUTPUT);
p.x = map(p.x, 450, 4088, 0, tft.height());
p.y = map(p.y, 380, -3072, 0, tft.width());
   tft.setTextSize (2);
   tft.print(char(p.x),p.y,p.z);
  Serial.print("x: ");Serial.print(p.x );
  Serial.print("\ty: ");Serial.print(p.y );
  Serial.print("\tz: ");Serial.println(p.z );

//    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
//  
//
//    // *** SPFD5408 change -- Begin
//    // Bug in in original code
//    //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
//    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
//    // *** SPFD5408 change -- End
//    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
//    if(p.y < 265)
//      tft.fillCircle(p.x, p.y, (1000- p.z)/60, BLACK);
//    Serial.print("X to map = "); Serial.print(p.x);
//    Serial.print("\tY to map= "); Serial.print(p.y);
//    Serial.print("\tPressure = "); Serial.println((1000- p.z)/60);
////    Serial.print("YP: ");Serial.println(YP );
////    Serial.print("XM: ");Serial.println(XM );
////    Serial.print("YM: ");Serial.println(YM );
////    Serial.print("XP: ");Serial.println(XP );
//    if(p.x < 60 && p.y >265)
//      tft_init();
//    else if(p.x > 180 && p.y > 265)
//      tft_init();
//
//
//    }
    

}


TSPoint waitOneTouch() {

  // wait 1 touch to exit function
  
  TSPoint p;
  
  do {
    p= ts.getPoint(); 
  
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
  
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  
  return p;
}

void drawBorder () {

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
  
}

void tft_init(){
  
  tft.fillScreen(BLACK);
  tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(8, 270, 80, 30, GREEN);
  tft.fillRect(150, 270, 80, 30, YELLOW);
  tft.setCursor (10, 12);
  tft.setTextSize (2);
  tft.setTextColor(RED);
//  tft.println("SPFD5408");
  tft.println("Hand written digit");
  //setup();
}#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include <TFT_eSPI.h>               // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();          // Invoke custom library

#define SENSIBILITY 400
#define MINPRESSURE 10
#define MAXPRESSURE 1000

//arduino
//#define YP A3  // must be an analog pin, use "An" notation! LCD_CS
//#define XM A2  // must be an analog pin, use "An" notation! LCD_RS
//#define YM 9   // can be a digital pin
//#define XP 8   // can be a digital pin
#define YP 14  // must be an analog pin, use "An" notation!
#define XM 12  // must be an analog pin, use "An" notation!
#define YM 22   // can be a digital pin
#define XP 21   // can be a digital pin

//// Calibrate values
#define TS_MINX 115
#define TS_MINY 55
#define TS_MAXX 955
#define TS_MAXY 875
//#define TS_MINX 90
//#define TS_MINY 140
//#define TS_MAXX 905
//#define TS_MAXY 965

TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);
////
//#define LCD_CS A3
//#define LCD_CD A2
//#define LCD_WR A1
//#define LCD_RD A0

#define LCD_CS 27
#define LCD_CD 12
#define LCD_WR 14
#define LCD_RD 13

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E3
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


// optional
#define LCD_RESET A4
#define BOXSIZE 224
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int flag =0;
uint16_t map_[112][112] = {0,};

uint16_t minx= 4087;
uint16_t maxx= 0;
uint16_t miny= 0;
uint16_t maxy= -3072;
int xmincnt =0;


void setup() {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  //tft.reset();
  tft.setRotation(0);
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(8, 270, 80, 30, GREEN);
  tft.fillRect(150, 270, 80, 30, YELLOW);
  
  //pinMode(13, OUTPUT);
  
  tft.setCursor (10, 12);
  tft.setTextSize (2);
  tft.setTextColor(RED);
//  tft.println("SPFD5408");
  tft.println("Hand written digit");
  
  waitOneTouch();
    tft.setCursor (16, 12);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
   //digitalWrite(13, LOW);
   

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
//  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
//  pinMode(YM, OUTPUT);
p.x = map(p.x, 450, 4088, 0, tft.height());
p.y = map(p.y, 380, -3072, 0, tft.width());
   tft.setTextSize (2);
   tft.print(char(p.x),p.y,p.z);
  Serial.print("x: ");Serial.print(p.x );
  Serial.print("\ty: ");Serial.print(p.y );
  Serial.print("\tz: ");Serial.println(p.z );

//    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
//  
//
//    // *** SPFD5408 change -- Begin
//    // Bug in in original code
//    //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
//    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
//    // *** SPFD5408 change -- End
//    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
//    if(p.y < 265)
//      tft.fillCircle(p.x, p.y, (1000- p.z)/60, BLACK);
//    Serial.print("X to map = "); Serial.print(p.x);
//    Serial.print("\tY to map= "); Serial.print(p.y);
//    Serial.print("\tPressure = "); Serial.println((1000- p.z)/60);
////    Serial.print("YP: ");Serial.println(YP );
////    Serial.print("XM: ");Serial.println(XM );
////    Serial.print("YM: ");Serial.println(YM );
////    Serial.print("XP: ");Serial.println(XP );
//    if(p.x < 60 && p.y >265)
//      tft_init();
//    else if(p.x > 180 && p.y > 265)
//      tft_init();
//
//
//    }
    

}


TSPoint waitOneTouch() {

  // wait 1 touch to exit function
  
  TSPoint p;
  
  do {
    p= ts.getPoint(); 
  
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
  
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  
  return p;
}

void drawBorder () {

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
  
}

void tft_init(){
  
  tft.fillScreen(BLACK);
  tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(8, 270, 80, 30, GREEN);
  tft.fillRect(150, 270, 80, 30, YELLOW);
  tft.setCursor (10, 12);
  tft.setTextSize (2);
  tft.setTextColor(RED);
//  tft.println("SPFD5408");
  tft.println("Hand written digit");
  //setup();
}
