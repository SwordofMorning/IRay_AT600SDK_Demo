#ifndef __SAMPLEGRABBERCALLBACK_H__
#define __SAMPLEGRABBERCALLBACK_H__
//#include <opencv2/core/core.hpp>  
//#include <opencv2/highgui/highgui.hpp>  
//#include <opencv2/opencv.hpp>
//#include <opencv2\imgproc\types_c.h>
//
//using namespace cv;
//extern IplImage *pImg;

#include "common.h"

class SampleGrabberCallback : public ISampleGrabberCB
{
public:
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void** ppvObject);

	HRESULT STDMETHODCALLTYPE SampleCB(double Time, IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE BufferCB(double Time, BYTE *pBuffer, long BufferLen);
	
	SampleGrabberCallback();
	BOOL SaveBitmap(BYTE * pBuffer, long lBufferSize ); //����bitmapͼƬ


public:
	BOOL m_bGetPicture;  // is get a picture
	long m_lWidth;       //�洢ͼƬ�Ŀ��
	long m_lHeight;		 //�洢ͼƬ�ĳ���
	int  m_iBitCount;    //the number of bits per pixel (bpp)
	TCHAR m_chTempPath[MAX_PATH];
	TCHAR m_chSwapStr[MAX_PATH];
	TCHAR m_chDirName[MAX_PATH];
};

#endif //__SAMPLEGRABBERCALLBACK_H__