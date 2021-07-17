#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSerif18pt7b.h>  // Add a custom font FreeSerif12pt7b
#include <Fonts/FreeSerif12pt7b.h>  // Add a custom font FreeSerif12pt7b
#include <TFT_ILI9163C.h>
#include <Audio.h>
//#include <SD.h>

#include <pins.h>

#define REV "Rev 0.2.85"
#define READ_SIG1 analogRead(SIG1)
#define READ_SIG2 analogRead(SIG2)

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

// Create display:
TFT_ILI9163C tft = TFT_ILI9163C(TFT_CS, TFT_DC, TFT_RST);

#define LogoTime 250

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=265,233.75
AudioOutputI2S           i2s1;           //xy=619,276
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=641.0000152587891,216.75000667572021
// GUItool: end automatically generated code

int Var, ABC, EDF, Variable1;  // Create a variable to have something dynamic to show on the display
int treshold = 50;
float Vol, a = 0;		// volume
int pinValue = 0;
int Count = 200;  // Create a variable for calibration TIMEs
bool state, buttonState = false;
int MenuState, B = 1;   // Create a variable for menu button


int atreshold()
{
  int a22, avalue, i = 0;
  long c = 0;
  for (i = 0; i < Count; i++)
  {
    c += READ_SIG1;
    delay(5);
  }  
  a22 = c / Count;
  avalue = map(a22, 0, 1023, 1023,0);
  return avalue;
}


void setup()  // Start of setup
{
  Serial.begin(9600);
  Serial.println("Setup");  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
{// MUX CD74HC4067 setup:
  pinMode(S0, OUTPUT); 
  pinMode(S1, OUTPUT); 
  pinMode(S2, OUTPUT); 
  pinMode(S3, OUTPUT);   

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  pinMode(SIG1, INPUT_PULLUP);    //  A22
  pinMode(SIG2, INPUT_PULLUP);    //  A16
}
{// Audio setup:
  AudioMemory(20);   
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  /////waveform1.amplitude(0.75);
  waveform1.frequency(240.00);
  waveform1.pulseWidth(0.15);

  AudioMemory(4);
  AudioNoInterrupts();
  waveform1.begin(WAVEFORM_SAWTOOTH);
  waveform1.frequency(240.00);
  waveform1.pulseWidth(0.15);
  //sine1.frequency(440);
  //sine1.amplitude(0.5);
  AudioInterrupts();
}
{// Display setup:

  // Use this initializer if you're using a TFT
  tft.begin();  // Initialize 
  //tft.setRotation(rotation);
  tft.fillScreen(); // (BLACK);  // Fill screen with black

  tft.setRotation(0);  // Set orientation of the display. Values are from 0 to 3. If not declared, orientation would be 0,
  //tft.setRotation(2);  // Set orientation of the display. Values are from 0 to 3. If not declared, orientation would be 0,
                         // which is portrait mode.

  tft.setTextWrap(false);  // By default, long lines of text are set to automatically "wrap" back to the leftmost column.
                           // To override this behavior (so text will run off the right side of the display - useful for
                           // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                           // with setTextWrap(true).
}
{  // Hello Screen
  // Start using a custom font:
  tft.setFont(&FreeSerif12pt7b);  // Set a custom font
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(0, 20);  // Set position (x,y)
  tft.setTextColor(WHITE);  // Set color of text. First is the color of text and after is color of background
  tft.println("Sev");  // Print a text or value
  tft.setCursor(35, 35);  // Set position (x,y)
  tft.setTextColor(YELLOW);  // Set color of text. First is the color of text and after is color of background
  tft.println("Electo");  // Print a text or value
  tft.setCursor(70, 55);  // Set position (x,y)
  tft.setTextColor(GREEN);  // Set color of text. First is the color of text and after is color of background
  tft.println("Zvuk");  // Print a text or value

  // Start using a custom font:
  tft.setFont(&FreeSerif18pt7b);  // Set a custom font
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(0, 75);  // Set position (x,y)
  tft.setTextColor(RED);  // Set color of text. First is the color of text and after is color of background
  tft.println("Prezents");  // Print a text or value

  tft.setFont(&FreeSerif12pt7b);  // Set a custom font
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(0, 100);  // Set position (x,y)
  tft.setTextColor(WHITE);  // Set color of text. First is the color of text and after is color of background
  tft.println("Andrumeda 5");  // Print a text or value

	tft.setFont();
  // Start using a custom font:
  //tft.setFont(&FreeSerif18pt7b);  // Set a custom font
  tft.setTextSize(1);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(0, 120);  // Set position (x,y)
  tft.setTextColor(RED);  // Set color of text. First is the color of text and after is color of background
  tft.println(REV);  // Print a text or value  

  treshold = atreshold();

  delay(LogoTime);
}
{//First screen
  // We are going to print on the display everything that is static on the setup, to leave the loop free for dynamic elements:
  tft.fillScreen(); // (BLACK);  // Fill screen with black

  tft.drawLine(-1, 0, -1, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)
	tft.setCursor(0, 4);  // Set position (x,y)
	tft.setTextColor(WHITE, BLACK);  // Set color of text. First is the color of text and after is color of background
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println("ADC-Screen");  // Print text

	tft.setTextSize(1);  // Set text size. We are using custom font so you should always set text size as 0
	tft.setCursor(80, 30);  // Set position (x,y)
	tft.setTextColor(RED);  // Set color of text. We are using custom font so there is no background color supported
	tft.println(REV);  // Print a text or value
	
	// Draw line:
	tft.drawLine(0, 55, 127, 55, CYAN);  // Draw line (x0,y0,x1,y1,color)

	// Draw rectangle:
	tft.drawRect(0, 60, 60, 30, CYAN);  // Draw rectangle (x,y,width,height,color) draws from the location to down-right
	tft.drawRoundRect(64, 60, 60, 30, 10, CYAN);  // Draw rounded rectangle (x,y,width,height,radius,color)

  // Draw triangle:
  tft.drawTriangle(60,120, 70,94, 80,120, BLUE);  // Draw triangle (x0,y0,x1,y1,x2,y2,color)
  tft.fillTriangle(100, 120, 110, 94, 120, 120, CYAN);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)

  digitalWrite(13, LOW);
  Serial.println("End of setup");
  tft.drawLine(-1, 0, -1, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)
}  
}  // End of setup


float mapInput(int readValue)
{
  int avalue = map(readValue, 0, 1023, 1023,0);
  float result = ((avalue - treshold)/1023.00);
  return result;
}


float volume(int Value)
{
  float result = (Value / (1023.00 - treshold));
  return result;
}


int adc()
{
  int a22 = READ_SIG1;   //  A8
  int avalue = map(a22, 0, 1023, 1023,0);
  int result = (avalue - treshold);
  return result;
}


int readMux(int channel)
{
  int controlPin[] = {S0, S1, S2, S3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  int val = adc();
  return val;  //return the value
}


void RawMux(int channel)
{
  int controlPin[] = {S0, S1, S2, S3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  digitalWrite(controlPin[0], muxChannel[channel][0]);
  digitalWrite(controlPin[1], muxChannel[channel][1]);
  digitalWrite(controlPin[2], muxChannel[channel][2]);
  digitalWrite(controlPin[3], muxChannel[channel][3]);
  
  return;
}


bool readEncMux(int channel)
{
  RawMux(channel);		// Convert Variable1 into a string, so we can change the text alignment to the right:
  bool enc = digitalRead(SIG2);
  return enc;
}

void setEncodersUp() {
//Encoder Test Part
// mux var = readEncMux(j);
  e1AOutPrev = readEncMux(3);   // (ENCODER_1_A);
  e1SOutPrev = readEncMux(5);   // (ENCODER_1_S);
  //e1AOutPrev = digitalRead(ENCODER_1_A);
  //e1SOutPrev = digitalRead(ENCODER_1_S);
 
  e2AOutPrev = digitalRead(ENCODER_2_A);
  e2SOutPrev = digitalRead(ENCODER_2_S);
 /*
  e3AOutPrev = digitalRead(ENCODER_3_A);
  e3SOutPrev = digitalRead(ENCODER_3_S);

  e4AOutPrev = digitalRead(ENCODER_4_A);
  e4SOutPrev = digitalRead(ENCODER_4_S);

  e5AOutPrev = digitalRead(ENCODER_5_A);
  e5SOutPrev = digitalRead(ENCODER_5_S);*/
}

void processEncoders() {
  if (encoderProcessingDelayCounter > 0) {
    encoderProcessingDelayCounter--;
    return;
  } else {
    //encoderProcessingDelayCounter = ENCODER_PROCESSING_DELAY;
  }


  //  ----------###   1   ###----------

  e1AOut = readEncMux(3);
  if (e1AOut != e1AOutPrev){
    if (readEncMux(4) != e1AOut) {
      counter1--;
    } else {
      counter1++;
    }
    if ((counter1 / 2.00) == (counter1 / 2)){
      if ((counter1 / 2) != encoder1Position) {
        // process encoder 1 rotation behavior here. probably using  if ((counter1 / 2) > encoder1Position)
        Serial.print("Encoder 1 is at ");
        Serial.println(counter1 / 2);
        
        encoder1Position = (counter1 / 2);
      }
    } 
  }
  e1AOutPrev = e1AOut;
  
  e1SOut = readEncMux(5);
  if (e1SOut != e1SOutPrev) {
    // process encoder 1 button behavior here
    if (e1SOut) {
      Serial.println("Encoder 1 is released");
    } else {
      Serial.println("Encoder 1 is pushed down");
    }
  }
  e1SOutPrev = e1SOut;


  //  ----------###   2   ###----------

  e2AOut = digitalRead(ENCODER_2_A);
  if (e2AOut != e2AOutPrev){
    if (digitalRead(ENCODER_2_B) != e2AOut) {
      counter2--;
    } else {
      counter2++;
    }
    Serial.print("Encoder 2 is at ");
    Serial.println(counter2);
    /*
    if ((counter2 / 2.00) == (counter2 / 2)){
      if ((counter2 / 2) != encoder2Position) {
        // process encoder 2 rotation behavior here. probably using  if ((counter2 / 2) > encoder2Position)
        Serial.print("Encoder 2 is at ");
        Serial.println(counter2 / 2);
        
        encoder2Position = (counter2 / 2);
      }
    } */
  }
  
  e2AOutPrev = e2AOut;
  
  e2SOut = digitalRead(ENCODER_2_S);
  if (e2SOut != e2SOutPrev) {
    // process encoder 2 button behavior here
    if (e2SOut) {
      Serial.println("Encoder 2 is released");
    } else {
      Serial.println("Encoder 2 is pushed down");
    }
  }
  e2SOutPrev = e2SOut;

/*
  //  ----------###   3   ###----------

  e3AOut = digitalRead(ENCODER_3_A);
  if (e3AOut != e3AOutPrev){
    if (digitalRead(ENCODER_3_B) != e3AOut) {
      counter3--;
    } else {
      counter3++;
    }
    if ((counter3 / 2.00) == (counter3 / 2)){
      if ((counter3 / 2) != encoder3Position) {
        // process encoder 3 rotation behavior here. probably using  if ((counter2 / 2) > encoder2Position)
        Serial.print("Encoder 3 is at ");
        Serial.println(counter3 / 2);
        
        encoder3Position = (counter3 / 2);
      }
    } 
  }
  
  e3AOutPrev = e3AOut;
  
  e3SOut = digitalRead(ENCODER_3_S);
  if (e3SOut != e3SOutPrev) {
    // process encoder 3 button behavior here
    if (e3SOut) {
      Serial.println("Encoder 3 is released");
    } else {
      Serial.println("Encoder 3 is pushed down");
    }
  }
  e3SOutPrev = e3SOut;


  //  ----------###   4   ###----------

  e4AOut = digitalRead(ENCODER_4_A);
  if (e4AOut != e4AOutPrev){
    if (digitalRead(ENCODER_4_B) != e4AOut) {
      counter4--;
    } else {
      counter4++;
    }
    if ((counter4 / 2.00) == (counter4 / 2)){
      if ((counter4 / 2) != encoder4Position) {
        // process encoder 4 rotation behavior here. probably using  if ((counter2 / 2) > encoder2Position)
        Serial.print("Encoder 4 is at ");
        Serial.println(counter4 / 2);
        
        encoder4Position = (counter4 / 2);
      }
    } 
  }
  
  e4AOutPrev = e4AOut;
  
  e4SOut = digitalRead(ENCODER_4_S);
  if (e4SOut != e4SOutPrev) {
    // process encoder 4 button behavior here
    if (e4SOut) {
      Serial.println("Encoder 4 is released");
    } else {
      Serial.println("Encoder 4 is pushed down");
    }
  }
  e4SOutPrev = e4SOut;


  //  ----------###   5   ###----------

  e5AOut = digitalRead(ENCODER_5_A);
  if (e5AOut != e5AOutPrev){
    if (digitalRead(ENCODER_5_B) != e5AOut) {
      counter5--;
    } else {
      counter5++;
    }
    if ((counter5 / 2.00) == (counter5 / 2)){
      if ((counter5 / 2) != encoder5Position) {
        // process encoder 5 rotation behavior here. probably using  if ((counter2 / 2) > encoder2Position)
        Serial.print("Encoder 5 is at ");
        Serial.println(counter5 / 2);
        
        encoder5Position = (counter5 / 2);
      }
    } 
  }
  
  e5AOutPrev = e5AOut;
  
  e5SOut = digitalRead(ENCODER_5_S);
  if (e5SOut != e5SOutPrev) {
    // process encoder 5 button behavior here
    if (e5SOut) {
      Serial.println("Encoder 5 is released");
    } else {
      Serial.println("Encoder 5 is pushed down");
    }
  }
  e5SOutPrev = e5SOut;
  */
}


void Draw_One()
{
  tft.drawLine(-1, 0, -1, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)
	tft.setCursor(0, 4);  // Set position (x,y)
	tft.setTextColor(WHITE, BLACK);  // Set color of text. First is the color of text and after is color of background
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println("ADC-Screen");  // Print text
	
	// Draw line:
	tft.drawLine(0, 55, 127, 55, CYAN);  // Draw line (x0,y0,x1,y1,color)

	// Draw rectangle:
	tft.drawRect(0, 60, 60, 30, CYAN);  // Draw rectangle (x,y,width,height,color) draws from the location to down-right
	tft.drawRoundRect(64, 60, 60, 30, 10, CYAN);  // Draw rounded rectangle (x,y,width,height,radius,color)

  // Draw triangle:
  tft.drawTriangle(60,120, 70,94, 80,120, BLUE);  // Draw triangle (x0,y0,x1,y1,x2,y2,color)
  tft.fillTriangle(100, 120, 110, 94, 120, 120, CYAN);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)  
	return;
}


void Draw_Two()
{
  // try with Lines
	// Draw line:
  tft.drawLine(-1, -1, 127, -1, CYAN);  // Draw line (x0,y0,x1,y1,color)
	for (int Y = 0; Y < 127; Y +=16)
  { 
		tft.drawLine(-1, Y, 127, Y, CYAN);  // Draw line (x0,y0,x1,y1,color)
	}
  tft.drawLine(-1, 127, 127, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)

	tft.drawLine(-1, 0, -1, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)
	tft.drawLine(63, 0, 63, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)
	tft.drawLine(126, 0, 126, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)

  // Set Numbers position (x,y)
  int i = 1;
  int Y = 6;
  for (i = 1; i < 9; i++)
  {
    tft.setCursor(5, Y);  // Set position (x,y)
		tft.setTextColor(GREEN, BLACK);  // Set color: First color of text and after is color of background
		tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
		tft.println(i);  // Print a text or value
     Y += 16;
  }

    Y = 6;
    for (i = 9; i < 17; i++)
  {
    tft.setCursor(68, Y);  // Set position (x,y)
		//tft.setTextColor(GREEN, BLACK);  // Set color: First color of text and after is color of background
		//tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
		tft.println(i);  // Print a text or value
    //X = -1;
    Y += 16;
  }
	return;
}


void Draw_Three()
{
  // try with Lines
	// Draw line:
  int Color = GREEN;
  tft.drawLine(-1, -1, 127, -1, Color);  // Draw line (x0,y0,x1,y1,color)
	for (int Y = 0; Y < 127; Y +=16)
  { 
		tft.drawLine(-1, Y, 127, Y, Color);  // Draw line (x0,y0,x1,y1,color)
	}
  tft.drawLine(-1, 127, 127, 127, Color);  // Draw line (x0,y0,x1,y1,color)

	tft.drawLine(-1, 0, -1, 127, Color);  // Draw line (x0,y0,x1,y1,color)
	tft.drawLine(63, 0, 63, 127, Color);  // Draw line (x0,y0,x1,y1,color)
	tft.drawLine(126, 0, 126, 127, Color);  // Draw line (x0,y0,x1,y1,color)

  // Set position (x,y)
  int i = 1;
  //int X = 5;
  int Y = 6;
  for (i = 0; i < 8; i++)
  {
    tft.setCursor(5, Y);  // Set position (x,y)
		tft.setTextColor(CYAN, BLACK);  // Set color: First color of text and after is color of background
		tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
		tft.println(i);  // Print a text or value
     Y += 16;
  }
  	//tft.drawLine(63, 0, 63, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)
    //X = 68;
    Y = 6;
    for (i = 8; i < 16; i++)
  {
    tft.setCursor(68, Y);  // Set position (x,y)
		//tft.setTextColor(GREEN, BLACK);  // Set color: First color of text and after is color of background
		//tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
		tft.println(i);  // Print a text or value
    //X = -1;
    Y += 16;
  }
	return;
  
}


void Draw_Four()
{
  
	return;
}


void Draw_Error()
{
	tft.setCursor(0, 1);  // Set position (x,y)
	tft.setTextColor(WHITE);  // Set color of text. First is the color of text and after is color of background
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println("Error");  // Print a text or value  
  
	// Start using a custom font:
	tft.setFont(&FreeSerif18pt7b);  // Set a custom font
	tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0

	// Write to the display the text "World":
	tft.setCursor(1, 50);  // Set position (x,y)
	tft.setTextColor(RED);  // Set color of text. We are using custom font so there is no background color supported
	tft.println("5 ver 0.21");  // Print a text or value

	// Stop using a custom font:
	tft.setFont();  // Reset to standard font, to stop using any custom font previously set
	return;
}


void menuButton()
{
  RawMux(0);
  buttonState = digitalRead(SIG2);
  if (buttonState == LOW) {
    if (B >= 3) {   // 4  указывает количество пунктов меню
      B = 1;
    } else {
      B = B + 1;
    }
    tft.fillScreen(); // (BLACK);  // Fill screen with black
    switch (B) {
      case 1:    // your hand is on the sensor
        Serial.println("Draw_One");
        Draw_One();
        break;
      case 2:    // your hand is close to the sensor
        Serial.println("Draw_Two");
        Draw_Two();
        break;
      case 3:    // your hand is a few inches from the sensor
        Serial.println("Draw_Three");
        Draw_Three();
        break;
      case 4:    // your hand is nowhere near the sensor
        Serial.println("Draw_Four");
        Draw_Four();
        break;
      default:
        Draw_Error();
    }
    delay(500);
  }
	return;
}


void wait(int times)
{
  times = times / 10;
  while (times > 0)
  {
    times-=10;
    menuButton();
    delay(10);
  }
  
  
}


void One()
{
    // Start using a custom font:
  tft.fillRect(0, 30, 127, 25, BLACK);  // Draw filled rectangle (x,y,width,height,color) 
	tft.setFont(&FreeSerif12pt7b);  // Set a custom font
	tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0

	// Write to the display the text "World":
	tft.setCursor(1, 50);  // Set position (x,y)
	tft.setTextColor(RED);  // Set color of text. We are using custom font so there is no background color supported
	tft.println(encoder1Position);  // Print a text or value

	// Stop using a custom font:
	tft.setFont();  // Reset to standard font, to stop using any custom font previously set

  processEncoders();
  RawMux(0);	
	Variable1 = adc();		// Convert Variable1 into a string, so we can change the text alignment to the right:
  Var = readMux(0);		// Convert Variable1 into a string, so we can change the text alignment to the right:

	{// Левое окошко
	tft.setCursor(10, 67);			// Set position (x,y)
	tft.setTextColor(WHITE, BLACK);	// Set color First text and after background
	if(treshold < 50)				// If Variable1 is less than 50...
	  {
			tft.setTextColor(GREEN);		// Set color First text and after background
		}
	if(treshold >= 50)				// If Variable1 is more than 50...
		{
			tft.setTextColor(RED);			// Set color First text and after background
		}
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println(treshold);  // Print a text or value
	}
		  
	{//Правое окошко
		tft.setCursor(70, 67);  // Set position (x,y)
		tft.setTextColor(BLUE, BLACK);  // Set color: First color of text and after is color of background
		tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
		Vol = volume(Var);
		tft.println(Vol);  // Print a text or value
	}
		
	{// текст внизу cлева
		tft.fillRect(0, 90, 127, 37, BLACK);  // Draw filled rectangle (x,y,width,height,color)
		tft.setFont(&FreeSerif12pt7b);  // Set a custom font
		tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
		tft.setCursor(0, 120);  // Set position (x,y)
		tft.setTextColor(MAGENTA);  // Set color of text. We are using custom font so there is no background color supported
		tft.println(Variable1);  // Print Variable1
		//tft.setFont();  // Reset to standard font, to stop using any custom font previously set
	}
  ABC = readMux(1);		// Convert Variable1 into a string, so we can change the text alignment to the right:	
	{// текст внизу справа
		tft.setCursor(40, 120);  // Set position (x,y)
		tft.setTextColor(GREEN);  // Set color of text. We are using custom font so there is no background color supported
		tft.println(ABC);  // Print Variable1
		//tft.setFont();  // Reset to standard font, to stop using any custom font previously set
	}
  EDF = readMux(2);		// Convert Variable1 into a string, so we can change the text alignment to the right:	
	{// текст внизу more справа
    tft.setCursor(80, 120);  // Set position (x,y)
		tft.setTextColor(YELLOW);  // Set color of text. We are using custom font so there is no background color supported
		tft.println(volume(EDF));  // Print Variable1
		tft.setFont();  // Reset to standard font, to stop using any custom font previously set
	}	
  waveform1.amplitude(volume(Var));
	//delay(10);  

	return;
}

void Two()
{
  // Set position (x,y)
  int j = 0;
  int var = 0;
  int Y = 6;
  for (j = 0; j < 8; j++)
  {
    tft.fillRect(15, Y, 40, 8, BLACK);  // Draw filled rectangle (x,y,width,height,color)
    tft.setCursor(15, Y);  // Set position (x,y)
		tft.setTextColor(YELLOW, BLACK);  // Set color: First color of text and after is color of background
		tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)

    var = readMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
	  char string[4];			// Create a character array of 10 characters
	  dtostrf(var, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
    tft.println(var);  // Print a text or value
    Y += 16;
    //menuButton();
  }

    Y = 6;
    for (j = 8; j < 16; j++)
  {
    tft.fillRect(80, Y, 39, 8, BLACK);  // Draw filled rectangle (x,y,width,height,color)
    tft.setCursor(83, Y);  // Set position (x,y)

		var = readMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
	  char string[4];			// Create a character array of 10 characters
	  dtostrf(var, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
    tft.println(var);  // Print a text or value

    Y += 16;
    menuButton();
  }
	return;
}

void Three()
{ // Reading Encoders data
  pinValue = readMux(0);
  while (pinValue < 500)
  {
    pinValue = readMux(0);
    int j = 0;
    int var = 0;
    int Y = 6;
    tft.setTextColor(YELLOW, BLACK);  // Set color: First color of text and after is color of background
    tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
    for (j = 0; j < 8; j++)
    {
      tft.setCursor(15, Y);  // Set position (x,y)   
      var = readEncMux(j);
      //RawMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
      //var = digitalRead(SIG2);
      char string[4];			// Create a character array of 10 characters
      dtostrf(var, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
      tft.println(string);  // Print a text or value
      Y += 16;
    }
    pinValue = readMux(0);
    Y = 6;
    for (j = 8; j < 16; j++)
    {
      tft.setCursor(83, Y);  // Set position (x,y)
      var = readEncMux(j);
      //RawMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
      //var = digitalRead(SIG2);
      char string[4];			// Create a character array of 10 characters
      dtostrf(var, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
      tft.println(string);  // Print a text or value
      Y += 16;
    }
  }
	return;
}

void Four()
{
  
	return;
}

void Error()
{
  
	return;
}

void loop()  // Start of loop
{
  //Serial.println("loop");
  menuButton();
  switch (B) {
    case 1:    // your hand is on the sensor
      One();
      break;
    case 2:    // your hand is close to the sensor
      Two();
      break;
    case 3:    // your hand is a few inches from the sensor
      Three();
      break;
    case 4:    // your hand is nowhere near the sensor
      Four();
      break;
    default:
      Error();
  }

}  // End of loop