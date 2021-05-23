#include "spiffs_drv.h"

#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#include "Arduino.h"
static const char *TAG = "SPIFFS-DRIVER";

static lv_fs_drv_t drv;
typedef FILE * file_t;

static lv_fs_res_t my_open_cb(lv_fs_drv_t *drv, void *file_p, const char *fn, lv_fs_mode_t mode)
{
  (void) drv;		/*Unused*/

	const char * flags = "";

	if(mode == LV_FS_MODE_WR) flags = "w";
	else if(mode == LV_FS_MODE_RD) flags = "r";
	else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "rb+";

	/*Make the path relative to the current directory (the projects root folder)*/

  char complete_path[strlen(fn) + 1];
  complete_path[0] = '/';
  complete_path[1] = '\0';
  strcat(complete_path, fn);

	file_t f = fopen(complete_path, flags);
	if(f == NULL) return LV_FS_RES_UNKNOWN;

	/*Be sure we are the beginning of the file*/
	fseek(f, 0, SEEK_SET);

	/* 'file_p' is pointer to a file descriptor and
	 * we need to store our file descriptor here*/
	file_t * fp = file_p;        /*Just avoid the confusing casings*/
	*fp = f;
  //  ESP_LOGI(TAG, "Open eof file_p %d", feof(file_p));
	return LV_FS_RES_OK;
}

static lv_fs_res_t my_close_cb(lv_fs_drv_t *drv, void *file_p)
{
  (void) drv;		/*Unused*/
	file_t * fp = file_p;        /*Just avoid the confusing casings*/
	fclose(*fp);
	return LV_FS_RES_OK;
}

static lv_fs_res_t my_read_cb(lv_fs_drv_t *drv, void *file_p, 
  void *buf, uint32_t btr, uint32_t *br)
{
 // ESP_LOGI(TAG, "Read eof %d", feof(file_p));
  (void) drv;		/*Unused*/
	file_t * fp = file_p;        /*Just avoid the confusing casings*/
	*br = fread(buf, 1, btr, *fp);
	return LV_FS_RES_OK;
}

static lv_fs_res_t my_seek_cb(lv_fs_drv_t *drv, void *file_p, uint32_t pos)
{
  (void) drv;		/*Unused*/
	file_t * fp = file_p;        /*Just avoid the confusing casings*/
	fseek(*fp, pos, SEEK_SET);
	return LV_FS_RES_OK;
}

static lv_fs_res_t my_tell_cb(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
  (void) drv;		/*Unused*/
	file_t * fp = file_p;        /*Just avoid the confusing casings*/
	*pos_p = ftell(*fp);
	return LV_FS_RES_OK;
}

static lv_fs_res_t my_size_cb(lv_fs_drv_t* drv, void* file_p, uint32_t* size_p)
{
  (void) drv;		/*Unused*/
	file_t * fp = file_p;        /*Just avoid the confusing casings*/
  fseek(*fp, 0L, SEEK_END);
	*size_p = ftell(*fp);
  fseek(*fp, 0L, SEEK_SET);
	return LV_FS_RES_OK;
}

void spiffs_drv_init()
{
  esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "spiffs",
        .max_files = 20,
        .format_if_mount_failed = false};

  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
    }
    else if (ret == ESP_ERR_NOT_FOUND)
    {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
    }
    else
    {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return ESP_FAIL;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(conf.partition_label, &total, &used);
  if (ret != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
  }
  else
  {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }

  ESP_LOGE(TAG, "Spiffs init");
  lv_fs_drv_init(&drv);                     /*Basic initialization*/

  drv.letter = 'D';                         /*An uppercase letter to identify the drive */
  drv.file_size = sizeof(file_t);           /*Size required to store a file object*/
  drv.open_cb = my_open_cb;                 /*Callback to open a file */
  drv.close_cb = my_close_cb;               /*Callback to close a file */
 // drv.remove_cb = my_remove_cb;             /*Callback to remove a file */
  drv.read_cb = my_read_cb;                 /*Callback to read a file */
  //drv.write_cb = my_write_cb;               /*Callback to write a file */
  drv.seek_cb = my_seek_cb;                 /*Callback to seek in a file (Move cursor) */
  drv.tell_cb = my_tell_cb;                 /*Callback to tell the cursor position  */
  drv.size_cb = my_size_cb;
  ESP_LOGE(TAG, "FS init");
  lv_fs_drv_register(&drv);                 /*Finally register the drive*/
  ESP_LOGE(TAG, "FS done");
}
