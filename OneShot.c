#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define Green_LED 11
#define Blue_LED 12
#define Red_LED 13
#define BUTTON 5

volatile int estado = 0;
volatile bool leds_ativo = false;

bool semafaro(struct repeating_timer *t) {
    // Desliga todos os LEDs
    gpio_put(Red_LED, 0);
    gpio_put(Green_LED, 0);
    gpio_put(Blue_LED, 0);

    // Liga o LED correspondente ao estado
    if (estado == 0) {
        gpio_put(Red_LED, 1);
    } else if (estado == 1) {
        gpio_put(Blue_LED, 1);
    } else if (estado == 2) {
        gpio_put(Green_LED, 1);
    }
    
    // Atualiza o estado do semáforo ciclicamente
    estado = (estado + 1) % 3;
    return true; // Mantém o timer repetindo
}

int64_t turn_off_green(alarm_id_t id, void *user_data);
int64_t turn_off_red(alarm_id_t id, void *user_data);
int64_t turn_off_blue(alarm_id_t id, void *user_data);

void button_callback(uint gpio, uint32_t events) {
    if (!leds_ativo) {
        leds_ativo = true;
        gpio_put(Blue_LED, 1);
        gpio_put(Red_LED, 1);
        gpio_put(Green_LED, 1);
        add_alarm_in_ms(3000, turn_off_green, NULL, false);
    }
}

int64_t turn_off_green(alarm_id_t id, void *user_data) {
    gpio_put(Green_LED, 0);
    add_alarm_in_ms(3000, turn_off_red, NULL, false);
    return 0;
}

int64_t turn_off_red(alarm_id_t id, void *user_data) {
    gpio_put(Red_LED, 0);
    add_alarm_in_ms(3000, turn_off_blue, NULL, false);
    return 0;
}

int64_t turn_off_blue(alarm_id_t id, void *user_data) {
    gpio_put(Blue_LED, 0);
    leds_ativo = false;
    return 0;
}

void init_gpio() {
    gpio_init(Red_LED);
    gpio_set_dir(Red_LED, GPIO_OUT);
    gpio_init(Blue_LED);
    gpio_set_dir(Blue_LED, GPIO_OUT);
    gpio_init(Blue_LED);
    gpio_set_dir(Green_LED, GPIO_OUT);
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    gpio_set_irq_edge_rise(BUTTON, true);
    gpio_set_irq_callback(&button_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

int main() {
    stdio_init_all();
    init_gpio();

    struct repeating_timer timer;
    add_repeating_timer_ms(-3000, semafaro, NULL, &timer);
    
    while (true) {
        printf("Semáforo em funcionamento\n");
        sleep_ms(1000);
    }
}
