#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>

#include "rak811.h"

static const char* TAG = "RAK811";

void rak811_init(uint8_t tx, uint8_t rx) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    //Configure UART1 parameters
    uart_param_config(uart_interface, &uart_config);
    //Set UART1 pins(TX: IO4, RX: I05, RTS: IO18, CTS: IO19)
    uart_set_pin(uart_interface, tx, rx, NULL, NULL);

    uart_driver_install(uart_interface, 2*uart_buf_size, 0, 0, NULL, 0);
/*
    while(1) {
        send_cmd("at+version\r\n");

        send_cmd("at+get_config=dev_addr\r\n");
        send_cmd("at+get_config=dev_eui\r\n");
        send_cmd("at+get_config=app_eui\r\n");
        send_cmd("at+get_config=app_key\r\n");
        send_cmd("at+get_config=nwks_key\r\n");
        send_cmd("at+get_config=apps_key\r\n");
        send_cmd("at+get_config=tx_power\r\n");
        send_cmd("at+get_config=adr\r\n");
        send_cmd("at+get_config=dr\r\n");
        send_cmd("at+get_config=public_net\r\n");
        send_cmd("at+get_config=rx_delay1\r\n");
        send_cmd("at+get_config=ch_list\r\n");
        send_cmd("at+get_config=rx2\r\n");
        send_cmd("at+get_config=retrans\r\n");
        send_cmd("at+get_config=duty\r\n");
        
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    */
}

uint8_t rak811_exchance_data(char *tx_data, uint8_t tx_length, char *rx_data) {
    uart_write_bytes(uart_interface, (const char*)tx_data, tx_length);

    int rx_length = uart_read_bytes(uart_interface, (const char*)rx_data, uart_buf_size, 500 / portTICK_PERIOD_MS);

    ESP_LOGD(TAG, "Exchanged data, tx %d Bytes, rx %d Bytes", tx_length, rx_length)

    // Doesn' work reliable
    // Cut of \r\n and place \0
    //rx_data[strlen(rx_data) - 2] = '\0';

    rx_data[rx_length - 2] = '\0';

    return rx_length; 
}

void rak811_send_cmd_response(char *response, char *cmd) {
    rak811_exchance_data(cmd, strlen(cmd), response);

    vTaskDelay(20 / portTICK_PERIOD_MS);
}

int rak811_send_cmd_cmp(char *cmp, char *cmd) {
    char response[200];
    char mycmd[200];

    strcpy(mycmd, cmd);

    rak811_send_cmd_response(response, mycmd);

    mycmd[strlen(mycmd) - 2] = '\0';

    if(strstr(response, cmp) != 0) {
        ESP_LOGD(TAG, "Success in command %s, response %s", mycmd, response);
        return 1;
    } else {
        ESP_LOGE(TAG, "Error in command %s, response %s", mycmd, response);
        return 0;
    }

}

void rak811_send_cmd(char *cmd) {
    char response[200];
    rak811_send_cmd_response(response, cmd);

    ESP_LOGD(TAG, "Command: %s, Response: %s", cmd, response);
}

void rak811_version(void) {
    rak811_send_cmd_cmp("OK", "at+version\r\n");
}

void rak811_sleep(void) {
    rak811_send_cmd_cmp("OK", "at+sleep\r\n");
}

void rak811_wakeup(void) {
    char response[200];
    rak811_send_cmd_response(response, "at+\r\n");

    ESP_LOGD(TAG, "Success in RAK811 Wakeup");
}

void rak811_mode(uint8_t mode) {
    if(mode == 0) {
        rak811_send_cmd_cmp("OK", "at+mode=0\r\n");
    } else if(mode == 1) {
        rak811_send_cmd_cmp("OK", "at+mode=1\r\n");
    }
}

void rak811_set_app_eui(char *app_eui) {
    char mycmd[50] = "at+set_config=app_eui:";
    strcat(mycmd, app_eui);
    strcat(mycmd, "\r\n");

    rak811_send_cmd_cmp("OK", mycmd);
}

void rak811_set_app_key(char *app_key) {
    char mycmd[80] = "at+set_config=app_key:";
    strcat(mycmd, app_key);
    strcat(mycmd, "\r\n");

    rak811_send_cmd_cmp("OK", mycmd);
}
