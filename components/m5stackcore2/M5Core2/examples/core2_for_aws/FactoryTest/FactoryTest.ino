/*
    Description: This case is used to test CORE FOR AWS.
    Please install library before compiling:  
    ArduinoECCX08: file in M5Core2 lib examples -> core2_for_aws -> ArduinoECCX08.zip（unzip the lib zip file to the Arduino Lib path）
*/

#include <M5Core2.h>
#include <driver/i2s.h>
#include "fft.h"
#include "Fonts/EVA_20px.h"
#include "Fonts/EVA_11px.h"
#include "line3D.h"
#include "Wire.h"
#include "WiFi.h"

#include "FastLED.h"
#include <ArduinoECCX08.h>

#define LEDS_PIN 25
#define LEDS_NUM 10
CRGB ledsBuff[LEDS_NUM];

ECCX08Class myECCX08(Wire1, 0x35);

extern const unsigned char CoverImage[21301];
extern const unsigned char clockImage[18401];
extern const unsigned char CoreMainImage[87169];
extern const unsigned char batPowerImage[13769];
extern const unsigned char touchImage[12262];
extern const unsigned char SDCardImage[14835];
extern const unsigned char imageMenu[14900];
extern const unsigned char previewR[120264];
extern const unsigned char wifiSacnImage[28123];
extern const unsigned char TimerAppImage[59165];
extern const unsigned char SettingAppImage[50771];
extern const unsigned char bibiSig[8820];

extern const unsigned char image_rect_0006[1394];
extern const unsigned char image_rect_0005[1394];
extern const unsigned char image_rect_0004[1394];
extern const unsigned char image_rect_0003[1394];
extern const unsigned char image_rect_0002[1394];
extern const unsigned char image_rect_0001[1394];

extern const unsigned char image_DigNumber_0000_0[504];
extern const unsigned char image_DigNumber_0001_1[504];
extern const unsigned char image_DigNumber_0002_2[504];
extern const unsigned char image_DigNumber_0003_3[504];
extern const unsigned char image_DigNumber_0004_4[504];
extern const unsigned char image_DigNumber_0005_5[504];
extern const unsigned char image_DigNumber_0006_6[504];
extern const unsigned char image_DigNumber_0007_7[504];
extern const unsigned char image_DigNumber_0008_8[504];
extern const unsigned char image_DigNumber_0009_9[504];
extern const unsigned char image_DigNumber_0010_10[504];

extern const unsigned char image_DigNumber_35px_0000_0[315];
extern const unsigned char image_DigNumber_35px_0001_1[315];
extern const unsigned char image_DigNumber_35px_0002_2[315];
extern const unsigned char image_DigNumber_35px_0003_3[315];
extern const unsigned char image_DigNumber_35px_0004_4[315];
extern const unsigned char image_DigNumber_35px_0005_5[315];
extern const unsigned char image_DigNumber_35px_0006_6[315];
extern const unsigned char image_DigNumber_35px_0007_7[315];
extern const unsigned char image_DigNumber_35px_0008_8[315];
extern const unsigned char image_DigNumber_35px_0009_9[315];
extern const unsigned char image_DigNumber_35px_0010_10[315];

extern const unsigned char image_rect320_20_0001[3200];
extern const unsigned char image_rect320_20_0002[3200];
extern const unsigned char image_rect320_20_0003[3200];
extern const unsigned char image_rect320_20_0004[3200];
extern const unsigned char image_rect320_20_0005[3200];
extern const unsigned char image_rect320_20_0006[3200];
extern const unsigned char image_rect320_20_0007[3200];

extern const unsigned char image_number8x7_01[35];
extern const unsigned char image_number8x7_02[35];
extern const unsigned char image_number8x7_03[35];
extern const unsigned char image_number8x7_04[35];
extern const unsigned char image_number8x7_05[35];
extern const unsigned char image_number8x7_06[35];
extern const unsigned char image_number8x7_07[35];
extern const unsigned char image_number8x7_08[35];
extern const unsigned char image_number8x7_09[35];
extern const unsigned char image_number8x7_10[35];
extern const unsigned char image_number8x7_11[35];
extern const unsigned char image_number8x7_12[35];
extern const unsigned char image_number8x7_13[35];
extern const unsigned char image_number8x7_14[35];
extern const unsigned char image_number8x7_15[35];
extern const unsigned char image_number8x7_16[35];

extern const unsigned char image_chaging_0001[149];
extern const unsigned char image_chaging_0002[149];
extern const unsigned char image_chaging_0003[149];
extern const unsigned char image_chaging_0004[149];
extern const unsigned char image_chaging_0005[149];
extern const unsigned char image_chaging_0006[149];
extern const unsigned char image_chaging_0007[149];

extern const unsigned char image_Sysinit_0000s_0000_L1[875];
extern const unsigned char image_Sysinit_0000s_0001_L2[875];
extern const unsigned char image_Sysinit_0001s_0000_R1[875];
extern const unsigned char image_Sysinit_0001s_0001_R2[875];

extern const unsigned char image_TouchFish_0001[1672];
extern const unsigned char image_TouchFish_0002[1672];
extern const unsigned char image_TouchFish_0003[1672];
extern const unsigned char image_TouchFish_0004[1672];
extern const unsigned char image_TouchFish_0005[1672];
extern const unsigned char image_TouchFish_0006[1672];

uint8_t *rectptrBuff[6] = 
{
    (uint8_t*)image_rect_0001,
    (uint8_t*)image_rect_0002,
    (uint8_t*)image_rect_0003,
    (uint8_t*)image_rect_0004,
    (uint8_t*)image_rect_0005,
    (uint8_t*)image_rect_0006,
};

uint8_t *rect320ptrBuff[7] = 
{
    (uint8_t*)image_rect320_20_0001,
    (uint8_t*)image_rect320_20_0002,
    (uint8_t*)image_rect320_20_0003,
    (uint8_t*)image_rect320_20_0004,
    (uint8_t*)image_rect320_20_0005,
    (uint8_t*)image_rect320_20_0006,
    (uint8_t*)image_rect320_20_0007,
};

uint8_t *DigNumber[11] = 
{
    (uint8_t*)image_DigNumber_0000_0,
    (uint8_t*)image_DigNumber_0001_1,
    (uint8_t*)image_DigNumber_0002_2,
    (uint8_t*)image_DigNumber_0003_3,
    (uint8_t*)image_DigNumber_0004_4,
    (uint8_t*)image_DigNumber_0005_5,
    (uint8_t*)image_DigNumber_0006_6,
    (uint8_t*)image_DigNumber_0007_7,
    (uint8_t*)image_DigNumber_0008_8,
    (uint8_t*)image_DigNumber_0009_9,
    (uint8_t*)image_DigNumber_0010_10,
};

uint8_t *DigNumber_35px[11] = 
{
    (uint8_t*)image_DigNumber_35px_0000_0,
    (uint8_t*)image_DigNumber_35px_0001_1,
    (uint8_t*)image_DigNumber_35px_0002_2,
    (uint8_t*)image_DigNumber_35px_0003_3,
    (uint8_t*)image_DigNumber_35px_0004_4,
    (uint8_t*)image_DigNumber_35px_0005_5,
    (uint8_t*)image_DigNumber_35px_0006_6,
    (uint8_t*)image_DigNumber_35px_0007_7,
    (uint8_t*)image_DigNumber_35px_0008_8,
    (uint8_t*)image_DigNumber_35px_0009_9,
    (uint8_t*)image_DigNumber_35px_0010_10,
};

uint8_t *Number_7x10px[16] = 
{
    (uint8_t*)image_number8x7_01, // 0
    (uint8_t*)image_number8x7_02, // 1
    (uint8_t*)image_number8x7_03, // 2
    (uint8_t*)image_number8x7_04, // 3
    (uint8_t*)image_number8x7_05, // 4
    (uint8_t*)image_number8x7_06, // 5
    (uint8_t*)image_number8x7_07, // 6
    (uint8_t*)image_number8x7_08, // 7
    (uint8_t*)image_number8x7_09, // 8
    (uint8_t*)image_number8x7_10, // 9
    (uint8_t*)image_number8x7_11, // V
    (uint8_t*)image_number8x7_12, // X
    (uint8_t*)image_number8x7_13, // Y
    (uint8_t*)image_number8x7_14, // :
    (uint8_t*)image_number8x7_15, // CHARGING
    (uint8_t*)image_number8x7_16, // G
};

uint8_t *batRect[7] = 
{
    (uint8_t*)image_chaging_0001,
    (uint8_t*)image_chaging_0002,
    (uint8_t*)image_chaging_0003,
    (uint8_t*)image_chaging_0004,
    (uint8_t*)image_chaging_0005,
    (uint8_t*)image_chaging_0006,
    (uint8_t*)image_chaging_0007,
};

uint8_t *TouchFishBuff[7] = 
{
    (uint8_t*)image_TouchFish_0001,
    (uint8_t*)image_TouchFish_0002,
    (uint8_t*)image_TouchFish_0003,
    (uint8_t*)image_TouchFish_0004,
    (uint8_t*)image_TouchFish_0005,
    (uint8_t*)image_TouchFish_0006,
};

extern const unsigned char image_External[1425];
extern const unsigned char image_Internal[1425];
extern const unsigned char image_PowerC[310];
extern const unsigned char image_power[924];

#define CONFIG_I2S_BCK_PIN 12
#define CONFIG_I2S_LRCK_PIN 0
#define CONFIG_I2S_DATA_PIN 2
#define CONFIG_I2S_DATA_IN_PIN 34

#define SPAKER_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1

typedef struct i2cDevice
{
    i2cDevice(){
        Name = "";
        addr = 0;
        nextPtr = nullptr;
    };
    String Name;
    uint8_t addr;
    struct i2cDevice* nextPtr;
}i2cDevice_t;

i2cDevice_t i2cParentptr;

typedef enum {
    kPOWER_EXTERNAL = 0,
    kPOWER_INTERNAL,
    kPOWER_MAX
}system_power_t;

struct systemState
{
    RTC_TimeTypeDef Rtctime;
    system_power_t power = kPOWER_MAX;
    uint16_t clockCount = 0;
    uint16_t timeCount = 0;
    uint16_t batCount = 0;

    uint8_t  batVoltageBuff[15];
    uint8_t  batVoltageWriteptr = 11;
    uint8_t  batVoltageReadptr = 0;
    uint16_t batVoltageWriteCount = 0;

    bool     touchState = false;
    bool    SDCardState = false;

    uint16_t SDCardscaneCount = 0;

    HotZone_t* App1Zone[6];
    HotZone_t* MPU6886;

    bool    soundFlag = true;

}sytState;

static QueueHandle_t fftvalueQueue = nullptr;
static QueueHandle_t i2sstateQueue = nullptr;

typedef struct
{
    uint8_t state;
    void* audioPtr;
    uint32_t audioSize;
}i2sQueueMsg_t;


TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
TFT_eSprite Dis3Dbuff = TFT_eSprite(&M5.Lcd);
TFT_eSprite DisClockbuff = TFT_eSprite(&M5.Lcd);
TFT_eSprite DisPowerbuff = TFT_eSprite(&M5.Lcd);
TFT_eSprite DisFFTbuff =  TFT_eSprite(&M5.Lcd);
TFT_eSprite DisBatbuff =  TFT_eSprite(&M5.Lcd);
TFT_eSprite DisTouchbuff =  TFT_eSprite(&M5.Lcd);
TFT_eSprite DisSDCardbuff =  TFT_eSprite(&M5.Lcd);
TFT_eSprite DisWarningbuff =  TFT_eSprite(&M5.Lcd);
TFT_eSprite DisCoverScrollbuff = TFT_eSprite(&M5.Lcd);
// 40-80-119   130-160-200  230-270-310
HotZone_t touchBtn0(10,241,120,280);
HotZone_t touchBtn1(130,241,200,280);
HotZone_t touchBtn2(230,241,310,280);

#define FAILD_COLOR     255,35,35
#define SUCCE_COLOR     255,255,255

void setCheckState(int number, bool state, bool flush = false);

bool InitI2SSpakerOrMic(int mode)
{
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(SPAKER_I2S_NUMBER);
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };
    if (mode == MODE_MIC)
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    }
    else
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        i2s_config.use_apll = false;
        i2s_config.tx_desc_auto_clear = true;
    }

    //Serial.println("Init i2s_driver_install");

    err += i2s_driver_install(SPAKER_I2S_NUMBER, &i2s_config, 0, NULL);
    i2s_pin_config_t tx_pin_config;

    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
    tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
    tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;

    //Serial.println("Init i2s_set_pin");
    err += i2s_set_pin(SPAKER_I2S_NUMBER, &tx_pin_config);
    //Serial.println("Init i2s_set_clk");
    err += i2s_set_clk(SPAKER_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    return true;
}


void CoverScrollText(String strNext,uint32_t color)
{
    static String strLast;
    uint32_t colorLast = 0xffff;
    uint32_t bkColor16 = DisCoverScrollbuff.color565(0x22,0x22,0x22); 
    DisCoverScrollbuff.setFreeFont(&EVA_20px);
    DisCoverScrollbuff.setTextSize(1);
    DisCoverScrollbuff.setTextColor(Disbuff.color565(255,0,0),bkColor16);
    DisCoverScrollbuff.setTextDatum(TC_DATUM);
    DisCoverScrollbuff.fillRect(0,0,320,60,bkColor16);
    DisCoverScrollbuff.setTextColor(color);
    for( int i = 0; i < 20 ; i++ )
    {
        DisCoverScrollbuff.fillRect(0,20,320,20,bkColor16);
        DisCoverScrollbuff.setTextColor(colorLast);
        DisCoverScrollbuff.drawString(strLast,160,20 - i);
        DisCoverScrollbuff.setTextColor(color);
        DisCoverScrollbuff.drawString(strNext,160,40 - i);
        DisCoverScrollbuff.fillRect(0,0,320,20,bkColor16);
        DisCoverScrollbuff.fillRect(0,40,320,20,bkColor16);
        delay(5); 
        DisCoverScrollbuff.pushSprite(0,150);
    }
    strLast = strNext;
    colorLast = color;
}

void sysErrorSkip()
{
    uint32_t bkColor16 = Disbuff.color565(0x22,0x22,0x22); 
    //Disbuff.drawJpg(CoverImage,21301,0,0,320,240,0,0);
    //Disbuff.fillRect(bkColor16)
    Disbuff.setFreeFont(&EVA_20px);
    //Disbuff.setTextFont(1);
    Disbuff.setTextSize(1);
    Disbuff.setTextColor(Disbuff.color565(0xff,0,0),bkColor16);
    Disbuff.setTextDatum(TC_DATUM);
    Disbuff.pushInSprite(&DisCoverScrollbuff,0,0,320,60,0,150);
    Disbuff.setCursor(94,220);
    Disbuff.print("Touch to Skip");
    //Disbuff.drawString("Touch to Skip",160,200);
    Disbuff.pushSprite(0,0);
    //M5.Axp.SetLDOEnable(3,false);

    HotZone_t toucZone(0,0,320,280);

    while(1)
    {
        if( M5.Touch.ispressed())
        {
            TouchPoint_t point = M5.Touch.getPressPoint();
            if( toucZone.inHotZone(point))
            {
                break;
            }
        }
        delay(10);
    }
}

void addI2cDevice(String name, uint8_t addr )
{
    i2cDevice_t *lastptr = &i2cParentptr;
    
    while(lastptr->nextPtr != nullptr )
    {
        lastptr = lastptr->nextPtr;
    }

    i2cDevice_t* ptr = ( i2cDevice_t * )calloc(1,sizeof(i2cDevice_t));
    ptr->Name = name;
    ptr->addr = addr;
    ptr->nextPtr = nullptr;
    lastptr->nextPtr = ptr;
}

int scani2caddr()
{
    for (int i = 0; i < 120; i++)
    {
        Wire1.beginTransmission(i);
        if (Wire1.endTransmission() == 0)
        {
            Serial.printf("%02X   |FIND", i);
            Serial.println(".");
        }
    }
    return 0;
}
int checkPsram()
{
    uint8_t* testbuff = ( uint8_t* )ps_calloc( 100*1024 ,sizeof(uint8_t));
    if( testbuff == nullptr )
    {
        CoverScrollText("PSRAM malloc failed",M5.Lcd.color565(FAILD_COLOR));
        sysErrorSkip();
        return -1;
    }
    else
    {
        CoverScrollText("PSRAM malloc Successful",M5.Lcd.color565(SUCCE_COLOR));
    }
    delay(100);
    
    for( size_t i = 0; i < 102400; i++ )
    {
        testbuff[i] = 0xA5;
        if( testbuff[i] != 0xA5 )
        {
            CoverScrollText("PSRAM read failed",M5.Lcd.color565(FAILD_COLOR));
            sysErrorSkip();
            return -1;
        }
    }
    CoverScrollText("PSRAM W&R Successful",M5.Lcd.color565(SUCCE_COLOR));
    return 0;
}

int checkI2cAddr()
{
    uint8_t count = 0;
    i2cDevice_t *lastptr = &i2cParentptr;
    do{
        lastptr = lastptr->nextPtr;
        Serial.printf("Addr:0x%02X - Name:%s\r\n",lastptr->addr,lastptr->Name.c_str());
        Wire1.beginTransmission( lastptr->addr );
        if ( Wire1.endTransmission() == ESP_OK )
        {
            String log = "I2C " + lastptr->Name + " Found";
            CoverScrollText(log,M5.Lcd.color565(SUCCE_COLOR));
        }
        else
        {
            String log = "I2C " + lastptr->Name + " Find failed";
            CoverScrollText(log,M5.Lcd.color565(FAILD_COLOR));
            sysErrorSkip();
        }
        delay(100);
        count++;
    }while(lastptr->nextPtr != nullptr );
    return 0;
}

int checkIMUInit()
{
    if( M5.IMU.Init() == 0 )
    {
        CoverScrollText("IMU Check Successful",M5.Lcd.color565(SUCCE_COLOR));
    }
    else
    {
        CoverScrollText("IMU Check failed",M5.Lcd.color565(FAILD_COLOR));
        sysErrorSkip();
    }
    return 0;
}

int checkAETCC608AInit()
{
    if (!myECCX08.begin())
    {
        CoverScrollText("AT608A Check failed",M5.Lcd.color565(FAILD_COLOR));
        while (1)delay(100);
    }
    else
    {
        CoverScrollText("AT608A Check Successful",M5.Lcd.color565(SUCCE_COLOR));
    }
    
    String serialNumber = myECCX08.serialNumber();

    Serial.print("ECCX08 Serial Number = ");
    Serial.println(serialNumber);
    Serial.println();

    myECCX08.end();
    Wire1.begin(21,22,100000);
}

int checkIOPort()
{
    pinMode(14,OUTPUT);
    pinMode(13,INPUT);
    pinMode(26,OUTPUT);
    pinMode(36,INPUT);

    uint8_t sendByte = 0xAA;
    uint8_t readBytePin13 = 0,readBytePin36 = 0;

    for( int i = 0; i < 8; i++ )
    {   
        uint8_t val = (( sendByte >> i ) & 0x01 ) ? HIGH : LOW;
        digitalWrite(14,val);
        digitalWrite(26,val);
        delay(5);
        readBytePin13 |= digitalRead(13);
        readBytePin36 |= digitalRead(36);

        readBytePin13 <<= 1;
        readBytePin36 <<= 1;
        delay(5);
    }
    Serial.printf("IO Read = %02X %02X\r\n",readBytePin13,readBytePin36);

    if( readBytePin13 != 0xAA )
    {
        CoverScrollText("SOCKET2 Check failed",M5.Lcd.color565(FAILD_COLOR));
        while (1)delay(100);
    }
    else if( readBytePin36 != 0xAA )
    {
        CoverScrollText("SOCKET1 Check failed",M5.Lcd.color565(FAILD_COLOR));
        while (1)delay(100);
    }
    else
    {
        CoverScrollText("SOCKET Check Successful",M5.Lcd.color565(SUCCE_COLOR));
    }
}

int checkSDCard()
{
    sdcard_type_t Type = SD.cardType();

    if( Type == CARD_UNKNOWN || Type == CARD_NONE )
    {
        CoverScrollText("SDCard Find failed",M5.Lcd.color565(FAILD_COLOR));
        sysErrorSkip();
    }
    else
    {
        CoverScrollText("SDCard Found",M5.Lcd.color565(SUCCE_COLOR));
        Serial.printf("SDCard Type = %d \r\n", Type);
        Serial.printf("SDCard Size = %d \r\n", (int)(SD.cardSize() / 1024 / 1024));
    }
    return 0;
}

void i2s_task(void *arg) {
    size_t bytes_written = 0;
    //Core2CovreSig[160908]
    i2s_write(SPAKER_I2S_NUMBER, previewR, 120264, &bytes_written, portMAX_DELAY);
    //delay(500);
    vTaskDelete(NULL);
}

void powerSetup()
{
    DisPowerbuff.createSprite(77,102);
    DisPowerbuff.fillRect(0,0,77,102,TFT_BLACK);
    DisPowerbuff.drawColorBitmap(0,39,77,24,
                                 (uint8_t*)image_power,
                                 0xff9c00,
                                 0x000000);
    DisPowerbuff.pushSprite(240,3);
}

void choosePower()
{
    uint32_t color1 = 0,color2 = 0;
    uint16_t posy = 6;              

    system_power_t PowerNow;

    PowerNow = ( M5.Axp.isACIN()) ? kPOWER_EXTERNAL : kPOWER_INTERNAL;

    if( PowerNow == sytState.power ) return;

    if( PowerNow == kPOWER_EXTERNAL )
    {
        color1 = 0x6c6c6c;
        color2 = 0xff9c00;
        posy = 68;
    }
    else
    {
        color2 = 0x6c6c6c;
        color1 = 0xff9c00;
        posy = 3;
    }

    DisPowerbuff.fillRect(0,3,77,37,TFT_BLACK);
    DisPowerbuff.fillRect(0,68,77,37,TFT_BLACK);

    DisPowerbuff.drawColorBitmap(0,0,77,37,
                                (uint8_t*)image_Internal,
                                color1,
                                0x000000);

    DisPowerbuff.drawColorBitmap(0,65,77,37,
                                (uint8_t*)image_External,
                                color2,
                                0x000000);

    DisPowerbuff.drawColorBitmap(54,posy,20,31,
                                (uint8_t*)image_PowerC,
                                0xae2828,
                                0x000000);

    DisPowerbuff.drawColorBitmap(0,39,77,24,
                                 (uint8_t*)image_power,
                                 0xff9c00,
                                 0x000000);
                                
    
    DisPowerbuff.pushSprite(240,3);
    sytState.power = PowerNow;
}

line_3d_t rect[12] = {
    {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
    {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
    {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
    {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
    {
        .start_point = {-1, -1, 1},
        .end_point = {-1, -1, -1},
    },
    {
        .start_point = {1, -1, 1},
        .end_point = {1, -1, -1},
    },
    {
        .start_point = {1, 1, 1},
        .end_point = {1, 1, -1},
    },
    {
        .start_point = {-1, 1, 1},
        .end_point = {-1, 1, -1},
    },
    {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
    {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
    {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
    {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
};

line_3d_t rect_source[12];
//85 56
void setup3D()
{
    Dis3Dbuff.createSprite(85,56);
    Dis3Dbuff.fillRect(0,0,85,56,Dis3Dbuff.color565(0x33,0x20,0x00));
    for (int n = 0; n < 12; n++)
    {
        rect_source[n].start_point.x = rect[n].start_point.x * 12;
        rect_source[n].start_point.y = rect[n].start_point.y * 12;
        rect_source[n].start_point.z = rect[n].start_point.z * 12;
        rect_source[n].end_point.x = rect[n].end_point.x * 12;
        rect_source[n].end_point.y = rect[n].end_point.y * 12;
        rect_source[n].end_point.z = rect[n].end_point.z * 12;
    }
}

void Polar2Cartesian(double Angle, double radius, point_2d_t *point, int16_t offsetX = 42, int16_t officeY = 28)
{
    point->x = radius * cos( Angle ) + offsetX;
    point->y = officeY - radius * sin( Angle );
}

void MPU6886Test()
{
    float accX = 0;
    float accY = 0;
    float accZ = 0;

    double theta = 0, last_theta = 0;
    double phi = 0, last_phi = 0;
    double alpha = 0.2;

    line_3d_t x = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t y = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t z = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 30}};

    line_3d_t rect_dis;

    line3D line3d;
    line3d.setZeroOffset(42,28);
    M5.IMU.getAccelData(&accX, &accY, &accZ);

    if ((accX < 1) && (accX > -1))
    {
        theta = asin(-accX) * 57.295;
    }
    if (accZ != 0)
    {
        phi = atan(accY / accZ) * 57.295;
    }

    theta = alpha * theta + (1 - alpha) * last_theta;
    phi = alpha * phi + (1 - alpha) * last_phi;

    Dis3Dbuff.fillRect(0,0,85,56,Dis3Dbuff.color565(0x33,0x20,0x00));
    z.end_point.x = 0;
    z.end_point.y = 0;
    z.end_point.z = 24;
    line3d.RotatePoint(&z.end_point, theta, phi, 0);
    line3d.RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
    line3d.RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

    line3d.printLine3D(&Dis3Dbuff, &x, TFT_GREEN);
    line3d.printLine3D(&Dis3Dbuff, &y, TFT_GREEN);
    line3d.printLine3D(&Dis3Dbuff, &z, TFT_GREEN);

    uint16_t linecolor = Dis3Dbuff.color565(0xff,0x9c,0x00);

    for (int n = 0; n < 12; n++)
    {
        line3d.RotatePoint(&rect_source[n].start_point, &rect_dis.start_point, theta, phi, (double)0);
        line3d.RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta, phi, (double)0);
        line3d.printLine3D(&Dis3Dbuff, &rect_dis, linecolor);
    }
    Dis3Dbuff.pushSprite(74,130);
    last_theta = theta;
    last_phi = phi;
}

void clockSetup()
{
    RTC_TimeTypeDef RTCTime;
    RTCTime.Hours = 23;
    RTCTime.Minutes = 33;
    RTCTime.Seconds = 33;
    M5.Rtc.SetTime(&RTCTime);

    DisClockbuff.createSprite(164,101);
    DisClockbuff.drawJpg(clockImage,18401,0,0,163,101);
}

void clockFlush()
{
    uint16_t posx[6] = { 4, 28, 66, 90, 127, 144 };

    DisClockbuff.fillRect(0, 20, 162, 50, DisClockbuff.color565(0x1a, 0x11, 0x00));
    DisClockbuff.fillRect(0, 25, 2  , 45, DisClockbuff.color565(0xff, 0x9c, 0x00));

    DisClockbuff.fillRect(56,35, 5  , 5 , DisClockbuff.color565(0xff, 0x9c, 0x00));
    DisClockbuff.fillRect(56,54, 5  , 5 , DisClockbuff.color565(0xff, 0x9c, 0x00));

    DisClockbuff.fillRect(118,40, 5  , 5 , DisClockbuff.color565(0xff, 0x9c, 0x00));
    DisClockbuff.fillRect(118,57, 5  , 5 , DisClockbuff.color565(0xff, 0x9c, 0x00));

    M5.Rtc.GetTime(&sytState.Rtctime);
    
    DisClockbuff.drawColorBitmap(posx[0],26,24,42,
                        DigNumber[ sytState.Rtctime.Hours / 10 ],
                        0xff9c00,
                        0x1a1100);
    DisClockbuff.drawColorBitmap(posx[1],26,24,42,
                        DigNumber[ sytState.Rtctime.Hours % 10 ],
                        0xff9c00,
                        0x1a1100);
    DisClockbuff.drawColorBitmap(posx[2],26,24,42,
                        DigNumber[ sytState.Rtctime.Minutes / 10 ],
                        0xff9c00,
                        0x1a1100);
    DisClockbuff.drawColorBitmap(posx[3],26,24,42,
                        DigNumber[ sytState.Rtctime.Minutes % 10 ],
                        0xff9c00,
                        0x1a1100);
    DisClockbuff.drawColorBitmap(posx[4],33,18,35,
                        DigNumber_35px[ sytState.Rtctime.Seconds / 10 ],
                        0xff9c00,
                        0x1a1100);
    DisClockbuff.drawColorBitmap(posx[5],33,18,35,
                        DigNumber_35px[ sytState.Rtctime.Seconds % 10 ],
                        0xff9c00,
                        0x1a1100);

    
    sytState.clockCount ++;
    sytState.clockCount %= 6;
    DisClockbuff.drawColorBitmap(0,0,164,17,
                            rectptrBuff[sytState.clockCount],
                            0xff9c00,
                            0x000000);


    DisClockbuff.pushSprite(72,4);
}

static void i2sMicroFFTtask(void *arg)
{
    uint8_t FFTDataBuff[128];
    uint8_t FFTValueBuff[24];
    uint8_t* microRawData = (uint8_t*)calloc(2048,sizeof(uint8_t));
    size_t bytesread;
    int16_t* buffptr;
    double data = 0;
    float adc_data;
    uint16_t ydata;
    uint32_t subData;

    uint8_t state = MODE_MIC;
    i2sQueueMsg_t QueueMsg;
    while(1)
    {
        if( xQueueReceive(i2sstateQueue,&QueueMsg,(TickType_t)0) == pdTRUE)
        {
            //Serial.println("Queue Now");
            if( QueueMsg.state == MODE_MIC )
            {
                InitI2SSpakerOrMic(MODE_MIC);
                state = MODE_MIC;
            }
            else
            {
                //Serial.println("Spaker");
                //Serial.printf("Length:%d",QueueMsg.audioSize);
                InitI2SSpakerOrMic(MODE_SPK);
                size_t written = 0;
                i2s_write(SPAKER_I2S_NUMBER,(unsigned char*)QueueMsg.audioPtr,QueueMsg.audioSize,&written,portMAX_DELAY);
                state = MODE_SPK;
            }
        }
        else if( state == MODE_MIC )
        {
            fft_config_t *real_fft_plan = fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
            i2s_read(I2S_NUM_0, (char *)microRawData, 2048, &bytesread, (100 / portTICK_RATE_MS));
            buffptr = ( int16_t*)microRawData;

            for ( int count_n = 0; count_n < real_fft_plan->size; count_n++)
            {
                adc_data = (float)map(buffptr[count_n], INT16_MIN, INT16_MAX, -2000, 2000);
                real_fft_plan->input[count_n] = adc_data;
            }
            fft_execute(real_fft_plan);

            for ( int count_n = 1; count_n < real_fft_plan->size / 4; count_n++)
            {
                data = sqrt(real_fft_plan->output[2 * count_n] * real_fft_plan->output[2 * count_n] + real_fft_plan->output[2 * count_n + 1] * real_fft_plan->output[2 * count_n + 1]);
                if ((count_n - 1) < 128)
                {
                    data = ( data > 2000 ) ? 2000 : data;
                    ydata = map(data, 0, 2000, 0, 255);
                    FFTDataBuff[128 - count_n] = ydata;
                }
            }

            for( int count = 0; count < 24; count++ )
            {
                subData = 0;
                for( int count_i = 0; count_i < 5; count_i++ )
                {
                    subData += FFTDataBuff[count * 5 + count_i ];
                }
                subData /= 5;
                FFTValueBuff[count] = map(subData,0,255,0,8);
            }
            xQueueSend( fftvalueQueue, (void * )&FFTValueBuff, 0 );
            fft_destroy(real_fft_plan);
            //Serial.printf("mmp\r\n");
        }
        else
        {
            delay(10);
        }
    }
}

void microPhoneSetup()
{
    fftvalueQueue = xQueueCreate(5, 24 * sizeof(uint8_t));
    if( fftvalueQueue == 0 )
    {
        return;
    }

    i2sstateQueue = xQueueCreate(5, sizeof(i2sQueueMsg_t));
    if( i2sstateQueue == 0 )
    {
        return;
    }

    InitI2SSpakerOrMic(MODE_MIC);
    xTaskCreatePinnedToCore(i2sMicroFFTtask, "microPhoneTask", 4096, NULL, 3, NULL, 0);

    DisFFTbuff.createSprite(143,54);
}

void MicroPhoneFFT()
{
    uint8_t FFTValueBuff[24];
    xQueueReceive( fftvalueQueue, (void * )&FFTValueBuff, portMAX_DELAY );
    DisFFTbuff.fillRect(0,0,143,54,DisFFTbuff.color565(0x33,0x20,0x00));
    uint32_t colorY = DisFFTbuff.color565(0xff,0x9c,0x00);
    uint32_t colorG = DisFFTbuff.color565(0x66,0xff,0x00);
    uint32_t colorRect;
    for( int x = 0; x < 24; x++ )
    {
        for( int y = 0; y < 9; y++ )
        {
            if( y < FFTValueBuff[23-x] )
            {
                colorRect = colorY;
            }
            else if( y == FFTValueBuff[23-x] )
            {
                colorRect = colorG;
            }
            else
            {
                continue;
            }
            DisFFTbuff.fillRect(x*6,54-y*6 - 5,5,5,colorRect);
        }
    }
    DisFFTbuff.pushSprite(170,130);
}

void batPowerSetup()
{
    DisBatbuff.createSprite(64,82);
    DisBatbuff.drawJpg(batPowerImage,13769,0,0,64,82);
    DisBatbuff.pushSprite(3,61);
}
void batpowerFlush()
{
    uint32_t colorY = DisFFTbuff.color565(0xff,0x9c,0x00);
    uint32_t color = ( M5.Axp.isACIN()) ? 0x66ff00 : 0x6c6c6c;

    DisBatbuff.drawColorBitmap(3,37,60,10,
                    Number_7x10px[14],
                    color,
                    0x000000);

    DisBatbuff.fillRect(10,27,2,2,colorY);
    DisBatbuff.fillRect(55,4,6,10,colorY);
    //80
    //3,13,20,30
    
    float batVoltage = M5.Axp.GetBatVoltage();
    float batPercentage = ( batVoltage < 3.2 ) ? 0 : ( batVoltage - 3.2 ) * 100;
    int   rectwidth = 27 - 27 * ( int )batPercentage / 100;
    uint8_t batVoltage1 = (uint16_t)batVoltage % 10;
    uint8_t batVoltage2 = (uint16_t)(batVoltage*10) % 10;

    DisBatbuff.drawColorBitmap(3,20,7,10,
                                Number_7x10px[batVoltage1],
                                0xff9c00,
                                0x000000);

    DisBatbuff.drawColorBitmap(13,20,7,10,
                                Number_7x10px[batVoltage2],
                                0xff9c00,
                                0x000000);

    DisBatbuff.drawColorBitmap(20,20,7,10,
                                Number_7x10px[10],
                                0xff9c00,
                                0x000000);
    
    DisBatbuff.drawRect(29,17,31,15,colorY);

    if( M5.Axp.isACIN())
    {
        sytState.batCount ++;
        sytState.batCount %= 7;
    }

    DisBatbuff.drawColorBitmap(31,19,27,11,
            batRect[sytState.batCount],
            0xff9c00,
            0x000000);

    DisBatbuff.fillRect(58 - rectwidth,19,rectwidth,11,TFT_BLACK);

    if( sytState.batVoltageWriteCount > 50 )
    {
        sytState.batVoltageWriteCount = 0;
        sytState.batVoltageBuff[sytState.batVoltageWriteptr] = ( batPercentage / 20 ) + 1;
        sytState.batVoltageWriteptr++;
        sytState.batVoltageReadptr++;

        sytState.batVoltageWriteptr %= 15;
        sytState.batVoltageReadptr %= 15;

        DisBatbuff.fillRect(6,50,55,29,DisBatbuff.color565(0x33, 0x20, 0x00));

        for( int i = 0; i < 11; i++ )
        {
            int lim = sytState.batVoltageBuff[(sytState.batVoltageReadptr + i) % 15 ];
            for( int y = 0; y < lim; y++ )
            {
                DisBatbuff.fillRect(6 + i * 5 ,74 - y * 5,4,4,colorY);
            }
        }
    }
    sytState.batVoltageWriteCount++;

    DisBatbuff.pushSprite(3,61);
}

void touchsetup()
{
    DisTouchbuff.createSprite(64,40);
    DisTouchbuff.drawJpg(touchImage,12262,0,0,64,40);

    //uint32_t colorY = DisTouchbuff.color565(0xff,0x9c,0x00);
    uint32_t colorGREY = DisTouchbuff.color565(0x30,0x30,0x30);

    DisTouchbuff.drawColorBitmap(12,15,7,10,
                                (uint8_t*)Number_7x10px[11],
                                0xff9c00,
                                0x000000);
    DisTouchbuff.drawColorBitmap(19,15,3,10,
                                (uint8_t*)Number_7x10px[13],
                                0xff9c00,
                                0x000000);

    DisTouchbuff.drawColorBitmap(12,26,7,10,
                                (uint8_t*)Number_7x10px[12],
                                0xff9c00,
                                0x000000);
    DisTouchbuff.drawColorBitmap(19,26,3,10,
                                (uint8_t*)Number_7x10px[13],
                                0xff9c00,
                                0x000000);
    for( int i = 0; i < 3; i++ )
    {
        int number = ( 0 / (int)( pow( 10 , ( 2 - i )))) % 10;
        DisTouchbuff.drawColorBitmap(22 + i * 7,15,7,10,
                                    (uint8_t*)Number_7x10px[number],
                                    0xff9c00,
                                    0x000000);
    }

    for( int i = 0; i < 3; i++ )
    {
        int number = ( 0 /  (int)( pow( 10 , ( 2 - i )))) % 10;
        DisTouchbuff.drawColorBitmap(22 + i * 7,26,7,10,
                                    (uint8_t*)Number_7x10px[number],
                                    0xff9c00,
                                    0x000000);
    }
    DisTouchbuff.fillRect(5,15,5,21,TFT_BLACK);
    DisTouchbuff.fillRect(46,4,15,8,colorGREY);
    DisTouchbuff.fillRect(46,16,15,8,colorGREY);
    DisTouchbuff.fillRect(46,28,15,8,colorGREY);

    DisTouchbuff.pushSprite(3,146);
}

void touchflush()
{
    TouchPoint_t pos= M5.Touch.getPressPoint();
    bool touchStateNow = ( pos.x == -1 ) ? false : true;
    
    uint32_t color;
    i2sQueueMsg_t msg;

    msg.state = MODE_SPK;
    msg.audioPtr = (void*)bibiSig;
    msg.audioSize = 8820;

    if( touchStateNow )
    {
        if( sytState.touchState == false )
        {
            xQueueSend(i2sstateQueue,&msg,(TickType_t)100);
            msg.state = MODE_MIC;
            xQueueSend(i2sstateQueue,&msg,(TickType_t)100);
        }

        uint32_t colorY = DisTouchbuff.color565(0xff,0x9c,0x00);
        uint32_t colorG = DisTouchbuff.color565(0x66,0xff,0x00);
        uint32_t colorGREY = DisTouchbuff.color565(0x30,0x30,0x30);
        DisTouchbuff.fillRect(5,15,5,21,colorG);

        DisTouchbuff.drawColorBitmap(12,15,7,10,
                                    (uint8_t*)Number_7x10px[11],
                                    0xff9c00,
                                    0x000000);
        DisTouchbuff.drawColorBitmap(19,15,3,10,
                                    (uint8_t*)Number_7x10px[13],
                                    0xff9c00,
                                    0x000000);

        DisTouchbuff.drawColorBitmap(12,26,7,10,
                                    (uint8_t*)Number_7x10px[12],
                                    0xff9c00,
                                    0x000000);
        DisTouchbuff.drawColorBitmap(19,26,3,10,
                                    (uint8_t*)Number_7x10px[13],
                                    0xff9c00,
                                    0x000000);
        for( int i = 0; i < 3; i++ )
        {
            int number = ( pos.x / (int)( pow( 10 , ( 2 - i )))) % 10;
            DisTouchbuff.drawColorBitmap(22 + i * 7,15,7,10,
                                        (uint8_t*)Number_7x10px[number],
                                        0xff9c00,
                                        0x000000);
        }

        for( int i = 0; i < 3; i++ )
        {
            int number = ( pos.y /  (int)( pow( 10 , ( 2 - i )))) % 10;
            DisTouchbuff.drawColorBitmap(22 + i * 7,26,7,10,
                                        (uint8_t*)Number_7x10px[number],
                                        0xff9c00,
                                        0x000000);
        }

        color = ( touchBtn0.inHotZone(pos)) ? colorY : colorGREY;
        DisTouchbuff.fillRect(46,4,15,8,color);

        color = ( touchBtn1.inHotZone(pos)) ? colorY : colorGREY;
        DisTouchbuff.fillRect(46,16,15,8,color);

        color = ( touchBtn2.inHotZone(pos)) ? colorY : colorGREY;
        DisTouchbuff.fillRect(46,28,15,8,color);

        if( sytState.touchState == false )
        {
            for( int i = 0; i < 6; i++ )
            {
                sytState.App1Zone[i]->inHotZoneDoFun(pos);
            }
        }
    }
    else
    {
        uint32_t colorGREY = DisTouchbuff.color565(0x30,0x30,0x30);
        DisTouchbuff.fillRect(5,15,5,21,TFT_BLACK);
        DisTouchbuff.fillRect(46,4,15,8,colorGREY);
        DisTouchbuff.fillRect(46,16,15,8,colorGREY);
        DisTouchbuff.fillRect(46,28,15,8,colorGREY);
    }

    DisTouchbuff.pushSprite(3,146);
    sytState.touchState = touchStateNow;
    //}
}

void sdcardSetup()
{
    TFT_eSprite DisSDCardbuff =  TFT_eSprite(&M5.Lcd);
    DisSDCardbuff.createSprite(64,54);
    DisSDCardbuff.drawJpg(SDCardImage,14835,0,0,64,54);

    sdcard_type_t Type = SD.cardType();

    if( Type == CARD_UNKNOWN || Type == CARD_NONE )
    {
        sytState.SDCardState = false;
        DisSDCardbuff.fillRect(55,4,6,10,TFT_BLACK);
    }
    else
    {
        sytState.SDCardState = true;
        uint32_t colorY = DisSDCardbuff.color565(0xff,0x9c,0x00);

        uint64_t sdcardSize = SD.cardSize() * 10 /1024 /1024/ 1024 ;
        uint64_t sdcardFreeSize = ( SD.cardSize() - SD.usedBytes())* 10 /1024 /1024/ 1024 ;

        DisSDCardbuff.fillRect(55,4,6,10,colorY);
        DisSDCardbuff.drawColorBitmap(3,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardFreeSize /100 % 10 ],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(10,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardFreeSize /10 % 10 ],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(20,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardFreeSize % 10 ],
                                0xff9c00,
                                0x000000);

        DisSDCardbuff.drawColorBitmap(33,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardSize /100 % 10],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(40,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardSize /10 % 10 ],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(50,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardSize % 10 ],
                                0xff9c00,
                                0x000000);
        Serial.printf("SDCard Type = %d \r\n", Type);
        Serial.printf("SDCard Size = %d \r\n", (int)(SD.cardSize() / 1024 / 1024));
    }
    DisSDCardbuff.pushSprite(3,4);
}

void sdCardFlush()
{
    sytState.SDCardscaneCount ++;
    if( sytState.SDCardscaneCount < 50 )
    {
        return;
    }
    sytState.SDCardscaneCount = 0;
    sdcard_type_t Type = SD.cardType();

    if( Type == CARD_UNKNOWN || Type == CARD_NONE )
    {
        sytState.SDCardState = false;
        DisSDCardbuff.fillRect(55,4,6,10,TFT_BLACK);
    }
    else
    {
        sytState.SDCardState = true;
        uint32_t colorY = DisSDCardbuff.color565(0xff,0x9c,0x00);

        uint64_t sdcardSize = SD.cardSize() * 10 /1024 /1024/ 1024 ;
        uint64_t sdcardFreeSize = ( SD.cardSize() - SD.usedBytes())* 10 /1024 /1024/ 1024 ;

        DisSDCardbuff.fillRect(55,4,6,10,colorY);
        DisSDCardbuff.drawColorBitmap(3,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardFreeSize /100 % 10 ],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(10,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardFreeSize /10 % 10 ],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(20,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardFreeSize % 10 ],
                                0xff9c00,
                                0x000000);

        DisSDCardbuff.drawColorBitmap(33,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardSize /100 % 10],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(40,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardSize /10 % 10 ],
                                0xff9c00,
                                0x000000);
        DisSDCardbuff.drawColorBitmap(50,19,7,10,
                                (uint8_t*)Number_7x10px[sdcardSize % 10 ],
                                0xff9c00,
                                0x000000);
        //Serial.printf("SDCard Type = %d \r\n", Type);
        //Serial.printf("SDCard Size = %d \r\n", (int)(SD.cardSize() / 1024 / 1024));
    }
    DisSDCardbuff.pushSprite(3,4);
}

void AppOut()
{
    Serial.println("Hello App");
}

TFT_eSprite menubuff =  TFT_eSprite(&M5.Lcd);

void drawJPEGImageFromSDCard(fs::FS &fs, const char * dirname)
{
    TouchPoint_t pos = M5.Touch.getPressPoint();
    uint16_t* buff = (uint16_t*)ps_calloc(10880,sizeof(uint16_t));
	int Count = 200, menuAmoCount = 0;
    int menuPosyNow = 0; 
    int menuState = 1;
    // 0 -> off
    // 1 -> runing
    // 2 -> open
    // 3 -> exiting
    bool flushJPEG = false;

    HotZone_t _touchBtn0(10 ,206,120,280);
    HotZone_t _touchBtn1(130,206,200,280);
    HotZone_t _touchBtn2(230,206,310,280);
    
    Disbuff.fillRect(0,0,320,240,TFT_BLACK);
    Disbuff.pushSprite(0,0);
    
    menubuff.createSprite(320,34);
    menubuff.drawJpg(imageMenu,14900,0,0,320,34);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }
    
    File file = root.openNextFile();
    while( 1 ){
		if( Count >= 200 )
		{
			if( !file.isDirectory() )
			{
                Disbuff.drawJpgFile(fs,file.name(),0,0,320,240);
                Disbuff.getSprite2Buff(buff,0,206,320,34);
                flushJPEG = true;
			}
			file = root.openNextFile();
			if( !file )
			{
				root = fs.open(dirname);
				file = root.openNextFile();
			}
			Count = 0;
		}
		delay(5);
		Count ++;

        pos = M5.Touch.getPressPoint();

        if( pos.x != -1 )
        {
            switch( menuState )
            {
                case 0: menuState = 1; menuPosyNow = 0; break;
                case 2: menuAmoCount = 0; 
                    if( _touchBtn2.inHotZone(pos)) goto endImage; break;
            }
        }

        switch( menuState )
        {
            case 1: 
                menuPosyNow += 4;
                if( menuPosyNow >= 34)
                {
                    menuPosyNow = 34;
                    menuState = 2;
                    menuAmoCount = 0;
                    flushJPEG = true;
                }
                break;
            case 2: 
                menuAmoCount++;
                if( menuAmoCount > 500 )
                {
                    menuAmoCount = 0;
                    menuPosyNow = 34;
                    menuState = 3;
                }
            break;
            case 3: 
                menuPosyNow -= 4;
                if( menuPosyNow <= 0)
                {
                    menuPosyNow = 0;
                    menuState = 0;
                    menuAmoCount = 0;
                    flushJPEG = true;
                }
            break;
        }
        
        if(( menuState == 1 )||( menuState == 3 )||( flushJPEG == true))
        {
            if( flushJPEG == true ) flushJPEG = false;
            if( menuState == 3 )    Disbuff.pushImage(0,206,320,34,buff);
            Disbuff.pushInSprite(&menubuff,0,0,320,34,0,241 - menuPosyNow);
            Disbuff.pushSprite(0,0);
        }
    }
    endImage:
    Disbuff.drawJpg(CoreMainImage,87169,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);
    menubuff.deleteSprite();
    sytState.power = kPOWER_MAX;
    free(buff);
}

void AppImage()
{
    if( sytState.SDCardState == false )return;
    drawJPEGImageFromSDCard(SD,"/image");
}

void APPSleep()
{
    delay(500);
    M5.Axp.SetSleep();
}

void AppWifi()
{
    bool first = true;
    int count = 0,count_n = 0;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Disbuff.drawJpg(wifiSacnImage,28123,0,0,320,240);
    Disbuff.pushSprite(0,0);

    Disbuff.setFreeFont(&EVA_20px);
    Disbuff.setTextSize(1);
    Disbuff.setTextColor(Disbuff.color565(0xff,0xa0,0),Disbuff.color565(0,0,0));
    Disbuff.setTextDatum(TL_DATUM);

    TouchPoint_t pos = M5.Touch.getPressPoint();

    uint16_t color_yellow = Disbuff.color565(0xff,0xa0,0);
    uint16_t color_gery = Disbuff.color565(0x43,0x43,0x43);

    WiFi.scanNetworks(true);

    HotZone escBtn(234,210,280,320);

    uint16_t colorWifiBar[10] = { TFT_RED, TFT_RED,
                                  color_yellow,color_yellow,color_yellow,
                                  TFT_GREEN,TFT_GREEN,TFT_GREEN,TFT_GREEN,TFT_GREEN};
    delay(100);
    while(1)
    {
        int WifiNumber = WiFi.scanComplete();
        if ( WifiNumber == -2 )
        {

        }
        else if ( WifiNumber == - 1 )
        {
            if( first )
            {
                //if( count >= 20 )
                //{
                //    count = 0;
                //    Disbuff.setTextDatum(CC_DATUM);
                //    Disbuff.fillRect( 0,34,320,173,TFT_BLACK);
                //    String scanStr = "Scan";
                //    for( int n = 0; n < count_n; n++)
                //    {
                //        scanStr = scanStr +'.';
                //    }
                //    Disbuff.drawString(scanStr,160,120);
                //    Disbuff.pushSprite(0,0);
                //    count_n++;
                //    if( count_n > 4 ) count_n = 0;
                //}
                if( count >= 10 )
                {
                    count = 0;
                    Disbuff.fillRect( 0,34,320,173,TFT_BLACK);
                    Disbuff.drawColorBitmap(122,98,76,44,
                        TouchFishBuff[count_n],
                        0xff9c00,
                        0x000000);
                    Disbuff.pushSprite(0,0);
                    count_n++;
                    if( count_n >= 6 ) count_n = 0;
                }
                count++;
            }
        }
        else if ( WifiNumber == 0 ) 
        {
            Serial.println("no networks found");
            M5.Lcd.drawString("No networks found",15,22);
        } 
        else 
        {
            Serial.print(WifiNumber);
            Serial.println(" networks found");
            Disbuff.fillRect( 0,34,320,173,TFT_BLACK);
            if( WifiNumber > 8 ) WifiNumber = 7 ;


            for (int i = 0; i < WifiNumber; i++) 
            {
                Disbuff.fillRect( 18,34+i*24 + 2,7,20,Disbuff.color565(0xff,0xa0,0));
                String SSIDStr = WiFi.SSID(i);
                if( SSIDStr.length() > 11 )
                {
                    SSIDStr = SSIDStr.substring(0,8);
                    SSIDStr += "...";
                }
                Disbuff.setTextDatum(TL_DATUM);
                Disbuff.drawString( SSIDStr,35,34+i*24 + 2 );
            }
            if( first )
            {
                for (int i = 0; i < WifiNumber; i++)
                {
                    for( int n = 9; n >= 0; n-- )
                    {
                        Disbuff.fillRect(298-12*n,34+i*24+2,8,20,color_gery);
                    }
                }
                Disbuff.pushSprite(0,0);
            }
            for( int n = 9; n >= 0; n-- )
            {
                for (int i = 0; i < WifiNumber; i++) 
                {
                    int32_t rssi = ( WiFi.RSSI(i) < -100 ) ? -100 : WiFi.RSSI(i);
                    rssi = map( rssi,-100,-20,-100,0);
                    uint16_t colorNow = color_yellow;

                    if( rssi < ( n * -10))
                    {
                        colorNow = color_gery;
                    }
                    else
                    {
                        colorNow = colorWifiBar[9-n];
                    }
                    Disbuff.fillRect(298-12*n,34+i*24+2,8,20,colorNow);
                }
                if( first )
                {
                    Disbuff.pushSprite(0,0);
                    delay(5);
                }
            }
            WiFi.scanDelete();
            WiFi.scanNetworks(true);
            if( first == false )
            {
                Disbuff.pushSprite(0,0);
            }
            first = false;

        }
        delay(10);

        pos = M5.Touch.getPressPoint();

        if( pos.x != -1 )
        {
            if( escBtn.inHotZone(pos)) break;
        }
    }

    Disbuff.drawJpg(CoreMainImage,87169,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);

    sytState.power = kPOWER_MAX;
}

void AppTimer()
{
    Disbuff.drawJpg(TimerAppImage,59165,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);

    TouchPoint_t pos = M5.Touch.getPressPoint();

    HotZone StopBtn(240,36,320,114);
    HotZone CleanBtn(240,118,320,198);
    HotZone SettingBtn(185,144,249,240);
    HotZone EscBtn(240,200,320,280);

    bool stpoed = true;
    bool zeroflag = false;
    bool pressed = false;
    uint16_t count_bibi = 0;

    uint16_t posx[6] = { 4, 28, 66, 90, 127, 144 };

    TFT_eSprite Timerbuff = TFT_eSprite(&M5.Lcd);
    Timerbuff.createSprite(176,56);

    TFT_eSprite TimerRectbuff = TFT_eSprite(&M5.Lcd);
    TimerRectbuff.createSprite(40,164);
    TimerRectbuff.fillRect(0,0,40,164,TFT_BLACK);

    unsigned long timeNow = micros();
    int64_t timeset = 65000;
    int64_t timeSetting = timeset;
    int min = timeSetting / 60000 % 60;
    int sec = timeSetting / 1000 % 60;
    int mill = timeSetting % 1000;
    uint32_t colorNum = 0xff9c00;

    uint16_t color16Yellow = Timerbuff.color565(0xff,0x9c,0);
    uint16_t color16Red = Timerbuff.color565(255,0,0);
    uint16_t color16green = Timerbuff.color565(0,255,0);
    uint16_t color_gery = Disbuff.color565(0x43,0x43,0x43);

    uint16_t colorBar[12] = {color16green,color16green,color16green,color16green,color16green,color16green,
                             color16Yellow,color16Yellow,color16Yellow,color16Yellow,
                             color16Red,color16Red};
    uint16_t color16 = color16Yellow;

    i2sQueueMsg_t msg;

    msg.state = MODE_SPK;
    msg.audioPtr = (void*)bibiSig;
    msg.audioSize = 8820;

    //msg.state = MODE_SPK;
    //msg.audioPtr = (void*)previewR;
    //msg.audioSize = 120264;
    //xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);
                     
    Timerbuff.fillRect(56,13, 5  , 5 , Timerbuff.color565(0xff, 0x9c, 0x00));
    Timerbuff.fillRect(56,32, 5  , 5 , Timerbuff.color565(0xff, 0x9c, 0x00));

    Timerbuff.fillRect(118,18, 5  , 5 , Timerbuff.color565(0xff, 0x9c, 0x00));
    Timerbuff.fillRect(118,35, 5  , 5 , Timerbuff.color565(0xff, 0x9c, 0x00));
    
    TimerRectbuff.fillRect(0,0,40,164,TFT_BLACK);
    for( int i = 0; i< 12; i++ )
    {
        TimerRectbuff.fillRect(0,154-i*14,30,10,colorBar[11-i]);
    }
    TimerRectbuff.pushSprite(7,36);

    while(1)
    {
        if( !stpoed )
        {
            timeSetting  = timeSetting - ( millis() - timeNow );
            if( timeSetting <= 0 )
            {
                timeSetting = 0;
                count_bibi = 0;
                //M5.Axp.SetLDOEnable(3,true);
                zeroflag = true;
                stpoed = true;
            }
        }

        if( zeroflag )
        {
            delay(10);
            count_bibi ++;
            if( count_bibi > 20 )
            {
                count_bibi = 0;

                msg.state = MODE_SPK;
                msg.audioPtr = (void*)bibiSig;
                msg.audioSize = 8820;

                xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);
            }
        }

        int timerect = map(timeSetting,0,timeset,1,12);
        TimerRectbuff.fillRect(0,0,40,164,TFT_BLACK);
        for( int i = 0; i < 12; i++ )
        {
            if( i < timerect )
                TimerRectbuff.fillRect(0,154-i*14,30,10,colorBar[11-i]);
            else
                TimerRectbuff.fillRect(0,154-i*14,30,10,color_gery);
        }
        TimerRectbuff.pushSprite(7,36);

        

        timeNow = millis();

        min = timeSetting / 60000 % 60;
        sec = timeSetting / 1000 % 60;
        mill = timeSetting / 10 % 100;
        
        colorNum = ( min == 0 ) ? 0xff0000 : 0xff9c00;
        color16 = ( min == 0 ) ? color16Red : color16Yellow; 

        Timerbuff.fillRect(56,13, 5  , 5 , color16);
        Timerbuff.fillRect(56,32, 5  , 5 , color16);

        Timerbuff.fillRect(118,18, 5  , 5 , color16);
        Timerbuff.fillRect(118,35, 5  , 5 , color16);

        Timerbuff.drawColorBitmap(posx[0],4,24,42,
                            DigNumber[ min / 10],
                            colorNum,
                            0x000000);
        Timerbuff.drawColorBitmap(posx[1],4,24,42,
                            DigNumber[ min % 10],
                            colorNum,
                            0x000000);
        Timerbuff.drawColorBitmap(posx[2],4,24,42,
                            DigNumber[ sec / 10 ],
                            colorNum,
                            0x000000);
        Timerbuff.drawColorBitmap(posx[3],4,24,42,
                            DigNumber[ sec % 10 ],
                            colorNum,
                            0x000000);
        Timerbuff.drawColorBitmap(posx[4],14,18,35,
                            DigNumber_35px[ mill / 10 ],
                            colorNum,
                            0x000000);
        Timerbuff.drawColorBitmap(posx[5],14,18,35,
                            DigNumber_35px[ mill % 10 ],
                            colorNum,
                            0x000000);
        Timerbuff.pushSprite(54,92);

        pos = M5.Touch.getPressPoint();
        
        if(( pos.x != -1 )&&( pressed == false ))
        {
            pressed = true;

            msg.state = MODE_SPK;
            msg.audioPtr = (void*)bibiSig;
            msg.audioSize = 8820;

            xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);

            if( StopBtn.inHotZone(pos))
            {
                stpoed = ( stpoed == true ) ? false : true;
            }

            if( zeroflag )
            {
                zeroflag = false;
                timeSetting = timeset;
                //M5.Axp.SetLDOEnable(3,false);
            }

            if( CleanBtn.inHotZone(pos))
            {
                timeSetting = timeset;
                stpoed = true;

                TimerRectbuff.fillRect(0,0,40,164,TFT_BLACK);
                for( int i = 0; i< 12; i++ )
                {
                    TimerRectbuff.fillRect(0,154-i*14,30,10,colorBar[11-i]);
                }
                TimerRectbuff.pushSprite(7,36);
            }

            if( EscBtn.inHotZone(pos))
            {
                break;
            }
        }
        else if( pos.x == -1 )
        {
            pressed = false;
        }
        
    }

    msg.state = MODE_MIC;
    msg.audioPtr = (uint8_t*)previewR;
    msg.audioSize = 120264;

    xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);

    Timerbuff.deleteSprite();

    Disbuff.drawJpg(CoreMainImage,87169,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);

    sytState.power = kPOWER_MAX;
}
void TFTTest()
{
    HotZone Btn(0,0,320,280);
    TouchPoint_t pos = M5.Touch.getPressPoint();

    int colorIndex = 0;
    uint32_t colorList[4][2] = {{TFT_RED ,  TFT_RED},
                                {TFT_GREEN ,TFT_GREEN},
                                {TFT_BLUE,  TFT_BLUE},
                                {TFT_WHITE, TFT_BLACK}};

    M5.Lcd.fillRect(0,0,320,240,colorList[colorIndex][1]);
    M5.Lcd.drawRect(0,0,320,240,colorList[colorIndex][0]);

    i2sQueueMsg_t msg;

    msg.state = MODE_SPK;
    msg.audioPtr = (void*)bibiSig;
    msg.audioSize = 8820;

    bool pressed = true;
    while(1)
    {
        pos = M5.Touch.getPressPoint();
        
        if(( pos.x != -1 )&&( pressed == false ))
        {
            pressed = true;

            msg.state = MODE_SPK;
            msg.audioPtr = (void*)bibiSig;
            msg.audioSize = 8820;

            xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);
            colorIndex++;
            if( colorIndex == 4 ) break;

            M5.Lcd.fillRect(0,0,320,240,colorList[colorIndex][1]);
            M5.Lcd.drawRect(0,0,320,240,colorList[colorIndex][0]);
        }
        else if( pos.x == -1 )
        {
            pressed = false;
        }

    }

    Disbuff.pushSprite(0,0);
}
void AppSetting()
{
    Disbuff.drawJpg(SettingAppImage,50771,0,0,320,240);

    HotZone IOBtn(7,99,104,154);
    HotZone MoterBtn(113,99,210,154);

    HotZone TFTBtn(7,161,104,216);
    HotZone SoundBtn(113,161,210,216);

    HotZone EscBtn(240,200,320,280);

    TouchPoint_t pos = M5.Touch.getPressPoint();

    bool pressed = false;
    uint8_t IOState = 0;
    uint8_t MoterState = 1;

    uint8_t ioBuff[6] = {14,13,32,33,19,27};
    uint8_t iostatebuff[4][6] = {
        {1,1,1,1,1,1},
        {0,1,0,1,0,1},
        {1,0,1,0,1,0},
        {0,0,0,0,0,0},
    };

    uint16_t adcReadCount = 0;
    i2sQueueMsg_t msg;

    msg.state = MODE_SPK;
    msg.audioPtr = (void*)bibiSig;
    msg.audioSize = 8820;

    pinMode(14,OUTPUT);
    pinMode(13,OUTPUT);
    pinMode(32,OUTPUT);
    pinMode(33,OUTPUT);
    pinMode(19,OUTPUT);
    pinMode(27,OUTPUT);

    Disbuff.fillRect(193,103,14,47,TFT_RED);

    if( sytState.soundFlag )
    {
        Disbuff.fillRect(193,165,14,47,TFT_GREEN);
    }
    else
    {
        Disbuff.fillRect(193,165,14,47,TFT_RED);
    }
    
    Disbuff.pushSprite(0,0);

    while(1)
    {
        pos = M5.Touch.getPressPoint();
        
        if(( pos.x != -1 )&&( pressed == false ))
        {
            pressed = true;

            msg.state = MODE_SPK;
            msg.audioPtr = (void*)bibiSig;
            msg.audioSize = 8820;

            xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);

            if( IOBtn.inHotZone(pos))
            {
                for( int i = 0; i < 6; i++ )
                {
                    digitalWrite(ioBuff[i],iostatebuff[IOState][i]);
                }
                IOState++;
                IOState = ( IOState >= 4 ) ? 0 : IOState;
                Serial.printf("Set IOState %d \n",IOState);
            }

            if( MoterBtn.inHotZone(pos))
            {
                M5.Axp.SetLDOEnable(3,MoterState);
                if( MoterState == 1 )
                {
                    Disbuff.fillRect(193,103,14,47,TFT_GREEN);
                }
                else
                {
                    Disbuff.fillRect(193,103,14,47,TFT_RED);
                }
                Disbuff.pushSprite(0,0);
                MoterState = ( MoterState == 0 ) ? 1 : 0;
                Serial.printf("Set Moter %d \n",MoterState);

            }
            //
            if( SoundBtn.inHotZone(pos))
            {
                sytState.soundFlag = ( sytState.soundFlag == true ) ? false : true;
                M5.Axp.SetSpkEnable(sytState.soundFlag);

                if( sytState.soundFlag )
                {
                    Disbuff.fillRect(193,165,14,47,TFT_GREEN);
                }
                else
                {
                    Disbuff.fillRect(193,165,14,47,TFT_RED);
                }
                Disbuff.pushSprite(0,0);
            }

            if( TFTBtn.inHotZone(pos))
            {
                TFTTest();
            }

            if( EscBtn.inHotZone(pos))
            {
                break;
            }
        }
        
        else if( pos.x == -1 )
        {
            pressed = false;
        }

        if( adcReadCount > 50 )
        {
            adcReadCount = 0;
            uint16_t pin35ADC = analogRead(35);
            double pin35vol = (double)pin35ADC *  3.3 / 4096;
            uint16_t pin36ADC = analogRead(36);
            double pin36vol = (double)pin36ADC *  3.3 / 4096;

            if(( pin35vol > 1.8 )&&( pin35vol < 2.2 ))
            {
                Disbuff.fillRect(86,103,14,21,TFT_GREEN);
            }
            else
            {
                Disbuff.fillRect(86,103,14,21,TFT_RED);
            }
            
            if(( pin36vol > 0.8 )&&( pin36vol < 1.2 ))
            {
                Disbuff.fillRect(86,129,14,21,TFT_GREEN);
            }
            else
            {
                Disbuff.fillRect(86,129,14,21,TFT_RED);
            }
            Disbuff.pushSprite(0,0);
            Serial.printf("ADC:%.2f,%.2f\n",pin35vol,pin36vol);
        }
        adcReadCount ++;
        delay(10);
    }
    msg.state = MODE_MIC;

    xQueueSend(i2sstateQueue,&msg,(TickType_t)portMAX_DELAY);

    Disbuff.drawJpg(CoreMainImage,87169,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);

    sytState.power = kPOWER_MAX;

}

void MPU6886Page()
{
    float accX = 0;
    float accY = 0;
    float accZ = 0;

    double theta = 0, last_theta = 0;
    double phi = 0, last_phi = 0;
    double alpha = 0.2;

    TouchPoint_t pos = M5.Touch.getPressPoint();
	int menuAmoCount = 0;
    int menuPosyNow = 0; 
    int menuState = 1;
    HotZone_t _touchBtn2(230,206,310,280);

    menubuff.createSprite(320,34);
    menubuff.drawJpg(imageMenu,14900,0,0,320,34);

    line_3d_t x = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t y = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t z = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 30}};

    line_3d_t rect_dis;

    line3D line3d;

    for (int n = 0; n < 12; n++)
    {
        rect_source[n].start_point.x = rect[n].start_point.x * 50;
        rect_source[n].start_point.y = rect[n].start_point.y * 50;
        rect_source[n].start_point.z = rect[n].start_point.z * 50;
        rect_source[n].end_point.x = rect[n].end_point.x * 50;
        rect_source[n].end_point.y = rect[n].end_point.y * 50;
        rect_source[n].end_point.z = rect[n].end_point.z * 50;
    }

    line3d.setZeroOffset(160,110);

    while(1)
    {
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        //M5.MPU6886.getAccelData(&accX, &accY, &accZ);
        if ((accX < 1) && (accX > -1))
        {
            theta = asin(-accX) * 57.295;
        }
        if (accZ != 0)
        {
            phi = atan(accY / accZ) * 57.295;
        }

        theta = alpha * theta + (1 - alpha) * last_theta;
        phi = alpha * phi + (1 - alpha) * last_phi;

        Disbuff.fillRect(0,0,320,240,Dis3Dbuff.color565(0x0,0x0,0x0));
        //Disbuff.setTextSize(1);
        //Disbuff.setCursor(10, 115);
        //Disbuff.printf("%.2f", theta);
        //Disbuff.setCursor(10, 125);
        //Disbuff.printf("%.2f", phi);

        z.end_point.x = 0;
        z.end_point.y = 0;
        z.end_point.z = 24;
        line3d.RotatePoint(&z.end_point, theta, phi, 0);
        line3d.RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
        line3d.RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

        line3d.printLine3D(&Disbuff, &x, TFT_GREEN);
        line3d.printLine3D(&Disbuff, &y, TFT_GREEN);
        line3d.printLine3D(&Disbuff, &z, TFT_GREEN);

        uint16_t linecolor = Disbuff.color565(0xff,0x9c,0x00);

        for (int n = 0; n < 12; n++)
        {
            line3d.RotatePoint(&rect_source[n].start_point, &rect_dis.start_point, theta, phi, (double)0);
            line3d.RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta, phi, (double)0);
            line3d.printLine3D(&Disbuff, &rect_dis, linecolor);
        }
        
        last_theta = theta;
        last_phi = phi;

        pos = M5.Touch.getPressPoint();

        if( pos.x != -1 )
        {
            switch( menuState )
            {
                case 0: menuState = 1; menuPosyNow = 0; break;
                case 2: menuAmoCount = 0; 
                    if( _touchBtn2.inHotZone(pos)) goto endImage; break;
            }
        }

        switch( menuState )
        {
            case 1: 
                menuPosyNow += 4;
                if( menuPosyNow >= 34)
                {
                    menuPosyNow = 34;
                    menuState = 2;
                    menuAmoCount = 0;
                }
                break;
            case 2: 
                menuAmoCount++;
                if( menuAmoCount > 500 )
                {
                    menuAmoCount = 0;
                    menuPosyNow = 34;
                    menuState = 3;
                }
            break;
            case 3: 
                menuPosyNow -= 4;
                if( menuPosyNow <= 0)
                {
                    menuPosyNow = 0;
                    menuState = 0;
                    menuAmoCount = 0;
                }
            break;
        }
        Disbuff.fillRect(0,206,320,34,TFT_BLACK);
        Disbuff.pushInSprite(&menubuff,0,0,320,34,0,241 - menuPosyNow);
        Disbuff.pushSprite(0,0);

    }
    endImage:

    for (int n = 0; n < 12; n++)
    {
        rect_source[n].start_point.x = rect[n].start_point.x * 12;
        rect_source[n].start_point.y = rect[n].start_point.y * 12;
        rect_source[n].start_point.z = rect[n].start_point.z * 12;
        rect_source[n].end_point.x = rect[n].end_point.x * 12;
        rect_source[n].end_point.y = rect[n].end_point.y * 12;
        rect_source[n].end_point.z = rect[n].end_point.z * 12;
    }

    Disbuff.drawJpg(CoreMainImage,87169,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);

    sytState.power = kPOWER_MAX;

}

void setCheckState(int number, bool state, bool flush)
{
    int posx = 0, posy = 0;
    uint8_t *ptr1 = nullptr,*ptr2 = nullptr;
    if( number >= 5 )
    {
        posx = 166;
        posy = 154- ( number - 5 ) * 33;
        ptr1 = (uint8_t*)image_Sysinit_0000s_0000_L1;
        ptr2 = (uint8_t*)image_Sysinit_0000s_0001_L2;
    }
    else
    {
        posx = 44;
        posy = 22 + number * 33;
        ptr1 = (uint8_t*)image_Sysinit_0001s_0000_R1;
        ptr2 = (uint8_t*)image_Sysinit_0001s_0001_R2;
    }
    int32_t color = ( state ) ? 0x35ffae : 0xff0000;
    
    Disbuff.drawColorBitmapAlpha(posx,posy,53,33,
                                (uint8_t*)ptr2,
                                color,
                                0x000000);

    Disbuff.drawColorBitmapAlpha(posx,posy + 1,53,33,
                                (uint8_t*)ptr1,
                                0xff9a00,
                                color);
    
    if( flush == false ) return;
    Disbuff.pushSprite(0,0);
}

void sysInitCheck()
{
    Disbuff.drawJpg(CoverImage,21301,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);
}

void setup()
{
    M5.begin(true, true, true, false);

    M5.Axp.SetCHGCurrent(AXP192::kCHG_100mA);
    Disbuff.createSprite(320, 240);
    
    DisCoverScrollbuff.createSprite(320,60);

    addI2cDevice("Axp192",0x34);
    addI2cDevice("CST Touch",0x38);
    addI2cDevice("IMU6886",0x68);
    addI2cDevice("BM8563",0x51);
    addI2cDevice("AT608A",0x35);

    M5.Axp.SetLcdVoltage(2800);

    sysInitCheck();
    SD.begin();

    sytState.App1Zone[0] = new HotZone( 20,191, 64,235,&AppImage);
    sytState.App1Zone[1] = new HotZone( 79,191,123,235,&AppWifi);
    sytState.App1Zone[2] = new HotZone(138,191,182,235,&AppTimer);
    sytState.App1Zone[3] = new HotZone(197,191,241,235,&APPSleep);
    sytState.App1Zone[4] = new HotZone(256,191,300,235,&AppSetting);
    sytState.App1Zone[5] = new HotZone(72,130,161,186,&MPU6886Page);

    M5.Axp.SetLcdVoltage(3300);

    M5.Axp.SetBusPowerMode(0);
    M5.Axp.SetCHGCurrent(AXP192::kCHG_190mA);

    M5.Axp.SetSpkEnable(sytState.soundFlag);
    InitI2SSpakerOrMic(MODE_SPK);
    xTaskCreatePinnedToCore(i2s_task, "i2s_task", 4096, NULL, 3, NULL, 0);

    M5.Axp.SetLDOEnable(3,true);
    CoverScrollText("Motor Test",M5.Lcd.color565(SUCCE_COLOR));
    delay(150);
    M5.Axp.SetLDOEnable(3,false); 

    M5.Axp.SetLed(1);
    CoverScrollText("LED Test",M5.Lcd.color565(SUCCE_COLOR));
    delay(100);
    M5.Axp.SetLed(0);
    
    FastLED.addLeds<SK6812, LEDS_PIN>(ledsBuff, LEDS_NUM);
    for (int i = 0; i < LEDS_NUM; i++)
    {
        ledsBuff[i].setRGB(20, 20, 20);
    }
    FastLED.show();
    
    Wire1.begin(21, 22, 10000);
    checkI2cAddr();
    checkAETCC608AInit();
    checkPsram();
    checkIMUInit();
    checkSDCard();
    checkIOPort();
    
    DisCoverScrollbuff.deleteSprite();
 
    M5.Axp.SetLDOVoltage(3,3300);
    M5.Axp.SetLed(1);
 
    Disbuff.drawJpg(CoreMainImage,87169,0,0,320,240,0,0);
    Disbuff.pushSprite(0,0);

    setup3D();
    clockSetup();
    powerSetup();
    microPhoneSetup();
    batPowerSetup();
    touchsetup();
    sdcardSetup();
    choosePower();
}

uint16_t imageCount = 0;
uint16_t timecount = 0;

void loop()
{
    if( timecount >= 2 )
    {
        timecount = 0;
        clockFlush();
        batpowerFlush();
    }
    timecount++;
    sdCardFlush();
    choosePower();
    MPU6886Test();
    MicroPhoneFFT();
    touchflush();
    delay(5);
}