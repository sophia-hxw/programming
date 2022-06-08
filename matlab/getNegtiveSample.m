clear;
clc;

folderName = 'rawdata';
gt_filename = strcat(folderName, '_annotation.txt');
rootFolder = 'F:/traincascade/';
drawNegFolder =strcat(rootFolder,folderName,'/negSample/');
gt_filename = strcat(rootFolder, gt_filename);

if(~exist(drawNegFolder))
        mkdir(drawNegFolder);
end

obj_num = 0;

fid = fopen(gt_filename);

str_gt = fgetl(fid);

while ischar(str_gt)
	close all;
    figure
	
	ingtmp2 = strfind(str_gt, '.');
	imgPrefix = str_gt(1:ingtmp2(1)-1);
	
	rawImageName = strcat(rootFolder, folderName, '/', str_gt);
	disp(rawImageName);
	img = imread(rawImageName);
	imagesc(img);
	imshow(img, 'InitialMagnification',100);
	
	[rows, cols, chn] = size(img);
	
	nNegPImg = 500;
	maxNegSize = 200;
	minNegSize = 55;
	
	for i0 = 1:1:nNegPImg
		
		negSize = rand(1,1)*(maxNegSize - minNegSize) +minNegSize-1;
		negX = rand(1,1)*(cols-negSize);
		negY = rand(1,1)*(rows-negSize);
		negRect =uint16( [negX, negY, negSize, negSize]);
		
		negImg = imcrop(img, negRect);
		negImgName = strcat(drawNegFolder, imgPrefix, sprintf('%03d', i0),'.jpg');
			
		rectangle('position',negRect, 'edgecolor','b', 'Curvature',[0 0], 'LineWidth',1, 'LineStyle', '-');
		hold on
		imwrite(negImg, negImgName);
    end
    str_gt = fgetl(fid);
end

fclose(fid);