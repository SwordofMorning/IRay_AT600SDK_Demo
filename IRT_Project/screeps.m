%% 此文件用于分videoStream

clear;clc;

vs = load ('videoStream.txt');

vs = uint8(vs);

imshow(vs);

%% ===== 查看最大值最小值 =====
MaxVal = max(max(vs));
MinVal = min(min(vs));