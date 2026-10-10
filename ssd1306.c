#include <stdint.h>
#include "ssd1306.h"
#include "driver/i2c_master.h"

static i2c_master_bus_handle_t busHandle;
static i2c_master_dev_handle_t oled;

uint8_t buffer[1024];

static void oledCommand(uint8_t cmd) { // commands/data differentiated by 0x00/0x40 prefix
    uint8_t load[2] = {0x00, cmd};
    i2c_master_transmit(oled, load, 2, -1);
};

static void oledData(uint8_t data) {
    uint8_t load[2] = {0x40, data};
    i2c_master_transmit(oled, load, 2, -1);
};

void oledInit(void) {
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .glitch_ignore_cnt = 7
    };

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x3C,
        .scl_speed_hz = 400000
    };

    i2c_new_master_bus(&bus_config, &busHandle);
    i2c_master_bus_add_device(busHandle, &dev_config, &oled);

    // following initialization sequence taken from https://cdn-shop.adafruit.com/datasheets/UG-2864HSWEG01.pdf section 4.4

    oledCommand(0xAE); // Set Display Off

    oledCommand(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency
    oledCommand(0x80);

    oledCommand(0xA8); // Set Multiplex Ratio
    oledCommand(0x3F);

    oledCommand(0xD3); // Set Display Offset
    oledCommand(0x00);

    oledCommand(0x40); // Set Display Start Line

    oledCommand(0x8D); // Set Charge Pump
    oledCommand(0x14);

    oledCommand(0xA1); // Set Segment Re-Map

    oledCommand(0xC8); // Set COM Output Scan Direction

    oledCommand(0xDA); // Set COM Pins Hardware Configuration
    oledCommand(0x12);

    oledCommand(0x81); // Set Contrast Control
    oledCommand(0xCF);

    oledCommand(0xD9); // Set Pre-Charge Period
    oledCommand(0xF1);

    oledCommand(0xDB); // Set VCOMH Deselect Level
    oledCommand(0x40);

    oledCommand(0xA4); // Set Entire Display On/Off

    oledCommand(0xA6); // Set Normal/Inverse Display

    clearDisplay(); // Clear Screen

    oledCommand(0xAF); // Set Display On
};

void sendBuffer() {
    for (int i = 0; i < 8; i++) {
        oledCommand(0xB0 + i);
        oledCommand(0x00);
        oledCommand(0x10);
        for (int j = 0; j < 128; j++) {
            oledData(buffer[j + (i * 128)]);
        }
    }
};

void clearBuffer() {
    for (int i = 0; i < 1024; i++) {
        buffer[i] = 0;
    }
};

void drawPixel(int x, int y) {
    uint8_t byte = 1;
    byte <<= (y % 8); // move the 1 into position
    int pos = (x + ((y / 8) * 128));
    buffer[pos] |= byte;
};

void clearDisplay() { // leaving it like this is more efficient than using other functions to clear the display
     for (int i = 0; i < 8; i++) {
        oledCommand(0xB0 + i);
        oledCommand(0x00);
        oledCommand(0x10);
        for (int j = 0; j < 128; j++) { // columns auto increment so no need to change address
            oledData(0x00);
        };
    };
};
