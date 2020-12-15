//
//       #                                     #     #   #   
//      # #   #    # #####  # #    #  ####      #   #   ##   
//     #   #  ##   # #    # # ##   # #    #      # #   # #   
//    #     # # #  # #    # # # #  # #    #       #      #   
//    ####### #  # # #    # # #  # # #    #      # #     #   
//    #     # #   ## #    # # #   ## #    #     #   #    #   
//    #     # #    # #####  # #    #  ####     #     # #####
//
// Default / Sample Firmware for the Andino X1.
// 
// This example counts impulses at the digital inputs. Count stops are cyclically sent to the Raspberry. 
// The digital inputs are additionally de-bounced. On the other hand, the relays can be switched or pulsed.
// The settings of the firmware are changed via commands and are stored in the eeprom. 
// This Version consider the extension boards 3DI and 1DI2DO (Command HARD).
// The Communication runs with 38400 Baud.
// This Sample needs the TimerOne library.
// Interface, Commands
// All commands or messages are sent and received via /dev/ttyS0. 
// Every command has to be terminated by CR or LF. Message ends with CR and LF.
// RESET          ( Restart controller)
// INFO           ( print settings)
// HARD           ( Hardware, 0=noShield, 1=1DI2DO, 2=3DI, 3=5DI)
// POLL 10        ( Poll cycle in ms )
// DEBO 3         ( Debounce n Scans stable to accept )
// SKIP 3         ( Skip n Scans after pulse reconized )
// EDGE 1|0       ( count on Edge HL or LH )
// SEND 5000      ( send all xxx ms )
// CHNG 0|1       ( send on Pin Change - carefull if many changes)
// CNTR 0|1       ( Send counter, Default 1 otherwise on Pin States )
// REL? 0|1       ( send relay state - 1 to on or off )
// REL1 0|1       ( set relay 1 to on or off )
// REL2 0|1       ( set relay 2 to on or off )
// REL3 0|1       ( set relay 3 to on or off )
// REL4 0|1       ( set relay 4 to on or off )
// RPU1 1000      ( pulse relay 1 for nnn ms )
// RPU2 1000      ( pulse relay 2 for nnn ms )
//
// Message from the Firmware to the Raspberry
//
// :Message-ID{counter1,counter2,..}{state1,state2}
// 
// The Message starts with a ':'. After that follows a Message-ID. This is a modulo HEX Counter from 0..FFFF.
// Then within a '{' '}' the counter follows. The number of counter depends on the Hardware shields.
// The Counter are HEX encoded and runs from 0 to FFFF (modulo).
// Then again within a '{' '}' the current state of the inputs follows. 0-off, 1-on.
// if REL? = 1 a third {,} is appended with the current state of the Relais
// The number depends on the Hardware shields. The Message ends with a CR / LF [0x0d + 0x0a]
// Example
// :0040{0002,0000,000B}{0,0,0}
// :0041{0002,0000,000B}{0,0,0}
// :0042{0004,0000,000C}{0,0,0}
// :0043{0004,0000,000C}{0,0,0}
// :0044{0008,0000,000F}{0,0,1}
// :0045{0008,0000,000F}{0,0,1}
// or if REL? = 1
// :0040{0002,0000,000B}{0,0,0}{0,0}
// :0041{0002,0000,000B}{0,0,0}{0,0}
// :0042{0004,0000,000C}{0,0,0}{0,0}
#define VERSION 201106
// History:
// 201106: New Option: send relay state
// 200929: void setup_in_out() correction for 5DI
// 181026: New Command CHNG, Version in Info, CNTR can switch off the Counter - Only Events will be send
// 181126: Count-up only on configured EDGE
// 181216: Fixed debouncing

#include <EEPROM.h>
#include <TimerOne.h>
#include <avr/wdt.h>

#define LED_PIN   13

#define SHIELD_NONE 0
#define SHIELD_1DI2DO 1
#define SHIELD_3DI 2
#define SHIELD_5DI 3

#define IN_1       7 // PD7
#define IN_1_PORT  PIND
#define IN_1_PIN   7

#define IN_2       6 // PD6
#define IN_2_PORT  PIND
#define IN_2_PIN   6

// Only available with 3DI or 1DI2DO or 5DI shield
#define IN_3       2 // PD2
#define IN_3_PORT  PIND
#define IN_3_PIN   2

// Only available with 3DI or 5DI
#define IN_4       27 // PC4
#define IN_4_PORT  PINC
#define IN_4_PIN   4

// Only available with 3DI or 5DI
#define IN_5       28 // PC5
#define IN_5_PORT  PINC
#define IN_5_PIN   5

// Only available with 5DI (PB4)
#define IN_6       12 // PB4
#define IN_6_PORT  PINB
#define IN_6_PIN   4

// Only available with 5DI (PB3)
#define IN_7       11 // PB3
#define IN_7_PORT  PINB
#define IN_7_PIN   3


#define OUT_1     14 // PC0 analog 0
#define OUT_2     15 // PC1 analog 1
// Only available with 1DI2DO shield
#define OUT_3     19 // PC5 analog 5
#define OUT_4     10 // PB2 

#define LF 10
#define CR 13

#define BAUD_RATE 38400

// ---------------------- [ Receive Data ]------------------
#define RX_SIZE  19
byte RxIndex = 0;
char RxBuffer[RX_SIZE+1];

// ---------------------- [ Setup Data ]------------------
struct Setup
{
  unsigned long CRC;          // CRC of the data
  byte          StructLen;    // Length of the structure
  unsigned int  PollCycle;    // Poll cycle in ms
  byte          PollCount;    // Poll-Count till level is stable
  bool          CountOnLH;    // Count on .. edge
  byte          SkipCount;    // Skip nn Scans after recognize a pulse
  unsigned int  SendCycle;    // Send cycle in ms
  byte          Shield;       // 0 = none, 1 = 1DI2DO, 2 = 3DI
  byte          SendOnChange; // 0 = Send only in clycle, 1 = send on Demand (Pin Change)
  byte          SendCounter;  // 0 = Send only Pin States, 1 = send counter and States
  byte          SendRelayInfo; // 0 = Send no RelayChange, 1 = send relay changes
} TheSetup;

// ---------------------- [ Input & Debounce ] ----------------
typedef struct {
  byte current_val = 0;
  byte poll_counter = 0;
  byte skip_counter = 0;
  unsigned long Counter = 0;
} CounterControl;

CounterControl Counter1;
CounterControl Counter2;
CounterControl Counter3;
CounterControl Counter4;
CounterControl Counter5;
CounterControl Counter6;
CounterControl Counter7;

int ledState = LOW;

// ---------------------- [ Releais ] ----------------

typedef struct {
  byte puls_timer  = 0;
} RelaisControl;

RelaisControl Relais1;
RelaisControl Relais2;
RelaisControl Relais3;
RelaisControl Relais4;

bool pinChanged = false;

void setup() 
{
  Serial.begin(BAUD_RATE);
  Serial.println( "STRT" );

  SetupRead();
  pinMode(LED_PIN, OUTPUT);
  setup_in_out();
  setup_interrupt();
}

void setup_in_out()
{
  pinMode(IN_1,INPUT); 
  digitalWrite(IN_1, HIGH); // activate pull up resistor 
  
  pinMode(IN_2,INPUT); 
  digitalWrite(IN_2, HIGH); // activate pull up resistor 

  pinMode(IN_3,INPUT); 
  digitalWrite(IN_3, HIGH); // activate pull up resistor 

  pinMode(OUT_1,OUTPUT);
  pinMode(OUT_2,OUTPUT);
  
  if( TheSetup.Shield == SHIELD_1DI2DO )
  {  
    pinMode(OUT_3,OUTPUT);
    pinMode(OUT_4,OUTPUT);
  }
  if( TheSetup.Shield == SHIELD_3DI || TheSetup.Shield == SHIELD_5DI)
  {  
    pinMode(IN_4,INPUT); 
    digitalWrite(IN_4, HIGH); // activate pull up resistor 
    pinMode(IN_5,INPUT); 
    digitalWrite(IN_5, HIGH); // activate pull up resistor 
  }
  if( TheSetup.Shield == SHIELD_5DI )
  {  
    pinMode(IN_6,INPUT); 
    digitalWrite(IN_6, HIGH); // activate pull up resistor 
    pinMode(IN_7,INPUT); 
    digitalWrite(IN_7, HIGH); // activate pull up resistor 
  }
  
}

void setup_interrupt()
{
  unsigned long timeInMicro = TheSetup.PollCycle;
  timeInMicro *= 1000;
  Timer1.initialize(timeInMicro); 
  Timer1.detachInterrupt();
  Timer1.attachInterrupt(timerInterrupt);
}

unsigned long lastSendMillis = 0;
unsigned long lastSecondMillis = 0;

word loopCounter = 0;

void loop() 
{
  DoCheckRxData();
  unsigned long currentMillis = millis();
  if(   (currentMillis - lastSendMillis  >= TheSetup.SendCycle) 
     || (TheSetup.SendOnChange && pinChanged ) )
  {
    pinChanged = false;
    lastSendMillis = currentMillis;
    Serial.write( ':' );
    PrintHex16(++loopCounter); 
    if( TheSetup.SendCounter )
    {
      Serial.write( '{' );
      PrintHex16(Counter1.Counter); 
      Serial.write( ',' );
      PrintHex16(Counter2.Counter);
      if( TheSetup.Shield == SHIELD_1DI2DO || TheSetup.Shield == SHIELD_3DI || TheSetup.Shield == SHIELD_5DI)
      {
        Serial.write( ',' );
        PrintHex16(Counter3.Counter);
      }
      if( TheSetup.Shield == SHIELD_3DI || TheSetup.Shield == SHIELD_5DI)
      {
        Serial.write( ',' );
        PrintHex16(Counter4.Counter);
        Serial.write( ',' );
        PrintHex16(Counter5.Counter);
      }    
      if( TheSetup.Shield == SHIELD_5DI)
      {
        Serial.write( ',' );
        PrintHex16(Counter6.Counter);
        Serial.write( ',' );
        PrintHex16(Counter7.Counter);
      }
      Serial.write( "}" );
    }    
    Serial.write( "{" );
    Serial.print(Counter1.current_val, DEC ); 
    Serial.write( ',' );
    Serial.print(Counter2.current_val, DEC ); 
    if( TheSetup.Shield == SHIELD_1DI2DO || TheSetup.Shield == SHIELD_3DI || TheSetup.Shield == SHIELD_5DI)
    {
      Serial.write( ',' );
      Serial.print(Counter3.current_val, DEC );
    }
    if( TheSetup.Shield == SHIELD_3DI || TheSetup.Shield == SHIELD_5DI )
    {
      Serial.write( ',' );
      Serial.print(Counter4.current_val, DEC );
      Serial.write( ',' );
      Serial.print(Counter5.current_val, DEC );
    }
    if( TheSetup.Shield == SHIELD_5DI )
    {
      Serial.write( ',' );
      Serial.print(Counter6.current_val, DEC );
      Serial.write( ',' );
      Serial.print(Counter7.current_val, DEC );
    }
    Serial.write( "}" );
    if( TheSetup.SendRelayInfo == 1 )
    {
      Serial.write( "{" );
      Serial.print(digitalRead(OUT_1));
      Serial.write( ',' );
      Serial.print(digitalRead(OUT_2));
      if( TheSetup.Shield == SHIELD_1DI2DO )
      {
        Serial.write( ',' );
        Serial.print(digitalRead(OUT_3));
        Serial.write( ',' );
        Serial.print(digitalRead(OUT_4));
      }
      Serial.write( "}" );
    }
    Serial.println();
  }

  if (currentMillis - lastSecondMillis  >= 100) 
  {
    lastSecondMillis = currentMillis;
    if( Relais1.puls_timer != 0 )
    {
      if( --Relais1.puls_timer == 0 )
      {
        digitalWrite(OUT_1, 0);
        Serial.println( "REL1 0" );
      }
    }
    if( Relais2.puls_timer != 0 )
    {
      if( --Relais2.puls_timer == 0 )
      {
        digitalWrite(OUT_2, 0);
        Serial.println( "REL2 0" );
      }
    }
    if( Relais3.puls_timer != 0 )
    {
      if( --Relais3.puls_timer == 0 )
      {
        digitalWrite(OUT_3, 0);
        Serial.println( "REL3 0" );
      }
    }
    if( Relais4.puls_timer != 0 )
    {
      if( --Relais4.puls_timer == 0 )
      {
        digitalWrite(OUT_4, 0);
        Serial.println( "REL4 0" );
      }
    }
  }
}

void timerInterrupt()
{
  byte input;
  
  if (ledState == LOW)
    ledState = HIGH;
  else
    ledState = LOW;
  digitalWrite(LED_PIN, ledState);

  doCounter( &Counter1, bitRead( IN_1_PORT, IN_1_PIN ) );
  doCounter( &Counter2, bitRead( IN_2_PORT, IN_2_PIN ) );
  if( TheSetup.Shield == SHIELD_NONE )
    return;
  doCounter( &Counter3, bitRead( IN_3_PORT, IN_3_PIN ) );
  if( TheSetup.Shield == SHIELD_1DI2DO )
    return;
  doCounter( &Counter4, bitRead( IN_4_PORT, IN_4_PIN ) );
  doCounter( &Counter5, bitRead( IN_5_PORT, IN_5_PIN ) );
  if( TheSetup.Shield == SHIELD_3DI )
    return;
  doCounter( &Counter6, bitRead( IN_6_PORT, IN_6_PIN ) );
  doCounter( &Counter7, bitRead( IN_7_PORT, IN_7_PIN ) );
  
}

void doCounter( CounterControl * pCounter, byte input )
{
  if( pCounter->skip_counter == 0 )
  {
    input = (input==1 )?0:1;
    if( pCounter->current_val  != input )
    {
        pCounter->poll_counter ++;
        if( pCounter->poll_counter == TheSetup.PollCount )
        {
          if( input == TheSetup.CountOnLH )
          {
             pCounter->Counter++;
          }
          pCounter->current_val   = input;
          pCounter->poll_counter  = 0;
          pCounter->skip_counter = TheSetup.SkipCount;
          pinChanged = true;
        }
    }
    else
    {
      pCounter->poll_counter = 0;
    }
  }
  else
  {
    pCounter->skip_counter--;
  }
}

// ----------------------------------------------------------------------------------
// Setup
// ----------------------------------------------------------------------------------
unsigned long SetupCalcCrc()
{
  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  unsigned long crc = ~0L;

  byte * p = (byte*)&TheSetup.StructLen;
  int len = sizeof( TheSetup ) - sizeof( TheSetup.CRC );
  for (int i = 0 ; i<len; i++)
  {
    crc = crc_table[(crc ^ *p) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (*p >> 4)) & 0x0f] ^ (crc >> 4);
    p++;
    crc = ~crc;
  }
  return crc;
}

void SetupDefault()
{
  TheSetup.PollCycle = 10;    // Poll cykle 
  TheSetup.PollCount = 3;     // Poll count till accepted
  TheSetup.SkipCount = 0;     // Skip nn Scans after recognize a pulse
  TheSetup.CountOnLH = 1;     // LH Edge
  TheSetup.SendCycle = 2000;  // Cycle in ms
  TheSetup.Shield = 0;        // No Shield 
  TheSetup.SendOnChange = 0;  // No Send on Pin Change 
  TheSetup.SendCounter = 1;   // Send Counter
  TheSetup.SendRelayInfo = 1; // No Relay Change
}

#define EEPROM_ADDRESS  0
void SetupRead()
{
  EEPROM.get(EEPROM_ADDRESS, TheSetup);
  if( TheSetup.CRC != SetupCalcCrc() )
  {
    Serial.println( "ERROR CONFIG SET TO DEFAULT" );
    DoCmdInfo();
    SetupDefault();
    SetupWrite();
  }
}

void SetupWrite()
{
  TheSetup.StructLen = sizeof( TheSetup );
  TheSetup.CRC = SetupCalcCrc();
  EEPROM.put(EEPROM_ADDRESS, TheSetup);
}

// ----------------------------------------------------------------------------------
// R X D
// ----------------------------------------------------------------------------------
void DoCheckRxData()
{
  if( Serial.available() > 0 )
  {
    if( RxIndex >= RX_SIZE )
    {
      RxIndex = 0;
    }
    byte rx = Serial.read();

    if( RxIndex == 0 && (rx == LF || rx == CR) )
      return;
    if( rx == LF || rx == CR )
    {
      RxBuffer[RxIndex++] = 0;
      OnDataReceived(); 
      RxIndex = 0;
    }
    else
    {
      RxBuffer[RxIndex++] = rx;
    }
  }
}


// ----------------------------------------------------------------------------------
// C O M M A N D S
// ----------------------------------------------------------------------------------
// See first page
void OnDataReceived()
{
  bool result = false;
  bool writeSetup = false;
  bool callSetupInterrupt = false;
  String cmd = RxBuffer;
  cmd.toUpperCase();
  
  if( cmd.startsWith("RESET"))
  {
    Serial.println( "OK" );
    DoCmdReset();
  }
  
  if(  cmd.startsWith("INFO")
    || cmd.startsWith("HELP"))
  {
    DoCmdInfo();
    result = true;
  }

  int value = toInt( cmd, 5 );
  if( value != -1 )
  {
    if( cmd.startsWith("POLL "))
    {
      if( !checkRange( value, 10, 1000 ) )
        return;
      Serial.print( "POLL ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.PollCycle != value;
      TheSetup.PollCycle = value;
      result = true;
      callSetupInterrupt = true;
    }
    else
    if( cmd.startsWith("SKIP "))
    {
      if( !checkRange( value, 0, 250 ) )
        return;
      Serial.print( "SKIP ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.SkipCount != value;
      TheSetup.SkipCount = value;
      result = true;
      callSetupInterrupt = true;
    }
    else
    if( cmd.startsWith("DEBO "))
    {
      if( !checkRange( value, 1, 20 ) )
        return;
      Serial.print( "DEBO ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.PollCount != value;
      TheSetup.PollCount = value;
      result = true;
    }
    else
    if( cmd.startsWith("EDGE "))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      TheSetup.CountOnLH = value;
      Serial.print( "EDGE ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.CountOnLH != value;
      TheSetup.CountOnLH = value;
      result = true;
    }
    else
    if( cmd.startsWith("REL? "))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      writeSetup = TheSetup.SendRelayInfo != value;
      TheSetup.SendRelayInfo = value;
      Serial.print( "REL? ");
      Serial.println( value, DEC );
      result = true;
    }
    else
    if( cmd.startsWith("REL1 "))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      digitalWrite(OUT_1, value);
      Serial.print( "REL1 ");
      Serial.println( value, DEC );
      result = true;
    }
    else
    if( cmd.startsWith("REL2 "))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      digitalWrite(OUT_2, value);
      Serial.print( "REL2 ");
      Serial.println( value, DEC );
      result = true;
    }
    else
    if( TheSetup.Shield == SHIELD_1DI2DO &&  cmd.startsWith("REL3 "))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      digitalWrite(OUT_3, value);
      Serial.print( "REL3 ");
      Serial.println( value, DEC );
      result = true;
    }
    else
    if( TheSetup.Shield == SHIELD_1DI2DO && cmd.startsWith("REL4 "))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      digitalWrite(OUT_4, value);
      Serial.print( "REL4 ");
      Serial.println( value, DEC );
      result = true;
    }
    else
    if( cmd.startsWith("RPU1 "))
    {
      if( !checkRange( value, 1, 255 ) )
        return;
      digitalWrite(OUT_1, 1);
      Relais1.puls_timer = value;
      Serial.print( "RPU1 ");
      Serial.println( value, DEC );
      Serial.println( "REL1 1" );
      result = true;
    }
    else
    if( cmd.startsWith("RPU2 "))
    {
      if( !checkRange( value, 0, 255 ) )
        return;
      digitalWrite(OUT_2, 1);
      Relais2.puls_timer = value;
      Serial.print( "RPU2 ");
      Serial.println( value, DEC );
      Serial.println( "REL2 1" );
      result = true;
    }
    else
    if( TheSetup.Shield == SHIELD_1DI2DO && cmd.startsWith("RPU3 "))
    {
      if( !checkRange( value, 1, 255 ) )
        return;
      digitalWrite(OUT_3, 1);
      Relais3.puls_timer = value;
      Serial.print( "RPU3 ");
      Serial.println( value, DEC );
      Serial.println( "REL3 1" );
      result = true;
    }
    else
    if( TheSetup.Shield == SHIELD_1DI2DO && cmd.startsWith("RPU4 "))
    {
      if( !checkRange( value, 0, 255 ) )
        return;
      digitalWrite(OUT_4, 1);
      Relais4.puls_timer = value;
      Serial.print( "RPU4 ");
      Serial.println( value, DEC );
      Serial.println( "REL4 1" );
      result = true;
    }
    else
    if( cmd.startsWith("HARD"))
    {
      if( !checkRange( value, 0, 3 ) )
        return;
      Serial.print( "HARD ");
      Serial.println( value, DEC );
      setup_in_out();
      writeSetup = TheSetup.Shield != value;
      TheSetup.Shield = value;
      result = true;
    }
    else
    if( cmd.startsWith("CHNG"))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      Serial.print( "CHNG ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.SendOnChange != value;
      TheSetup.SendOnChange = value;
      result = true;
    }
    else
    if( cmd.startsWith("CNTR"))
    {
      if( !checkRange( value, 0, 1 ) )
        return;
      Serial.print( "CNTR ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.SendCounter != value;
      TheSetup.SendCounter = value;
      result = true;
    }
    else
    if( cmd.startsWith("SEND "))
    {
      if( !checkRange( value, 100, 50000 ) )
        return;
      Serial.print( "SEND ");
      Serial.println( value, DEC );
      writeSetup = TheSetup.SendCycle != value;
      TheSetup.SendCycle = value;
      result = true;
    }
  }
  if( result )
  {
    if( writeSetup )
      SetupWrite();
    if( callSetupInterrupt )
      setup_interrupt();
  }
  else
    Serial.println( "SYNTAX" );
}

void DoCmdInfo()
{
    Serial.print( "HARD "); Serial.println( TheSetup.Shield, DEC );
    Serial.print( "POLL "); Serial.println( TheSetup.PollCycle, DEC );
    Serial.print( "DEBO "); Serial.println( TheSetup.PollCount, DEC );
    Serial.print( "SKIP "); Serial.println( TheSetup.SkipCount, DEC );
    Serial.print( "EDGE "); Serial.println(TheSetup.CountOnLH, DEC );
    Serial.print( "SEND "); Serial.println( TheSetup.SendCycle, DEC);
    Serial.print( "CHNG "); Serial.println( TheSetup.SendOnChange, DEC);
    Serial.print( "CNTR "); Serial.println( TheSetup.SendCounter, DEC);
    Serial.print( "REL? "); Serial.println( TheSetup.SendRelayInfo, DEC);
    Serial.print( "REL1 "); Serial.println( digitalRead(OUT_1), DEC);
    Serial.print( "REL2 "); Serial.println( digitalRead(OUT_2), DEC);
    if( TheSetup.Shield == SHIELD_1DI2DO)
    {
      Serial.print( "REL3 "); Serial.println( digitalRead(OUT_3), DEC);
      Serial.print( "REL4 "); Serial.println( digitalRead(OUT_4), DEC);
    }
    Serial.println( "HARD 0 (no extension)" );
    Serial.println( "HARD 1 (1DI2DO)" );
    Serial.println( "HARD 2 (3DI)" );
    Serial.println( "HARD 3 (5DO)" );
    Serial.print( "VERS " ); Serial.println(VERSION);
}

void DoCmdReset()
{
  wdt_enable( WDTO_60MS );
   while(1) {}
}

bool checkRange( unsigned int val, unsigned int mini, unsigned int maxi )
{
  if( val < mini || val > maxi )
  {
    Serial.print( "INVALID. MIN=" );
    Serial.print( mini, DEC );
    Serial.print( " MAX=" );
    Serial.println( maxi, DEC );
    return false;
  }
  return true;
}

int toInt( String s, int pos  )
{
  String valString = s.substring(pos);
  valString.trim();
  if( valString.length() == 0 )
    return -1;
  if( !isNumeric(valString) )
    return -1;
  return valString.toInt();
}

boolean isNumeric(String str) 
{
    for(char i = 0; i < str.length(); i++) 
    {
        if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )) 
        {
            return false;
        }
    }
    return true;
}


void PrintHex16(word data)
{
 char tmp[5];
 byte first;
 int j=0;

   first = (data >> 12) | 48;
   if (first > 57) tmp[0] = first + (byte)7;
   else tmp[0] = first ;
  
   first = ((data>>8) & 0x0F) | 48;
   if (first > 57) tmp[1] = first + (byte)7; 
   else tmp[1] = first;

   first = ((data>>4) & 0x0F) | 48;
   if (first > 57) tmp[2] = first + (byte)7; 
   else tmp[2] = first;

   first = (data & 0x0F) | 48;
   if (first > 57) tmp[3] = first + (byte)7; 
   else tmp[3] = first;
   
   tmp[4] = 0;
   Serial.print(tmp);
}
