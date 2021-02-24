# file(GLOB_RECURSE LVGL_SRC "libraries/lvgl/src/*.c")

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

file(GLOB FONTS_SRC
    "config/font/*.c"
)

set(FS_SRC_PATH "fs")
file(GLOB FS_SRC
  "${FS_SRC_PATH}/ConfigReader.cpp"
  "${FS_SRC_PATH}/Filesystem.cpp"
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
if(BUTTONS OR BOARD STREQUAL "M5STACK")
set(TOUCH_SRC
  "${TOUCH_SRC_PATH}/ButtonDriver.cpp"
)
endif(BUTTONS OR BOARD STREQUAL "M5STACK")

set(TOUCH_SRC
  ${TOUCH_SRC}
)

set(UI_SRC_PATH "ui")
file(GLOB UI_SRC
    "${UI_SRC_PATH}/UIStatusBar.cpp"
)

set(UIINCLUDES
  "/ui"
  PARENT_SCOPE
)

set(WIFI_SRC_PATH "wifi")
file(GLOB WIFI_SRC
  "${WIFI_SRC_PATH}/CaptiveServer.cpp"
  "${WIFI_SRC_PATH}/WifiContext.cpp"
)

set(WEBINCLUDES
  "/web"
  PARENT_SCOPE
)

set(WEBSERVER_SRC_PATH "web")
file(GLOB WEB_SRC
  "${WEBSERVER_SRC_PATH}/WebServer.cpp"
)

set(GLOBALINCLUDES
  "./"
 # "./libraries/lvgl/"
  "./config/"
)

set(LIBRARY_SRC
    ${JPEGDEC_SRC}
)

set(PLATFORM_SRC
    ${TFTESPI_SRC}
    ${TFTEFEX_SRC}
)

set(COMPONENT_SRCS 
    ${FONTS_SRC}
  # ${LVGL_SRC}
    "AppContext.cpp"
    "AppNewScreen.cpp"
    "main.cpp"
    ${FS_SRC}
    ${UI_SRC}
    ${MQTT_SRC}
    ${NTP_SRC}
    ${TOUCH_SRC}
    ${WEB_SRC}
    ${WIFI_SRC}
    ${LIBRARY_SRC}
)

set_property(SOURCE ${LIBRARY_SRC} PROPERTY COMPILE_FLAGS "-Wno-all -Wno-shift-negative-value -Wno-cpp")

  set(PLATFORM_INCLUDES
      "libraries/TFT_eSPI/"
      "libraries/TFT_eFEX/"
  )
  
  set(COMPONENT_ADD_INCLUDEDIRS
      "."
      ${UIINCLUDES}
      ${GLOBALINCLUDES}
      "libraries/JPEGDecoder/src"
      "libraries/rapidjson/include/"
  )

if(BOARD STREQUAL "M5STACKCORE2")
  message(STATUS "Compiling for M5Stack Core2")
  list(APPEND priv_requires m5stackcore2)
endif(BOARD STREQUAL "M5STACKCORE2")

  set(PLATFORM_INCLUDES
      "libraries/TFT_eSPI/"
      "libraries/TFT_eFEX/"
  )

  set(COMPONENT_SRCS
    ${COMPONENT_SRCS}
    ${PLATFORM_SRC}
    "config/PlatformM5StackCore2Helpers.c"
  )

  set(COMPONENT_ADD_INCLUDEDIRS
      ${COMPONENT_ADD_INCLUDEDIRS}
      ${PLATFORM_INCLUDES}
  )

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTOUCH_CS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTOUCH_CS -fconcepts -Wno-shift-negative-value -Wno-cpp")

register_component()

target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_LVGL_H_INCLUDE_SIMPLE")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DESP32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DESP32")

option(M5STACK "Compile for M5Stack" OFF) # Enabled by default
option(BUTTONS "Compile for M5Stack" OFF) # Enabled by default

file(REMOVE ${CMAKE_SOURCE_DIR}/main/libraries/TFT_eSPI/User_Setup.h)

if(BOARD STREQUAL "M5STACK")
  message(STATUS "Compiling for M5Stack")
  list(APPEND compile_definitions "M5Stack")
  list(APPEND compile_definitions "M5StackCore1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DM5Stack -DM5StackCore1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DM5Stack -DM5StackCore1")
  file(COPY ${CMAKE_SOURCE_DIR}/main/config/TFT_eSPI/M5Stack/User_Setup.h
    DESTINATION ${CMAKE_SOURCE_DIR}/main/libraries/TFT_eSPI)
endif(BOARD STREQUAL "M5STACK")

if(BOARD STREQUAL "M5STACKCORE2")
  message(STATUS "Compiling for M5Stack Core2")
  list(APPEND compile_definitions "M5StackCore2")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LV_TFT_DISPLAY_PROTOCOL_SPI -DCONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9341 -DM5StackCore2 -DTFT_WIDTH=320 -DTFT_HEIGHT=240 -DLV_HIGH_RESOURCE_MCU=1 -DLV_CONF_INCLUDE_SIMPLE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${CMAKE_C_FLAGS} -DM5StackCore2")
  file(COPY ${CMAKE_SOURCE_DIR}/main/config/TFT_eSPI/M5StackCore2/User_Setup.h
    DESTINATION ${CMAKE_SOURCE_DIR}/main/libraries/TFT_eSPI)
endif(BOARD STREQUAL "M5STACKCORE2")

if(BOARD STREQUAL "AZTOUCHMOD")
  message(STATUS "Compiling for AZ-Touch MOD")
  file(COPY ${CMAKE_SOURCE_DIR}/main/config/TFT_eSPI/AZTouch-MOD/User_Setup.h
    DESTINATION ${CMAKE_SOURCE_DIR}/main/libraries/TFT_eSPI)
endif(BOARD STREQUAL "AZTOUCHMOD")

if(NOT BOARD)
message(STATUS "Compiling for Generic ESP23")
  file(COPY ${CMAKE_SOURCE_DIR}/main/config/TFT_eSPI/Generic/User_Setup.h
    DESTINATION ${CMAKE_SOURCE_DIR}/main/libraries/TFT_eSPI)
endif(NOT BOARD)

if(BUTTONS)
  message(STATUS "Compiling for Button Interface")
  list(APPEND compile_definitions "BUTTONS")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUTTONS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUTTONS")
endif(BUTTONS)

spiffs_create_partition_image(spiffs ../data FLASH_IN_PROJECT)