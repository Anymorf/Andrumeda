#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Fonts/FreeSerif18pt7b.h>  // Add a custom font FreeSerif12pt7b
#include <Fonts/FreeSerif12pt7b.h>  // Add a custom font FreeSerif12pt7b
#include <TFT_ILI9163C.h>

#include <Audio.h>

// Declare pins for the MUX CD74HC4067:
//#define EN 3
#define S0 4
#define S1 5
#define S2 6
#define S3 7 
#define SIG1 22   //  A8
#define SIG2 16   //  A8
#define READ_SIG1 analogRead(SIG1)
#define READ_SIG2 analogRead(SIG2)

#define Z1 22   //  A8
#define Z2 16   //  A2

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


// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=265,233.75
AudioOutputI2S           i2s1;           //xy=619,276
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, i2s1, 1);
//AudioControlSGTL5000     sgtl5000_1;     //xy=641.0000152587891,216.75000667572021
// GUItool: end automatically generated code

int Variable1;  // Create a variable to have something dynamic to show on the display
int treshold = 200;
float Vol, a = 0;		// volume
int pinValue = 0;
int count = 200;  // Create a variable for calibration TIMEs
bool state = true;

int atreshold()
{
  // Write to the display the text "Calibration":
  tft.setCursor(0, 1);  // Set position (x,y)
  tft.setTextColor(WHITE);  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println("Calibration");  // Print a text or value
  int b,c = 0;
  for (int i = 0; i < count; i++)
  {
    b = analogRead(22);
    c = c + b;
  }  
  int a22 = c / count;
  //int a22 = analogRead(22); 
  int avalue = map(a22, 0, 1023, 1023,0);
  tft.fillScreen(GREEN); // (BLACK);  // Fill screen with black
  return avalue;
}

void drawScreenADC()
{
  tft.fillScreen(); // (BLACK);  // Fill screen with black
	// Write to the display the text "Hello":
	tft.setCursor(0, 0);  // Set position (x,y)
	tft.setTextColor(WHITE, BLACK);  // Set color of text. First is the color of text and after is color of background
	tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
	tft.println("ADC-Screen");  // Print text
	
	// Draw line:
	tft.drawLine(0, 55, 127, 55, CYAN);  // Draw line (x0,y0,x1,y1,color)

	// Draw rectangle:
	tft.drawRect(0, 60, 60, 30, CYAN);  // Draw rectangle (x,y,width,height,color)
												 // It draws from the location to down-right							 
	// Draw rounded rectangle:
	tft.drawRoundRect(68, 60, 60, 30, 10, CYAN);  // Draw rounded rectangle (x,y,width,height,radius,color)

	return;
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
  //pinMode(EN, OUTPUT); 
  //digitalWrite(EN, LOW);
}
{// Audio setup:
  //AudioMemory(20);   
  //sgtl5000_1.enable();
  //sgtl5000_1.volume(0.6);
  //waveform1.begin(WAVEFORM_SAWTOOTH);
  ///////waveform1.amplitude(0.75);
  //waveform1.frequency(240.00);
  //waveform1.pulseWidth(0.15);

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

  tft.setRotation(2);  // Set orientation of the display. Values are from 0 to 3. If not declared, orientation would be 0,
                         // which is portrait mode.

  tft.setTextWrap(false);  // By default, long lines of text are set to automatically "wrap" back to the leftmost column.
                           // To override this behavior (so text will run off the right side of the display - useful for
                           // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                           // with setTextWrap(true).




  // We are going to print on the display everything that is static on the setup, to leave the loop free for dynamic elements:
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
  tft.println("Rev 0.20");  // Print a text or value  

  delay(5000);

  tft.fillScreen(); // (BLACK);  // Fill screen with black
}


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
	tft.println("5 ver 0.2");  // Print a text or value

	// Stop using a custom font:
	tft.setFont();  // Reset to standard font, to stop using any custom font previously set

	drawScreenADC();
/*
  // Draw line:
  tft.drawLine(0, 55, 127, 55, CYAN);  // Draw line (x0,y0,x1,y1,color)

  // Draw rectangle:
  tft.drawRect(0, 60, 60, 30, CYAN);  // Draw rectangle (x,y,width,height,color)
                                             // It draws from the location to down-right
                                             
  // Draw rounded rectangle:
  tft.drawRoundRect(68, 60, 60, 30, 10, CYAN);  // Draw rounded rectangle (x,y,width,height,radius,color)
                                                       // It draws from the location to down-right


  // Draw triangle:
  tft.drawTriangle(60,120,    70,94,    80,120, YELLOW);  // Draw triangle (x0,y0,x1,y1,x2,y2,color)


  // Draw filled triangle:
  tft.fillTriangle(100,120,    110,94,    120,120, CYAN);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)

  //  Draw circle:
  //tft.drawCircle(15, 124, 14, GREEN);  //  Draw circle (x,y,radius,color)

  // Draw rounded rectangle and fill:
  //tft.fillRoundRect(88, 28, 40, 27, 5, 0xF81B);  // Draw rounded filled rectangle (x,y,width,height,color)
 */ 
  digitalWrite(13, LOW);
}  // End of setup


float mapInput(int readValue)
{
  digitalWrite(13, HIGH);
  int avalue = map(readValue, 0, 1023, 1023,0);
  float result = ((avalue - treshold)/1023.00);
  Serial.println(result);
  return result;
}


int adc()
{
  digitalWrite(13, HIGH);
  int a22 = READ_SIG1;   //  A8
  int avalue = map(a22, 0, 1023, 1023,0);
  int result = (avalue - treshold);
  Serial.println(result);
  digitalWrite(13, LOW);
  return result;
}


void Draw_One()
{
  tft.fillScreen(GREEN); // (BLACK);  // Fill screen with black
	// try with Lines
	// Draw line:
	//tft.drawLine(0, 0, 127, 0, CYAN);  // Draw line (x0,y0,x1,y1,color)
  //for(int i = 0; i < 4; i ++)	
  //for (int Y = 0; Y == 127; 16)
	for (int Y = 1; Y <= 127; Y +=16)
  { 
		tft.drawLine(0, Y, 127, Y, CYAN);  // Draw line (x0,y0,x1,y1,color)
	}
	tft.drawLine(0, 0, 0, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)
	tft.drawLine(63, 0, 63, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)
	tft.drawLine(100, 0, 127, 127, CYAN);  // Draw line (x0,y0,x1,y1,color)
	return;
}


void Zet2()
{
  if(READ_SIG2 < 100)  // If Variable1 is less than 10...SIG2
  {
    state = false;
    tft.fillTriangle(100, 120, 110, 94, 120, 120, RED);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)
  }
  else
  {
    state = true;
    tft.fillTriangle(100, 120, 110, 94, 120, 120, CYAN);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)
  }
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
  int val = adc();

  //return the value
  return val;
}


void loop()  // Start of loop
{
  //Zet2();
 	if(READ_SIG2 < 100)  // If Variable1 is less than 10...SIG2
	{
		drawScreenTable();    
    // Draw triangle:
    tft.drawTriangle(60,120, 70,94, 80,120, YELLOW);  // Draw triangle (x0,y0,x1,y1,x2,y2,color)
    tft.fillTriangle(100, 120, 110, 94, 120, 120, RED);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)
	}
	else
	{    
		drawScreenADC();
    // Draw triangle:
    tft.drawTriangle(60,120, 70,94, 80,120, BLUE);  // Draw triangle (x0,y0,x1,y1,x2,y2,color)
    tft.fillTriangle(100, 120, 110, 94, 120, 120, CYAN);  // Draw filled triangle (x0,y0,x1,y1,x2,y2,color)
		
		Variable1 = adc();		// Convert Variable1 into a string, so we can change the text alignment to the right:
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
			tft.setCursor(75, 67);  // Set position (x,y)
			tft.setTextColor(GREEN, BLACK);  // Set color: First color of text and after is color of background
			tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
			Vol = mapInput(Variable1);
			tft.println(a);  // Print a text or value
		}
		
		{// текст внизу
			tft.setFont(&FreeSerif12pt7b);  // Set a custom font
			tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
			tft.setCursor(0, 120);  // Set position (x,y)
			tft.setTextColor(MAGENTA);  // Set color of text. We are using custom font so there is no background color supported
			tft.println(Variable1);  // Print Variable1
			tft.setFont();  // Reset to standard font, to stop using any custom font previously set
			// Draw a black square in the background to "clear" the previous text:
			// This is because we are going to use a custom font, and that doesn't support brackground color.
			//tft.fillRect(0, 90, 55, 34, BLACK);  // Draw filled rectangle (x,y,width,height,color)
		}
		//pinValue = analogRead(22);
		waveform1.amplitude(mapInput(Variable1));
		delay(10);
		
	}
}  // End of loop