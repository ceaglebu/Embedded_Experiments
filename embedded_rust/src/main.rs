#![no_std]
#![no_main]

use cortex_m::asm::nop;
use cortex_m_rt::entry;
use hal::pac;
use panic_halt as _;
use stm32f4xx_hal::{
    self as hal,
    gpio::{GpioExt, Speed},
    hal::{delay::DelayNs, spi},
    rcc::RccExt,
    spi::SpiExt,
    time::Hertz, timer::TimerExt,
};

mod lcd;

use rtt_target::{rtt_init_print, rprintln};
#[entry]
fn main() -> ! {
    rtt_init_print!();
    // rprintln!("before loop");

    // get hal handles
    let p = pac::Peripherals::take().unwrap();

    // configufre / get clock info
    let rcc = p.RCC.constrain();
    let clocks = rcc
        .cfgr
        .use_hse(Hertz::MHz(48))
        .freeze();

    rprintln!("{:#?}", clocks);

    // init GPIO 
    let gpioa = p.GPIOA.split();
    let mut d_cx = gpioa.pa5.into_push_pull_output(); // data / command select
    let mut cs = gpioa.pa4.into_push_pull_output();
    cs.set_high();

    // init spi
    let gpiob = p.GPIOB.split();
    let sck = gpiob.pb3.into_alternate().speed(Speed::Medium);
    let miso = gpiob.pb4.into_alternate().speed(Speed::Medium);
    let mosi = gpiob.pb5.into_alternate().speed(Speed::Medium);

    let spi = p.SPI1.spi(
        (sck, miso, mosi),
        spi::Mode {
            polarity: spi::Polarity::IdleLow,
            phase: spi::Phase::CaptureOnFirstTransition,
        },
        Hertz::MHz(12),
        &clocks,
    );

    // 8 bit data frame is default
    // MSB first is default

    let mut spi = spi.init();

    // init TIM1 for delay timer
    let mut delay = p.TIM1.delay_us(&clocks);

    // post-reset delay
    delay.delay_ms(120);

    cs.set_low();
    d_cx.set_low();
    spi.write(&[
        lcd::Command::SLEEP_OUT as u8,
        lcd::Command::SW_RES as u8
    ]).unwrap(); // software reset of screen
    delay.delay_ms(10);
    // turn on display and start write
    spi.write(&[
        lcd::Command::SLEEP_OUT as u8,
        lcd::Command::DISPLAY_ON as u8,
        lcd::Command::MEM_WRITE as u8
    ]).unwrap();

    d_cx.set_high();

    // write frame
    for _ in 0u8.. 240 {
        for _ in 0u16..320 {
            spi.write(&[
                0u8, 0u8, 0u8
            ]).unwrap();
        }
    }

    d_cx.set_low();
    spi.write(&[lcd::Command::INVERT_DISPLAY as u8]).unwrap();

    cs.set_high();

    loop {
        for _ in 0..1_000_000 {
            nop();
        }
    }
}
