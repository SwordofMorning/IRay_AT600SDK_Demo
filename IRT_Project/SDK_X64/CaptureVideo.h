#ifndef __CAPTUREVIDEO_H__
#define __CAPTUREVIDEO_H__

#include <vector>
using namespace std;

#include "common.h"

class CaptureVideo
{
public:
	CaptureVideo(ISampleGrabberCB *p);
	~CaptureVideo();
	HRESULT InitializeEnv(HWND hWnd); //initialize environment
	//HRESULT EnumAllDevices(vector<char*> &p);//(HWND hCombox);  //Enumeration all devices
	HRESULT EnumAllDevices(vector<string> &p);//(HWND hCombox);  //Enumeration all devices
	void CloseInterface(); //close all interface
	HRESULT OpenDevice(int deviceID, LONG nLeft, LONG nTop, LONG nWide, LONG nHeight);
	HRESULT BindFilter(int deviceID, IBaseFilter **pBaseFilter);
	HRESULT SetupVideoWindow(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
	void ResizeVideoWindow(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
	void GrabOneFrame(BOOL bGrab); // use ISampleGrabberCallbackCB get a picture
	HRESULT HandleGraphCapturePicture(void); //capture picture  use GetCurrentBuffer

private:
	
	ICaptureGraphBuilder2 *m_pCaptureGB;
	IMediaControl *m_pMediaControl;
	IGraphBuilder *m_pGraphBuilder;
	ISampleGrabber *m_pSampGrabber;  
	IMediaEventEx *m_pMediaEvent;
	
public:
	int m_nCaptureDeviceNumber;      //Device Count
	//vector<string> m_vecCapDeviceName; //the Device name
	TCHAR m_pCapDeviceName[10][MAX_PATH]; //the Device name
	BOOL m_bGetOneShot;
	BOOL m_bConnect;
	IVideoWindow *m_pVideoWindow;
	IAMCameraControl *m_pCameraControl;
	IBaseFilter *m_pDevFilter;
	BOOL CameraControlInterface(unsigned short value);
	//SampleGrabberCallback g_sampleGrabberCB;  //CallBack

	ISampleGrabberCB *g_sampleGrabberCB = NULL;
	
	
};


#endif  //__CAPTUREVIDEO_H__