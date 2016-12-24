// USB DESCRIPTOR BEGIN
const unsigned int USB_VENDOR_ID = 0x1234;
const unsigned int USB_PRODUCT_ID = 0x0001;
const char USB_SELF_POWER = 0x80;            // Self powered 0xC0,  0x80 bus powered
const char USB_MAX_POWER = 50;               // Bus power required in units of 2 mA
const char HID_INPUT_REPORT_BYTES = 64;
const char HID_OUTPUT_REPORT_BYTES = 64;
const char USB_TRANSFER_TYPE = 0x03;         //0x03 Interrupt
const char EP_IN_INTERVAL = 1;
const char EP_OUT_INTERVAL = 1;

const char USB_INTERRUPT = 0;
const char USB_HID_EP = 1;
const char USB_HID_RPT_SIZE = 33;

/* Device Descriptor */
const struct {
    char bLength;               // bLength         - Descriptor size in bytes (12h)
    char bDescriptorType;       // bDescriptorType - The constant DEVICE (01h)
    unsigned int bcdUSB;        // bcdUSB          - USB specification release number (BCD)
    char bDeviceClass;          // bDeviceClass    - Class Code
    char bDeviceSubClass;       // bDeviceSubClass - Subclass code
    char bDeviceProtocol;       // bDeviceProtocol - Protocol code
    char bMaxPacketSize0;       // bMaxPacketSize0 - Maximum packet size for endpoint 0
    unsigned int idVendor;      // idVendor        - Vendor ID
    unsigned int idProduct;     // idProduct       - Product ID
    unsigned int bcdDevice;     // bcdDevice       - Device release number (BCD)
    char iManufacturer;         // iManufacturer   - Index of string descriptor for the manufacturer
    char iProduct;              // iProduct        - Index of string descriptor for the product.
    char iSerialNumber;         // iSerialNumber   - Index of string descriptor for the serial number.
    char bNumConfigurations;    // bNumConfigurations - Number of possible configurations
} device_dsc = {
      0x12,                   // bLength
      0x01,                   // bDescriptorType
      0x0200,                 // bcdUSB
      0x00,                   // bDeviceClass
      0x00,                   // bDeviceSubClass
      0x00,                   // bDeviceProtocol
      0x40,                      // bMaxPacketSize0
      USB_VENDOR_ID,          // idVendor
      USB_PRODUCT_ID,         // idProduct
      0x0001,                 // bcdDevice
      0x01,                   // iManufacturer
      0x02,                   // iProduct
      0x00,                   // iSerialNumber
      0x01                    // bNumConfigurations
  };

/* Configuration 1 Descriptor */
const char configDescriptor1[]= {
    // Configuration Descriptor
    0x09,                   // bLength             - Descriptor size in bytes
    0x02,                   // bDescriptorType     - The constant CONFIGURATION (02h)
    0x29,0x00,              // wTotalLength        - The number of bytes in the configuration descriptor and all of its subordinate descriptors
    1,                      // bNumInterfaces      - Number of interfaces in the configuration
    1,                      // bConfigurationValue - Identifier for Set Configuration and Get Configuration requests
    0,                      // iConfiguration      - Index of string descriptor for the configuration
    USB_SELF_POWER,         // bmAttributes        - Self/bus power and remote wakeup settings
    USB_MAX_POWER,          // bMaxPower           - Bus power required in units of 2 mA

    // Interface Descriptor
    0x09,                   // bLength - Descriptor size in bytes (09h)
    0x04,                   // bDescriptorType - The constant Interface (04h)
    0,                      // bInterfaceNumber - Number identifying this interface
    0,                      // bAlternateSetting - A number that identifies a descriptor with alternate settings for this bInterfaceNumber.
    2,                      // bNumEndpoint - Number of endpoints supported not counting endpoint zero
    0x03,                   // bInterfaceClass - Class code
    0,                      // bInterfaceSubclass - Subclass code
    0,                      // bInterfaceProtocol - Protocol code
    0,                      // iInterface - Interface string index

    // HID Class-Specific Descriptor
    0x09,                   // bLength - Descriptor size in bytes.
    0x21,                   // bDescriptorType - This descriptor's type: 21h to indicate the HID class.
    0x01,0x01,              // bcdHID - HID specification release number (BCD).
    0x00,                   // bCountryCode - Numeric expression identifying the country for localized hardware (BCD) or 00h.
    1,                      // bNumDescriptors - Number of subordinate report and physical descriptors.
    0x22,                   // bDescriptorType - The type of a class-specific descriptor that follows
    USB_HID_RPT_SIZE,0x00,  // wDescriptorLength - Total length of the descriptor identified above.

    // Endpoint Descriptor
    0x07,                   // bLength - Descriptor size in bytes (07h)
    0x05,                   // bDescriptorType - The constant Endpoint (05h)
    USB_HID_EP | 0x80,      // bEndpointAddress - Endpoint number and direction
    USB_TRANSFER_TYPE,      // bmAttributes - Transfer type and supplementary information
    0x40,0x00,              // wMaxPacketSize - Maximum packet size supported
    EP_IN_INTERVAL,         // bInterval - Service interval or NAK rate

    // Endpoint Descriptor
    0x07,                   // bLength - Descriptor size in bytes (07h)
    0x05,                   // bDescriptorType - The constant Endpoint (05h)
    USB_HID_EP,             // bEndpointAddress - Endpoint number and direction
    USB_TRANSFER_TYPE,      // bmAttributes - Transfer type and supplementary information
    0x40,0x00,              // wMaxPacketSize - Maximum packet size supported
    EP_OUT_INTERVAL         // bInterval - Service interval or NAK rate
};

const struct {
  char report[USB_HID_RPT_SIZE];
}hid_rpt_desc =
  {
     {0x06, 0x00, 0xFF,       // Usage Page = 0xFF00 (Vendor Defined Page 1)
      0x09, 0x01,             // Usage (Vendor Usage 1)
      0xA1, 0x01,             // Collection (Application)
  // Input report
      0x19, 0x01,             // Usage Minimum
      0x29, 0x40,             // Usage Maximum
      0x15, 0x00,             // Logical Minimum (data bytes in the report may have minimum value = 0x00)
      0x26, 0xFF, 0x00,       // Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
      0x75, 0x08,             // Report Size: 8-bit field size
      0x95, HID_INPUT_REPORT_BYTES,// Report Count
      0x81, 0x02,             // Input (Data, Array, Abs)
  // Output report
      0x19, 0x01,             // Usage Minimum
      0x29, 0x40,             // Usage Maximum
      0x75, 0x08,             // Report Size: 8-bit field size
      0x95, HID_OUTPUT_REPORT_BYTES,// Report Count
      0x91, 0x02,             // Output (Data, Array, Abs)
      0xC0}                   // End Collection
  };

//Language code string descriptor
const struct {
  char bLength;
  char bDscType;
  unsigned int string[1];
  } strd1 = {
      4,
      0x03,
      {0x0409}
    };


//Manufacturer string descriptor
const struct{
  char bLength;
  char bDscType;
  unsigned int string[16];
  }strd2={
    34,           //sizeof this descriptor string
    0x03,
    {'M','i','k','r','o','e','l','e','k','t','r','o','n','i','k','a'}
  };

//Product string descriptor
const struct{
  char bLength;
  char bDscType;
  unsigned int string[23];
}strd3={
    12,          //sizeof this descriptor string
    0x03,
    {'b', 'o', 'a', 'r', 'd'}
 };

//Array of configuration descriptors
const char* USB_config_dsc_ptr[1];

//Array of string descriptors
const char* USB_string_dsc_ptr[3];

void USB_Init_Desc(){
  USB_config_dsc_ptr[0] = &configDescriptor1;
  USB_string_dsc_ptr[0] = (const char*)&strd1;
  USB_string_dsc_ptr[1] = (const char*)&strd2;
  USB_string_dsc_ptr[2] = (const char*)&strd3;
}
// USB DESCRIPTOR END

#include "simple_maze_objects.h"

/*
 * Project name:
     simple_maze.vtft
 * Generated by:
     Visual TFT
 * Date of creation
     01-05-2012
 * Time of creation
     1:00:00 PM
 * Test configuration:
     Device:          STM32F107VC
                      http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00220364.pdf
     Dev.Board:       EasyMx v7 for STM32(R) ARM(R)
                      http://www.mikroe.com/easymx-pro/stm32/
     Oscillator:      HSE, 72.000MHz
     Ext. Modules:    EasyTFT
                      http://www.mikroe.com/add-on-boards/display/easytft/
                      ac:EasyTFT
     SW:              mikroC PRO for ARM
                      http://www.mikroe.com/mikroc/arm/
 */

// RECTANGLES BEGIN
int ux[1000], uy[1000], color[1000], size[1000];
int nrect = 0;

// Crta pravougaonik sa gornjim levim temenom (xs, ys), zadatom bojom i velicinom (u px).
void drawRectangle(int xs, int ys, int color, int size) {
  int i;
  if (color == 0) {
    // Zelena
    color = TFT_RGBToColor16bit(0, 255, 0);
  } else if (color == 1) {
    // Crvena boja
    color = TFT_RGBToColor16bit(255, 0, 0);
  } else {
    // Crna
   color = TFT_RGBToColor16bit(255, 255, 255);
  }
  for (i = 0; i < size; i++) {
    TFT_SET_Pen(color, 1);
    TFT_H_Line(xs, xs+size, ys+i);
  }
}

// Dodaje u listu i crta pravougaonik sa gornjem desnim koordinatama(xs, ys) 
// bojom clr (0 = Zeleno, 1 = Crveno, 2 = Crno)
void add(int xs, int ys, int clr, int sz) {
  ux[nrect] = xs;
  uy[nrect] = ys;
  color[nrect] = clr;
  size[nrect] = sz;
  nrect++;
  drawRectangle(xs, ys, clr, sz);
}

// Vraca 0/1 da li je unutra pravouganoika.
int isInside(int xs, int ys, int idx) {
   if (xs >= ux[idx] && xs <= ux[idx]+size[idx] &&
       ys >= uy[idx] && ys <= uy[idx]+size[idx]) return 1;
   return 0;
}

// Ponovno crta sve pravougaonike.
void redrawAll() {
  int j;
  for (j = 0; j < nrect; j++) {
    drawRectangle(ux[j], uy[j], color[j], size[j]);
  }
}

// Brise pravougaonik sa indeksom 'idx' iz liste pravougaonika.
void remove(int idx) {
  int j;
  drawRectangle(ux[idx], uy[idx], 2, size[idx]);
  for (j = idx; j < nrect-1; j++) {
    ux[j] = ux[j+1];
    uy[j] = uy[j+1];
    color[j] = color[j+1];
    size[j] = size[j+1];
  }
  nrect--;
  redrawAll(); // U slucaju da se preklapaju pravougaonici posle brisanja.
}
// RECTANGLES END

//--------------------------------------
// SKOR BEGIN
int game_skor = 0; // ukupan skor od pocetka igre

// Prikazivanje skora na TFT ekranu
void show_skor() {
  int skor_x = 280;
  int skor_y = 20;
  
  char skor_chr[20];
  IntToStr(game_skor, skor_chr);
  
  drawRectangle(skor_x, skor_y, 2, 40);
  TFT_Write_Text(skor_chr, skor_x, skor_y);
}

// Za koordinate (xs, ys) racuna skor 
// +1 za sve zelene pravougaonike, -3 za sve crvene koje sadrze (xs, ys)
// i brise date prvaougaonike iz liste. 
int scoreAndRemove(int xs, int ys) {
  int i, skor;
  skor = 0;
  for (i = 0; i < nrect; i++) {
    if (isInside(xs, ys, i)) {
      if (color[i] == 1) skor -= 3;
      else if (color[i] == 0) skor++;
      remove(i);
      i--;
    }
  }
  return skor;
}
// SKOR END


// TFT BEGIN
int x, y;
int push_down = 0;

void initTFT() {
  int i;
  for (i = 0; i < 240; i++) {
    TFT_SET_Pen(TFT_RGBToColor16bit(255, 255, 255), 1);
    TFT_H_Line(0, 320, i);
  }
  show_skor();
}

// Funkcija se izvrasa nakon push_down ekrana
// Kooridnate (x, y) su koordinate pritiska
void tft_down_handler() {
  game_skor += scoreAndRemove(x, y);
  show_skor();
}
// TFT END


// COMMUNCATION BEGIN
char readbuff[64];
char writebuff[64];
int _xs, _ys;
int _color;

void USB0Interrupt() iv IVT_INT_OTG_FS {
  USB_Interrupt_Proc();
}

int ctoi(char ch) {
  return ch - '0';
}

// Parsira input po format cxxxyyy
// c - boja
// xxx - x koordinata
// yyy - y koordinata
// base je pocetak niza (0-based ili 1-based)
void parseInput(int base) {
  _color = 0;
  _xs = 0;
  _ys = 0;

  _color = readbuff[base] - '0';
  _xs = ctoi(readbuff[base+1])*100 + ctoi(readbuff[base+2])*10 + ctoi(readbuff[base+3]);
  _ys = ctoi(readbuff[base+4])*100 + ctoi(readbuff[base+5])*10 + ctoi(readbuff[base+6]);
}
// COMMUNCATION END

// DEBUG BEGIN
void test_primer() {
  add(50, 20, 0, 50);
  add(20, 20, 1, 50);
  add(70, 100, 0, 50);
  add(120, 120, 0, 50);
  add(130, 180, 0, 50);
  add(150, 150, 1, 50);
  add(200, 200, 0, 50);
  add(220, 220, 1, 50);
}
// DEBUG END
//-----------------------------------------------------

void main() {
  int k;
  Start_TP();
  initTFT();
  HID_Enable(&readbuff, &writebuff);

  // TEST 
  //test_primer();

  while (1) {
    if (TP_TFT_Press_Detect()) {
      if (TP_TFT_Get_Coordinates(&x, &y) == 0) {
        if (push_down == 0) {
          push_down = 1;
          tft_down_handler();
        } else {
          push_down = 0;
        }
      }
    }
    if (HID_Read()) {
      parseInput(0);
      if (_color == 0 || _color == 1) { 
        add(_xs, _ys, _color, 50); // 50px velicina (moze se menjati)
      } else if (_color == 2) {
        for (k = 0; k < nrect; k++) {
            if (ux[k] == _xs && uy[k] == _ys) {
               remove(k);
               break;
            }
        }
      }
      HID_Write(&writebuff, 64); // posle read-a mora da ima write, cak i ako se ne koristi.
    }
  }
}