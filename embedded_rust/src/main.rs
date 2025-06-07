#![no_std]
#![no_main]

use core::ptr::write_volatile;

use cortex_m::asm::nop;
use panic_halt as _;
use cortex_m_rt::entry;
// use rtt_target::{rtt_init_print, rprintln};
#[entry]
fn main() -> ! {
    // rtt_init_print!();
    // rprintln!("before loop");
    const RCC_AHB1ENR: *mut u32 = 0x4002_3830 as *mut u32;
    const RCC_AHB1ENR_GPIOA_EN: u32 = 0x1; 
    const GPIOA_MODER: *mut u32 = 0x4002_0000 as *mut u32;
    const GPIO_MODER5_POS: u8 = 10;
    const GPIOA_BSSR: *mut u32 = 0x4002_0018 as *mut u32;

    // GPIO A initialization
    unsafe {
        write_volatile(RCC_AHB1ENR, *RCC_AHB1ENR | RCC_AHB1ENR_GPIOA_EN);
        write_volatile(GPIOA_MODER, *GPIOA_MODER | (1 << GPIO_MODER5_POS));
    }
    let mut led_state = false;

    loop {
        led_state = !led_state;

        unsafe {
            write_volatile(GPIOA_BSSR, *GPIOA_BSSR | (1 << 5) << match led_state {
                true => 0,
                false => 16,
            });
        }

        for _ in 0..100_000 {
            nop();
        }
    }
}
