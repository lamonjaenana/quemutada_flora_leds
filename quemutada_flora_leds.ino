
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//Leer esto para activar las luces por movimiento:
//https://learn.adafruit.com/3d-printed-flora-band/circuit-diagram

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Perlin.h>
#include <EEPROM.h>

#define PIN 6
#define NUMLEDS 10
#define NUMCANCIONES 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_LSM303 lsm;

Perlin perlin(255);

float fTimer;
int heartbeat = 0;
int state;

void setup() {

  strip.begin();
  strip.show(); 

  Serial.begin(9600);

  if (!lsm.begin()) {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }
  
  state = EEPROM.read(0);
  if (state <= 0 || state >= NUMCANCIONES) {
    state = 1;
  } else {
    state++;
  }
  EEPROM.write(0, state);
  
  for (char x=0; x<min(strip.numPixels(), (state>8?state-8:state));x++){
    if (state>8) {
      strip.setPixelColor(x, 0, 0, 75);
    } else {
      strip.setPixelColor(x, 0, 50, 0);
    }
  }
  strip.show();
  delay(1000);
  
  for (char x=0; x<strip.numPixels();x++){
    strip.setPixelColor(x, 0, 0, 0);
  }
  strip.show();  
  
}

void loop() {

  switch(state) {
  case 1: 
    humoEspejos();
    break;  
  case 2: 
    casaMunecas();
    break;  
  case 3: 
    noMasRobots();
    break;  
  case 4: 
    enamoreRobot();
    break;  
  case 5: 
    amorCuantico();
    break;  
  case 6: 
    lovecraft();
    break;   
  case 7: 
    numeros();
    break;
  case 8: 
    perkyPat();
    break;  
  case 9: 
    cancionAmor();
    break;  
  case 10: 
    dinosaurio();
    break;  
  case 11: 
    inocentes();
    break;  
   case 12: 
    comun();
    break;     
  }

  strip.show(); 

}








void perlinLine(float delta, float dampen, float scale, float rscale, float gscale, float bscale){

  float pr,pg,pb;
  int r,g,b;

  for (char x=0; x<strip.numPixels();x++){

    pr = perlin.perlin2d(   x*scale, 1 + fTimer);
    pg = perlin.perlin2d(17+x*scale, 2 - fTimer);      
    pb = perlin.perlin2d(11+x*scale, 3 + fTimer);            
    r= perlin.dampen(pr,dampen) * rscale;
    g= perlin.dampen(pg,dampen) * gscale;      
    b= perlin.dampen(pb,dampen) * bscale;      

    strip.setPixelColor(x, r, g, b);  

  }

  fTimer += delta;

}



void perlinRain(float delta, float dampen, float scale, float rscale){

  for (char x=0; x<strip.numPixels();x++){

    float fperlin = perlin.perlin2d(x*scale+fTimer,perlin.perlin2d(fTimer,10));
    int r = perlin.limit(fperlin,dampen) * rscale;                  

    strip.setPixelColor(x, r, r, r);      

  }

  fTimer += delta;

}



void sineLed(float delta, float scale, float rscale, float gscale, float bscale){

  double sr,sg,sb;
  int r,g,b; 

  for (char x=0; x<strip.numPixels();x++){

    sr = sin(x*scale+fTimer);
    sg = sin(x*scale+fTimer);
    sb = sin(x*scale+fTimer);

    r = sr < 0.8 ? 0 : map(sr*1000,800,1000,0,255) * rscale;  
    g = sg < 0.8 ? 0 : map(sg*1000,800,1000,0,255) * gscale;  
    b = sb < 0.8 ? 0 : map(sb*1000,800,1000,0,255) * bscale;  

    //Serial.print(r);   
    //Serial.print(" ");   


    strip.setPixelColor(x, r, g, b);      

  }

  fTimer += delta;

}


void delayDinamico(int slow, int fast) {

  int offCounter = 0;
  for (char x=0; x<strip.numPixels();x++){
    offCounter += strip.getPixelColor(x)==0 ? 0 : 1;
  }


  if (offCounter == 0) {
    Serial.println("*");
  }

  delay (offCounter > 0 ? slow : fast);

}



void amorCuantico() {

  int normMillis = millis() % 1050;
  int r = 0;

  if (normMillis < 75) {
    r = 255;
  } 
  else if (normMillis > 375 && normMillis < 450){
    r = 200;
  } 
  else {
    r = 0;
  }

  r = heartbeat * 0.75 + r * 0.25;
  heartbeat = r;

  for (char x=0; x<strip.numPixels();x++){
    strip.setPixelColor(x, r, 0, 0);      
  }

  delay(12);

}



void casaMunecas() {
  perlinLine(0.0025, 0.25, 0.1, 0, 0, 1);
  delayDinamico(10,5);
}



void enamoreRobot() {
  perlinLine(0.0025, 0.075, 0.1, 1, 0, 0);
  delayDinamico(10,1);
}



void numeros(){
  perlinLine(0.004, 0.03, 0.1, 1, 0.8, 1);
  delayDinamico(10,1);
}



void cancionAmor(){
  perlinLine(0.001, 0.03, 1, 1, 0.8, 1);
  delayDinamico(10,1);
}



void dinosaurio(){
  perlinLine(0.006, 0.1, 0.05, 0.5, 0.5, 0.5);
  delayDinamico(8,1);
}



void inocentes(){
  perlinRain(0.01, 0.4, 0.2, 0.5);
  delayDinamico(10,1);
}  



void humoEspejos(){
  perlinRain(0.02, 0.3, 0.6, 0.85);
  delayDinamico(10,1);
}  



void perkyPat() {
  sineLed(-0.062, 0.6, 1, 0, 0);
  delay(9);
}



void noMasRobots(){
  sineLed(0.2, 0, 0.5, 0.5, 0.5);
  delay(12);
}



void lovecraft() {

  lsm.read();

  int r = 0;

  if (lsm.accelData.y > 1800) {
    r= 255;
  }

  for (char x=0; x<strip.numPixels();x++){
    strip.setPixelColor(x, r/2, r/2, r/2);      
  }
  strip.show(); 
  
  delay(r);

  //    Serial.print("X: "); 
  //    Serial.print((int)lsm.accelData.y); 
  //    Serial.print(" Y: "); 
  //    Serial.print((int)lsm.accelData.y);   
  //    Serial.print(" Z: "); 
  //    Serial.println((int)lsm.accelData.z);   


  //  
  //  Serial.print("Mag X: "); 
  //  Serial.print((int)lsm.magData.x);
  //  Serial.print(" Y: "); 
  //  Serial.print((int)lsm.magData.y);      
  //  Serial.print(" Z: "); 
  //  Serial.println((int)lsm.magData.z);   

}


void comun(){
  
  float pr,pg,pb;
  int r,g,b;

  pr = perlin.perlin2d(1 + fTimer, 1 + fTimer);
  pg = perlin.perlin2d(1, 2 - fTimer);      
  pb = perlin.perlin2d(1, 3 + fTimer);            
  r = perlin.dampen(pr,0.4) * 0.25;
  g = perlin.dampen(pg,0.4) * 0.25;      
  b = perlin.dampen(pb,0.4) * 0.25;      

  for (char x=0; x<strip.numPixels();x++){
    strip.setPixelColor(x, r, g, b);  
  }

  fTimer += 0.002;
  delay(10);

}



