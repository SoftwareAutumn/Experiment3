- # **实验代码**

~~~c++
// lesson3.cpp: 定义控制台应用程序的入口点。`
`//`

`#include "stdafx.h"`
`#include <iostream>`
`using namespace std;`
`#include "./gdal/gdal_priv.h"`
`#pragma comment(lib, "gdal_i.lib")`

`int main()`
`{`
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	char* srcPath = "superman.jpg";
	char* src1Path = "space.jpg";
	char* dstPath = "final.tif";
	GByte* buffTmp1, *buffTmp2, *buffTmp3, *buffTmp11, *buffTmp12, *buffTmp13;
	int imgXlen, imgYlen;
	int img1Xlen, img1Ylen, bandNum1;
	int i;
	int j;

```
GDALAllRegister();

/*
将superman的三个通道分别读到buffTmp1, buffTmp2, buffTmp3
*/
poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
imgXlen = poSrcDS->GetRasterXSize();
imgYlen = poSrcDS->GetRasterYSize();
buffTmp1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
buffTmp2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
buffTmp3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));	
poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Byte, 0, 0);
poSrcDS->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp2, imgXlen, imgYlen, GDT_Byte, 0, 0);
poSrcDS->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp3, imgXlen, imgYlen, GDT_Byte, 0, 0);

//将space的所有通道读到缓冲区
poSrcDS = (GDALDataset*)GDALOpenShared(src1Path, GA_ReadOnly);
img1Xlen = poSrcDS->GetRasterXSize();
img1Ylen = poSrcDS->GetRasterYSize();
bandNum1 = poSrcDS->GetRasterCount();
buffTmp11 = (GByte*)CPLMalloc(img1Xlen*img1Ylen * sizeof(GByte));
buffTmp12 = (GByte*)CPLMalloc(img1Xlen*img1Ylen * sizeof(GByte));
buffTmp13 = (GByte*)CPLMalloc(img1Xlen*img1Ylen * sizeof(GByte));
poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, img1Xlen, img1Ylen, buffTmp11, img1Xlen, imgYlen, GDT_Byte, 0, 0);
poSrcDS->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, img1Xlen, img1Ylen, buffTmp12, img1Xlen, imgYlen, GDT_Byte, 0, 0);
poSrcDS->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, img1Xlen, img1Ylen, buffTmp13, img1Xlen, imgYlen, GDT_Byte, 0, 0);

//将满足条件的像素抠出
for (j = 0; j < imgYlen; j++)
{
	for (i = 0; i < imgXlen; i++)
	{
		if (!((buffTmp1[j*imgXlen + i] > 10 && buffTmp1[j*imgXlen + i] < 160)&& (buffTmp2[j*imgXlen + i] > 100 && buffTmp2[j*imgXlen + i] < 220)&& (buffTmp3[j*imgXlen + i] > 10 && buffTmp3[j*imgXlen + i] < 110)))
		{
			buffTmp11[j*imgXlen + i] = (GByte)buffTmp1[j*imgXlen + i];
			buffTmp12[j*imgXlen + i] = (GByte)buffTmp2[j*imgXlen + i];
			buffTmp13[j*imgXlen + i] = (GByte)buffTmp3[j*imgXlen + i];
		}
		
	}
}

//将数据写到输出中
poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, img1Xlen, img1Ylen, bandNum1, GDT_Byte, NULL);
poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, img1Xlen, img1Ylen, buffTmp11, img1Xlen, img1Ylen, GDT_Byte, 0, 0);
poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, img1Xlen, img1Ylen, buffTmp12, img1Xlen, img1Ylen, GDT_Byte, 0, 0);
poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, img1Xlen, img1Ylen, buffTmp13, img1Xlen, img1Ylen, GDT_Byte, 0, 0);

CPLFree(buffTmp1);
CPLFree(buffTmp2);
CPLFree(buffTmp3);
CPLFree(buffTmp11);
CPLFree(buffTmp12);
CPLFree(buffTmp13);
GDALClose(poDstDS);
GDALClose(poSrcDS);
system("PAUSE");
return 0;


```

`}
~~~





- # 实验结果截图：

![](C:\Users\dell\AppData\Roaming\Typora\typora-user-images\1540610663727.png)





- # 实验思路：

  将superman.jpg的三个通道的像素值分别读到三个缓冲区中，然后将space.jpg中的像素值也分别读出，找出superman中超人部分中的像素，覆盖即可。