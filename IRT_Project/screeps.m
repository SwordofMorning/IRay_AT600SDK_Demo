%% 此文件用于分videoStream

clear;clc;

vs = load ('videoMat.txt');

MaxVal = max(max(vs));
MinVal = min(min(vs));

%% ===== 查看最大值最小值 =====
vs = uint8(vs);

imshow(vs);

%% ===== 