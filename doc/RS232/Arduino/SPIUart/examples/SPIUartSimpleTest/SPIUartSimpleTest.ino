#define XTAL_FREQUENCY 11059200UL
#define PRESCALER 1UL // Default prescaler after reset

#include <SPI.h>
#include <SpiUart.h>

const int PIN_INT = 2;

SpiUartDevice SpiSerial;

void setup() 
{
  channelT channel = (channelT) -1;

  Serial.begin(115200);

  while( channel == -1 )
  {
    Serial.println( "Channel A or B" );
    while( Serial.available() == 0);
    char r = tolower(Serial.read());
    if( r == 'a' ) channel = channelA;
    if( r == 'b' ) channel = channelB;
  }
  
  pinMode(PIN_INT, INPUT);  // INT auf input

  Serial.println("-------------[ Start ]----------------");  
 
  Serial.print("Connecting to SC16IS752...Channel = ");Serial.println( channel );
  SpiSerial.begin(channel, 9600);
  Serial.print("Successfully connected to Channel = "); Serial.println( SpiSerial.uartChannel );
  Serial.println();


  Serial.println("Teste Register write/read [55,AA]");
  SpiSerial.writeRegister( SPR, 0x55 );
  Serial.println(SpiSerial.readRegister(SPR),HEX);
  SpiSerial.writeRegister( SPR, 0xAA );
  Serial.println(SpiSerial.readRegister(SPR),HEX);

 attachInterrupt(digitalPinToInterrupt(PIN_INT), isr, FALLING );
 
  Serial.println( "enable interrupts" );
  SpiSerial.writeRegister( IER, 0xCD );
  Serial.print( "IER: " ); Serial.println(SpiSerial.readRegister(IER),HEX);
  cmdPrintReg();
  printHelp();
}


void writeRegister(byte registerAddress, byte data) 
{
  Serial.println( registerAddress, HEX );
  SpiSerial.select();
  SPI.transfer(registerAddress);
  SPI.transfer(data);
  SpiSerial.deselect();
}


byte readRegister(byte registerAddress) 
{
  Serial.println( SPI_READ_MODE_FLAG | registerAddress, HEX );
  const byte SPI_DUMMY_BYTE = 0xFF; 
  char result;
  SpiSerial.select();
  SPI.transfer(SPI_READ_MODE_FLAG | registerAddress);
  result = SPI.transfer(SPI_DUMMY_BYTE);
  SpiSerial.deselect();
  return result;  
}



volatile int irqPending = 0;
void isr() 
{
  if( digitalRead(PIN_INT) == 0 )
    irqPending = 1;
}

char buffer[50];
void loop()
{
  checkCommand();

  if( irqPending == 1 )
  { 
    Serial.print( "IRQ " );
    cmdPrintReg();
    irqPending = 0;
  }
}

void readSpiUart()
{
  while(SpiSerial.available() > 0) {
    Serial.write(SpiSerial.read());
  }
}

void cmdPrintReg()
{
    sprintf( buffer, "IIR: %02X  LSR: %02X  MSR: %02X  MCR: %02X  RXLVL: %d  TXLVL: %d"
      ,SpiSerial.readRegister(IIR)
      ,SpiSerial.readRegister(LSR)
      ,SpiSerial.readRegister(MSR)
      ,SpiSerial.readRegister(MCR)
      ,SpiSerial.readRegister(RXLVL)
      ,SpiSerial.readRegister(TXLVL)
      );
    Serial.println(buffer);
}

void cmdReadAll()
{
  while( SpiSerial.available() > 0 )
  {
    byte r = SpiSerial.read();
    sprintf( buffer, "%02X[%c]", r, isalnum(r)?r:'?' );
    Serial.print( buffer );
  }
  Serial.println();
}

void cmdToggleRTS()
{
  byte r = SpiSerial.readRegister(MCR);
  r ^= 0x02;
  SpiSerial.writeRegister(MCR,r);
}

void cmdToggleDTR()
{
  byte r = SpiSerial.readRegister(MCR);
  r ^= 0x01;
  SpiSerial.writeRegister(MCR,r);
}

void checkCommand()
{
  byte cmd = 0;
  if( Serial.available() )
  {
    cmd = Serial.read();
    switch( tolower( cmd ) )
    {
    case 's':    SpiSerial.write('0');SpiSerial.write('1');SpiSerial.write('2');SpiSerial.write('3');   break;
    case 'p':    cmdPrintReg(); break;
    case 'r':    cmdReadAll(); break;
    case 't':    cmdToggleRTS();  break;
    case 'd':    cmdToggleDTR();  break;
    case '\r':   break;
    case '\n':   break;
    default: printHelp();break;
    }
  }
}

void printHelp()
{
  Serial.println("\r\n"
  "s......send pattern [0123]\r\n" 
  "r......read all data\r\n" 
  "p......print register\r\n" 
  "t......toggle rts\r\n" 
  "d......toggle dtr\r\n" );
}




