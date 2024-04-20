#include <lwip/apps/http_client.h>
#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <stdint.h>
#include <stdio.h>

// #include <expected>
#include <string_view>

enum class parse_error { invalid_input, overflow };

// std::expected < int, parse_error> foo(){return 1; }

char myBuff[1000];

static constexpr std::string_view SSID = "";
static constexpr std::string_view PWD = "";
static constexpr std::string_view HOSTNAME = "rpico";

uint32_t country = CYW43_COUNTRY_UK;
uint32_t auth = CYW43_AUTH_WPA2_MIXED_PSK;

static uint32_t T1 = to_ms_since_boot(get_absolute_time());

void result(void *arg, httpc_result_t httpc_result,
        u32_t rx_content_len, u32_t srv_res, err_t err)

{
    printf("transfer complete\n");
    printf("local result=%d\n", httpc_result);
    printf("http result=%d\n", srv_res);
}

err_t headers(httpc_state_t *connection, void *arg, 
    struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("headers recieved\n");
    printf("content length=%d\n", content_len);
    printf("header length %d\n", hdr_len);
    pbuf_copy_partial(hdr, myBuff, hdr->tot_len, 0);
    printf("headers \n");
    printf("%s", myBuff);
    return ERR_OK;
}

err_t body(void *arg, struct altcp_pcb *conn, 
                            struct pbuf *p, err_t err)
{
    printf("body\n");
    pbuf_copy_partial(p, myBuff, p->tot_len, 0);
    printf("%s", myBuff);
    return ERR_OK;
}

int setup(uint32_t country, const char *ssid, const char *pass, uint32_t auth,
          const char *hostname, ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw) {
    if (cyw43_arch_init_with_country(country)) {
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    if (hostname != NULL) {
        netif_set_hostname(netif_default, hostname);
    }

    if (cyw43_arch_wifi_connect_async(ssid, pass, auth)) {
        return 2;
    }

    int flashrate = 1000;
    int status = CYW43_LINK_UP + 1;
    while (status >= 0 && status != CYW43_LINK_UP) {
        int new_status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
        if (new_status != status) {
            status = new_status;
            flashrate = flashrate / (status + 1);
            printf("connect status: %d %d\n", status, flashrate);
        }
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(flashrate);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(flashrate);
    }
    if (status < 0) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    } else {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        if (ip != NULL) {
            netif_set_ipaddr(netif_default, ip);
        }
        if (mask != NULL) {
            netif_set_netmask(netif_default, mask);
        }
        if (gw != NULL) {
            netif_set_gw(netif_default, gw);
        }
        printf("IP: %s\n", ip4addr_ntoa(netif_ip_addr4(netif_default)));
        printf("Mask: %s\n", ip4addr_ntoa(netif_ip_netmask4(netif_default)));
        printf("Gateway: %s\n", ip4addr_ntoa(netif_ip_gw4(netif_default)));
        printf("Host Name: %s\n", netif_get_hostname(netif_default));
    }
    return status;
}
static char event_str[128];

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    auto timeNow = get_absolute_time();

    if (to_ms_since_boot(timeNow) - T1 > 1000) {
        ip_addr_t ip;
        IP4_ADDR(&ip, 192, 168, 0, 101);
        httpc_connection_t settings;
        settings.result_fn = result;
        settings.headers_done_fn = headers;

        err_t err = httpc_get_file(&ip, 8080, "/doorbell.html", &settings, body,
                                   NULL, NULL);
        printf("status %d \n", err);

        T1 = to_ms_since_boot(timeNow);
    }
}

int main()
{
    // auto result = foo();
    // result.has_value();
    stdio_init_all();

    gpio_init(16);
    gpio_set_dir(16, false);
    setup(country, SSID.data(), PWD.data(), auth, HOSTNAME.data(), NULL, NULL,
          NULL);

    gpio_set_irq_enabled_with_callback(16, GPIO_IRQ_EDGE_RISE, true,
                                       &gpio_callback);

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(1000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(1000);
        printf("I'm working... ");
    }
}
