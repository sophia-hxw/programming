clear;
clc;

%图片文件夹名称
folderName = 'rawdata';
%标注文件名称
gt_filename = strcat(folderName, '_annotation.txt');
%根目录，图片和标注文件的上层目录
rootFolder = 'F:/helmet/touku 1/touku 1/';
%给出新建文件夹drawGT的路径
drawGTFolder =strcat(rootFolder,folderName,'/drawGT/');
gtRectFolder =strcat( rootFolder,folderName,'/gtRects/');
gtArRectFolder = strcat(rootFolder,folderName,'/gtArRects/');
gtBgFolder = strcat(rootFolder,folderName,'/gtBgRects/');
%带了完整目录和名称的标注文件
gt_filename = strcat(rootFolder,folderName, '/', gt_filename);

%输出带完整目录和名称的标注文件以供查看！！！
disp(gt_filename);

%如果不存在文件夹就新建
if(~exist(drawGTFolder))
	mkdir(drawGTFolder);
end

if(~exist(gtRectFolder))
	mkdir(gtRectFolder);
end

if(~exist(gtArRectFolder))
	mkdir(gtArRectFolder);
end

if(~exist(gtBgFolder))
	mkdir(gtBgFolder);
end

%待处理图片个数？
obj_num = 0;

%打开标注文件
fid = fopen(gt_filename);

%获取标注文件一行
str_gt = fgetl(fid);

%?
scaleNum = 5;

%?
posRndNum = 5;


while ischar(str_gt)
	% disp(tline)
	
	close all;
	figure
	
	%定位空格，取出空格前带相对路径的每个图片文件名称
	ingtmp = findstr(str_gt,' ');
	filename_gt = str_gt(1:ingtmp(1)-1);
	
	%定位文件后缀前的.和文件名称前的/用来取出不带后缀和路径的图片文件名称
	ingtmp2 = findstr(filename_gt, '.');
	ingtmp3 = findstr(filename_gt, '/');
	imgPrefix = filename_gt(ingtmp3+1:ingtmp2(1)-1);
	
	%视频文件名称
	videoName = folderName;
	k = strfind(filename_gt, '/');
	
	%带后缀而不带路径的图片文件名称
	[mm,nn] = size(filename_gt);
	imgShortName = filename_gt(max(k)+1: nn);
	
	%带绝对路径和后缀的图片文件，为读图片准备；
	rawImageName = strcat(rootFolder, folderName, '/', imgShortName)
	disp(rawImageName)
	
	%读图
	img = imread(rawImageName);
	
	%imagesc(img)将矩阵img中的元素数值按大小转化为不同颜色，并在坐标轴对应位置处以这种颜色染色
	imagesc(img);
	imshow(img, 'InitialMagnification',100);
	
	%取出图像行，列，通道数
	[rows, cols, chn] = size(img);	
	
	%取出给定的ROI个数，并由string转化为int
	numbbx_gt = str2num(str_gt(ingtmp(1)+1:ingtmp(2)-1));
	
	%取出rect形状的ROI的参数，由string转化为int
	bbs = str2num(str_gt(ingtmp(2)+1:end));
	
	%调整行和列，每一列是一个ROI
	bbs_gt = reshape(bbs,4,numbbx_gt)';
	
	%负样本数，最大和最小的负样本大小
	nNegPImg = 100;
	maxNegSize = 200;
	minNegSize = 20;
	
	%？
	mincentreStdThres = 50;
	
	for i0 = 1:1:nNegPImg
		%随机生成负样本的Rect
		negSize = rand(1,1)*(maxNegSize - minNegSize) +minNegSize;
		negX = rand(1,1)*(cols-negSize);
		negY = rand(1,1)*(rows-negSize);
		negRect =uint16( [negX, negY, negSize, negSize]);
		
		%一系列随机Rect的中心点
		negCx= (negX+0.5*negSize);
		negCy= (negY+0.5*negSize);
		
		max_overlap=0;
		min_centreStd = 1000;
		
		for igt = 1:numbbx_gt
			area_neg = negRect(3)*negRect(4);
			area_gt = bbs_gt(igt,3)*bbs_gt(igt,4);
			overlap = rectint(negRect,bbs_gt(igt,:));
			% overlaprat = overlap/(area_neg+area_gt-overlap);
			% overlaprat = overlap/min(area_neg, area_gt);
			overlaprat = overlap/ area_gt;
			if overlaprat>max_overlap
				max_overlap = overlaprat;
			end
			
			gtbbxCx = (bbs_gt(igt,1)+0.5*bbs_gt(igt,3));
			gtbbxCy = (bbs_gt(igt,2)+0.5*bbs_gt(igt,4));
			
			centreStd = sqrt((gtbbxCx - negCx)^2+(gtbbxCy - negCy)^2);
			
			if centreStd<min_centreStd
				min_centreStd = centreStd;
			end
			
		end
		
		if (max_overlap <0.5 && min_centreStd > mincentreStdThres)
			negImg = imcrop(img, negRect);
			negImgName = strcat(gtBgFolder, videoName, '_neg_' , imgPrefix,  '_', sprintf('%04d', i),sprintf('%03d', i0), '.jpg');
			
			rectangle('position',negRect, 'edgecolor','b', 'Curvature',[0 0], 'LineWidth',1, 'LineStyle', '-');
			hold on
			imwrite(negImg, negImgName);
		end
		
		
	end
	
	
	for i = 1:1:numbbx_gt
		
		tmpbbs = bbs_gt(i, :);
		% enlarge gt bbx
		scale = 1.4;
%             x = tmpbbs(1);
%             y = tmpbbs(2);
		w = tmpbbs(3);
		h = tmpbbs(4);
		cx = tmpbbs(1)+round(0.5*w);
		cy = tmpbbs(2)+round(0.5*h);
		bigX = max(cx-scale*round(0.5*w), 0);
		bigY = max(cy-scale*round(0.5*h), 0);      
		bigW = min(scale*w, 2*(cols-cx));
		bigH = min(scale*h, 2*(rows-cy));
		tmpbbs2 = [bigX,bigY,bigW, bigH];  
		
		gtRectName = strcat(gtRectFolder,videoName, '_' , imgPrefix,  '_', sprintf('%04d', i),'_000', '.jpg');
		disp(gtRectName)
		imgRect = imcrop(img, tmpbbs2);
		imwrite(imgRect, gtRectName);
		
		x = tmpbbs(1);
		y = tmpbbs(2);
		w = tmpbbs(3);
		h = tmpbbs(4);
		cx = tmpbbs(1)+round(0.5*w);
		cy = tmpbbs(2)+round(0.5*h);
		
		orgscale = 1.15;
		
		
		rndImgIndx = 0;
		
		for j = 1:1:scaleNum
			
			scale = orgscale^j;
			scale2 = orgscale^(j-1);
			bigX = max(cx-scale*round(0.5*w), 0);
			bigY = max(cy-scale*round(0.5*h), 0);
			
			bigW = min(scale*w, 2*(cols-cx));
			bigH = min(scale*h, 2*(rows-cy));
			
			
			for i1 = 1:1:posRndNum
				
				rndRecW = w*scale2;
				rndRecH = h*scale2;
				
				rndX = round(rand(1,1)*((scale-scale2)*rndRecW) +bigX);
				rndY = round(rand(1,1)*((scale-scale2)*rndRecH) +bigY);
				
				
				rndRect = [rndX, rndY, rndRecW, rndRecH];
				rndImgIndx = rndImgIndx+1 ;
				rndImgRect = imcrop(img, rndRect);
				gtArRectName = strcat(gtArRectFolder,videoName, '_' , imgPrefix,  '_', sprintf('%04d', i),'_', sprintf('%03d', rndImgIndx), '.jpg');
				imwrite(rndImgRect, gtArRectName);
				rectangle('position',rndRect, 'edgecolor','y', 'Curvature',[0 0], 'LineWidth',1, 'LineStyle', '-');
			end
		end
		
		
		rectangle('position',tmpbbs, 'edgecolor','g', 'Curvature',[0 0], 'LineWidth',1, 'LineStyle', '-');
		hold on
	end
	
	
	
	
	f=getframe(gca);
	[img, map] = frame2im(f);
	
	
	
	resultName = strcat(drawGTFolder, imgShortName);
	
	imwrite(img,resultName,'jpeg')
	
	str_gt = fgetl(fid);
end
fclose(fid);


%%get precision and recall
fid_gt=fopen(gt_filename);
str_gt=fgetl(fid_gt);
while ischar(str_gt)
	indtmp = findstr(str_gt,' ');
	numbbx_gt = str2num(str_gt(indtmp(1)+1:indtmp(2)-1));
	obj_num = obj_num+numbbx_gt;
	str_gt=fgetl(fid_gt);
end
fclose(fid_gt);
    

