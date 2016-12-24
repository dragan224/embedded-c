/* Project name:
     HID_Read_Write (USB HID Read & Write Test)
 * Copyright:
     (c) Mikroelektronika, 2011.
 * Revision History:
     20111223:
       - initial release;
 * Description:
     This example establishes connection with the HID terminal that is active
     on the PC. Upon connection establishment, the HID Device Name will appear
     in the respective window. The character that user sends to ARM from the HID
     terminal will be re-sent back to user.
 * Test configuration:
     MCU:             STM32F107VC
                      http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00220364.pdf
     Dev.Board:       EasyMx PRO v7 for STM32(R) ARM(R) - ac:USB
                      http://www.mikroe.com/easymx-pro/stm32/
     Oscillator:      HSE-PLL, 72.000MHz
     Ext. Modules:    -
     SW:              mikroC PRO for ARM
                      http://www.mikroe.com/mikroc/arm/
 * NOTES:  -  Turn on USB switches on SW12
*/

// TFT module connections
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST at GPIOE_ODR.B8;
sbit TFT_RS at GPIOE_ODR.B12;
sbit TFT_CS at GPIOE_ODR.B15;
sbit TFT_RD at GPIOE_ODR.B10;
sbit TFT_WR at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;
// End TFT module connections

char readbuff[64];
char writebuff[64];
char i;
unsigned int oldstate0 = 0;
unsigned int oldstate6 = 0;
unsigned int oldstate4 = 0;
unsigned int oldstate5 = 0;

unsigned int yPos = 0, lapY = 0;
char white[64];

void USB0Interrupt() iv IVT_INT_OTG_FS {
  USB_Interrupt_Proc();
}

void clear(int y) {
  unsigned int i;
  TFT_Set_Pen(CL_WHITE, 1);
  for (i = 0; i < 20; i++) TFT_Rectangle(0, 20 * y + i, 320, 20 * y + i);
  TFT_Set_Pen(CL_BLACK, 1);
}

void main(void){
  for (i = 0; i < 64; i++) {
    white[i] = ' ';
  }
  GPIO_Digital_Input(&GPIOA_IDR, _GPIO_PINMASK_0 | _GPIO_PINMASK_6 | _GPIO_PINMASK_4 | _GPIO_PINMASK_5);         // Set PA0 as digital input
  GPIO_Digital_Output(&GPIOD_ODR, _GPIO_PINMASK_ALL);      // Set PORTD as digital output
  HID_Enable(&readbuff,&writebuff);
  TFT_Init_ILI9341_8bit(320, 240);

  for (i = 0; i < 20; i++) clear(i);
  while (1) {
    if (HID_Read()) {
      clear(0);
      TFT_Write_Text(readbuff, 0, 0);
    }

    if (Button(&GPIOA_IDR, 0, 1, 1)) {                      // detect logical one on PA0 pin
      oldstate0 = 1;
    }
    if (oldstate0 && Button(&GPIOA_IDR, 0, 1, 0)) {        // detect one-to-zero transition on PA0 pin
      GPIOD_ODR = ~GPIOD_ODR;
      for (i = 0; i < 20; i++) clear(i);
      lapY = 0;
      writebuff[0] = 'S';
      while (!HID_Write(&writebuff, 64));                            // invert PORTD value
      oldstate0 = 0;
    }

    if (Button(&GPIOA_IDR, 6, 1, 1)) {                      // detect logical one on PA0 pin
      oldstate6 = 1;
    }
    if (oldstate6 && Button(&GPIOA_IDR, 6, 1, 0)) {        // detect one-to-zero transition on PA0 pin
      GPIOD_ODR = ~GPIOD_ODR;
      lapY += 20;
      TFT_Write_Text(readbuff, 0, lapY);
      oldstate6 = 0;
    }

    if (Button(&GPIOA_IDR, 4, 1, 1)) {                      // detect logical one on PA0 pin
      oldstate4 = 1;
    }
    if (oldstate4 && Button(&GPIOA_IDR, 4, 1, 0)) {        // detect one-to-zero transition on PA0 pin
      GPIOD_ODR = ~GPIOD_ODR;
      for (i = 0; i < 20; i++) clear(i);
      lapY = 0;
      writebuff[0] = 'E';
      while (!HID_Write(&writebuff, 64));                            // invert PORTD value
      oldstate4 = 0;
    }

    if (Button(&GPIOA_IDR, 5, 1, 1)) {                      // detect logical one on PA0 pin
      oldstate5 = 1;
    }
    if (oldstate5 && Button(&GPIOA_IDR, 5, 1, 0)) {        // detect one-to-zero transition on PA0 pin
      GPIOD_ODR = ~GPIOD_ODR;
      lapY += 20;
      TFT_Write_Text(readbuff, 0, lapY);
      writebuff[0] = 'R';
      while (!HID_Write(&writebuff, 64));                            // invert PORTD value
      oldstate5 = 0;
    }
  }
}