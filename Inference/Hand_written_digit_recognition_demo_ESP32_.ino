//필요 라이브러리 선언
#include <MLP_32_16_95.45_27.h>
#include <stdio.h>
#include <math.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
#include <TFT_eSPI.h>               // Hardware-specific library
// configuration for tft display only
// Calibrate values

//터치 입력 값 설정
#define point_high 0.95
#define point_low 0.65

#define WINPUT 784
#define HIDDEN1 32
#define HIDDEN2 16
#define WOUTPUT 10

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E3
#define CYAN    0x07FFi
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
TFT_eSPI tft = TFT_eSPI();          // Invoke custom library

// 2) configuration for touch screen read
#define SENSIBILITY 300
#define MINPRESSURE 10
#define MAXPRESSURE 3000

#define YP 14  // must be an analog pin, use "An" notation!
#define XM 12  // must be an analog pin, use "An" notation!
#define YM 22   // can be a digital pin
#define XP 21   // can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);

// optional
#define LCD_RESET A4
#define BOXSIZE 224

int flag = 0;
uint16_t map_[112][112] = {0,};

int16_t minx = 4087;
int16_t maxx = 0;
int16_t miny = 0;
int16_t maxy = -3072;
int xmincnt = 0;
int16_t mapx;
int16_t mapy;
int16_t mapz;

float input[WINPUT] = {0,};
void inference();

int16_t dx[8] = {1, 1, -1, -1};
int16_t dy[8] = {1, -1, 1, -1};

float conf_score = 0.00;
int16_t label = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.setRotation(0);
  tft.begin(0x9341); // Hardware driver define
  tft.fillScreen(BLACK);  // screen setting
  tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);  // Blank for digit

  tft.fillRect(8, 270, 80, 30, GREEN);    // buttons GUI G:: inference Y:: reset  
  tft.fillRect(150, 270, 80, 30, YELLOW);
  
  // text info
  tft.setCursor (10, 12);
  tft.setTextSize (2);
  tft.setTextColor(RED);
  tft.println("Hand written digit");
}

void loop() {

  TSPoint p = ts.getPoint();      // point instance(structer) 

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

    // x,y,z값 map 함수로 범위 scaling / x 0-240, y 0-320, z 양수
    p.z = abs(p.z);
    //지나치게 잦은 입력 방지 delay 1이 적당해보임
    delay(1);
    
    //입력 발생한 경우 동작문
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    
      //터치 입력 scailing
      p.x = map(p.x, -2872, 600, 0, tft.width());
      p.y = map(p.y, 400, 3800, 0, tft.height());

      //화면 필기 출력
      tft.fillCircle(p.x, p.y, 10, BLACK);

      //Serial.print("\tz: ");Serial.println(((p.z / 100)%100% 10));
      
      mapz = map(p.z,0,100,0,255)/255.0 * 1.0;
      mapx = map(p.x, 0, tft.width(), -2, 30);
      mapy = map(p.y, 0, tft.height(), -4, 36);

      //28*28 mapping 입력 부분
      //가운데 점 입력
      input[(mapy * 28) + mapx] = point_high;

      //대각선 방향 입력
       for(int i = 0; i < 4; i++){
            if(input[((mapy + dy[i]) * 28) + mapx + dx[i]] < point_low){
                input[((mapy + dy[i]) * 28) + mapx + dx[i]] = point_low;
              }
        }

      // 좌우 버튼 동작 [초기화 / 추론]
      if(p.x < 80 && p.y >255 && p.x > 0 )
        ESP.restart();
      if(p.x > 150 && p.y > 255)
        inference();
    }
}

float relu(float x)
{
    return (x > 0) * x;
}

void inference() {
    int inferenced_res = -1;

    float layer1[HIDDEN1] = { 0, };
    float layer2[HIDDEN2] = { 0, };
    float output[WOUTPUT] = { 0, };
    float max_out = 0;
    float exp_sum = 0;

    // ----------------- Inference ---------------------- //
    //if(label < 10){
    //input to layer1
    for (int x = 0; x < HIDDEN1; x++)
    {
        for (int y = 0; y < WINPUT; y++)
        {
            layer1[x] += w1[x][y] * input[y];
            //layer1[x] += w1[x][y] * input_ref[label][y];
        }
        layer1[x] += b1[x];
        layer1[x] = relu(layer1[x]);
    }
    //layer1 to layer2
    for (int x = 0; x < HIDDEN2; x++)
    {
        for (int y = 0; y < HIDDEN1; y++)
        {
            layer2[x] += w2[x][y] * layer1[y];
        }
        layer2[x] += b2[x];
        layer2[x] = relu(layer2[x]);
    }
    //layer2 to output
    for (int x = 0; x < WOUTPUT; x++)
    {
        for (int y = 0; y < HIDDEN2; y++)
        {
            output[x] += w3[x][y] * layer2[y];
        }
        output[x] += b3[x];
        output[x] = relu(output[x]);
    }
    //apply log_softmax to output
    for (int i = 0; i < WOUTPUT; i++)
    {
        if (max_out < output[i])
            max_out = output[i];
    }
    for (int i = 0; i < WOUTPUT; i++)
    {
        output[i] = (float)exp((double)output[i] - (double)max_out);
        exp_sum += output[i];
    }
    for (int i = 0; i < WOUTPUT; i++)
    {
        output[i] = output[i] / exp_sum;
    }

    Serial.println("\ninference!");
    //print output
    for (int i = 0; i < WOUTPUT; i++){
        Serial.print(i);
        Serial.print(" ");
        Serial.println(output[i]);
        
        if(output[i] > conf_score){
            conf_score = output[i];
            inferenced_res = i;
          }
    }
    // ----------------- Inference ---------------------- //

    
    // ----------- Action after inference --------------- //
    Serial.print("label : ");
    Serial.println(label);
    Serial.print("inferenced result : ");
    Serial.println(inferenced_res);

    //결과 출력 화면 3초
    tft.fillScreen(WHITE);  // screen setting
    delay(1000);
    
    tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);  // Blank for digit
    tft.fillRect(8, 30, BOXSIZE, BOXSIZE, WHITE);  // Blank for digit
    tft.setCursor (15, 80);
    tft.setTextSize (2);
    tft.setTextColor(RED);
    tft.println("Inferenced Class");
    
    tft.setCursor (100, 150);
    tft.setTextSize (20);
    tft.setTextColor(RED);
    tft.print(inferenced_res);
    delay(3000);
    inferenced_res = -1;
    conf_score = 0;
    // ----------- Action after inference --------------- //
    ESP.restart();
    return;
}
