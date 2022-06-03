clear;
clc;

Nimg=30;
img1Size=100;
imgSize=100*100;
matImg=zeros(Nimg,imgSize);

for ia=1:Nimg
    mathNum=num2str(ia,'%04d');
    filePath='.\IDcards\img_';
    fileLast='.jpg';
    fileNameC=strcat(filePath,mathNum);
    fileName=strcat(fileNameC,fileLast);
    f1=imread(fileName); 
    f1gray=rgb2gray(f1);
    f1size=imresize(f1gray,[img1Size,img1Size]);
    f1oneVec=reshape(f1size,1,imgSize);
    matImg(ia,:)=f1oneVec;
end

meanVal=mean(matImg);
% meanImg=uint8(reshape(meanVal,250,250));
% figure(1)
% imshow(meanImg);

matImg=matImg-ones(Nimg,1)*meanVal;

cmat=cov(matImg);

[V,D]=eig(cmat);
diag(-D)
[dummy,order]=sort(diag(-D));
dummy
V=V(:,order);
