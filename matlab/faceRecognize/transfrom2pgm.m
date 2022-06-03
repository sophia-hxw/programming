clc
clear

img=imread('img_0001.jpg');
subplot(121);
imshow(img);
title('original');

imgGray=rgb2gray(img);
subplot(122);
imshow(imgGray);
title('gray');

imgGSize=size(imgGray);
maxVal=max(max(imgGray));

fid=fopen('data.pgm','wt');
fprintf(fid,'%s\n','P5');
fprintf(fid,'%d\t%d\n',imgGSize(1),imgGSize(2));
fprintf(fid,'%d\n',maxVal);
fclose(fid);

fids=fopen('data.pgm','a');
for ia=1:imgGSize(1)
    for ib=1:imgGSize(2)
        binVal=dec2bin(imgGray(ia,ib));
        fwrite(fids,binVal);
    end
end
 fclose(fids); 




