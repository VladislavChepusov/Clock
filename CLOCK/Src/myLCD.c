#include "myLCD.h"
#include "stm32f1xx_hal.h"



void myLCD_PulseLCD()
{
    LCM_OUT &= ~LCM_PIN_EN;
    HAL_Delay(1);
    LCM_OUT |= LCM_PIN_EN;
    HAL_Delay(1);
    LCM_OUT &= (~LCM_PIN_EN);
    HAL_Delay(1);
}

void myLCD_SendByte(char ByteToSend, int IsData)
{
    LCM_OUT &= (~LCM_PIN_MASK);
    LCM_OUT |= (ByteToSend & 0xF0)>>4;
 
    if (IsData == 1)
        LCM_OUT |= LCM_PIN_RS;
    else
        LCM_OUT &= ~LCM_PIN_RS;
    myLCD_PulseLCD();
    LCM_OUT &= (~LCM_PIN_MASK);
    LCM_OUT |= (ByteToSend & 0x0F);
 
    if (IsData == 1)
        LCM_OUT |= LCM_PIN_RS;
    else
        LCM_OUT &= ~LCM_PIN_RS;
 
    myLCD_PulseLCD();
}

void myLCD_Cursor(char Row, char Col)
{
   char address;
   if (Row == 0)
   address = 0;
   else
   address = 0x40;
   address |= Col;
   myLCD_SendByte(0x80 | address, 0);
}

void myLCD_ClearLCDScreen()
{
    myLCD_SendByte(0x01, 0);
    myLCD_SendByte(0x02, 0);
}
void myLCD_Initialize(void)
{
  LCM_OUT &= ~(LCM_PIN_MASK);
  HAL_Delay(50);
	LCM_OUT &= ~LCM_PIN_RS;
  LCM_OUT &= ~LCM_PIN_EN;
	LCM_OUT = 0x2;
	myLCD_PulseLCD();
	myLCD_SendByte(0x28, 0);
  myLCD_SendByte(0xC, 0);
  myLCD_SendByte(0x06, 0);
}

void myLCD_PrintStr(char *Text)
{
    char *c;
    c = Text;
    while ((c != 0) && (*c != 0))
    {
        myLCD_SendByte(*c, 1);
        c++;
    }
}

void myLCD_PrintInt(int number)
{
	int count=0,copy=number;
	while(copy>9)
	{
		count++;
		copy=copy/10;
	}
	copy=number;
	count++;
	char result[16];
	int v=0;
	while(copy>9)
	{
		result[v++]=(copy%10)+'0';
		copy=copy/10;
	}
	result[v++]=copy+'0';
	result[v]='\0';
	char t;
	for (int i = 0; i < v / 2; i++)
    {
        t = result[i];
        result[i] = result[v - 1 - i];
        result[v - 1 - i] = t;
    }
	myLCD_PrintStr(result);
}

void myLCD_PrintInt_clock(int number)
{
	if(number<10) myLCD_PrintInt(0);
	myLCD_PrintInt(number);
}

void myLCD_PrintByte(char byte)
{
	char result[10];
	result[7] = (byte&1) +48;
	byte = byte>>1;
	result[6] = (byte&1) +48;
	byte = byte>>1;
	result[5] = (byte&1) +48;
	byte = byte>>1;
	result[4] = (byte&1) +48;
	byte = byte>>1;
	result[3] = (byte&1) +48;
	byte = byte>>1;
	result[2] = (byte&1) +48;
	byte = byte>>1;
	result[1] = (byte&1) +48;
	byte = byte>>1;
	result[0] = (byte&1) +48;
	result[8]='\0';
	//byte = byte>>1;
	myLCD_PrintStr(result);
}

void myLCD_PrintWeekDay(char week_day)
{
	switch(week_day)
	{
		case 0 :
		{
			myLCD_PrintStr("SUN");
			break;
		}
		case 1 :
		{
			myLCD_PrintStr("MON");
			break;
		}
		case 2 :
		{                                               
			myLCD_PrintStr("TUE");                        
			break;                                        
		}                                               
		case 3 :                                        
		{                                               
			myLCD_PrintStr("WED");                        
			break;
		}
		case 4 :
		{
			myLCD_PrintStr("THU");
			break;
		}
		case 5 :
		{
			myLCD_PrintStr("FRI");
			break;
		}
		case 6 :
		{
			myLCD_PrintStr("SAT");
			break;
		}
	}
}


void myLCD_TransformStrs(char* input,char* up,char* down)
{
	for(int i=0;i<16;i++)up[i]=*input++;
	up[16]=0;
	for(int i=0;i<16;i++)down[i]=*input++;
	up[16]=0;
}
