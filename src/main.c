#include "stm32f1xx.h"

void delay(volatile uint32_t count) {
    while (count--) {
        __NOP();
    }
}

static void gpioa_init_outputs_0_7(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    for (uint32_t pin = 0; pin <= 7; ++pin) {
        uint32_t shift = pin * 4U;
        GPIOA->CRL &= ~(0xFU << shift);
        GPIOA->CRL |=  (0x3U << shift); // MODE=11, CNF=00 => output push-pull, 50 MHz
    }
}

int main(void) {
    gpioa_init_outputs_0_7();

    while (1) {
        // 1) Один светодиод на PA0 мигает 5 раз
        for (int i = 0; i < 5; ++i) {
            GPIOA->BSRR = (1U << 0U);
            delay(300000U);
            GPIOA->BSRR = (1U << (0U + 16U));
            delay(300000U);
        }

        // 2) Эффект "бегущий огонь" PA0..PA7
        for (int cycle = 0; cycle < 3; ++cycle) {
            for (uint32_t pin = 0; pin <= 7; ++pin) {
                GPIOA->BSRR = (1U << pin);
                delay(150000U);
                GPIOA->BSRR = (1U << (pin + 16U));
            }

            for (int pin = 7; pin >= 0; --pin) {
                GPIOA->BSRR = (1U << pin);
                delay(150000U);
                GPIOA->BSRR = (1U << (pin + 16U));
            }
        }

        delay(500000U);
    }
}
