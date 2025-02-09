#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    10
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int incomingByte = 0; //for incoming serial data

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    //currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    Serial.begin(9600); //open serial port set data rate to 9600 bps
    
}


void loop()
{
    //ChangePalettePeriodically();
    
    //SetupRedPalette();
    
    //static uint8_t startIndex = 0;
    //startIndex = startIndex + 1; /* motion speed */
    
    //FillLEDsFromPaletteColors(startIndex);
    
    SetColors(1);
    //FastLED.show();
    //Serial.print("here");
    while(true) {
        FastLED.show();
        if (Serial.available() > 0) {
            String incoming = Serial.readString();
            incomingByte = incoming.toInt();
            Serial.println(incomingByte);
            SetColors(incomingByte);
        }
    }
    //FastLED.show();
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void SetColors(int colorKey) {
    switch (colorKey) {
        case 1:
          SetupWhitePalette();
          break;
        case 2:
          SetupRedPalette();
          break;
        case 3:
          SetupGreenPalette();
          break;
        case 4:
          SetupBluePalette();
          break;
        case 5:
          SetupYellowPalette();
          break;
        case 6:
          SetupPurplePalette();
          break;
        case 7:
          SetupOrangePalette();
          break;
        case 8:
          SetupPinkPalette();
          break;
        case 9:
          SetupAquaPalette();
          break;
         default:
          break;
        
    }
    
    if (colorKey != 0) {
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */
    
      FillLEDsFromPaletteColors(startIndex);
    }
    
    }

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
;    currentPalette[12] = CRGB::White;
    
}


void SetupWhitePalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Gray;
    currentPalette[1] = CRGB::Gray;
    currentPalette[2] = CRGB::Gray;
    currentPalette[3] = CRGB::Gray;
    currentPalette[4] = CRGB::Gray;
    currentPalette[5] = CRGB::Gray;
    currentPalette[6] = CRGB::Gray;
    currentPalette[7] = CRGB::Gray;
    currentPalette[8] = CRGB::Gray;
    currentPalette[9] = CRGB::Gray;
    
}

void SetupRedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Red;
    currentPalette[1] = CRGB::Red;
    currentPalette[2] = CRGB::Red;
    currentPalette[3] = CRGB::Red;
    currentPalette[4] = CRGB::Red;
    currentPalette[5] = CRGB::Red;
    currentPalette[6] = CRGB::Red;
    currentPalette[7] = CRGB::Red;
    currentPalette[8] = CRGB::Red;
    currentPalette[9] = CRGB::Red;
    
    
}

void SetupGreenPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Green;
    currentPalette[1] = CRGB::Green;
    currentPalette[2] = CRGB::Green;
    currentPalette[3] = CRGB::Green;
    currentPalette[4] = CRGB::Green;
    currentPalette[5] = CRGB::Green;
    currentPalette[6] = CRGB::Green;
    currentPalette[7] = CRGB::Green;
    currentPalette[8] = CRGB::Green;
    currentPalette[9] = CRGB::Green;  
}

void SetupBluePalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Blue;
    currentPalette[1] = CRGB::Blue;
    currentPalette[2] = CRGB::Blue;
    currentPalette[3] = CRGB::Blue;
    currentPalette[4] = CRGB::Blue;
    currentPalette[5] = CRGB::Blue;
    currentPalette[6] = CRGB::Blue;
    currentPalette[7] = CRGB::Blue;
    currentPalette[8] = CRGB::Blue;
    currentPalette[9] = CRGB::Blue;  
}

void SetupYellowPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Yellow;
    currentPalette[1] = CRGB::Yellow;
    currentPalette[2] = CRGB::Yellow;
    currentPalette[3] = CRGB::Yellow;
    currentPalette[4] = CRGB::Yellow;
    currentPalette[5] = CRGB::Yellow;
    currentPalette[6] = CRGB::Yellow;
    currentPalette[7] = CRGB::Yellow;
    currentPalette[8] = CRGB::Yellow;
    currentPalette[9] = CRGB::Yellow;  
}

void SetupPurplePalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Purple;
    currentPalette[1] = CRGB::Purple;
    currentPalette[2] = CRGB::Purple;
    currentPalette[3] = CRGB::Purple;
    currentPalette[4] = CRGB::Purple;
    currentPalette[5] = CRGB::Purple;
    currentPalette[6] = CRGB::Purple;
    currentPalette[7] = CRGB::Purple;
    currentPalette[8] = CRGB::Purple;
    currentPalette[9] = CRGB::Purple;  
}

void SetupOrangePalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::DarkOrange;
    currentPalette[1] = CRGB::DarkOrange;
    currentPalette[2] = CRGB::DarkOrange;
    currentPalette[3] = CRGB::DarkOrange;
    currentPalette[4] = CRGB::DarkOrange;
    currentPalette[5] = CRGB::DarkOrange;
    currentPalette[6] = CRGB::DarkOrange;
    currentPalette[7] = CRGB::DarkOrange;
    currentPalette[8] = CRGB::DarkOrange;
    currentPalette[9] = CRGB::DarkOrange;  
}


void SetupPinkPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.

    CRGB CPink  = CRGB(255, 0, 166);
    
    currentPalette[0] = CPink;
    currentPalette[1] = CPink;
    currentPalette[2] = CPink;
    currentPalette[3] = CPink;
    currentPalette[4] = CPink;
    currentPalette[5] = CPink;
    currentPalette[6] = CPink;
    currentPalette[7] = CPink;
    currentPalette[8] = CPink;
    currentPalette[9] = CPink;  
}


void SetupAquaPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Aqua;
    currentPalette[1] = CRGB::Aqua;
    currentPalette[2] = CRGB::Aqua;
    currentPalette[3] = CRGB::Aqua;
    currentPalette[4] = CRGB::Aqua;
    currentPalette[5] = CRGB::Aqua;
    currentPalette[6] = CRGB::Aqua;
    currentPalette[7] = CRGB::Aqua;
    currentPalette[8] = CRGB::Aqua;
    currentPalette[9] = CRGB::Aqua;  
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
