#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "Graphics.h"

#define TFT_CS       A5
#define TFT_RST      -1
#define TFT_DC       A4

Adafruit_ST7789 lcd = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
const uint8_t lcdW = 240;
const uint8_t lcdH = 240;

enum Suit { HEART, DIAMOND, SPADE, CLUB };

unsigned long curMillis = 0;
unsigned long prevMillis = 0;
float delta = 0.0f;

bool redraw = true;

void setup() {
  lcd.init(lcdW, lcdH);
  lcd.setRotation(2);

  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);

  drawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 2, 3, 3);
  drawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 9, 2, 2);
  drawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 8, 1, 1);
  drawString("abcdefghijklmnopqrstuvwxyz", 3, 1, 12);
  drawString("0123456789.,!?'^$%&", 7, 1, 22);

  //drawSpriteRegion(Graphics::suitData, 2, 50, 29, 1, 84, 5, 5);
}

void loop() {
  draw();
}

void draw() {
  if (!redraw) return;

  // TODO: drawing routine

  // Confirm the draw by resetting the redraw flag
  redraw = false;
}

/*
void drawCard(byte x, byte y, uint8_t value, Suit suit) {
  // Draw card background
  for (int v = 0; v < Graphics::cardHeight; ++v) {
    for (int h = 0; h < Graphics::cardWidth - 2; ++h) {
      drawPixel(x + h + 1, y + v, Graphics::cardBGColour);
    }
  }
  for (int v = 0; v < Graphics::cardHeight - 2; ++v) {
    drawPixel(x, y + v + 1, Graphics::cardBGColour);
    drawPixel(x + Graphics::cardWidth - 1, y + v + 1, Graphics::cardBGColour);
  }

  // TODO: Draw card value

  // TODO: Draw card suit
  switch (suit) {
    case Suit::HEART:
      break;
    case Suit::DIAMOND:
      break;
    case Suit::SPADE:
      break;
    case Suit::CROSS:
      break;
  }
  // drawSprite(Graphics::heartData,
  //   x + Graphics::cardWidth - Graphics::cardPadding - Graphics::suitSize,
  //   y + Graphics::cardPadding,
  //   Graphics::suitSize, Graphics::suitSize);
}

void drawSprite(const uint8_t* indices, byte x, byte y, byte w, byte h) {
  drawSpriteRegion(indices, x, y, 0, 0, w, w, h);
}

uint8_t spriteIndex = 0;
void drawSpriteRegion(const uint8_t* indices, byte x, byte y,
  byte offset_x, byte offset_y, byte total_w, byte w, byte h) {
  for (int v = 0; v < h; ++v) {
    for (int h = 0; h < w; ++h) {
      spriteIndex = indices[(offset_y + v) * total_w + offset_x + h];
      if (spriteIndex > 0) {
        drawPixel(x + h, y + v, indices[v * total_w + h]);
      }
    }
  }
}
*/

void drawString(const char* str, uint8_t colour, byte x, byte y) {
  char c;
  uint index = 0;
  while (1) {
    c = str[index];
    if (c == '\0')
      break;
    x += drawChar(c, colour, x, y) + 1;
    ++index;
  }
}

uint8_t drawChar(char c, uint8_t colour, uint8_t x, uint8_t y) {
  uint data_x = 0;
  uint data_y = 0;
  uint width = 0;
  uint height = 0;
  uint baselineOffset = 0;

  if (c >= '0' && c <= '9') {
    char numberIndex = c - '0';
    data_x = Graphics::fontNumberInfo[numberIndex][0];
    data_y = Graphics::fontNumberInfo[numberIndex][1];
    width = Graphics::fontNumberInfo[numberIndex][2];
    height = Graphics::fontNumberInfo[numberIndex][3];
  } else if (c >= 'A' && c <= 'Z') {
    char upperIndex = c - 'A';
    data_x = Graphics::fontUpperInfo[upperIndex][0];
    data_y = Graphics::fontUpperInfo[upperIndex][1];
    width = Graphics::fontUpperInfo[upperIndex][2];
    height = Graphics::fontUpperInfo[upperIndex][3];
  } else if (c >= 'a' && c <= 'z') {
    char lowerIndex = c - 'a';
    data_x = Graphics::fontLowerInfo[lowerIndex][0];
    data_y = Graphics::fontLowerInfo[lowerIndex][1];
    width = Graphics::fontLowerInfo[lowerIndex][2];
    height = Graphics::fontLowerInfo[lowerIndex][3];
    baselineOffset = data_y - Graphics::fontUpperBaseline;
  } else {
    char symbolIndex = 0;
    switch (c) {
      case '.':  symbolIndex = 0; break;
      case ',':  symbolIndex = 1; break;
      case '!':  symbolIndex = 2; break;
      case '?':  symbolIndex = 3; break;
      case '\'': symbolIndex = 4; break;
      case '^':  symbolIndex = 5; break;
      case '$':  symbolIndex = 6; break;
      case '%':  symbolIndex = 7; break;
      case '&':  symbolIndex = 8; break;
    }
    data_x = Graphics::fontSymbolInfo[symbolIndex][0];
    data_y = Graphics::fontSymbolInfo[symbolIndex][1];
    width = Graphics::fontSymbolInfo[symbolIndex][2];
    height = Graphics::fontSymbolInfo[symbolIndex][3];
    baselineOffset = data_y - Graphics::fontSymbolBaseline;
  }

  for (int v = 0; v < height; ++v) {
    for (int h = 0; h < width; ++h) {
      if (Graphics::fontData[
        (v + data_y) * Graphics::fontDataWidth + h + data_x] > 0) {
        lcd.drawPixel(x + h, y + v + baselineOffset,
          Graphics::palette[colour]);
      }
    }
  }
  return width;
}