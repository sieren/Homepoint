
file(GLOB TFTESPI_SRC
    "libraries/TFT_eSPI/*.cpp"
)

file(GLOB TFTEFEX_SRC
    "libraries/TFT_eFEX/*.cpp"
)

file(GLOB JPEGDEC_SRC
    "libraries/JPEGDecoder/src/*.cpp"
    "libraries/JPEGDecoder/src/*.c"
)

set(FS_SRC_PATH "fs")
file(GLOB FS_SRC
  "${FS_SRC_PATH}/Filesystem.cpp"
)


set(HOMEKIT_SRC_PATH "homekit")
file(GLOB HOMEKIT_SRC
  "${HOMEKIT_SRC_PATH}/hkapi.c"
)


set(MQTT_SRC_PATH "mqtt")
file(GLOB MQTT_SRC
  "${MQTT_SRC_PATH}/MQTTConnection.cpp"
)

set(NTP_SRC_PATH "ntp")
file(GLOB NTP_SRC
  "${NTP_SRC_PATH}/NTPSync.cpp"
)

set(TOUCH_SRC_PATH "touch")
if(BUTTONS OR M5STACK)
set(TOUCH_SRC
  "${TOUCH_SRC_PATH}/ButtonDriver.cpp"
)
endif(BUTTONS OR M5STACK)
set(TOUCH_SRC
  ${TOUCH_SRC}
  "${TOUCH_SRC_PATH}/TouchDriver.cpp"
)

set(UI_SRC_PATH "ui")
file(GLOB UI_SRC
    "${UI_SRC_PATH}/UIButton.cpp"
    "${UI_SRC_PATH}/UIMosaicMenuWidget.cpp"
    "${UI_SRC_PATH}/UIPageButton.cpp"
    "${UI_SRC_PATH}/UISensorComboWidget.cpp"
    "${UI_SRC_PATH}/UIStatusBarWidget.cpp"
    "${UI_SRC_PATH}/UIWidget.cpp"
)

set(UIINCLUDES
  "/ui"
  PARENT_SCOPE
)

set(WIFI_SRC_PATH "wifi")
file(GLOB WIFI_SRC
  "${WIFI_SRC_PATH}/WifiContext.cpp"
)

set(GLOBALINCLUDES
  "./"
  PARENT_SCOPE
)

set(LIBRARY_SRC
    ${TFTESPI_SRC}
    ${TFTEFEX_SRC}
    ${JPEGDEC_SRC}
)
set(COMPONENT_SRCS 
    "AppContext.cpp"
    "main.cpp"
    ${FS_SRC}
    ${HOMEKIT_SRC}
    ${UI_SRC}
    ${MQTT_SRC}
    ${NTP_SRC}
    ${TOUCH_SRC}
    ${WIFI_SRC}
    ${LIBRARY_SRC}
)

set_property(SOURCE ${LIBRARY_SRC} PROPERTY COMPILE_FLAGS -Wno-all)

set(COMPONENT_ADD_INCLUDEDIRS
    "."
    ${UIINCLUDES}
    "libraries/TFT_eSPI/"
    "libraries/TFT_eFEX/"
    "libraries/JPEGDecoder/src"
    "libraries/rapidjson/include/"
    "libraries/variant/include/")

register_component()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DESP32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DESP32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTOUCH_CS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTOUCH_CS")

option(M5STACK "Compile for M5Stack" OFF) # Enabled by default
option(BUTTONS "Compile for M5Stack" OFF) # Enabled by default

if(M5STACK)
  message(STATUS "Compiling for M5Stack")
  list(APPEND compile_definitions "M5Stack")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DM5Stack")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DM5Stack")
endif(M5STACK)

if(BUTTONS)
  message(STATUS "Compiling for Button Interface")
  list(APPEND compile_definitions "BUTTONS")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUTTONS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUTTONS")
endif(BUTTONS)

spiffs_create_partition_image(spiffs ../data FLASH_IN_PROJECT)