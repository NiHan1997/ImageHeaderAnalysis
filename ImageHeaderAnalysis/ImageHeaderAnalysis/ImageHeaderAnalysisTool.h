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
    static ImageSize GetImageSizeByHeaderInfo(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            return { -1, -1 };
        }

        // 文件类型判断, 暂时只需要 4 个字节.
        std::vector<unsigned char> header(4);
        file.read(reinterpret_cast<char*>(header.data()), header.size());

        // JPG 图片.
        if (header[0] == 0xFF && header[1] == 0xD8)
        {
            return GetJPGImageSizeByHeaderInfo(file, header);
        }

        // GIF 图片.
        if (header[0] == 'G' && header[1] == 'I' && header[2] == 'F')
        {
            return GetGIFImageSizeByHeaderInfo(file);
        }

        // PNG 图片.
        if (header[0] == 0x89 && header[1] == 'P' && header[2] == 'N' && header[3] == 'G')
        {
            return GetPNGImageSizeByHeaderInfo(file);
        }

        // BMP 图片.
        if (header[0] == 'B' && header[1] == 'M')
        {
            return GetBMPImageSizeByHeaderInfo(file);
        }

        file.close();
        return { -1, -1 };
    }

private:
    static ImageSize GetJPGImageSizeByHeaderInfo(std::ifstream& file, const std::vector<unsigned char>& header);
    static ImageSize GetGIFImageSizeByHeaderInfo(std::ifstream& file);
    static ImageSize GetPNGImageSizeByHeaderInfo(std::ifstream& file);
    static ImageSize GetBMPImageSizeByHeaderInfo(std::ifstream& file);    
};

