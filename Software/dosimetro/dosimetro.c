#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "inc/ssd1306.h"

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

// Pinos do LED RGB e Buzzer
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13
#define BUZZER 10

// Parâmetros e macros do ADC.
#define ADC_CLOCK_DIV 24.f 
#define SAMPLES 800 
#define ADC_ADJUST(x) ((x * 3.3f / (1 << 12u)) - 1.65f) 
#define REFERENCE_DB 20e-6 

// Níveis de som ajustados
#define SAFE_LEVEL 60
#define WARNING_LEVEL_LOW 70
#define WARNING_LEVEL_HIGH 80
#define HIGH_RISK_LEVEL 90
#define DANGER_LEVEL 90

// Configuração do I2C para o OLED
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Canal e configurações do DMA
uint dma_channel;
dma_channel_config dma_cfg;

// Buffer de amostras do ADC.
uint16_t adc_buffer[SAMPLES];

void sample_mic();
float mic_power();
float convert_to_db(float amplitude);
void set_led_color(uint8_t r, uint8_t g, uint8_t b);
void activate_buzzer(bool activate);
void siren_sound();

void sample_mic() {
    adc_fifo_drain();
    adc_run(false);
    dma_channel_configure(dma_channel, &dma_cfg,
        adc_buffer,
        &(adc_hw->fifo),
        SAMPLES,
        true
    );
    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_channel);
    adc_run(false);
}

float mic_power() {
    static float avg = 0.f;
    float sum = 0.f;
    for (uint i = 0; i < SAMPLES; ++i)
        sum += pow(ADC_ADJUST(adc_buffer[i]), 2);
    sum /= SAMPLES;
    avg = (avg * 0.8) + (sqrt(sum) * 0.2);
    return avg;
}

float convert_to_db(float amplitude) {
    if (amplitude <= 0) {
        return 0;
    }
    float db = 20 * log10(amplitude / REFERENCE_DB);
    return (db > 120) ? 120 : db;
}

void set_led_color(uint8_t r, uint8_t g, uint8_t b) {
    gpio_put(LED_RED, r);
    gpio_put(LED_GREEN, g);
    gpio_put(LED_BLUE, b);
}

void activate_buzzer(bool activate) {
    gpio_put(BUZZER, activate);
}

void siren_sound() {
    for (int i = 0; i < 3; i++) {
        activate_buzzer(true);
        sleep_ms(200);
        activate_buzzer(false);
        sleep_ms(200);
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_init(BUZZER);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(BUZZER, GPIO_OUT);

    // Inicializa I2C e OLED
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    sleep_ms(5000);
    adc_gpio_init(MIC_PIN);
    adc_init();
    adc_select_input(MIC_CHANNEL);
    adc_fifo_setup(true, true, 1, false, false);
    adc_set_clkdiv(ADC_CLOCK_DIV);

    dma_channel = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);

    sample_mic();
    while (true) {
        sample_mic();
        float avg = mic_power();
        float db_level = convert_to_db(avg);

        memset(ssd, 0, ssd1306_buffer_length);
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Som: %.2f dB", db_level);
        ssd1306_draw_string(ssd, 10, 20, buffer);
        render_on_display(ssd, &frame_area);

        if (db_level < SAFE_LEVEL) {
            set_led_color(0, 1, 0);
            activate_buzzer(false);
        } else if (db_level >= WARNING_LEVEL_LOW && db_level <= WARNING_LEVEL_HIGH) {
            set_led_color(1, 1, 0);
            activate_buzzer(false);
        } else if (db_level >= HIGH_RISK_LEVEL && db_level < DANGER_LEVEL) {
            set_led_color(1, 0, 0);
            activate_buzzer(true);
        } else if (db_level >= DANGER_LEVEL) {
            set_led_color(1, 0, 0);
            siren_sound();
        }

        printf("Nível de som: %.2f dB\n", db_level);

        sleep_ms(250);
    }
}
