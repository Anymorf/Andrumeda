#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Fonts/FreeSerif18pt7b.h>  // Add a custom font FreeSerif12pt7b
#include <Fonts/FreeSerif12pt7b.h>  // Add a custom font FreeSerif12pt7b
#include <TFT_ILI9163C.h>

#include <Audio.h>
//#include <SD.h>
#define REV "Rev 0.2.76"
// Declare pins for the MUX CD74HC4067:
#define S0 0
#define S1 1
#define S2 2
#define S3 3 
#define SIG1 22   //  A8
#define SIG2 16   //  A8
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

// Declare pins for the display:
#define TFT_CS     14
#define TFT_RST    10  // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_DC     15
//#define _TFTWIDTH      128//the REAL W resolution of the TFT
//#define _TFTHEIGHT    128//the REAL H resolution of the TFT

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
  pinMode(13, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
 
  digitalWrite(13, HIGH);
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);

{// MUX CD74HC4067 setup:
  pinMode(S0, OUTPUT); 
  pinMode(S1, OUTPUT); 
  pinMode(S2, OUTPUT); 
  pinMode(S3, OUTPUT);   

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  pinMode(SIG1, INPUT_PULLUP);    //  A8
  pinMode(SIG2, INPUT_PULLUP);    //  A8
  Serial.println("Setup");
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

  // We are going to print on the display everything that is static on the setup, to leave the loop free for dynamic elements:
  tft.fillScreen(); // (BLACK);  // Fill screen with black
	// Write to the display the text "Hello":
	tft.setCursor(0, 1);  // Set position (x,y)
	tft.setTextColor(WHITE);  // Set color of text. First is the color of text and after is color of background
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println("Andrumeda");  // Print a text or value

  // Start using a custom font:
	tft.setFont(&FreeSerif18pt7b);  // Set a custom font
	tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0

	// Write to the display the text "World":
	tft.setCursor(1, 50);  // Set position (x,y)
	tft.setTextColor(RED);  // Set color of text. We are using custom font so there is no background color supported
	tft.println(REV);  // Print a text or value

	// Stop using a custom font:
	tft.setFont();  // Reset to standard font, to stop using any custom font previously set

  digitalWrite(13, LOW);
  Serial.println("End of setup");
  tft.drawLine(-1, 0, -1, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)
}  // End of setup


float mapInput(int readValue)
{
  int avalue = map(readValue, 0, 1023, 1023,0);
  float result = ((avalue - treshold)/1023.00);
  return result;
}

float volume(int Value)
{
  //int avalue = map(Value, 0, 1023, 1023,0);  
  //float result = ((avalue - treshold)/1023 - treshold);
  float result = (Value / (1023.00 - treshold));
  return result;
}

int adc()
{
  //digitalWrite(13, HIGH);
  int a22 = READ_SIG1;   //  A8
  int avalue = map(a22, 0, 1023, 1023,0);
  int result = (avalue - treshold);
  //Serial.println(result);
  //digitalWrite(13, LOW);
  return result;
}


void Draw_One()
{
  tft.drawLine(-1, 0, -1, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)
  //tft.drawLine(0, 0, 0, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)  
  //tft.drawLine(1, 0, 1, 127, BLACK);  // Draw line (x0,y0,x1,y1,color)
  	// Write to the display the text "Hello":
	tft.setCursor(0, 4);  // Set position (x,y)
	tft.setTextColor(WHITE, BLACK);  // Set color of text. First is the color of text and after is color of background
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println("ADC-Screen");  // Print text
	
	// Draw line:
	tft.drawLine(0, 55, 127, 55, CYAN);  // Draw line (x0,y0,x1,y1,color)

	// Draw rectangle:
	tft.drawRect(0, 60, 60, 30, CYAN);  // Draw rectangle (x,y,width,height,color)
												 // It draws from the location to down-right							 
	// Draw rounded rectangle:
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
  for (i = 1; i < 9; i++)
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


void Draw_Four()
{
  
	return;
}


void Draw_Error()
{
    // We are going to print on the display everything that is static on the setup, to leave the loop free for dynamic elements:

	// Write to the display the text "Error":
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

  //read the value at the SIG pin
  //int val = READ_SIG1;
  int val = adc();

  //return the value
  return val;
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
  
  /*
  switch (channel) {
    case 0:    // your hand is on the sensor
      digitalWrite(S0, LOW);
      digitalWrite(S1, LOW);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 1:    // your hand is on the sensor
      digitalWrite(controlPin[1], muxChannel[channel][1]);
      digitalWrite(controlPin[2], muxChannel[channel][2]);
      digitalWrite(controlPin[3], muxChannel[channel][3]);
      digitalWrite(controlPin[4], muxChannel[channel][4]);
      break;
    case 2:    // your hand is close to the sensor
      digitalWrite(S0, LOW);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 3:    // your hand is a few inches from the sensor
      digitalWrite(S0, muxChannel[channel][1]);
      digitalWrite(S1, muxChannel[channel][2]);
      digitalWrite(S2, muxChannel[channel][3]);
      digitalWrite(S3, muxChannel[channel][4]);
      break;
    case 4:    // your hand is nowhere near the sensor
      digitalWrite(S0, LOW);
      digitalWrite(S1, LOW);
      digitalWrite(S2, HIGH);
      digitalWrite(S3, LOW);
      break;
    case 5:    // your hand is on the sensor
      digitalWrite(controlPin[1], muxChannel[channel][1]);
      digitalWrite(controlPin[2], muxChannel[channel][2]);
      digitalWrite(controlPin[3], muxChannel[channel][3]);
      digitalWrite(controlPin[4], muxChannel[channel][4]);
      break;
    case 6:    // your hand is close to the sensor
      digitalWrite(controlPin[1], muxChannel[channel][1]);
      digitalWrite(controlPin[2], muxChannel[channel][2]);
      digitalWrite(controlPin[3], muxChannel[channel][3]);
      digitalWrite(controlPin[4], muxChannel[channel][4]);
      break;
    case 7:    // your hand is a few inches from the sensor
      digitalWrite(controlPin[1], muxChannel[channel][1]);
      digitalWrite(controlPin[2], muxChannel[channel][2]);
      digitalWrite(controlPin[3], muxChannel[channel][3]);
      digitalWrite(controlPin[4], muxChannel[channel][4]);
      break;
    case 8:    // your hand is nowhere near the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 9:    // your hand is on the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 10:    // your hand is close to the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 11:    // your hand is a few inches from the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 12:    // your hand is nowhere near the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 13:    // your hand is on the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 14:    // your hand is close to the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 15:    // your hand is a few inches from the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 16:    // your hand is nowhere near the sensor
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
  }
  */
  //int val = adc();  
  //delay(5);
  //digitalWrite(13, LOW);
  //digitalWrite(13, LOW);
  //digitalWrite(13, LOW);
  //digitalWrite(13, LOW);
  //read the value at the SIG pin
  //int val = READ_SIG1;
  //int val = adc();

  //return the value
  //return val;
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
  RawMux(0);
  //Variable1 = readMux(0);		// Convert Variable1 into a string, so we can change the text alignment to the right:		
	Variable1 = adc();		// Convert Variable1 into a string, so we can change the text alignment to the right:
  Var = readMux(0);		// Convert Variable1 into a string, so we can change the text alignment to the right:

									// It can be also used to add or remove decimal numbers.
	char string[10];			// Create a character array of 10 characters
									// Convert float to a string:
	dtostrf(Variable1, 3, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)

		// We are going to print on the display everything that is dynamic on the loop, to refresh continuously:
	{// Левое окошко
			// Write to the display the Variable1 with left text alignment:
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
			// There is a problem when we go, for example, from 100 to 99 because it doesn't automatically write a background on
			// the last digit we are not longer refreshing. We need to check how many digits are and fill the space remaining.
	}
		  
	{//Правое окошко
		tft.setCursor(70, 67);  // Set position (x,y)
		tft.setTextColor(BLUE, BLACK);  // Set color: First color of text and after is color of background
		tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
		Vol = volume(Var);
    //Vol = mapInput(Variable1);
		tft.println(Vol);  // Print a text or value
	}
		
	{// текст внизу cлева
    // Draw a black square in the background to "clear" the previous text:
		// This is because we are going to use a custom font, and that doesn't support brackground color.
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
    // Draw a black square in the background to "clear" the previous text:
		// This is because we are going to use a custom font, and that doesn't support brackground color.
		//tft.fillRect(56, 90, 110, 34, BLACK);  // Draw filled rectangle (x,y,width,height,color)
		//tft.setFont(&FreeSerif12pt7b);  // Set a custom font
	//	tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
		tft.setCursor(40, 120);  // Set position (x,y)
		tft.setTextColor(GREEN);  // Set color of text. We are using custom font so there is no background color supported
		tft.println(ABC);  // Print Variable1
		//tft.setFont();  // Reset to standard font, to stop using any custom font previously set
	}
  EDF = readMux(2);		// Convert Variable1 into a string, so we can change the text alignment to the right:	
	{// текст внизу more справа
  // Draw a black square in the background to "clear" the previous text:
	// This is because we are going to use a custom font, and that doesn't support brackground color.
	//tft.fillRect(56, 90, 110, 34, BLACK);  // Draw filled rectangle (x,y,width,height,color)
	//tft.setFont(&FreeSerif12pt7b);  // Set a custom font
	//	tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
		tft.setCursor(80, 120);  // Set position (x,y)
		tft.setTextColor(YELLOW);  // Set color of text. We are using custom font so there is no background color supported
		tft.println(volume(EDF));  // Print Variable1
		tft.setFont();  // Reset to standard font, to stop using any custom font previously set
	}	
  //waveform1.amplitude(mapInput(Var));
  //pinValue = analogRead(SIG1);

	//waveform1.amplitude(volume(Var));

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
	  char string[8];			// Create a character array of 10 characters
	  dtostrf(var, 6, 2, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)

    tft.println(var);  // Print a text or value
    Y += 16;
    menuButton();
  }

    Y = 6;
    for (j = 8; j < 16; j++)
  {
    tft.fillRect(80, Y, 100, 8, BLACK);  // Draw filled rectangle (x,y,width,height,color)
    tft.setCursor(83, Y);  // Set position (x,y)

		var = readMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
	  char string[8];			// Create a character array of 10 characters
	  dtostrf(var, 6, 2, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)

    tft.println(var);  // Print a text or value

    Y += 16;
    menuButton();
  }
	return;
}

void Three()
{ // Set position (x,y)
  int Last = B;
  int j = 0;
  int var = 0;
  int Y = 6;
  for (j = 0; j < 8; j++)
  {
    //tft.fillRect(15, Y, 50, 8, BLACK);  // Draw filled rectangle (x,y,width,height,color)
    tft.setCursor(15, Y);  // Set position (x,y)
		tft.setTextColor(YELLOW, BLACK);  // Set color: First color of text and after is color of background
		tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)

    RawMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
    var = adc();				// It can be also used to add or remove decimal numbers.
	  char string[4];			// Create a character array of 10 characters
	  dtostrf(var, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
    tft.println(string);  // Print a text or value
    Y += 16;
    wait(500);
    if (Last != B)
    {
      return;
    }
  }

    Y = 6;
    for (j = 8; j < 16; j++)
  {
    //tft.fillRect(80, Y, 120, 8, BLACK);  // Draw filled rectangle (x,y,width,height,color)
    tft.setCursor(83, Y);  // Set position (x,y)

    RawMux(j);		// Convert Variable1 into a string, so we can change the text alignment to the right:
    var = adc();				// It can be also used to add or remove decimal numbers.
	  char string[4];			// Create a character array of 10 characters
	  dtostrf(var, 4, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
    tft.println(string);  // Print a text or value

    Y += 16;
    wait(500);
    if (Last != B)
    {
      return;
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