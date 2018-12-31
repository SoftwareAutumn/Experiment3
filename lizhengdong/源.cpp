#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int spaceAndSuperman()
{
	//太空输入图像
	GDALDataset* poSrcDSSpace;
	//超人输入图像
	GDALDataset* poSrcDSSuperman;
	//输出图像，超人和太空
	GDALDataset* poDstDS;
	//太空图像的宽度和高度
	int imgXlen, imgYlen;
	//超人输入图像路径
	char* srcPathSuperman = "superman.jpg";
	//太空输入图像路径
	char* srcPathSpace = "space.jpg";
	//输出图像路径
	char* dstPath = "supermanAndSpace.tif";

	GByte* buffTmp1;
	GByte* buffTmp2; 
	GByte* buffTmp3;

	//符合要求的部分，即抠出的超人图像
	GByte* buffSuperman1;
	GByte* buffSuperman2;
	GByte* buffSuperman3;

	//太空图像
	GByte* buffSpace1;
	GByte* buffSpace2;
	GByte* buffSpace3;

	//图像波段数
	int bandNum;

	//注册驱动
	GDALAllRegister();

	//打开超人图像,太空图像
	poSrcDSSuperman = (GDALDataset*)GDALOpenShared(srcPathSuperman, GA_ReadOnly);
	poSrcDSSpace = (GDALDataset*)GDALOpenShared(srcPathSpace, GA_ReadOnly);
	
	//获取超人图像宽度，高度和波段数量
	imgXlen = poSrcDSSuperman->GetRasterXSize();
	imgYlen = poSrcDSSuperman->GetRasterYSize();
	bandNum = poSrcDSSuperman->GetRasterCount();
	
	//根据图像的高度和宽度分配内存
	buffTmp1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	buffSuperman1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSuperman2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSuperman3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	buffSpace1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSpace2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSpace3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	//逐通道将图像数据传入buffTmp1/2/3，buffSpace1/2/3中,遍历图像，找出符合要求的部分，复制到buffSuperman中
	poSrcDSSuperman->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDSSuperman->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDSSuperman->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp3, imgXlen, imgYlen, GDT_Byte, 0, 0);

	poSrcDSSpace->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffSpace1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDSSpace->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffSpace2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDSSpace->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffSpace3, imgXlen, imgYlen, GDT_Byte, 0, 0);

	for (int j = 0; j < imgYlen; j++)
	{
		for (int i = 0; i < imgXlen; i++)
		{
			if (((GByte)10 < buffTmp1[j*imgXlen + i] && buffTmp1[j*imgXlen + i] < (GByte)160) && ((GByte)100 < buffTmp2[j*imgXlen + i] && buffTmp2[j*imgXlen + i] < (GByte)220) && ((GByte)10 < buffTmp3[j*imgXlen + i] && buffTmp3[j*imgXlen + i] < (GByte)110))
			{
				buffSuperman1[j*imgXlen + i] = buffSpace1[j*imgXlen + i];
				buffSuperman2[j*imgXlen + i] = buffSpace2[j*imgXlen + i];
				buffSuperman3[j*imgXlen + i] = buffSpace3[j*imgXlen + i];
			}
			else
			{
				buffSuperman1[j*imgXlen + i] = buffTmp1[j*imgXlen + i];
				buffSuperman2[j*imgXlen + i] = buffTmp2[j*imgXlen + i];
				buffSuperman3[j*imgXlen + i] = buffTmp3[j*imgXlen + i];
			}
		}
	}

	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//尝试输出超人和宇宙
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffSuperman1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffSuperman2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffSuperman3, imgXlen, imgYlen, GDT_Byte, 0, 0);
	
	//清除内存
	CPLFree(buffTmp1);
	CPLFree(buffTmp2);
	CPLFree(buffTmp3);
	CPLFree(buffSuperman1);
	CPLFree(buffSuperman2);
	CPLFree(buffSuperman3);
	CPLFree(buffSpace1);
	CPLFree(buffSpace3);
	CPLFree(buffSpace2);

	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDSSuperman);
	GDALClose(poSrcDSSpace);

	system("PAUSE");
	return 0;
}
int main()
{
	spaceAndSuperman();
	return 0;
}