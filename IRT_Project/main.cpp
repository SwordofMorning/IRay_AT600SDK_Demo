#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <USBSDK.h>

using namespace std;

// video使用，预留额外的5倍空间大小
unsigned short pBufShow[512 * 640 * 5];
// 镜头数据，用以转换为温度数据
unsigned short camera_data[2560 * 1024];
// 温度数据
float temp_data[1280 * 1024];
// 温度图像数据，对温度数据归一化，便于显示
float temp_data_img[640 * 512];

// 视频回调
void VCB(unsigned char* pBuffer, int width, int height, void* pContext)
{
	memcpy(pBufShow, pBuffer, width * height * sizeof(unsigned short));
}

// 温度回调
void TCB(unsigned char* pBuffer, int width, int height, void* pContext)
{
	memcpy(camera_data, pBuffer, width * height * sizeof(unsigned short));

	// 温度转换
	double fTempValue = 30.0;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			
			temp_data[j * width + i] = (camera_data[j * width + i] + 7000) / fTempValue - 273.2;
		}
	}

	// 最高低温度设置
	float minTemp = 10000;
	float maxTemp = 0;

	for (int i = 0; i < 640 * 512; i++) 
	{
		double iTemp = temp_data[i];

		if ((iTemp <= minTemp) && (iTemp != 0)) {
			minTemp = iTemp;
		}
		if (iTemp >= maxTemp) {
			maxTemp = iTemp;
		}
	}

	// 温度归一化
	for (int i = 0; i < 640 * 512; i++) 
	{

		double iTemp = temp_data[i];

		float value = (iTemp - minTemp) * 255 / (maxTemp - minTemp);
		// std::cout << value << std::endl;
		if (value >= 0) {
			temp_data_img[i] = value;
		}
		else {
			temp_data_img[i] = 0;
		}
	}
}

int main()
{
	// 创建接口
	auto handle = sdk_create();
	HWND hWnd { nullptr };

	sdk_loginDevice(handle, hWnd);

	// 打开设备
	int portIdx = 4;
	int iGetCurSel = 0;

	std::cout << "OpenDevice: " << OpenDevice(handle, iGetCurSel, portIdx) << std::endl;

	// 获取面阵宽高
	int width, height;
	sdk_get_width(handle, &width);
	sdk_get_height(handle, &height);
	// 设置回调函数
	SetVideoCallBack(handle, VCB, nullptr);
	SetTempCallBack(handle, TCB, nullptr);

	for (;;)
	{
		// 视频显示
		cv::Mat vidIn{ height, width, CV_8UC2, (unsigned char*)pBufShow };
		cv::Mat vidOut;
		cv::cvtColor(vidIn, vidOut, cv::COLOR_YUV2BGR_YUYV);

		// 温度显示
		cv::Mat tempImg{ height, width, CV_32FC1, temp_data_img };
		cv::Mat temp{ height, width, CV_32FC1, temp_data };

		// 合并图片
		//cv::Mat dst;

		// cv::hconcat(tempImg, vidOut, dst);

		cv::imwrite("output_tmpImg.png", tempImg);
		cv::imwrite("output_video.png", vidOut);

		cv::imshow("dst", vidOut);

		if (cv::waitKey(50) >= 0) break;
	}

	// 关闭设备
	CloseDevice(handle);

	return 0;
}