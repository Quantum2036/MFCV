#pragma once
class dcMap
{
//���� & ����
public:
	dcMap();
	dcMap(Mat image);
	~dcMap();

//����
private:
	COLORREF** data;
	uint nWidth;
	uint nHeight;
	int flag;

//����
public:
	void Draw(CDC* pDC);
private:
	void InitMap(void);
	COLORREF getData(uint i, uint j);
};

