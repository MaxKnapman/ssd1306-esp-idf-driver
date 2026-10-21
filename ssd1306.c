#include <stdint.h>
#include <string.h>
#include "ssd1306.h"
#include "driver/i2c_master.h"

static i2c_master_bus_handle_t busHandle;
static i2c_master_dev_handle_t oled;

uint8_t buffer[8][128]; // 8 pages 128 columns
uint8_t dataPrefix = 0x40;

static void oledCommand(uint8_t cmd) { // commands/data differentiated by 0x00/0x40 prefix
    uint8_t load[2] = {0x00, cmd};
    i2c_master_transmit(oled, load, 2, -1);
};

static void oledData(uint8_t *data) { // two transmits saves having to put 0x40 at the start of each row
    i2c_master_transmit(oled, &dataPrefix, 1, -1);
    i2c_master_transmit(oled, data, 128, -1);
};

void oledInit(void) {
    i2c_master_bus_config_t busConfig = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .glitch_ignore_cnt = 7
    };

    i2c_device_config_t devConfig = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x3C,
        .scl_speed_hz = 400000
    };

    i2c_new_master_bus(&busConfig, &busHandle);
    i2c_master_bus_add_device(busHandle, &devConfig, &oled);

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

    oledCommand(0xAF); // Set Display On
};

void sendBuffer() {
    for (int i = 0; i < 8; i++) {
        oledCommand(0xB0 + i);
        oledCommand(0x00);
        oledCommand(0x10);
        oledData(buffer[i]);
    };
};

void clearBuffer() {
    for (int i = 0; i < 8; i++) {
        memset(buffer, 0, sizeof buffer);
    };
};

void drawPixel(int x, int y) {
    uint8_t byte = 1;
    byte <<= (y % 8); // move the 1 into position
    buffer[y / 8][x] |= byte;
};
