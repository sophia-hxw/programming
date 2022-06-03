clear
clc

img1=imread('ques1.PNG');
img2=imread('ques2.PNG');
img3=imread('ques3.PNG');
img4=imread('ques4.PNG');

Dimg1=double(img1);
Dimg2=double(img2);
Dimg3=double(img3);
Dimg4=double(img4);

img=round((Dimg1+Dimg2+Dimg3+Dimg4)./4);

img4See=uint8(img);

img1Def=img1-img4See;

img2Def=rgb2gray(img1Def);
[m,n,k]=size(img2Def);

imgMeans=double(img2Def);

ptSets=k_means(imgMeans);

figure(1);
imshow(img2Def);

figure(2);
setN=size(ptSets,1);
matRes=zeros(m,n);

allpts=ptSets{1};
set1Num=length(allpts);
ys=floor(allpts/m)+1;
xs=mod(allpts,m)+1;
for ib=1:length(xs)
    matRes(xs(ib),ys(ib))=255;
end
imshow(matRes);





