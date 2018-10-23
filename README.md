# 简单抠像

**一、任务描述：提供两张图：space.jpg和superman.jpg，两张图的大小均为640*480像素。要求把superman.jpg中的超人抠出，加到 space.jpg 的太空背景。两张图像分别如下：**

***space.jpg***

![space](/img3/space.jpg)

***superman.jpg***

![superman](/img3/superman.jpg)

**二、思路：superman.jpg图像中的像素值用（r,g,b）表示，条件1：10<r<160; 条件2：100<g<220; 条件3：10<b<110；同时满足这三个条件的像素为绿幕区域像素。把满条件像素的值，修改为相应的太空背景图片的像素值，即相当于把超人抠到太空背景中。**

***代码如下：***

```
`#include "pch.h"
#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	//输入图像
	GDALDataset* poSrcDS1;
	GDALDataset* poSrcDS2;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen1, imgYlen1,imgXlen2, imgYlen2;
	//输入图像路径
	char* srcPath1 = (char*)"space.jpg";
	char* srcPath2 = (char*)"superman.jpg";
	//输出图像路径
	char* dstPath = (char*)"result.jpg";
	//图像内存存储
	GByte* buffTmp1_1;
	GByte* buffTmp1_2;
	GByte* buffTmp1_3;
	GByte* buffTmp2_1;
	GByte* buffTmp2_2;
	GByte* buffTmp2_3;
	//图像波段数
	int i, j, bandNum1, bandNum2;
	//像素值
	int r, g, b;`
```



	//注册驱动
	GDALAllRegister();
	
	//打开图像
	poSrcDS1 = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);
	poSrcDS2 = (GDALDataset*)GDALOpenShared(srcPath2, GA_ReadOnly);
	//获取图像宽度，高度，和波段数量
	imgXlen1 = poSrcDS1->GetRasterXSize();
	imgYlen1 = poSrcDS1->GetRasterYSize();
	bandNum1 = poSrcDS1->GetRasterCount();
	
	imgXlen2 = poSrcDS2->GetRasterXSize();
	imgYlen2 = poSrcDS2->GetRasterYSize();
	bandNum2 = poSrcDS2->GetRasterCount();


	//根据图像的宽度和高度分配内存
	buffTmp1_1 = (GByte*)CPLMalloc(imgXlen1 * imgYlen1 * sizeof(GByte));
	buffTmp1_2 = (GByte*)CPLMalloc(imgXlen1 * imgYlen1 * sizeof(GByte));
	buffTmp1_3 = (GByte*)CPLMalloc(imgXlen1 * imgYlen1 * sizeof(GByte));
	buffTmp2_1 = (GByte*)CPLMalloc(imgXlen2 * imgYlen2 * sizeof(GByte));
	buffTmp2_2 = (GByte*)CPLMalloc(imgXlen2 * imgYlen2 * sizeof(GByte));
	buffTmp2_3 = (GByte*)CPLMalloc(imgXlen2 * imgYlen2 * sizeof(GByte));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen2, imgYlen2, bandNum2, GDT_Byte, NULL);
	//修改超人图像中同时满足条件的像素值为相应的太空背景图像中的像素值
	poSrcDS1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen1, imgYlen1, buffTmp1_1, imgXlen1, imgYlen1, GDT_Byte, 0, 0);
	poSrcDS2->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen2, imgYlen2, buffTmp2_1, imgXlen2, imgYlen2, GDT_Byte, 0, 0);
	poSrcDS1->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen1, imgYlen1, buffTmp1_2, imgXlen1, imgYlen1, GDT_Byte, 0, 0);
	poSrcDS2->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen2, imgYlen2, buffTmp2_2, imgXlen2, imgYlen2, GDT_Byte, 0, 0);
	poSrcDS1->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen1, imgYlen1, buffTmp1_3, imgXlen1, imgYlen1, GDT_Byte, 0, 0);
	poSrcDS2->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen2, imgYlen2, buffTmp2_3, imgXlen2, imgYlen2, GDT_Byte, 0, 0);
	for (j = 0; j < imgYlen2; j++)
	{
		for (i = 0; i < imgXlen2; i++)
		{
			r = buffTmp2_1[j * imgXlen2 + i];
			g = buffTmp2_2[j * imgXlen2 + i];
			b = buffTmp2_3[j * imgXlen2 + i];
			if( (r > 10 && r < 160) && (g > 100 && g < 220) && (b > 10 && b < 110))
			{
				buffTmp2_1[j * imgXlen2 + i] = buffTmp1_1[j * imgXlen2 + i];
				buffTmp2_2[j * imgXlen2 + i] = buffTmp1_2[j * imgXlen2 + i];
				buffTmp2_3[j * imgXlen2 + i] = buffTmp1_3[j * imgXlen2 + i];
			}
		}
	}
	//写入图像
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen2, imgYlen2, buffTmp2_1, imgXlen2, imgYlen2, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen2, imgYlen2, buffTmp2_2, imgXlen2, imgYlen2, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen2, imgYlen2, buffTmp2_3, imgXlen2, imgYlen2, GDT_Byte, 0, 0);
	
	//清除内存
	CPLFree(buffTmp1_1);
	CPLFree(buffTmp2_1);
	CPLFree(buffTmp1_2);
	CPLFree(buffTmp2_2);
	CPLFree(buffTmp1_3);
	CPLFree(buffTmp2_3);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS2);
	GDALClose(poSrcDS1);
	
	system("PAUSE");
	return 0;
}

***结果如下：***

![result](/img3/result.jpg)

**三、经验总结：若想知道像素值（r,g,b）是否同时满足条件1：10<r<160; 条件2：100<g<220; 条件3：10<b<110；则需要分别对图像的每一通道的所有像素值分配内存，并比较。若同时满足条件，则将像素值修改为对应的太空图像的像素值，即将太空背景填充到绿幕之中。**



