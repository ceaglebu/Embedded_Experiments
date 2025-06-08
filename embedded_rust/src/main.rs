#![no_std]
#![no_main]

use cortex_m::asm::nop;
use panic_halt as _;
use cortex_m_rt::entry;
use stm32f4xx_hal::{self as hal, gpio::GpioExt, hal::spi, spi::SpiExt};
use hal::pac;
// use rtt_target::{rtt_init_print, rprintln};
#[entry]
fn main() -> ! {
    // rtt_init_print!();
    // rprintln!("before loop");

    // GPIO A initialization
    let p = pac::Peripherals::take().unwrap();
    let gpioa = p.GPIOA.split();
    let mut pa5 = gpioa.pa5.into_push_pull_output();

    loop {
        match pa5.is_set_high() {
            true => pa5.set_low(),
            false => pa5.set_high(),
        }
        for _ in 0..100_000 {
            nop();
        }
    }
}
