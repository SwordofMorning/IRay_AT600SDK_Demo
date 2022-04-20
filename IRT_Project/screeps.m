%% 此文件用于分videoStream

clear;clc;

Luma = load ('videoLuma.txt');
Chroma = load("videoChroma.txt");
Vm = load("videoMat.txt");

Luma = uint8(Luma);
Chroma = uint8(Chroma);
Vm = uint8(Vm);

figure(1);
imshow(Luma);
figure(2);
imshow(Chroma);
figure(3);
imshow(Vm);