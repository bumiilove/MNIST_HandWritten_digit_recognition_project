#include <stdio.h>
#include <math.h>
float relu(float x);
void inference();

void setup() {
  Serial.begin(9600);
}

void loop() {
  inference();
  delay(3000);
}

float relu(float x)
{
  return (x > 0) * x;
}

void inference() {
  int inferenced_res = -1;
  float weight[5][10] = {
  {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 
  {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 
  {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}
  };
  float bias[5] = {0.1, 0.1, 0.1, 0.1, 0.1};
  float input[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
  float output[5] = { 0, };
  float max_out = 0;
  float exp_sum = 0;
  
  //input to output
  for (int x = 0; x < 5; x++)
  {
      for (int y = 0; y < 10; y++)
      {
          output[x] += weight[x][y] * input[y];
      }
      output[x] += bias[x];
      output[x] = relu(output[x]);
  }

  /*
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
  */
  
  //print output
  Serial.println("\ninference!");
  for (int i = 0; i < 5; i++){
      Serial.print(i);
      Serial.print(" ");
      Serial.println(output[i]);
   }
}
