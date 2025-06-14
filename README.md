# 通过图片的文件头信息获取图片大小

## 1. JPG

需要解析识别到文件的 SOF 区域，特征码是 FFC0、FFC1、FFC2 等关键字，并且还可能重复。对于游戏开发来说读取到 FFC0 获取大小信息足够了。



## 2. GIF

文件固定的位置获取。



## 3. PNG

文件固定的位置获取。



## 4. BMP

文件固定的位置获取。



## 5. 参考资料

* [JPG 头在线查看](https://cyber.meme.tips/jpdump/#)
* [JPG 解码(1)](https://www.cnblogs.com/Dreaming-in-Gottingen/p/14285605.html)
* [JPG 解码(3)](https://www.cnblogs.com/Dreaming-in-Gottingen/p/14347789.html)
* [GIF 头文件介绍](https://blog.csdn.net/runafterhit/article/details/119848902)

* [BMP 图片解析](https://blog.csdn.net/u012877472/article/details/50272771)

* [PNG 图片解析](https://blog.csdn.net/u013943420/article/details/76855416)