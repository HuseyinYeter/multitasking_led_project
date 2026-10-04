#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define LED1_PIN 2
#define LED2_PIN 26
#define SENSOR_PIN 12
#define BUTTON_PIN 32
static QueueHandle_t queue = NULL;

static void IRAM_ATTR isr1(void *arg)
{
    int olay = 1;
    BaseType_t woke = pdFALSE;

    xQueueSendFromISR(queue, &olay, &woke);

    if (woke)
    {
        portYIELD_FROM_ISR();
    }
}

static void IRAM_ATTR isr2(void *arg)
{
    int olay = 2;
    BaseType_t woke = pdFALSE;

    xQueueSendFromISR(queue, &olay, &woke);

    if (woke)
    {
        portYIELD_FROM_ISR();
    }
}

static void led_task(void *p)
{
    int olay = 2;
    bool led1 = false;
    bool led2 = false;

    while (1)
    {
        if (xQueueReceive(queue, &olay, portMAX_DELAY) == pdTRUE)
        {

            if (olay == 1)
            {
                led1 = !led1;
                led2 = false;
                gpio_set_level(LED1_PIN, led1);
                gpio_set_level(LED2_PIN, led2);
            }

            else if (olay == 2)
            {
                led2 = !led2;
                led1 = false;
                gpio_set_level(LED2_PIN, led2);
                gpio_set_level(LED1_PIN, led1);
            }
        }
    }
}

void app_main(void)
{
    queue = xQueueCreate(10, sizeof(int));

    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << LED1_PIN) | (1ULL << LED2_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config_t sensor_cfg = {
        .pin_bit_mask = (1ULL << SENSOR_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE};

    gpio_config_t button_cfg = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE};

    gpio_config(&led_cfg);
    gpio_config(&sensor_cfg);
    gpio_config(&button_cfg);

    xTaskCreate(led_task, "led_task", 2048, NULL, 5, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(SENSOR_PIN, isr1, NULL);
    gpio_isr_handler_add(BUTTON_PIN, isr2, NULL);
}
