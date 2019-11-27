

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include <nokialcd.h>

void SPI_MasterTransmit(char data) {
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while (!(SPSR & (1 << SPIF)));
}

void LCD_command(uint8_t cmd) {
	LCD_PORT &= ~(_BV(LCD_CE));
	LCD_PORT &= ~(_BV(LCD_DC));

	SPI_MasterTransmit(cmd);

	LCD_PORT |= _BV(LCD_CE);
}

void LCD_data(uint8_t dat) {
	LCD_PORT &= ~(_BV(LCD_CE));
	LCD_PORT |= _BV(LCD_DC);

	SPI_MasterTransmit(dat);

	LCD_PORT |= _BV(LCD_CE);
}

void LCD_init(void) {
	// SPI
	/* Set MOSI and SCK output, all others input */
	LCD_DDR = _BV(LCD_DC) | _BV(LCD_CE) | _BV(LCD_RST) | _BV(DD_MOSI) | _BV(DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16, CPOL=0 CPHA=0 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);


	LCD_PORT &= ~(_BV(LCD_RST));
	_delay_us(100);
	LCD_PORT |= _BV(LCD_RST);
	_delay_us(100);


	LCD_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
	LCD_command(PCD8544_SETBIAS | 0x03);
	LCD_command(PCD8544_FUNCTIONSET);

	LCD_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
	LCD_command(PCD8544_SETVOP | 0x3F);
	LCD_command(PCD8544_FUNCTIONSET);

	// normal mode
	LCD_command(PCD8544_FUNCTIONSET);

	// Set display to Normal
	LCD_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

void LCD_setPixel(uint8_t x, uint8_t y) {
	LCD_pixelBuffer[x + (y>>3)*84] |= _BV(y&0b111);
}

void LCD_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
   //upraveno
   //zdroj: http://www.edaboard.com/thread68526.html
   int16_t  x, y, addx, addy, dx, dy;
   int16_t P;
   int16_t i;

   dx = (x2 - x1);
	if(dx<0)dx*=-1;
   dy = (y2 - y1);
   if(dy<0)dy*=-1;

   x = x1;
   y = y1;

   if(x1 > x2)
      addx = -1;
   else
      addx = 1;
   if(y1 > y2)
      addy = -1;
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         LCD_setPixel(x, y);

         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         LCD_setPixel(x, y);

         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
      }
   }
}

void LCD_clear(void) {
	for(uint16_t i=0; i<84*6; ++i) {
		LCD_pixelBuffer[i] = 0x00;
	}
}

void LCD_showBuffer(void) {
	LCD_command(PCD8544_SETXADDR | 0x00);
	LCD_command(PCD8544_SETYADDR | 0x00 );
	for(uint16_t cnt=0; cnt<84*6; ++cnt) {
		LCD_data(LCD_pixelBuffer[cnt]);
	}		
}