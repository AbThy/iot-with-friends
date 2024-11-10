# Overview
This example program demonstrates how the UID of a card read by the RFID-RC522 module connected to an Arduino Mega is sent to a Raspberry Pi via the Serial Port.

## Raspberry Pi

Rx GPIO 16 (RxD UART PIN10) -> Arduino Mega Tx0

GND PIN14

## Arduino Mega

Arduino Mega Tx0 -> Rx GPIO 16 (RxD UART PIN10)

| Signal      | MFRC522 Reader/PCD Pin | Arduino Mega Pin |
|-------------|-------------------------|-------------------|
| RST/Reset   | RST                     | 5                |
| SPI SS      | SDA(SS)                 | 53               |
| SPI MOSI    | MOSI                    | 51               |
| SPI MISO    | MISO                    | 50               |
| SPI SCK     | SCK                     | 52               |

> The RFID-RC522 requires a stable fixed current (3.3V).

## Voltage Divider (Arduino -> Raspberry Pi)

### Setting Up Connections
* #### Arduino Mega TX0 –> Raspberry Pi RX (GPIO 16) (via a level shifter)

> Arduino Mega RX0 –> Raspberry Pi TX (GPIO 14) GND between both devices

### Configuring the Voltage Divider (if no logic level shifter is used):<br />
Use a *1 kΩ* and a *2 kΩ* resistor to step down the **Arduino TX** signal. Connect the **Arduino TX** signal to the *1 kΩ* resistor, its other end to the **Raspberry Pi RX**, and connect the *2 kΩ* resistor to ground.

# Code
Run `rpi-rfid-read-rx.py` on the Raspberry Pi to read from the Serial Port.

Load the `arduino-rfid-read-send-tx.ino` code onto the Arduino.

### `rpi-rfid-read-rx.py`

> This code monitors incoming data on a serial port, reads it, and outputs it to the console until the program is stopped.

```python
import serial
import time
```

`serial`: The **pySerial** library, enabling serial communication from Python.

`time`: The **time** module provides functions for timing and delays.

```python
ser = serial.Serial('/dev/ttyS0', 230400, timeout=1)
time.sleep(2)
```

`serial.Serial('/dev/ttyS0', 230400, timeout=1)`: Opens a serial connection on the **/dev/ttyS0** device (the *<ins>serial port</ins>* in Linux; on Windows, for example, it might be *COM3*). **230400** is the baud rate, the data transfer speed in *bits per second*. **timeout=1** sets a **1** second timeout for *<ins>serial</ins>* port read operations.


`time.sleep(2)`: A **2-second** delay to allow the serial connection to stabilize before use.

```python
try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
```

`while True`: An infinite loop that continues to run until the program is interrupted.

`ser.in_waiting`: Checks if there is incoming data on the *<ins>serial port</ins>* (if present, the value will be greater than zero).

`ser.readline()`: Reads the incoming data until it finds a `\n` (newline) character.

`decode('utf-8')`: Decodes the bytes read into *UTF-8* text, making it readable as a string.

`rstrip()`: Removes any trailing `\n` (newline) or *whitespace* characters from the line.

`print(line)`: Outputs the decoded and formatted data to the console.

```python
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()
```

`except KeyboardInterrupt`: If the user interrupts the program (e.g., with Ctrl+C), this exception is triggered, avoiding an error on program interruption.

`finally`: The finally block ensures that `ser.close()` runs after the program is stopped, closing the *<ins>serial port</ins>*.

### `arduino-rfid-read-send-tx.ino`

 > This code continuously checks for a new card near the RFID reader. If detected, it reads the UID and displays it on the Serial Monitor.

```cpp
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 53
```

`SPI.h`: The library for managing **SPI communication**.

`MFRC522.h`: The library for managing the **MFRC522 RFID** reader.

`RST_PIN` és `SS_PIN`: The **RST** and **SS** pins for the **RC522 module** on the **Arduino Mega**. Pin **53** is the **SS** (*slave select*) pin, and pin **5** is the **reset pin**.

```cpp
MFRC522 mfrc522(SS_PIN, RST_PIN);  // MFRC522 reader instance

byte lastUID[10];
byte uidLength = 0;
```

`mfrc522`: The **MFRC522** instance, enabling the use of the **RFID** module.

`lastUID`: A *byte* array that stores the last-read card's **UID**.

`uidLength`: Stores the length of the **UID** in *bytes*, determined later.

```cpp
void setup() {
	Serial.begin(230400);
	while (!Serial);
	SPI.begin();
	mfrc522.PCD_Init();
	delay(4);
	mfrc522.PCD_DumpVersionToSerial();
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
```

`Serial.begin(230400)`: Opens a serial connection at **230400** *baud* for communication.

`while (!Serial)`: Waits until the *<ins>serial port<ins>* is open (required on some Arduino models).

`SPI.begin()`: Initializes the **SPI bus** for **RC522** communication.

`mfrc522.PCD_Init()`: Initializes the **RFID reader**.

`delay(4)`: Short delay for stability.

`mfrc522.PCD_DumpVersionToSerial()`: Outputs reader information (version, type) to the serial monito

`Serial.println(...)`: Displays a message on the serial monitor indicating readiness for card scanning.

```cpp
void loop() {
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}
```

`mfrc522.PICC_IsNewCardPresent()`: Checks if there is a new card near the reader. If not, it exits the *loop*, avoiding an attempt to read.

`mfrc522.PICC_ReadCardSerial()`: Reads the card's UID if present. If reading fails, it exits.

```cpp
	uidLength = mfrc522.uid.size;
	for (byte i = 0; i < uidLength; i++) {
		lastUID[i] = mfrc522.uid.uidByte[i];
	}

	Serial.print("Last read UID: ");
	for (byte i = 0; i < uidLength; i++) {
		Serial.print(lastUID[i], HEX);
		Serial.print(" ");
	}
	Serial.println();
```

`uidLength = mfrc522.uid.size`: Stores the **UID** length.

`for ciklus`: Copies each *byte* of the **UID** to the `lastUID` array.

`Serial.print() és Serial.println()`: Outputs the **UID** in *HEX* format to the serial monitor, separated by spaces.

```cpp
	mfrc522.PICC_HaltA();
	mfrc522.PCD_StopCrypto1();
}
```

`mfrc522.PICC_HaltA()`: Stops reading the **RFID** card.

`mfrc522.PCD_StopCrypto1()`: Ends encryption between the reader and the card, completing the read process.