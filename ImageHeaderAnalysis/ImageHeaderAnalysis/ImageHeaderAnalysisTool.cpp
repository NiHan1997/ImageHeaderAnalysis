#include "ImageHeaderAnalysisTool.h"

ImageSize ImageHeaderAnalysisTool::GetImageSizeByHeaderInfo(const std::string& filename)
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

ImageSize ImageHeaderAnalysisTool::GetJPGImageSizeByHeaderInfo(std::ifstream& file, const std::vector<unsigned char>& header)
{
    // JPEG
    // https://www.cnblogs.com/Dreaming-in-Gottingen/p/14285605.html
    // FFC0 对应 JPG 的 SOF 信息区域开始, 其中偏移 3 字节后就是文件大小.
    // 照片可能有多个 FFC0 和 FFCN, 需要综合判断才正确.
    // 如果有 FFC1, 则 FFC1 的信息可能才正确.
    // 游戏中的图片一般没有 FFC1, 可以去掉 FFC1 搜索的代码.
#if !GAME_MODE
    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();        // 获取文件大小.
    file.seekg(header.size(), std::ios::beg);
#endif

    // 查找 FFC0.
    ImageSize sof0Size = { 0, 0 };
    while (true)
    {
        unsigned char val;
        file.read(reinterpret_cast<char*>(&val), 1);
        if (val != 0xFF)
        {
            continue;
        }
        file.read(reinterpret_cast<char*>(&val), 1);
        if (val != 0xC0)
        {
            continue;
        }

        file.seekg(3, std::ios::cur);

        std::vector<unsigned char> sizeInfo(4);
        file.read(reinterpret_cast<char*>(sizeInfo.data()), sizeInfo.size());
                
        int height = sizeInfo[0] << 8 | sizeInfo[1];
        int width = sizeInfo[2] << 8 | sizeInfo[3];
                
        sof0Size = { width, height };
        break;
    }

#if !GAME_MODE
    // 查找 FFC1, 游戏开发中可以去掉.
    // 复杂情况还需要查找 FFCN, 在游戏开发中已经不合适了.
    ImageSize sof1Size = { 0, 0 };
    while (true)
    {
        int currPos = file.tellg();
        if (currPos >= fileSize)
        {
            break;
        }
            
        unsigned char val;
        file.read(reinterpret_cast<char*>(&val), 1);
        if (val != 0xFF)
        {
            continue;
        }
        file.read(reinterpret_cast<char*>(&val), 1);
        if (val != 0xC1)
        {
            continue;
        }
            
        file.seekg(3, std::ios::cur);

        std::vector<unsigned char> sizeInfo(4);
        file.read(reinterpret_cast<char*>(sizeInfo.data()), sizeInfo.size());
                
        int height = sizeInfo[0] << 8 | sizeInfo[1];
        int width = sizeInfo[2] << 8 | sizeInfo[3];
                
        sof1Size = { width, height };
        break;
    }

    if (sof1Size.width > 0 && sof1Size.height > 0)
    {
        return sof1Size;
    }
#endif
    
    return sof0Size;
}

ImageSize ImageHeaderAnalysisTool::GetGIFImageSizeByHeaderInfo(std::ifstream& file)
{
    // GIF
    // https://blog.csdn.net/runafterhit/article/details/119848902
    file.seekg(6, std::ios::beg);

    std::vector<unsigned char> sizeInfo(4);
    file.read(reinterpret_cast<char*>(sizeInfo.data()), sizeInfo.size());
        
    int width = sizeInfo[0] + (sizeInfo[1] << 8);
    int height = sizeInfo[2] + (sizeInfo[3] << 8);
    return { width, height };
}

ImageSize ImageHeaderAnalysisTool::GetPNGImageSizeByHeaderInfo(std::ifstream& file)
{
    // PNG
    // https://blog.csdn.net/u013943420/article/details/76855416

    file.seekg(16, std::ios::beg);

    std::vector<unsigned char> sizeInfo(8);
    file.read(reinterpret_cast<char*>(sizeInfo.data()), sizeInfo.size());
        
    int width = (sizeInfo[0] << 24) | (sizeInfo[1] << 16) | (sizeInfo[2] << 8) | sizeInfo[3];
    int height = (sizeInfo[4] << 24) | (sizeInfo[5] << 16) | (sizeInfo[6] << 8) | sizeInfo[7];
    return { width, height };
}

ImageSize ImageHeaderAnalysisTool::GetBMPImageSizeByHeaderInfo(std::ifstream& file)
{
    //BMP
    // https://blog.csdn.net/u012877472/article/details/50272771

    file.seekg(18, std::ios::beg);

    std::vector<unsigned char> sizeInfo(8);
    file.read(reinterpret_cast<char*>(sizeInfo.data()), sizeInfo.size());
        
    int width = (sizeInfo[0]) | (sizeInfo[1] << 8) | (sizeInfo[2] << 16) | (sizeInfo[3] << 24);
    int height = (sizeInfo[4]) | (sizeInfo[5] << 8) | (sizeInfo[6] << 16) | (sizeInfo[7] << 24);
    return { width, height };
}
