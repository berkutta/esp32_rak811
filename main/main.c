#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include <string.h>

#include "rak811.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{
    printf("ESP32 alive \n");

    rak811_init(4, 5);

    rak811_version();
    rak811_version();

    while(1) {
        //rak811_sleep();
        
        //vTaskDelay(500 / portTICK_PERIOD_MS);

        //rak811_wakeup();

        //rak811_version();

        //rak811_mode(1);
        
        rak811_set_app_eui("39d7119f920f7952");

        rak811_set_app_key("a6b08140dae1d795ebfa5a6dee1f4dbd");

        char mydata[100];

        //rak811_exchance_data("at+set_config=app_eui:39d7119f920f7952\r\n", strlen("at+set_config=app_eui:39d7119f920f7952\r\n"), &mydata);

        //printf("%s", mydata);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    

    //A uart read/write example without event queue;
    //xTaskCreate(rak811_task, "rak811_task", 4096, NULL, 10, NULL);
}

