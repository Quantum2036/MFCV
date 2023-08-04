#pragma once
class dcMap
{
//构造 & 析构
public:
	dcMap();
	dcMap(Mat image);
	~dcMap();

//数据
private:
	COLORREF** data;
	uint nWidth;
	uint nHeight;
	int flag;

//方法
public:
	void Draw(CDC* pDC);
private:
	void InitMap(void);
	COLORREF getData(uint i, uint j);
};

