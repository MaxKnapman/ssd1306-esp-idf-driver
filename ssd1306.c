#include <stdint.h>
#include "ssd1306.h"
#include "driver/i2c_master.h"

static i2c_master_bus_handle_t bus_handle;
static i2c_master_dev_handle_t oled;

static void oled_cmd(uint8_t cmd) { // commands/data differentiated by 0x00/0x40 prefix
    uint8_t load[2] = {0x00, cmd};
    i2c_master_transmit(oled, load, 1, -1);
};

static void oled_data(uint8_t data) {
    uint8_t load[2] = {0x40, data};
    i2c_master_transmit(oled, load, 1, -1);
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

    i2c_new_master_bus(&bus_config, &bus_handle);
    i2c_master_bus_add_device(bus_handle, &dev_config, &oled);

    // following initialization sequence taken from https://cdn-shop.adafruit.com/datasheets/UG-2864HSWEG01.pdf section 4.4

    oled_cmd(0xAE); // Set Display Off

    oled_cmd(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency
    oled_cmd(0x80);

    oled_cmd(0xA8); // Set Multiplex Ratio
    oled_cmd(0x3F);

    oled_cmd(0xD3); // Set Display Offset
    oled_cmd(0x00);

    oled_cmd(0x40); // Set Display Start Line

    oled_cmd(0x8D); // Set Charge Pump
    oled_cmd(0x14);

    oled_cmd(0xA1); // Set Segment Re-Map

    oled_cmd(0xC8); // Set COM Output Scan Direction

    oled_cmd(0xDA); // Set COM Pins Hardware Configuration
    oled_cmd(0x12);

    oled_cmd(0x81); // Set Contrast Control
    oled_cmd(0xCF);

    oled_cmd(0xD9); // Set Pre-Charge Period
    oled_cmd(0xF1);

    oled_cmd(0xDB); // Set VCOMH Deselect Level
    oled_cmd(0x40);

    oled_cmd(0xA4); // Set Entire Display On/Off

    oled_cmd(0xA6); // Set Normal/Inverse Display

    clearDisplay(); // Clear Screen

    oled_cmd(0xAF); // Set Display On
};

void drawPixel(int x, int y) { // currently overwrites whole byte ez fix
    oled_cmd(x % 16); // set column left and right address
    oled_cmd(0x10 + (x / 16));

    oled_cmd(0xB0 + (y / 8)); // set page number

    oled_data(1 << (y % 8)); // move bit into position
};

void clearDisplay() {
    for (int i = 0; i < 8; i++) {
        oled_cmd(0xB0 + i);
        oled_cmd(0x00);
        oled_cmd(0x10);
        for (int j = 0; j < 128; j++) { // columns auto increment so no need to change address
            oled_data(0x00);
        };
    };
};
