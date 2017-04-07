////////////////////////////////////////
// Bitmap.h
////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////

class Bitmap {
public:
	Bitmap(int x,int y)						{XRes=x; YRes=y; Pixel=new int[XRes*YRes];}
	~Bitmap()								{delete []Pixel;}

	bool SaveBMP(const char *filename);

	int GetXRes() const						{return XRes;}
	int GetYRes() const						{return YRes;}
	int &GetPixel(int x,int y)				{return Pixel[y*XRes+x];}
	void SetPixel(int x,int y,int pix)		{Pixel[y*XRes+x]=pix;}
    void Resize(int x, int y)				{XRes=x; YRes=y; delete[]Pixel; Pixel =new int[XRes*YRes];}

private:
	int XRes;
	int YRes;
	int *Pixel;
};

////////////////////////////////////////////////////////////////////////////////
