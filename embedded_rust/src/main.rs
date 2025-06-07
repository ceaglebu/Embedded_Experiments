#![no_std]
#![no_main]

use cortex_m::asm::nop;
use panic_halt as _;
use cortex_m_rt::entry;
use stm32_metapac::{gpio::vals::{Odr, Moder}, GPIOA, RCC};
// use rtt_target::{rtt_init_print, rprintln};
#[entry]
fn main() -> ! {
    // rtt_init_print!();
    // rprintln!("before loop");

    // GPIO A initialization
    RCC.ahb1enr().modify(|r| r.set_gpioaen(true));
    GPIOA.moder().modify(|r| r.set_moder(5, Moder::OUTPUT));

    loop {
        if GPIOA.odr().read().odr(5) == Odr::HIGH {
            GPIOA.bsrr().write(|r| r.set_br(5, true));
        } else {
            GPIOA.bsrr().write(|r| r.set_bs(5, true));
        }

        for _ in 0..1_000_000 {
            nop();
        }
    }
}
