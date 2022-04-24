%% 此文件用于分videoStream

clear;clc;

Luma = load ('videoLuma.txt');
Vm = load("videoMatCh1.txt");
Target = zeros([512, 640]);

Luma = uint8(Luma);
Vm = uint8(Vm);

figure(1);
imshow(Luma);
figure(2);
imshow(Vm);

%%  检查二者是否相等

Left = Luma(:, 1:320);
Right = Luma(:, 321:640);

if (Left == Right)
    disp("yes");
else
    disp("no");
end

Minus = Left - Right;

%% 左右图片合并

for i = 1 : 320
    Target(:, i * 2 - 1) = Left(:, i);
    Target(:, i * 2) = Right(:, i);
end

if (Target == Vm)
    disp("yes");
else
    disp("no");
end

figure(3);
imshow(Target);