
// Name  : Usage                        Lens Pin 1 - 8 ( from left)       Arduino
// CLK   : SPI Clock                    (Pin 1)                           D13
// DLC   : SPI Data Camera Input        (Pin 2) Lens --> Camera (MISO)    D12
// DCL   : SPI Data Camera Output       (Pin 3) Lens <-- Camera (MOSI)    D11
// ATT   : Attention from Camera        (Pin 4) Lens <-- Camera           D9
// LDR   : Lens Data Ready              (Pin 5)                           D8
// GND   : Ground                       (Pin 6)                           GND
// VDD   : Logic Power (3.3V)           (Pin 7)                           3V3
// VCC   : Focusing Motor Power (5V)    (Pin 8)                           5V
#include <SPI.h>

const int ldrPin = 8;
const int attPin = 9;
const int dclPin = 11;
const int dlcPin = 12;
const int clkPin = 13;

// buffer 255 bytes
byte dlcBuf[255];
byte dclBuf[255];

byte resEcho[2] = { 0xAA, 0x55 };

volatile byte dclIndex = 0;
volatile bool dclPacketReady = false;

void setup() {
   pinMode(dlcPin, OUTPUT);
   pinMode(attPin, INPUT); 
   pinMode(ldrPin, OUTPUT); 

   // set LDR HIGH to indicate the code is ready to take data from body
   digitalWrite(ldrPin, HIGH);

   // Set up SPI to communicate with NX body
   SPCR |= _BV(SPE);      // turn on SPI in slave mode
   SPCR |= _BV(SPIE);     // turn on interrupts
   SPI.setDataMode(SPI_MODE3);  

  // ready for receiving data
  dclIndex = 0;
  dclPacketReady = false;
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  // get one byte from SPI Data Register
  dclBuf[dclIndex++] = SPDR;
  
  // if all data from body is received.
  if (dclBuf[0] == dclIndex) {
    dclPacketReady = true;
    SPCR &= ~_BV(SPIE);     // disable the interrupt
  } 
}

void loop() {
  if (dclPacketReady) {
      handleCommand();

      // clear dcl info.
      dclIndex = 0;
      dclBuf[dclIndex] = 0;  
      dclPacketReady = false;
  }  
}

void handleCommand() {
  // 3 bytes commands
  if (dclBuf[0] == 0x03) {
    if (dclBuf[1] == 0xAA && dclBuf[2] == 0x55) {   // ECHO : 0xAA 0x55
      
      // response 0x03 0xAA 0x55
      dlcBuf[0] = (sizeof resEcho) + 1;
      memcpy( &(dlcBuf[1]), resEcho, sizeof resEcho );
    }
  }

  delayMicroseconds(40);
  
  digitalWrite(ldrPin, LOW);
  for (byte i = 0; i < dlcBuf[0]; i++) {
    SPI.transfer(dlcBuf[i]);
  }
  digitalWrite(ldrPin, HIGH);
}

