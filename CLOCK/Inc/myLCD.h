#ifndef MY_LCD_LIB
#define MY_LCD_LIB


#define     LCM_OUT               GPIOA->ODR
#define     LCM_PIN_EN            GPIO_PIN_5
#define     LCM_PIN_RS            GPIO_PIN_4
#define     LCM_PIN_D7            GPIO_PIN_3
#define     LCM_PIN_D6            GPIO_PIN_2
#define     LCM_PIN_D5            GPIO_PIN_1
#define     LCM_PIN_D4            GPIO_PIN_0
#define     LCM_PIN_MASK  ((LCM_PIN_RS | LCM_PIN_EN | LCM_PIN_D7 | LCM_PIN_D6 | LCM_PIN_D5 | LCM_PIN_D4))


void myLCD_PulseLCD(void);
void myLCD_SendByte(char ByteToSend, int IsData);
void myLCD_Initialize(void);

void myLCD_Cursor(char Row, char Col);

void myLCD_ClearLCDScreen(void);

void myLCD_PrintStr(char *Text);

void myLCD_PrintInt(int number);

void myLCD_PrintInt_clock(int number);

void myLCD_PrintByte(char byte);

void myLCD_PrintWeekDay(char week_day);

void myLCD_TransformStrs(char* input,char* up,char* down);

#endif
