#include "ssd1351.h"

#if LUN1K_VERSION == 10
const uint32_t portc_map[64]= {
	3145728,2097168,1048608,48,3145728,2097168,1048608,48,3145728,2097168,1048608,48,3145728,
	2097168,1048608,48,3145728,2097168,1048608,48,3145728,2097168,1048608,48,3145728,2097168,
	1048608,48,3145728,2097168,1048608,48,3145728,2097168,1048608,48,3145728,2097168,1048608,
	48,3145728,2097168,1048608,48,3145728,2097168,1048608,48,3145728,2097168,1048608,48,3145728,
	2097168,1048608,48,3145728,2097168,1048608,48,3145728,2097168,1048608,48
};

const uint32_t portb_map[64]= {
	3221422080,3221422080,3221422080,3221422080,3221356545,3221356545,3221356545,3221356545,
	3221291010,3221291010,3221291010,3221291010,3221225475,3221225475,3221225475,3221225475,
	1073971200,1073971200,1073971200,1073971200,1073905665,1073905665,1073905665,1073905665,
	1073840130,1073840130,1073840130,1073840130,1073774595,1073774595,1073774595,1073774595,
	2147696640,2147696640,2147696640,2147696640,2147631105,2147631105,2147631105,2147631105,
	2147565570,2147565570,2147565570,2147565570,2147500035,2147500035,2147500035,2147500035,
	245760,245760,245760,245760,180225,180225,180225,180225,114690,114690,114690,114690,
	49155,49155,49155,49155
};
#endif


static const lcdProperties_t ssd1351Properties = { 128, 128 };

uint8_t leds[LED_HEIGHT][LED_WIDTH][3] CCM_ATTRIBUTES; 


/*************************************************/
/* Private Methods                               */
/*************************************************/

#define CMD(c)        do {  CLR_DC; ssd1351SendByte( c ); } while (0)
#define DATA(c)       do {  SET_DC; ssd1351SendByte( c ); } while (0);

void ssd1351SendByte(uint8_t byte)
{

#ifndef I6800

	int8_t i;

	// Write from MSB to LSB
	__ASM volatile ("nop");
	for (i=7; i>=0; i--) 
	{
		// Set clock pin low
		__ASM volatile ("nop");
		CLR_SCK;
		__ASM volatile ("nop");
		// Set data pin high or low depending on the value of the current bit
		if (byte & (1 << i))
		{
			SET_SID;
		}
		else
		{
			CLR_SID;
		}
		// Set clock pin high
		__ASM volatile ("nop");
		SET_SCK;
	}
	__ASM volatile ("nop");
#endif

#ifdef I6800


	SET_E;

	if (byte & (1 << 0))
	{
		SET_SCK;
	}
	else
	{
		CLR_SCK;
	}
	if (byte & (1 << 1))
	{
		SET_SID;
	}
	else
	{
		CLR_SID;
	}
	if (byte & (1 << 2))
	{
		SET_D2;
	}
	else
	{
		CLR_D2;
	}
	if (byte & (1 << 3))
	{
		SET_D3;
	}
	else
	{
		CLR_D3;
	}
	if (byte & (1 << 4))
	{
		SET_D4;
	}
	else
	{
		CLR_D4;
	}
	if (byte & (1 << 5))
	{
		SET_D5;
	}
	else
	{
		CLR_D5;
	}
	if (byte & (1 << 6))
	{
		SET_D6;
	}
	else
	{
		CLR_D6;
	}
	if (byte & (1 << 7))
	{
		SET_D7;
	}
	else
	{
		CLR_D7;
	}

	__ASM volatile ("nop");

	CLR_E;





#endif

}

/**************************************************************************/
/*! 
  @brief  Sets the cursor to the specified X/Y position
  */
/**************************************************************************/
void ssd1351SetCursor(uint8_t x, uint8_t y)
{
	if ((x >= ssd1351Properties.width) || (y >= ssd1351Properties.height))
		return;

	CMD(SSD1351_CMD_WRITERAM);
	CMD(SSD1351_CMD_SETCOLUMNADDRESS);
	DATA(x);                            // Start Address
	DATA(ssd1351Properties.width-1);    // End Address (0x7F)

	CMD(SSD1351_CMD_SETROWADDRESS);
	DATA(y);                            // Start Address
	DATA(ssd1351Properties.height-1);   // End Address (0x7F)
	CMD(SSD1351_CMD_WRITERAM);
}

/*************************************************/
/* Public Methods                                */
/*************************************************/

/**************************************************************************/
/*! 
  @brief  Configures any pins or HW and initialises the LCD controller
  */
/**************************************************************************/
void lcdInit(void)
{

	//Reset the LCD

	SET_RST;
	Delay(20);
	CLR_RST;
	Delay(200);
	SET_RST;
	Delay(20);

	CMD(SSD1351_CMD_SETCOMMANDLOCK);
	DATA(0x12);                               // Unlocked to enter commands
	CMD(SSD1351_CMD_SETCOMMANDLOCK);
	DATA(0xB1);                               // Make all commands accessible 
	CMD(SSD1351_CMD_SLEEPMODE_DISPLAYOFF);
	CMD(SSD1351_CMD_SETFRONTCLOCKDIV);
	DATA(0xf1);//was F1
	CMD(SSD1351_CMD_SETMUXRRATIO);
	DATA(0x7f);//7f
	CMD(SSD1351_CMD_COLORDEPTH);

#ifdef C262K
	DATA(0xB4);
#endif
#ifndef C262K
	//#warning 65k colors
	DATA(0x74);                               // Bit 7:6 = 65,536 Colors, Bit 3 = BGR or RGB
#endif
	CMD(SSD1351_CMD_SETCOLUMNADDRESS);
	DATA(0x00);
	DATA(0x7F);
	CMD(SSD1351_CMD_SETROWADDRESS);
	DATA(0x00);
	DATA(0x7F);
	CMD(SSD1351_CMD_SETDISPLAYSTARTLINE);
	DATA(0x00);
	CMD(SSD1351_CMD_SETDISPLAYOFFSET);
	DATA(0x00);
	CMD(SSD1351_CMD_SETGPIO);
	DATA(0x00);                               // Disable GPIO pins
	CMD(SSD1351_CMD_FUNCTIONSELECTION);
	DATA(0x01);                               // External VDD (0 = External, 1 = Internal)
	CMD(SSD1351_CMD_SETPHASELENGTH);
	DATA(0x32);
	CMD(SSD1351_CMD_ENHANCEDDRIVINGSCHEME);
	DATA(0xA4);
	DATA(0x00);
	DATA(0x00);
	CMD(SSD1351_CMD_SETSEGMENTLOWVOLTAGE);
	DATA(0xA0);                               // Enable External VSL
	DATA(0xB5);
	DATA(0x55);
	CMD(SSD1351_CMD_SETPRECHARGEVOLTAGE);
	DATA(0x17);
	CMD(SSD1351_CMD_SETVCOMHVOLTAGE);
	DATA(0x05);
	CMD(SSD1351_CMD_SETCONTRAST);
	DATA(0xa0);
	DATA(0xc8);
	DATA(0xb0);
	CMD(SSD1351_CMD_MASTERCONTRAST);
	DATA(0x0F);                               // Maximum contrast
	CMD(SSD1351_CMD_SETSECONDPRECHARGEPERIOD);
	DATA(0x01);//was 0x01
	CMD(SSD1351_CMD_SETDISPLAYMODE_RESET);

	CMD(SSD1351_CMD_GRAYSCALELOOKUP);
	
	DATA(0x05);
	DATA(0x06);
	DATA(0x07);
	DATA(0x08);
	DATA(0x09);
	DATA(0x0a);
	DATA(0x0b);
	DATA(0x0c);
	DATA(0x0D);
	DATA(0x0E);
	DATA(0x0F);
	DATA(0x10);
	DATA(0x11);
	DATA(0x12);
	DATA(0x13);
	DATA(0x14);
	DATA(0x15);
	DATA(0x16);
	DATA(0x18);
	DATA(0x1a);
	DATA(0x1b);
	DATA(0x1C);
	DATA(0x1D);
	DATA(0x1F);
	DATA(0x21);
	DATA(0x23);
	DATA(0x25);
	DATA(0x27);
	DATA(0x2A);
	DATA(0x2D);
	DATA(0x30);
	DATA(0x33);
	DATA(0x36);
	DATA(0x39);
	DATA(0x3C);
	DATA(0x3F);
	DATA(0x42);
	DATA(0x45);
	DATA(0x48);
	DATA(0x4C);
	DATA(0x50);
	DATA(0x54);
	DATA(0x58);
	DATA(0x5C);
	DATA(0x60);
	DATA(0x64);
	DATA(0x68);
	DATA(0x6C);
	DATA(0x70);
	DATA(0x74);
	DATA(0x78);
	DATA(0x7D);
	DATA(0x82);
	DATA(0x87);
	DATA(0x8C);
	DATA(0x91);
	DATA(0x96);
	DATA(0x9B);
	DATA(0xA0);
	DATA(0xA5);
	DATA(0xAA);
	DATA(0xAF);
	DATA(0xB4);

	// Clear screen
	lcdFillRGB(0,0,0);

	// Turn the display on
	CMD(SSD1351_CMD_SLEEPMODE_DISPLAYON);  
	return;
}

void getLedXY(uint8_t x, uint8_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	*red = leds[y][x][0];
	*green = leds[y][x][1];
	*blue = leds[y][x][2];
}


void invLedXY(uint8_t x, uint8_t y) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	leds[y][x][0] = 255 - leds[y][x][0];
	leds[y][x][1] = 255 - leds[y][x][1];
	leds[y][x][2] = 255 - leds[y][x][2];
}

void setLedXY(uint8_t x, uint8_t y, uint8_t r,uint8_t g, uint8_t b)
{
	if ((x >= ssd1351Properties.width) || (y >= ssd1351Properties.height))
		return;
	
	leds[y][x][0] = r;
	leds[y][x][1] = g;
	leds[y][x][2] = b;
}


void lcdFillRGB(uint8_t r,uint8_t g,uint8_t b)
{
	uint8_t x,y;
	ssd1351SetCursor(0, 0);

#ifdef C262K

	uint8_t r2 = r>>2;
	uint8_t g2 = g>>2;
	uint8_t b2 = b>>2;

	SET_DC; 

	for (x=0; x<ssd1351Properties.width ;x++)
	{
		for (y=0; y< ssd1351Properties.height;y++)
		{	
			DATA(r2);
			DATA(g2);
			DATA(b2);
			//sendBy(r2);
			//sendBy(g2);
			//sendBy(b2);
			leds[y][x][0] = r;
			leds[y][x][1] = g;
			leds[y][x][2] = b;
		}
	}
#endif
#ifndef C262K
	uint8_t data1 = (r&0xF8) | (g>>5);
	uint8_t data2 = (b>>3) | ((g>>2)<<5);

	for (x=0; x<ssd1351Properties.width ;x++)
	{
		for (y=0; y< ssd1351Properties.height;y++)
		{
			DATA(data1);
			DATA(data2);
			leds[y][x][0] = r;
			leds[y][x][1] = g;
			leds[y][x][2] = b;
		}
	}
#endif
}


void lcdFlush(void)
{
	ssd1351SetCursor(0, 0);
	SET_DC; 

	for(int y = 0; y < LED_HEIGHT; y++) 
	{
		for(int x = 0; x < LED_WIDTH; x++) 
		{
#if LUN1K_VERSION >= 11
			DATA(leds[y][x][0]>>2);
			DATA(leds[y][x][1]>>2);
			DATA(leds[y][x][2]>>2);
#endif

#if LUN1K_VERSION == 10
			GPIOC->BSRRL = (1<<7);
			GPIOC->BSRR = portc_map[leds[y][x][0]>>2];
			GPIOB->BSRR = portb_map[leds[y][x][0]>>2];
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			GPIOC->BSRRH = (1<<7);


			GPIOC->BSRRL = (1<<7);
			GPIOC->BSRR = portc_map[leds[y][x][1]>>2];
			GPIOB->BSRR = portb_map[leds[y][x][1]>>2];
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			GPIOC->BSRRH = (1<<7);


			GPIOC->BSRRL = (1<<7);
			GPIOC->BSRR = portc_map[leds[y][x][2]>>2];
			GPIOB->BSRR = portb_map[leds[y][x][2]>>2];
			__ASM volatile ("nop");
			__ASM volatile ("nop");
			GPIOC->BSRRH = (1<<7);
#endif
		}
	}
}
