#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int spaceAndSuperman()
{
	//̫������ͼ��
	GDALDataset* poSrcDSSpace;
	//��������ͼ��
	GDALDataset* poSrcDSSuperman;
	//���ͼ�񣬳��˺�̫��
	GDALDataset* poDstDS;
	//̫��ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;
	//��������ͼ��·��
	char* srcPathSuperman = "superman.jpg";
	//̫������ͼ��·��
	char* srcPathSpace = "space.jpg";
	//���ͼ��·��
	char* dstPath = "supermanAndSpace.tif";

	GByte* buffTmp1;
	GByte* buffTmp2; 
	GByte* buffTmp3;

	//����Ҫ��Ĳ��֣����ٳ��ĳ���ͼ��
	GByte* buffSuperman1;
	GByte* buffSuperman2;
	GByte* buffSuperman3;

	//̫��ͼ��
	GByte* buffSpace1;
	GByte* buffSpace2;
	GByte* buffSpace3;

	//ͼ�񲨶���
	int bandNum;

	//ע������
	GDALAllRegister();

	//�򿪳���ͼ��,̫��ͼ��
	poSrcDSSuperman = (GDALDataset*)GDALOpenShared(srcPathSuperman, GA_ReadOnly);
	poSrcDSSpace = (GDALDataset*)GDALOpenShared(srcPathSpace, GA_ReadOnly);
	
	//��ȡ����ͼ���ȣ��߶ȺͲ�������
	imgXlen = poSrcDSSuperman->GetRasterXSize();
	imgYlen = poSrcDSSuperman->GetRasterYSize();
	bandNum = poSrcDSSuperman->GetRasterCount();
	
	//����ͼ��ĸ߶ȺͿ�ȷ����ڴ�
	buffTmp1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	buffSuperman1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSuperman2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSuperman3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	buffSpace1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSpace2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffSpace3 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	//��ͨ����ͼ�����ݴ���buffTmp1/2/3��buffSpace1/2/3��,����ͼ���ҳ�����Ҫ��Ĳ��֣����Ƶ�buffSuperman��
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

	//�������ͼ��
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//����������˺�����
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffSuperman1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffSuperman2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffSuperman3, imgXlen, imgYlen, GDT_Byte, 0, 0);
	
	//����ڴ�
	CPLFree(buffTmp1);
	CPLFree(buffTmp2);
	CPLFree(buffTmp3);
	CPLFree(buffSuperman1);
	CPLFree(buffSuperman2);
	CPLFree(buffSuperman3);
	CPLFree(buffSpace1);
	CPLFree(buffSpace3);
	CPLFree(buffSpace2);

	//�ر�dataset
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