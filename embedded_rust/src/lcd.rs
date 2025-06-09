pub enum Command {
    NOP = 0x00,
    SW_RES = 0x01, // software reset, must wait 5ms after sending this to send any other commands
    SLEEP_OUT = 0x11, // in sleep mode after reset
    DISPLAY_ON = 0x29, // display is off by default after reset
    COL_ADD_SET = 0x2A, // four parameters: SC 15->8, SC 7->0, EC 15->8, EC 7->0,
    PAGE_ADD_SET = 0x2B, // four parameters: SP, EP,
    MEM_WRITE = 0x2C, // followed by D1[17:0]..Dn[17:0]
    PIXEL_FORMAT_SET = 0x3A,
    INVERT_DISPLAY = 0x21,
}

pub enum Parameter {
    PIXEL_FORMAT = 0b01010101,
}
