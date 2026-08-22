# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 
    	"main.c" 
        "sntp.c"
    	"telemetry.c"
        "wifi.c"
    INCLUDE_DIRS 
        "." 
        "../include"
    PRIV_INCLUDE_DIRS
    REQUIRES
        esp_event
        esp_http_client
        esp_netif
        esp_wifi
        nvs_flash
    PRIV_REQUIRES
)
