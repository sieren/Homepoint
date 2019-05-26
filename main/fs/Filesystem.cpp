#include "Filesystem.h"
#include <FS.h>
#include "SPIFFS.h"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
}
#include <fstream>
#include <iostream>
#include <string>

static const char *FSTAG = "example";

namespace fs
{
  FileSystem::FileSystem()
  {
 
  }

  void FileSystem::loadPartitions()
  {
    // JPEG drawing unfortunately relies on the Arduino library
    if (!SPIFFS.begin(false, "/spiffs", 100)) {
      ESP_LOGE(FSTAG, "Failed to load Arduino Version of  SPIFFS partition");
    }
    if (!SPIFFS.exists("/SFProText-Regular6.vlw"))
    {
      ESP_LOGE(FSTAG, "Failed to find Font");
    }
  }

  std::string FileSystem::readJsonConfig(const std::string filepath) const
  {
    std::ifstream ifs(filepath);
    std::string content((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));
    return content;
  }
} // namespace fs
