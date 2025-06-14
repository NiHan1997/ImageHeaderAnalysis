#include <iostream>

#include "ImageHeaderAnalysisTool.h"

int main()
{
    ImageSize size = ImageHeaderAnalysisTool::GetImageSizeByHeaderInfo("DIY_Nikki.jpeg");
    if (size.width != -1)
    {
        std::cout << "Width: " << size.width << ", Height: " << size.height << std::endl;
    }
    else
    {
        std::cout << "Failed to get image size." << std::endl;
    }
    return 0;
}