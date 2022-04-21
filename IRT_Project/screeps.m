%% 此文件用于分videoStream

clear;clc;

Luma_1 = load ('videoLuma.txt');
Luma_2 = load ('videoLuma_2.txt');
Chroma = load("videoChroma.txt");
Vm = load("videoMatCh1.txt");

Luma_1 = uint8(Luma_1);
Luma_2 = uint8(Luma_2);
Chroma = uint8(Chroma);
Vm = uint8(Vm);

figure(1);
imshow(Luma_1);
figure(2);
imshow(Luma_2);
figure(3);
imshow(Vm);