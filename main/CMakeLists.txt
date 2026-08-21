# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 
    	"main.c" 
    	"wifi.c"
    	"telemetry.c"
    INCLUDE_DIRS "." "../include"
    PRIV_INCLUDE_DIRS
    REQUIRES
        esp_wifi
        esp_event
        esp_netif
        nvs_flash
        esp_http_client
    PRIV_REQUIRES
)
