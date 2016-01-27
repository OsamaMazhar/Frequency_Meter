/*
 * Title: Freqency Counter v1
 * Author: Osama Mazhar
 * Date: 30th March 2012
 * Email: osamazhar@yahoo.com
 */
 
#include<reg51.h>
#include<intrins.h>

#define HOLD _nop_(); _nop_(); _nop_() ;_nop_(); _nop_(); _nop_(); _nop_()

sfr ldata = 0x90;						// P1 = LCD Data Pins
sbit rs	= P2^0;
sbit rw	= P2^1;
sbit en	= P2^2;
sbit busy = P1^7;

void MSDelay(unsigned int);
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void lcdready(void);
void longtoLCD(long int value);
void lcdinit(void);

unsigned char cnt = 0;

void timer0(void) interrupt 1
	{
	 cnt++;
	}

void main(void)
	{ 
	 unsigned char x;
	 unsigned int y;
	 long Freq;
	 lcdinit();
	 TMOD = 0x15;
	 // Timer Clk=12/12*1 = 1.0 MHz
	 // Register values for 50ms delay
	 TL1  = 0xB0;	//Lower Byte		
	 TH1  = 0x3C;	//Upper Byte
	 IE = 0x82;
	  while(1)
	 	{
		 TR0 = 1;
		 for(x = 0; x<15; x++)	// 50ms multiplied by 15 equals 750ms
		 	{
			 TR1 = 1;							// Timer1 starts
		 	 while(TF1 == 0);			// Wait until Timer1 trips
			 TR1 = 0;
			 TF1 = 0;
			 for(y=0; y<26; y++)	// for fine tuning
			 	{
				 HOLD; HOLD; HOLD; HOLD; HOLD; 	
				}
				HOLD;
			}
		 Freq = (cnt * 65536) + (256*TH0) + TL0;	// TH0 is incremented when TL0 overflows, and
																							// cnt is incremented when TH0 overflows.
																							// TH0 is from 15 downto 8 bits thus multiplied by
																							// 256, and cnt is 31 downto 16 bits thus muliplied
																							// by 65536
		 TR0 = 0;
		 TH0 = 0; TL0 = 0;
		 longtoLCD(Freq);
		 lcdcmd(0x80);
		 cnt = 0;
		}
	}

void longtoLCD(long int value)
	{
	 long int a, b, c, i, e, f, g, h, d[10];
	 char l;
	 a = value / 10; 										// => 6553
	 d[0] = (value % 10) + 48; 					// => 6 (LSD) *
	 d[1] = (a % 10) + 48;	 						// => 3 *
	 b = a / 10; 												// => 655
	 d[2] = (b % 10) + 48; 							// => 5 *
	 c = b / 10; 												// => 65
	 d[3] = (c % 10) + 48;							// => 5 *
	 i = c / 10;
	 d[4] = (i % 10) + 48;							// => 6 (MSD) *
	 e = i / 10;
	 d[5] =	(e % 10) + 48;
	 f = e / 10;
	 d[6] =	(f % 10) + 48;
	 g = f / 10;
	 d[7] =	(g % 10) + 48;
	 h = g / 10;
	 d[8] =	(h % 10) + 48;
	 d[9] =	(h / 10) + 48;
	 
	  if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] == 48 & d[5] == 0 & d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] == 48)
		{
		  lcddata(d[0]);
		  for(l=0; l<9; l++)
		 	 lcddata(' ');
		} 
	  if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] == 48 & d[5] == 48 & d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] != 0)
		{
		  for(l=1; l>=0; l--)
	 	 	lcddata(d[l]);
		  for(l=0; l<8; l++)
		 	 lcddata(' ');
		} 
	  if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] == 48 & d[5] == 48 & d[4] == 48 & d[3] == 48 & d[2] != 48)
		{
		  for(l=2; l>=0; l--)
	 	 	lcddata(d[l]);
		  for(l=0; l<7; l++)
		 	 lcddata(' ');
		}
	 if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] == 48 & d[5] == 48 & d[4] == 48 & d[3] != 48)
	 	{
	 	  for(l=3; l>=0; l--)
	 	 	lcddata(d[l]);
	 	  for(l=0; l<6; l++)
	 	 	 lcddata(' ');
	 	}
	 if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] == 48 & d[5] == 48 & d[4] != 48)
	 	{
	 	  for(l=4; l>=0; l--)
	 	 	lcddata(d[l]);
	 	  for(l=0; l<5; l++)
	 	 	 lcddata(' ');
	 	}
	 if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] == 48 & d[5] != 48)
	 	{
	 	  for(l=5; l>=0; l--)
	 	 	lcddata(d[l]);
	 	  for(l=0; l<4; l++)
	 	 	 lcddata(' ');
	 	}
	 if(d[9] == 48 & d[8] == 48 & d[7] == 48 & d[6] != 48)
	 	{
	 	 for(l=6; l>=0; l--)
	 	 	lcddata(d[l]);
	 	 for(l=0; l<3; l++)
		 	 lcddata(' ');
		}
	 if(d[9] == 48 & d[8] == 48 & d[7] != 48)
	 	{
	 	 for(l=7; l>=0; l--)
	 	 	lcddata(d[l]);
	 	 for(l=0; l<2; l++)
	 	 	 lcddata(' ');
	 	}
	 if(d[9] == 48 & d[8] != 48)
	 	{
		 for(l=8; l>=0; l--)
	 	 	lcddata(d[l]);
		 lcddata(' ');
		}
	 if(d[9] != 48)
	 	{
		 for(l=9; l>=0; l--)
		 lcddata(d[l]);
		}
	 }	
	
void lcdinit(void)
	{
	 lcdcmd(0x38);					//2 lines and 5X7 matrix
	 lcdcmd(0x0C);					//Display ON, Cursor Blinking = 0x0E But this command is Display ON, Cursor OFF
	 lcdcmd(0x01);					//Clear Display Screen
	 lcdcmd(0x80);					//Force Cursor to the beginning of the First Line
	 lcdcmd(0x06);
	}

void MSDelay(unsigned int value)
	{
	 unsigned int x,y;
	 for(x=0; x<value; x++)
		for(y=0; y<1275; y++);
	}

void lcdcmd(unsigned char value)
	{
	 lcdready();
	 ldata = value;			//put the value on the pins
	 rs = 0;
	 rw = 0;
	 en = 1;						//strobe the enable pin
 	 MSDelay(1);
	 en = 0;
	}

void lcddata(unsigned char value)
	{
	 lcdready();
	 ldata = value;			//put the values on the pins
	 rs = 1;
	 rw = 0;
	 en = 1;						//strobe the enable pin
	 MSDelay(1);
	 en = 0;
	}

void lcdready()
	{
	 busy = 1;
	 rs = 0;
 	 rw = 1;
 	 do
		{
		 en = 0;
		 MSDelay(1);
		 en = 1;
		}
	 while(busy == 1);
	 return;
	}