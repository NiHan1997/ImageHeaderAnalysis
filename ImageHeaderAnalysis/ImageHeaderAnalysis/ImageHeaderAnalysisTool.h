#pragma once

#include <fstream>
#include <vector>

struct ImageSize
{
    int width;
    int height;
};

#define GAME_MODE 1

class ImageHeaderAnalysisTool
{
public:
    static ImageSize GetImageSizeByHeaderInfo(const std::string& filename);

private:
    static ImageSize GetJPGImageSizeByHeaderInfo(std::ifstream& file, const std::vector<unsigned char>& header);
    static ImageSize GetGIFImageSizeByHeaderInfo(std::ifstream& file);
    static ImageSize GetPNGImageSizeByHeaderInfo(std::ifstream& file);
    static ImageSize GetBMPImageSizeByHeaderInfo(std::ifstream& file);    
};

