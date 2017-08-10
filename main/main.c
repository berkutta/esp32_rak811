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

    rak811_init(18, 19);

    rak811_version();
    rak811_version();

    while(1) {
        //rak811_sleep();
        
        //vTaskDelay(500 / portTICK_PERIOD_MS);

        //rak811_wakeup();

        //rak811_version();

        //rak811_mode(1);

        rak811_get_dev_eui();
        
        rak811_set_app_eui("70b3d57ef0006431");

        rak811_set_app_key("432F69BDC9BED9FFFBC2818CB001607D");

        char mydata[100];

        //rak811_exchance_data("at+set_config=app_eui:39d7119f920f7952\r\n", strlen("at+set_config=app_eui:39d7119f920f7952\r\n"), &mydata);

        //printf("%s", mydata);

        rak811_exchance_data_waiting("at+join=otaa\r\n", strlen("at+join=otaa\r\n"), &mydata);

        printf("data: %s\n", mydata);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    

    //A uart read/write example without event queue;
    //xTaskCreate(rak811_task, "rak811_task", 4096, NULL, 10, NULL);
}

