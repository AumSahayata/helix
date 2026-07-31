#include "Storage.h"

Storage::Storage(SPIClass &spi, uint8_t csPin)
    : spi(spi),
      csPin(csPin)
{
}

bool Storage::begin()
{
    mounted = SD.begin(csPin, spi);

    return mounted;
}

bool Storage::isMounted() const
{
    return mounted;
}

bool Storage::listFiles(
    const char *path,
    std::vector<String> &files)
{
    files.clear();

    File dir = SD.open(path);

    if (!dir || !dir.isDirectory())
        return false;

    File file = dir.openNextFile();

    while (file)
    {
        if (!file.isDirectory())
        {
            files.push_back(file.name());
        }

        file.close();
        file = dir.openNextFile();
    }

    dir.close();

    return true;
}

bool Storage::listFilesRecursive(
    const char *path,
    std::vector<String> &files)
{
    File dir = SD.open(path);

    if (!dir)
    {
        return false;
    }

    if (!dir.isDirectory())
    {
        dir.close();
        return false;
    }

    File file = dir.openNextFile();

    while (file)
    {
        if (file.isDirectory())
        {
            String newPath = String(path);

            if (!newPath.endsWith("/"))
            {
                newPath += "/";
            }

            newPath += file.name();

            if (!listFilesRecursive(newPath.c_str(), files))
            { 
                file.close();
                dir.close();
                return false;
            }
        }
        else
        {
            files.push_back(
                String(path) + "/" + file.name());
        }

        file.close();

        file = dir.openNextFile();
    }

    dir.close();

    return true;
}