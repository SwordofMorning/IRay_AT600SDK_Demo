%% ���ļ����ڷ�videoStream

clear;clc;

vs = load ('videoStream.txt');

vs = uint8(vs);

imshow(vs);

%% ===== �鿴���ֵ��Сֵ =====
MaxVal = max(max(vs));
MinVal = min(min(vs));