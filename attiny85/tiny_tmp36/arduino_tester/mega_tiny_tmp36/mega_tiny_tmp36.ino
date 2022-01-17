#include <SPI.h>

// How fast SPI communication happens. ATTiny is running at 1MHz, so it needs to be slower than that.
#define SPI_CLOCK 200000

// Pin that indicates ready state. Shared with Data In on SPI.
#define READY_PIN 8

void setup()
{
    Serial.begin(9600);
    SPI.begin();

    // Set the ready pin as an input (this is a default)
    pinMode(READY_PIN, INPUT);
}

void loop()
{
    // Block until ready.
    while(!digitalRead(READY_PIN));

    // Read one LSB.
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    uint8_t lsb = SPI.transfer(0);
    SPI.endTransaction();

    // Block until ready.
    while(!digitalRead(READY_PIN));

    // Read MSB
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    uint8_t msb = SPI.transfer(0);
    SPI.endTransaction();

    // Output value
    int value = msb << 8 | lsb;
    float tempr = value / 100.0;

    Serial.print("T: ");
    Serial.print(tempr);
    Serial.print("(");
    Serial.print(value);
    Serial.print(" sent)");
    Serial.println();

    delay(1000);
}

#if 0
// Loop with manual reading.
void loop()
{
    // Block until ready pulled high.
    wait_ready();

    // Read one LSB.
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    uint8_t lsb = SPI.transfer(0);
    SPI.endTransaction();

    // Block until ready pulled high.
    delay(1);
    wait_ready();

    // Read MSB
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    uint8_t msb = SPI.transfer(0);
    SPI.endTransaction();

    // -----
    int value = msb << 8 | lsb;
    float tempr = value / 100.0;

    Serial.print("T: ");
    Serial.print(tempr);
    Serial.print("(");
    Serial.print(value);
    Serial.print(" sent)");
    Serial.println();
}


// Block until the ready pin is high.
void wait_ready()
{
    Serial.println("Wainting for ready signal...");

    // Block until ready.
    while(!digitalRead(READY_PIN));

    Serial.println("Ready! Send message to start.");
    while(!Serial.available());

    // Clear out the serial buffer.
    while(Serial.available())
    {
        Serial.read();
    }
    
}
#endif