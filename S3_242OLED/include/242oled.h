#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 37, 36); 

#define u8g2_logo_width 128
#define u8g2_logo_height 64

void fillScreen() {

  u8g2.clearBuffer();
  
  for(int i=0; i<128; i++) {
    for(int j=0; j<64; j++) {
      u8g2.drawPixel(i, j); 
    }
  }
  
  u8g2.sendBuffer();
  delay(100);
}

void pixelFill()
{
  // Randomly pick a pixel
  int x = random(0, 128);
  int y = random(0, 64);

  // Toggle pixel on/off  
  u8g2.drawPixel(x, y);

  // Display 
  u8g2.sendBuffer();
  
  // Delay
  //delay(10); 
}

void fonts()
{
  // Draw text using various fonts
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 10, "Hello, OLED!");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  u8g2.setFont(u8g2_font_profont12_tf);
  u8g2.drawStr(10, 10, "ProFont 12");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  u8g2.setFont(u8g2_font_timB10_tf);
  u8g2.drawStr(10, 10, "Tim10");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.drawStr(10, 10, "Helvetica 08");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

void ripples()
{
  int x = random(0, 128);
  int y = random(0, 64);

    for (int i = 0; i<128; i++)
    {
    // Draw a circle
      u8g2.drawCircle(x, y, i);
      //u8g2.drawCircle(64, 32, i);
      u8g2.sendBuffer();
      //delay(1);
      u8g2.clearBuffer();
    }
}

void speedRipples(int pause)
{
  int x = random(0, 128);
  int y = random(0, 64);

    for (int i = 0; i<128; i++)
    {
    // Draw a circle
      u8g2.drawCircle(x, y, i);
      //u8g2.drawCircle(64, 32, i);
      u8g2.sendBuffer();
      //delay(1);
      u8g2.clearBuffer();
      delay(pause);
    }
}

void setupOLED()
{
  //u8g2.setI2CAddress(121);
  u8g2.setBusClock(400000);
  u8g2.begin();
  // Clear buffer
  u8g2.clearBuffer();  
}
