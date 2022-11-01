#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define SENSIBILITY 300
#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

//// Calibrate values
//#define TS_MINX 125
//#define TS_MINY 85
//#define TS_MAXX 965
//#define TS_MAXY 905

#define TS_MINX 75
#define TS_MINY 135
#define TS_MAXX 855
#define TS_MAXY 935
//#define TS_MINX 90
//#define TS_MINY 140
//#define TS_MAXX 905
//#define TS_MAXY 965

TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
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
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int flag =0;
float map_[28][28] = {0,};
uint16_t i =0;
uint16_t j =0;

uint16_t minz = 500;
uint16_t maxz = 0;
void setup() {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(8, 270, 80, 30, GREEN);
  tft.fillRect(150, 270, 80, 30, YELLOW);
  
  pinMode(13, OUTPUT);
  
  tft.setCursor (10, 12);
  tft.setTextSize (2);
  tft.setTextColor(RED);
//  tft.println("SPFD5408");
  tft.println("Hand written digit");
  
  waitOneTouch();
}

void loop() {
  // put your main code here, to run repeatedly:
   
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
   digitalWrite(13, LOW);
//    Serial.print("X to map = "); Serial.print(p.x);
//    Serial.print("\tY to map= "); Serial.print(p.y-30);
//    Serial.print("\tPressure = "); Serial.println(p.z);
//   if(maxz < p.z && p.z != 0)
//      maxz = p.z;
//   if(minz > p.z && p.z != 0)
////      minz = p.z;
//     Serial.print("\tp.min = "); Serial.println(minz);
//     Serial.print("\tp.maz = "); Serial.println(maxz);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  

    // *** SPFD5408 change -- Begin
    // Bug in in original code
    //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    p.x = (map(p.x, TS_MINX, TS_MAXX, 0, tft.width()));
    // *** SPFD5408 change -- End
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if(p.y < 265)
      tft.fillCircle(p.x, p.y, (1000- p.z)/60, BLACK);
    //Serial.print("X to map = "); 
    p.x = (map(p.x, 0, tft.width(), 0, 28));
    // *** SPFD5408 change -- End
    p.y = map(p.y, 0, tft.height(), -4, 34);
    p.z = map(p.z, 0,800,0,255);
    {
    Serial.print(p.x);
    Serial.print(" ");
    //Serial.print("\tY to map= "); 
    Serial.print(p.y);
    Serial.print(" ");
    //Serial.print("\tPressure = "); 
    Serial.println(p.z);
    }
//    {
//    Serial.print(p.x+1);
//    Serial.print(" ");
//    //Serial.print("\tY to map= "); 
//    Serial.print(p.y);
//    Serial.print(" ");
//    //Serial.print("\tPressure = "); 
//    Serial.println(p.z-40);
//    }
//    {
//    Serial.print(p.x);
//    Serial.print(" ");
//    //Serial.print("\tY to map= "); 
//    Serial.print(p.y+1);
//    Serial.print(" ");
//    //Serial.print("\tPressure = "); 
//    Serial.println(p.z-40);
//    }
//    {
//    Serial.print(p.x-1);
//    Serial.print(" ");
//    //Serial.print("\tY to map= "); 
//    Serial.print(p.y);
//    Serial.print(" ");
//    //Serial.print("\tPressure = "); 
//    Serial.println(p.z-40);
//    }
//    {
//    Serial.print(p.x);
//    Serial.print(" ");
//    //Serial.print("\tY to map= "); 
//    Serial.print(p.y-1);
//    Serial.print(" ");
//    //Serial.print("\tPressure = "); 
//    Serial.println(p.z-40);
//    }
    // 압력의 정수값을 매핑 
    if(p.x >= 0 && p.x < 28 && p.y >= 0 && p.y <28){
        map_[p.x][p.y] = map(p.z,0,255,0,1);
      }

    //btn inf
//    if(p.x < 60 && p.y > 28){
//      for( i =0; i <28;i++){
//          for( j = 0; j <28;j++){
//              Serial.print(map_[i][j]);
//              Serial.print(", ");
//            }
//            Serial.print("\n");
//        }
//      }
      //btn init
    else if(p.x > 16 && p.y > 28)
      tft_init();


    }
    

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
