#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <USBSDK.h>

using namespace std;

// video使用，预留额外的5倍空间大小
unsigned short pBufferShow[512 * 640 * 5];
// 镜头数据，用以转换为温度数据
unsigned short camera_data[2560 * 1024];
// 温度数据
float temp_data[1280 * 1024];
// 温度图像数据，对温度数据归一化，便于显示
float temp_data_img[640 * 512];

// 视频回调 UYVY
void VCB(unsigned char* pBuffer, int width, int height, void* pContext)
{
	memcpy(pBufferShow, pBuffer, width * height * sizeof(unsigned short));
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

// 保存视频流
void saveVideoStream(int height, int width)
{
	std::ofstream Luma, Chroma;
	Luma.open("./videoLuma.txt");
	Chroma.open("./videoChroma.txt");

	// pBufferShow现在为short类型，高八位储存Y，第八位储存UV
	unsigned char* ptr = (unsigned char*)pBufferShow;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Chroma << int(ptr[(j * height + width) * 2]) << ' ';
			Luma << int(ptr[(j * height + width) * 2 + 1]) << ' ';
		}
		Luma << "\n";
		Chroma << "\n";
	}
}

// 遍历保存Mat
void saveMat(cv::Mat m)
{
	std::ofstream ch0, ch1;
	ch0.open("./videoMatCh0.txt");
	ch1.open("./videoMatCh1.txt");

	for (int i = 0; i < m.rows; ++i)
	{
		for (int j = 0; j < m.cols; ++j)
		{
			ch0 << int(m.at<cv::Vec2b>(i, j)[0]) << ' ';
			ch1 << int(m.at<cv::Vec2b>(i, j)[1]) << ' ';
		}
		ch1 << '\n';
		ch0 << '\n';
	}
}

void client()
{
	// 创建接口
	auto handle = sdk_create();
	HWND hWnd{ nullptr };

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
		cv::Mat vidIn{ height, width, CV_8UC2, (unsigned char*)pBufferShow };
		cv::Mat vidOut;
		cv::cvtColor(vidIn, vidOut, cv::COLOR_YUV2BGR_YUYV);

		/* ===== Debug Output ===== */
		std::cout << width << ' ' << height << std::endl;
		std::cout << "vidInChannels: " << vidIn.channels() << std::endl;
		std::cout << "vidOutChannels: " << vidOut.channels() << std::endl;
		/* ===== Debug Output ===== */

		// 温度显示
		cv::Mat tempImg{ height, width, CV_32FC1, temp_data_img };
		cv::Mat temp{ height, width, CV_32FC1, temp_data };

		// 保存数据
		// cv::imwrite("output_tmpImg.png", tempImg);
		cv::imwrite("RGB.png", vidOut);
		saveVideoStream(height, width);
		saveMat(vidIn);

		std::cout << "save finished!" << std::endl;


		// 显示图片
		cv::imshow("RGB", vidOut);
		cv::waitKey(0);
	}

	// 关闭设备
	CloseDevice(handle);
}

int main()
{
	client();

	return 0;
}