#include <hue/HueScanner.h>

namespace hue
{

  static const char* TAG = "Hue";

  HueScanner::HueScanner()
  {
    mpHTTPHandler = std::make_shared<LinHttpHandler>();
  }

  void HueScanner::scan()
  {
    HueFinder finder(mpHTTPHandler);
    ESP_LOGI(TAG, "SCANNING");
    std::vector<HueFinder::HueIdentification> bridges = finder.FindBridges();
        ESP_LOGI(TAG, "SCAN COMPLETED");
    for (auto const& id: bridges)
    {
      ESP_LOGI(TAG, "got ip:%s", id.ip.c_str());
    }
  }
} // hue