#include <SPI.h>

#define SPI_CLOCK 200000

void setup()
{
    Serial.begin(9600);

    SPI.begin();

    Serial.println("Press enter to start...");
    while(!Serial.available())
    {

    }
}

void loop()
{
    constexpr int delay_amt = 2000;

    Serial.println("Sending On (0x01)");
    send_spi_instr(0x01); // On
    delay(delay_amt);
    
    Serial.println("Sending Off (0x02)");
    send_spi_instr(0x02); // Off
    delay(delay_amt);

    read_temp();
    delay(delay_amt);
}

void send_spi_instr(char out_value)
{
    // Send data
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    int s = SPI.transfer(out_value);
    SPI.endTransaction();

    // Wait a little bit for processing.
    delay(1);

    // Read the response.
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    int r = SPI.transfer(0);
    SPI.endTransaction();

    Serial.print("Got from slave: ");
    Serial.print(r);
    Serial.print(" (");
    Serial.print(s);
    Serial.println(" from send)");
    Serial.println();
}

void read_temp()
{
    Serial.println("Sending temperature command");
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));

    SPI.transfer(0x10);

    delay(1);

    // Read response.
    int r = SPI.transfer(0x00);
    Serial.print("Got response: ");
    Serial.println(r);

    // Wait a while.
    delay(1); // TODO - is this long enough?

    // Send get temperature command.
    SPI.transfer(0x11);
    delay(1);

    // Read 2 bytes
    uint8_t lsb = SPI.transfer(0x00);
    Serial.print("Byte 1: ");
    Serial.println(lsb);

    delay(1);

    uint8_t msb = SPI.transfer(0x00);
    Serial.print("Byte 2: ");
    Serial.println(msb);

    int16_t result = (int16_t)((msb << 8) | lsb);

    Serial.print("Got: ");
    Serial.println(result);
    Serial.println();

    SPI.endTransaction();
}

void read_two_test()
{
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE1));
    SPI.transfer(0x03);

    delay(1);

    Serial.println("Reading two bytes");

    // Read 2 bytes
    uint8_t lsb = SPI.transfer(0x00);
    Serial.print("Byte 1: ");
    Serial.println(lsb);

    delay(1);

    uint8_t msb = SPI.transfer(0x00);
    Serial.print("Byte 2: ");
    Serial.println(msb);

    int16_t result = (int16_t)((msb << 8) | lsb);

    Serial.print("Got: ");
    Serial.println(result);

    SPI.endTransaction();
}