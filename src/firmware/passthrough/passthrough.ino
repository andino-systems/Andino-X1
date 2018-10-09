/*
Simple passthrough firmware for the andino platform

--------------------------------------------------------------------------------------------
commands:

reset                               //  restarts the controller
status                              //  prints the value of all inputs and outputs
led {1|0}                           //  sets led to on or off
setup {shieldId}                    //  initializes the inputs and outputs specific to the given shield id
out {outputNo} {1|0}                //  turns on or off {outputNo}
--------------------------------------------------------------------------------------------
messages from the firmware are encoded in json format:

examples:
{"in":1,"val":false}                // no signal on input 1
{"in":2,"val":true}                 // signal on input 2
{"err":"some error message"}        // error
{"out":1,"val":true}                // response to out 1 1
{"led":"on"}                        // response to led 1
{"ready": true}                     // response when ready
{"shield":1,"inputs":3,"outputs":4} // response of setup
--------------------------------------------------------------------------------------------

*/

#include <avr/wdt.h>

enum { SHIELD_DEFAULT = 0, SHIELD_1DI2DO = 1, SHIELD_3DI = 2, SHIELD_5DI = 3 };
enum { PORT_PIN_B, PORT_PIN_C, PORT_PIN_D };
#define LED_PIN             13

#define MAX_INPUTS          7
#define MAX_OUTPUTS         4
#define BAUD_RATE           38400
#define SERIAL_BUFFER_SIZE  32

typedef struct {
  byte          pinNo;
  byte          port;
  byte          pinBit;
  byte          value;
} AndinoInput;

typedef struct {
  byte          pinNo;
  byte          value;
} AndinoOutput;

struct AndinoShield {
  AndinoInput   inputs[7];
  AndinoOutput  outputs[4];
  byte          shieldId;
  byte          nInputs;
  byte          nOutputs;
} MyShield;

char serialBuffer[SERIAL_BUFFER_SIZE];
byte serialBufferIndex = 0;

void definePorts() {
  pinMode(LED_PIN, OUTPUT);

  MyShield.inputs[0].pinNo = 7;
  MyShield.inputs[0].port = PORT_PIN_D;
  MyShield.inputs[0].pinBit = 7;

  MyShield.inputs[1].pinNo = 6;
  MyShield.inputs[1].port = PORT_PIN_D;
  MyShield.inputs[1].pinBit = 6;

  // Only available with 3DI or 1DI2DO or 5DI shield
  MyShield.inputs[2].pinNo = 2;
  MyShield.inputs[2].port = PORT_PIN_D;
 
  MyShield.inputs[2].pinBit = 2;

  // Only available with 3DI or 5DI
  MyShield.inputs[3].pinNo = 27;
  MyShield.inputs[3].port = PORT_PIN_C;
  MyShield.inputs[3].pinBit = 4;

  // Only available with 3DI or 5DI
  MyShield.inputs[4].pinNo = 28;
  MyShield.inputs[4].port = PORT_PIN_C;
  MyShield.inputs[4].pinBit = 5;

  // Only available with 5DI (PB4)
  MyShield.inputs[5].pinNo = 12;
  MyShield.inputs[5].port = PORT_PIN_B;
  MyShield.inputs[5].pinBit = 4;

  // Only available with 5DI (PB3)
  MyShield.inputs[6].pinNo = 11;
  MyShield.inputs[6].port = PORT_PIN_B;
  MyShield.inputs[6].pinBit = 3;

  //outputs:
  MyShield.outputs[0].pinNo = 14; // PC0 analog 0
  MyShield.outputs[1].pinNo = 15; // PC1 analog 1

  // Only available with 1DI2DO shield
  MyShield.outputs[2].pinNo = 19; // PC5 analog 5
  MyShield.outputs[3].pinNo = 10; // PB2
}

void setupShield(byte shieldId) {
  MyShield.shieldId = shieldId;

  int nInputs = 2;
  int nOutputs = 2;

  switch (shieldId) {
    case SHIELD_1DI2DO:
        nInputs += 1;
        nOutputs += 2;
    break;

    case SHIELD_3DI:
        nInputs += 3;
    break;

    case SHIELD_5DI:
        nInputs += 5;
    break;
  }

  for (int i = 0; i < nInputs; i++) {
    pinMode(MyShield.inputs[i].pinNo, INPUT);
    digitalWrite(MyShield.inputs[i].pinNo, HIGH); // activate pull up resistor
  }

  for (int i = 0; i < nOutputs; i++) {
    pinMode(MyShield.outputs[i].pinNo, OUTPUT);
  }

  MyShield.nInputs = nInputs;
  MyShield.nOutputs = nOutputs;
}

void writeSerialMessage(char key[], char error[]) {
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":\"");
  Serial.print(error);
  Serial.println("\"}");
}

void writeSerialError(char error[]) {
  writeSerialMessage("err", error);
}

void doReset() {
  wdt_enable(WDTO_60MS);
  while(1) {}
}

void writeSerialStatus(char key[], byte no, byte val) {
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":");
  Serial.print(no);
  Serial.print(",\"val\":");
  Serial.print(val == 1 ? "true" : "false");
  Serial.println("}");
}

byte getPortValue(byte port) {
  switch (port) {
    case PORT_PIN_B:
      return PINB;

    case PORT_PIN_C:
      return PINC;

    case PORT_PIN_D:
      return PIND;
  }
}

void doInputStatus(bool changesOnly) {
  for (int i = 0; i < MyShield.nInputs; i++) {
    byte val = bitRead(getPortValue(MyShield.inputs[i].port), MyShield.inputs[i].pinBit);
    val = !val;

    if (!changesOnly || val != MyShield.inputs[i].value) {
      writeSerialStatus("in", i + 1, val);
    }
    MyShield.inputs[i].value = val;
  }

  if (!changesOnly) {
    for (int i = 0; i < MyShield.nOutputs; i++) {
      writeSerialStatus("out", i + 1, MyShield.outputs[i].value);
    }
  }
}

void doShieldInfo() {
  Serial.print("{\"shield\":");
  Serial.print(MyShield.shieldId, DEC);

  Serial.print(",\"inputs\":");
  Serial.print(MyShield.nInputs, DEC);

  Serial.print(",\"outputs\":");
  Serial.print(MyShield.nOutputs, DEC);
  Serial.println("}");
}

void parseSerialData(char data[]) {
  String command = data;

  command.toLowerCase();

  if (command.startsWith("reset")) {
    doReset();

  } else if (command.startsWith("info")) {
    doShieldInfo();

  } else if (command.startsWith("status")) {
    doInputStatus(false);

  } else if (command.startsWith("led")) {
    String ledStateStr = command.substring(4);
    ledStateStr.trim();
    int ledState = ledStateStr.toInt();

    digitalWrite(LED_PIN, ledState);
    if (ledState == 1) {
      writeSerialMessage("led", "on");
    } else {
      writeSerialMessage("led", "off");
    }

  } else if (command.startsWith("setup")) {
    String shieldIdStr = command.substring(6);
    shieldIdStr.trim();
    int shieldId = shieldIdStr.toInt();

    if (shieldId < SHIELD_DEFAULT || shieldId > SHIELD_5DI) {
      writeSerialError("shield not found");
    } else {
      setupShield(shieldId);
      doShieldInfo();
    }
  } else if (command.startsWith("out")) {
    String outputNoStr = command.substring(4, 5);
    outputNoStr.trim();
    int outputNo = outputNoStr.toInt();

    String outputValueStr = command.substring(6);
    outputValueStr.trim();
    int outputValue = outputValueStr.toInt();

    if (outputNo < 1 || outputNo > MyShield.nOutputs) {
      writeSerialError("output not found");

    } else if (outputValue < 0 || outputValue > 1) {
      writeSerialError("state invalid");

    } else {
      digitalWrite(MyShield.outputs[outputNo - 1].pinNo, outputValue);
      MyShield.outputs[outputNo - 1].value = outputValue;
      writeSerialStatus("out", outputNo, outputValue);
    }

  } else {
    writeSerialError("unknown command");
  }
}

void readSerialData() {
 bool full = false;

 while (Serial.available() > 0 && serialBufferIndex < SERIAL_BUFFER_SIZE - 1) {
   char c = Serial.read();

   if (c >= 32) {
     serialBuffer[serialBufferIndex++] = c;
   } else if (c == 10 || c == 13) {
     serialBuffer[serialBufferIndex++] = '\0';
     full = true;
     break;
   }
 }

 if (full && serialBufferIndex > 1) {
   parseSerialData(serialBuffer);
   serialBufferIndex = 0;
 } else if (serialBufferIndex >= SERIAL_BUFFER_SIZE - 1) {
   writeSerialError("no newline found");
   serialBufferIndex = 0;
 }
}

void setup() {
  Serial.begin(BAUD_RATE);
  definePorts();
  setupShield(SHIELD_DEFAULT);

  Serial.println("{\"ready\": true}");
}

void loop() {
  readSerialData();
  doInputStatus(true);
}