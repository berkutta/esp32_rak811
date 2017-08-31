#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_deep_sleep.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include <string.h>

#include "rak811.h"

#define rak_reset 25

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1<<rak_reset);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    gpio_set_level(rak_reset, 0);

    vTaskDelay(20 / portTICK_PERIOD_MS);

    gpio_set_level(rak_reset, 1);

    printf("ESP32 alive \n");

    rak811_init(18, 19);

    while(1) {
        rak811_wakeup();

        rak811_get_dev_eui();

        rak811_mode(0);
        rak811_set_app_eui("70B3D57EF0006431");
        rak811_set_app_key("432F69BDC9BED9FFFBC2818CB001607D");
        rak811_join_otaa();

        rak811_send("1", "AAFF");

        rak811_version();

        rak811_sleep();

        printf("Now going to sleep \n");

        esp_deep_sleep_enable_timer_wakeup(10 * 1000000);

        uart_driver_delete(UART_NUM_0);
        uart_driver_delete(UART_NUM_1);
        uart_driver_delete(UART_NUM_2);

        esp_deep_sleep_start();
    }
    

    //A uart read/write example without event queue;
    //xTaskCreate(rak811_task, "rak811_task", 4096, NULL, 10, NULL);
}

