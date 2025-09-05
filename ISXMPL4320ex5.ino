/*
 * Coding with Arduino for the IS4320 is very simple. It does not require 
 * any INACKS-specific library, just the standard Arduino Wire functions:
 * Wire.write(), Wire.read() and related.  
 *
 * This Arduino project is based on the Arduino UNO board, and the 
 * Kappa4320Ard Evaluation Board, which features the IS4320 I2C Modbus 
 * RTU Master chip. The Kappa4320Ard has an Arduino form factor and 
 * directly fits into the Arduino UNO board, so no additional connections 
 * between the Arduino and the IS4320 are required, making it very easy 
 * to test the example.
 *
 * The project demonstrates how to use the Arduino to communicate with 
 * the IS4320 over I2C. In this example, the microcontroller instructs 
 * the IS4320 to read Holding Register 0 using the Function Code 3 from 
 * a Modbus Slave, and prints the result via Serial Port. 
 * 
 * To test the example, you will need a Modbus Slave. You can use the 
 * pyModSlave software, which is a Modbus TCP/RTU Slave simulator. 
 * Configure the Slave with these values: 
 * Slave Address 1, 19200 baud, Even parity, and 1 Stop bit.
 * 
 * More info at:
 * -	Kappa4320Ard Evaluation Board: https://www.inacks.com/kappa4320ard
 * -	IS4320 Datasheet: https://www.inacks.com/is4320
 * -	pyModSlave https://www.sourceforge.net/projects/pymodslave
 *
 * You can download this Arduino project at: https://github.com/inacks/ISXMPL4320ex5
 */

#include <Wire.h>

// IS4320 Memory Map Addresses:
#define CFG_MBBDR       0
#define CFG_MBPAR       1
#define CFG_MBSTP       2
#define REQ_EXECUTE     6
#define REQ_SLAVE       7
#define REQ_FC          8
#define REQ_STARTING    9
#define REQ_QTY         10
#define RES_STATUS      138
#define RES_DATA1       139

void writeIS4320Register(uint16_t registerAdressToWrite, uint16_t value) {
  Wire.beginTransmission(0x14); // I2C address of the IS4320.

  // Send the 16-bit Holding Register address (2 bytes).
  Wire.write((registerAdressToWrite >> 8) & 0xFF); // High byte.
  Wire.write(registerAdressToWrite & 0xFF);        // Low byte.

  // Send the 16-bit data (2 bytes).
  Wire.write((value >> 8) & 0xFF); // High byte.
  Wire.write(value & 0xFF);        // Low byte.

  Wire.endTransmission();
}


uint16_t readIS4320Register(uint16_t holdingRegisterAddress) {
  uint16_t result; // Variable to store the read data.
  
  Wire.beginTransmission(0x14); // I2C address of the IS4320.
  
  // Send the 16-bit Holding Register address (2 bytes).
  Wire.write((holdingRegisterAddress >> 8) & 0xFF); // High byte.
  Wire.write(holdingRegisterAddress & 0xFF);        // Low byte.
  Wire.endTransmission(false); // Send a repeated start condition.
  
  // Request 2 bytes from the IS4320 (a full 16-bit Holding Register).
  Wire.requestFrom(0x14, 2);  
  result = Wire.read();        // Read high byte.
  result <<= 8;                // Shift to make space for low byte.
  result |= Wire.read();       // Read and append low byte.

  return result; // Return the full 16-bit value.
}

void setup() {
  Wire.begin();           // Initialize the I2C interface.
  Serial.begin(9600);     // Initialize the serial port for debug printing.

  /* First, configure the Modbus communication parameters
   * to match the slave characteristics.
   * This only needs to be done once before communicating
   * with a Modbus device that has a different configuration. */

  // Set baud rate to 19200:
  uint16_t baudRate = 113;
  writeIS4320Register(CFG_MBBDR, baudRate);

  // Set parity to Even:
  uint16_t parityBit = 122;
  writeIS4320Register(CFG_MBPAR, parityBit);

  // Set stop bits to 1:
  uint16_t stopBits = 131;
  writeIS4320Register(CFG_MBSTP, stopBits);

}

void loop() {
  /* Example: Read Holding Register 0 and print its value
    * to the PC via the Serial port */

    // Set the Modbus Slave ID:
    uint16_t modbusSlaveId = 1;
    writeIS4320Register(REQ_SLAVE, modbusSlaveId);

    // Set the Function Code. For reading Holding Registers, use FC = 3:
    uint16_t functionCode = 3;
    writeIS4320Register(REQ_FC, functionCode);

    // Set the Starting Register address. Here we want to read Holding Register 0:
    uint16_t startingRegister = 0;
    writeIS4320Register(REQ_STARTING, startingRegister);

    // Set the number of Holding Registers to read (minimum is 1):
    uint16_t quantity = 1;
    writeIS4320Register(REQ_QTY, quantity);

    // Send the request to the Modbus Slave:
    writeIS4320Register(REQ_EXECUTE, 1);

    // Read back the result/status of the operation:
    uint16_t status = readIS4320Register(RES_STATUS);

    if (status == 2) {
      // OK! The request was sent and a response was received
      // Read the data:
      uint16_t holdingRegisterData = readIS4320Register(RES_DATA1);
      Serial.print("Holding Register 0 content = ");
      Serial.println(holdingRegisterData);
    }
    else if (status == 3) {
      // Timeout: no response from the Modbus Slave
      Serial.println("Timeout, the slave did not answer.");
      Serial.println("Did you start the pyModSlave? Did you set its configuration to: Slave Address 1, 19200 baud, Even parity, and 1 Stop bit?");
    }
    else if (status == 4) {
      Serial.println("Broadcast message sent.");
    }
    else if (status == 5) {
      Serial.println("You configured wrongly the REQ_SLAVE register.");
    }
    else if (status == 6) {
      Serial.println("You configured wrongly the REQ_FC register.");
    }
    else if (status == 7) {
      Serial.println("You configured wrongly the REQ_QTY register.");
    }
    else if (status == 8) {
      Serial.println("There was a Frame Error.");
    }
    else if (status == 201) {
      Serial.println("Modbus Exception Code 1: Illegal Function.");
    }
    else if (status == 202) {
      Serial.println("Modbus Exception Code 2: Illegal Data Address.");
      Serial.println("If using pyModSlave, make sure the 'Start Addr' parameter is set to 0.");
    }
    else if (status == 203) {
      Serial.println("Modbus Exception Code 3: Illegal Data Value.");
    }
    else if (status == 204) {
      Serial.println("Modbus Exception Code 4: Server Device Failure.");
    }
    else {
      Serial.println("Unkown Error");
    }
    
    // Add a delay to give the Modbus Slave time to respond and avoid stressing it: 
    delay(1000);

}