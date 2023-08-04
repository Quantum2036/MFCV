#include "pch.h"
#include "dcMap.h"

dcMap::dcMap()
{
	InitMap();
}

dcMap::dcMap(Mat image)
{
	if (image.empty()) {
		InitMap();
		return;
	}

	flag = image.flags;
	nWidth = image.cols;
	nHeight = image.rows;
	
	data = (COLORREF**)malloc(sizeof(COLORREF*) * nWidth);
	if (!data) {
		return;
	}

	COLORREF** pd = data;
	for (uint i = 0; i < nWidth; i++, pd++) {
		*pd = (COLORREF*)malloc(sizeof(COLORREF) * nHeight);
		if (!*pd) {
			return;
		}
	}

	int channel = image.channels();
	if (channel == 1) {

		COLORREF** pd = data;
		COLORREF* pt = NULL;
		for (uint i = 0; i < nWidth; i++, pd++) {
			pt = *pd;
			for (uint j = 0; j < nHeight; j++, pt++) {
				uchar it = image.at<uchar>(i, j);
				*pt = RGB(it, it, it);
			}
		}
	}
	else if (channel == 3) {

		COLORREF** pd = data;
		COLORREF* pt = NULL;
		for (uint i = 0; i < nWidth; i++, pd++) {
			pt = *pd;
			for (uint j = 0; j < nHeight; j++, pt++) {
				Vec3b it = image.at<Vec3b>(i, j);
				*pt = RGB(it[2], it[1], it[0]);
			}
		}
	}
	
	FILE* fp = NULL;
	fopen_s(&fp, "C:/Users/29462/Desktop/dcMat.dat", "w+");
	if (!fp)
		return;

	fprintf_s(fp, "%u %u\n", nWidth, nHeight);
	for (uint i = 0; i < nWidth; i++) {
		for (uint j = 0; j < nHeight; j++) {
			fprintf_s(fp, "%lu ", getData(i,j));
		}
		fprintf_s(fp, "\n");
	}


}

dcMap::~dcMap()
{
	flag = 0;
	nWidth = 0;
	nHeight = 0;
	
	for (uint i = 0; i < nWidth; i++) {
		free(*data++);
	}
}

void dcMap::InitMap(void)
{
	flag = 0;
	nWidth = 0;
	nHeight = 0;
	data = NULL;
}

COLORREF dcMap::getData(uint nX, uint nY)
{
	if (data == NULL || nX > nWidth || nY > nHeight) {
		return COLORREF();
	}

	COLORREF** pd = data;
	COLORREF* pt = *(pd + nX);
	pt += nY;
	return *pt;
}

void dcMap::Draw(CDC* pDC)
{
	for (uint i = 0; i < nWidth; i++) {
		for (uint j = 0; j < nHeight; j++) {
			pDC->SetPixelV(j, i, getData(i, j));
		}
	}
}
