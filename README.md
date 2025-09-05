# Coding with Arduino for the IS4320

Coding with Arduino for the IS4320 is very simple. It does not require any INACKS-specific library, just the standard Arduino Wire functions:  
`Wire.write()`, `Wire.read()` and related.  

---

## Hardware Setup

This Arduino project is based on:  

- **Arduino UNO board**  
- **Kappa4320Ard Evaluation Board**, which features the IS4320 I2C Modbus RTU Master chip  

The Kappa4320Ard has an Arduino form factor and directly fits into the Arduino UNO board, so no additional connections between the Arduino and the IS4320 are required — making it very easy to test the example.  

---

## Project Description

The project demonstrates how to use the Arduino to communicate with the IS4320 over I2C.  

In this example:  

- The microcontroller instructs the IS4320 to **read Holding Register 0**  
- It uses **Function Code 3** from a Modbus Slave  
- The result is printed via the **Serial Port**  

---

## Testing Requirements

To test the example, you will need a **Modbus Slave**.  
You can use the **pyModSlave** software, which is a Modbus TCP/RTU Slave simulator.  

Configure the Slave with these values:  

- **Slave Address:** 1  
- **Baud Rate:** 19200  
- **Parity:** Even  
- **Stop Bits:** 1  

---

## More Info

- [Kappa4320Ard Evaluation Board](https://www.inacks.com/kappa4320ard)  
- [IS4320 Datasheet](https://www.inacks.com/is4320)  
- [pyModSlave](https://www.sourceforge.net/projects/pymodslave)  

---

## Download

You can download this Arduino project at:  
👉 [GitHub – ISXMPL4320ex5](https://github.com/inacks/ISXMPL4320ex5)  
