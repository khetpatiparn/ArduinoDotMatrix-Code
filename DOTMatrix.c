#include <SPI.h>
#define REG_DIGIT(x)      (0x1+(x))
#define REG_DECODE_MODE   (0x9)
#define REG_INTENSITY     (0xA)
#define REG_SCAN_LIMIT    (0xB)
#define REG_SHUTDOWN      (0xC)
#define REG_DISPLAY_TEST  (0xF)


const int CS_PIN   = 10;    // SPI /SS
const int CLK_PIN  = 13;  // SPI SCK
const int DIN_PIN  = 11;  // SPI MOSI

void MAX7219_write_reg( uint8_t addr, uint8_t data ) {
  digitalWrite( CS_PIN, LOW );
  SPI.transfer( addr );
  SPI.transfer( data );
  digitalWrite( CS_PIN, HIGH );
}

void MAX7219_init(void) {
  MAX7219_write_reg( REG_DECODE_MODE, 0x00 );  // decode mode: no decode for digits 0-7
  MAX7219_write_reg( REG_INTENSITY, 0x07 );    // set intensity: 0x07=15/32
  MAX7219_write_reg( REG_SCAN_LIMIT, 0x07 );   // scan limit: display digits 0-7
  MAX7219_write_reg( REG_SHUTDOWN, 0x01 );     // shutdown: normal operation
  MAX7219_write_reg( REG_DISPLAY_TEST, 0x00 ); // display test: no display test
}

void setup() {
  SPI.begin();
  SPI.setBitOrder( MSBFIRST );
  SPI.setClockDivider( SPI_CLOCK_DIV16 );// 16MHz/16 -> 1MHz SCK frequency
  SPI.setDataMode( SPI_MODE0 ); // use SPI mode 0
  pinMode( CS_PIN, OUTPUT );
  digitalWrite( CS_PIN, HIGH );
  MAX7219_init();
}

void test_mode() {
  MAX7219_write_reg( REG_SHUTDOWN, 0x01 );     // normal operation
  MAX7219_write_reg( REG_DISPLAY_TEST, 0x01 ); // enter display test mode
  delay(100);
  MAX7219_write_reg( REG_DISPLAY_TEST, 0x00 ); // exit display test mode
  MAX7219_write_reg( REG_SHUTDOWN, 0x00 );     // shutdown operation
  delay(900);
}

const byte char_data[][11] = { // 'start','K','H','E','T','0','4','7','8','end'
  { B10000000,  //start
    B11000000,
    B10100000,
    B10010000,
    B10001000,
    B10000100,
    B10000010,
    B11111111
  },
  { B11000111,  //K   
    B11001110,
    B11011100,
    B11111000,
    B11111000,
    B11011100,
    B11001110,
    B11000111
  },
  { B01100110,  //H
    B01100110,
    B01100110,
    B01111110,
    B01111110,
    B01100110,
    B01100110,
    B01100110
  },
  { B01111110,  //E
    B01111110,
    B01100000,
    B01111100,
    B01111100,
    B01100000,
    B01111110,
    B01111110
  },
  { B01111110,  //T
    B01111110,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000
  },
  { B00111100, //0
    B01100110,
    B01100110,
    B01100110,
    B01100110,
    B01100110,
    B01100110,
    B00111100
  },
  { B00001110,  //4
    B00011110,
    B00110110,
    B01100110,
    B11111111,
    B11111111,
    B00000110,
    B00000110
  },
  { B11111110,  //7
    B11111110,
    B00000110,
    B00011100,
    B00111000,
    B01110000,
    B01110000,
    B01110000
  },
  { B00111100,  //8
    B01100110,
    B01100110,
    B00111100,
    B01100110,
    B01100110,
    B01100110,
    B01111110
  },
  { B11000011,  //end
    B10001001,
    B00011000,
    B00111000,
    B01111000,
    B00000000,
    B10000011,
    B11000011
  }

  
};

void loop() {
  test_mode();
  MAX7219_write_reg( REG_SHUTDOWN, 0x01 );     // normal operation
  while (1) {
    static uint8_t ch = 0;
    for (uint8_t i = 0; i < 8; i++) {
      
      MAX7219_write_reg( REG_DIGIT(i), char_data[ch][i] );
    }
    delay(1500);
    ch = (ch + 1) % 10;
  }
}
