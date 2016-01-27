/*
 * Title: Freqency Counter v2
 * Author: Osama Mazhar
 * Date: 30th March 2012
 * Email: osamazhar@yahoo.com
 */

#include<reg51.h>

sfr ldata = 0x90;						//P1 = LCD Data Pins
sbit rs	= P2^0;
sbit rw	= P2^1;
sbit en	= P2^2;
sbit busy = P1^7;

unsigned char Unit[4];

void MSDelay(unsigned int);
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void lcdready(void);
void lcdinit(void);
void inttoLCD(unsigned int);

void main(void)
	{
	 unsigned int HI, LO, TOTAL;
	 unsigned char l, OV, OFR[16] = "Out Of Range...";
	 OV = 0;
	 T0 = 1;
	 TMOD = 0x05;
	 TL0 = 0;
	 TL1 = 0;
	 lcdinit();
	 while(1)
	 	{
		 do
		 	{
start: TR0 = 1;
			 MSDelay(145);
		 	 LO = TL0;
		 	 HI = TH0;
			 HI = HI << 8;
			 TOTAL = HI | LO;
			 Unit[0] = 'H';
			 Unit[1] = 'z';
			 Unit[2] = ' ';
			 Unit[3] = ' ';
			 if(OV == 1)
			 	{
				 TOTAL = TOTAL / 1000;
				 TOTAL = TOTAL + 65;
				 if(TOTAL > 125)
				 	{
					 for(l=0; l<16; l++)
					 	lcddata(OFR[l]);
					 for(l=0; l<16; l++)
					 	lcdcmd(0x10);
					 goto start; 
					 }
				 OV = 0;
				 Unit[0] = 'K';
			 	 Unit[1] = 'H';
			 	 Unit[2] = 'z';
				 Unit[3] = ' ';
				}
		 	 inttoLCD(TOTAL);

		 	 TL0 = 0;
		 	 TH0 = 0;
			}
		 while(TF0 == 0);
		 OV = 1;
		 TR0 = 0;
		 TF0 = 0;
		}
	}
void MSDelay(unsigned int value)	// This function is NOT millisecond delay function.
	{
	 unsigned int x,y;
	 for(x = 0; x < value; x++)
		for(y = 0; y < 1275; y++);		// 1275 is just a random number
	}

void lcdinit(void)
	{
	 lcdcmd(0x38);					//2 lines and 5X7 matrix
	 lcdcmd(0x0C);					//Display ON, Cursor Blinking = 0x0E But this command is Display ON, Cursor OFF
	 lcdcmd(0x01);					//Clear Display Screen
	 lcdcmd(0x80);					//Force Cursor to the beginning of the First Line
	 lcdcmd(0x06);
	}
void lcdcmd(unsigned char value)
	{
	 lcdready();
	 ldata = value;					//put the value on the pins
	 rs = 0;
	 rw = 0;
	 en = 1;								//strobe the enable pin
 	 MSDelay(1);
	 en = 0;
	}

void lcddata(unsigned char value)
	{
	 lcdready();
	 ldata = value;					//put the values on the pins
	 rs = 1;
	 rw = 0;
	 en = 1;								//strobe the enable pin
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

void inttoLCD(unsigned int value)
	{
	 unsigned int x, y, z, d[5];
	 char l;
	 x = value / 10; 										// => 6553
	 d[0] = (value % 10) + 48; 					// => 6 (LSD) *
	 d[1] = (x % 10) + 48;	 						// => 3 *
	 y = x / 10; 												// => 655
	 d[2] = (y % 10) + 48; 							// => 5 *
	 z = y / 10; 												// => 65
	 d[3] = (z % 10) + 48;							// => 5 *
	 d[4] = (z / 10) + 48;							// => 6 (MSD) *
	 if(d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] == 48)
		{
		  lcddata(d[0]);
		  lcddata(' ');
		  for(l=0; l<4; l++)
		 	 lcddata(Unit[l]);
		  for(l=0 ; l<11 ; l++)
		  	 lcddata(' ');
		  for(l=0 ; l<17 ; l++)
		 	 lcdcmd(0x10);
		  
		}
	 if(d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] != 48)
		{
		 for(l=1; l>=0; l--)
	 	 	lcddata(d[l]);
		 lcddata(' ');
		 for(l=0; l<4; l++)
		 	 lcddata(Unit[l]);
		 for(l=0 ; l<10 ; l++)
		  	 lcddata(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdcmd(0x10);
		 
		}
	 if(d[4] == 48 & d[3] == 48 & d[2] != 48)
		{
		 for(l=2; l>=0; l--)
	 	 	lcddata(d[l]);
		 lcddata(' ');
		 for(l=0; l<4; l++)
		 	 lcddata(Unit[l]);
		 for(l=0 ; l<9 ; l++)
		  	 lcddata(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdcmd(0x10);
		 
		}
	 if(d[4] == 48 & d[3] != 48)
	 	{
		 for(l=3; l>=0; l--)
	 	 	lcddata(d[l]);
		 lcddata(' ');
		 for(l=0; l<4; l++)
		 	lcddata(Unit[l]);
		 for(l=0 ; l<8 ; l++)
		  	 lcddata(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdcmd(0x10);
		 
		}
	 if(d[4] != 48)
	 	{
		 for(l=4; l>=0; l--)
		 	lcddata(d[l]);
		 lcddata(' ');
		 for(l=0; l<4; l++)
		 	lcddata(Unit[l]);
		 		 for(l=0 ; l<7 ; l++)
		  	 lcddata(' ');
		 for(l=0 ; l<17 ; l++)
		 	 lcdcmd(0x10);

		}
	 }