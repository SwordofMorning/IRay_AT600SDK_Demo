%% ���ļ����ڷ�videoStream

clear;clc;

vs = load ('videoMat.txt');

MaxVal = max(max(vs));
MinVal = min(min(vs));

%% ===== �鿴���ֵ��Сֵ =====
vs = uint8(vs);

imshow(vs);

%% ===== 