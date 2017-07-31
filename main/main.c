#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "rak811.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{
    printf("ESP32 alive \n");

    rak811_init(4, 5);

    while(1) {
        rak811_sleep();
        
        vTaskDelay(500 / portTICK_PERIOD_MS);

        rak811_wakeup();

        rak811_send_cmd("at+version\r\n");
        
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    

    //A uart read/write example without event queue;
    //xTaskCreate(rak811_task, "rak811_task", 4096, NULL, 10, NULL);
}

