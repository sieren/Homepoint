#pragma once

#include <util/warnings.h>
#include <model/Model.hpp>
#include <memory>
#include "libraries/hueplusplus/hueplusplus/include/Hue.h"
#include "libraries/hueplusplus/hueplusplus/include/LinHttpHandler.h"

extern "C" 
{
  #include "esp_log.h"
}

namespace hue
{
class HueScanner
{
  public:
    HueScanner();
    void scan();
  private:
    std::shared_ptr<LinHttpHandler> mpHTTPHandler;


};

} // hue
