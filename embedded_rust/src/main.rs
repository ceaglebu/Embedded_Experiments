#![no_std]
#![no_main]

use cortex_m::asm::nop;
use panic_halt as _;
use cortex_m_rt::entry;
use stm32f4xx_hal::{self as hal, gpio::GpioExt, hal::spi, rcc::RccExt, spi::SpiExt, time::Hertz};
use hal::pac;
// use rtt_target::{rtt_init_print, rprintln};
#[entry]
fn main() -> ! {
    // rtt_init_print!();
    // rprintln!("before loop");

    // GPIO A initialization
    let p = pac::Peripherals::take().unwrap();
    let gpioa = p.GPIOA.split();
    let d_cx = gpioa.pa5.into_push_pull_output(); // data / command select
    let mut cs = gpioa.pa4.into_push_pull_output();

    let gpiob = p.GPIOB.split();
    let sck = gpiob.pb3.into_alternate();
    let miso = gpiob.pb4.into_alternate();
    let mosi = gpiob.pb5.into_alternate();

    let rcc = p.RCC.constrain();

    let clocks = rcc.cfgr.freeze();
    // rprintln!("{}", clocks.pclk1().to_Hz());

    let spi = p.SPI1.spi(
        (sck, miso, mosi), 
        spi::Mode {
            polarity: spi::Polarity::IdleLow, 
            phase: spi::Phase::CaptureOnFirstTransition
        }, 
        Hertz::MHz(8), 
        &clocks
    );

    // 8 bit data frame is default
    // MSB first is default

    let mut spi = spi.init();

    cs.set_low();
    spi.write(&[0xDE, 0xAD, 0xBE, 0xEF]).unwrap();

    loop {
        for _ in 0..1_000_000 {
            nop();
        }
    }
}
