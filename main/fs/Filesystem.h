#include <string>

namespace fs
{
class FileSystem
{
  public:
    static FileSystem& getInstance()
    {
      static FileSystem instance; 
      return instance;
    }
    std::string readJsonConfig(const std::string filepath) const;
  private:
    FileSystem();

  public:
    FileSystem(FileSystem const&)               = delete;
    void operator=(FileSystem const&)  = delete;
    void loadPartitions();
};
} // namespace fs
