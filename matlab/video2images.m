clear;
clc;

 % choose a video file
[filename, pathname] = uigetfile('*.avi', 'E:/work/video2image/', 'ACC-Export-2016-07-07-06.09.16-PM.avi', 'Multiselect', 'on');

fprintf('filename = %s \npathname = %s \n\n', filename, pathname);

 if ischar(filename)
    
    fprintf('choose file success.\n\n');
    
    video = VideoReader([pathname filename]);
    len = video.NumberOfFrames; % get length of the video
    
    fprintf('length of video : %d \n\n', len);
    
    dir=strcat(pathname,strrep(filename,'.avi',''),'\pic');
    mkdir(dir); % create folder for saving picture
    fn=strrep(filename,'.avi',''); 
    
   % len = 3000;
    for k = 1 :1: len - 1
        %frame = rgb2gray(read(video, k));
        frame = read(video, k);
       % imwrite(frame,strcat(dir,'\',fn,'-mp4-000',int2str(k),'.tif'),'tif'); 
       % imwrite(frame,strcat(dir,'\',fn,'-mp4-000',int2str(k),'.jpg'),'jpg'); 
       %if mod(k,10)==0
        imwrite(frame,strcat(dir,'\',sprintf('%06d',k) ,'.jpg'),'jpg'); 
       %end
    end
    
end