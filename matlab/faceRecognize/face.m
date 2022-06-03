clc,clear  
npersons=40;%选取40个人的脸  
global imgrow;  
global imgcol;  
imgrow=112;  
imgcol=92;  
  
disp('读取训练数据...')  
f_matrix=ReadFace(npersons,0);%读取训练数据  
nfaces=size(f_matrix,1);%样本人脸的数量  
disp('.................................................')  
%低维空间的图像是(nperson*5)*k的矩阵，每行代表一个主成分脸，每个脸20维特征  
  
disp('训练数据PCA特征提取...')  
mA=mean(f_matrix);  
k=50;%降维至20维  
[pcaface,V]=fastPCA(f_matrix,k,mA);%主成分分析法特征提取  
disp('.................................................')  
  
disp('显示主成分脸...')  
visualize(V)%显示主分量脸  
disp('.................................................')  
  
disp('训练特征数据规范化...')  
disp('.................................................')  
lowvec=min(pcaface);  
upvec=max(pcaface);  
scaledface = scaling( pcaface,lowvec,upvec);  
  
disp('SVM样本训练...')  
disp('.................................................')  
gamma=0.0078;  
c=128;  
multiSVMstruct=multiSVMtrain( scaledface,npersons,gamma,c);  
  
disp('读取测试数据...')  
disp('.................................................')  
[testface,realclass]=ReadFace(npersons,1);  
  
disp('测试数据特征降维...')  
disp('.................................................')  
m=size(testface,1);  
for i=1:m  
    testface(i,:)=testface(i,:)-mA;  
end    
pcatestface=testface*V;  
  
disp('测试特征数据规范化...')  
disp('.................................................')  
scaledtestface = scaling( pcatestface,lowvec,upvec);  
  
disp('SVM样本分类...')  
disp('.................................................')  
class= multiSVM(scaledtestface,multiSVMstruct,npersons);  
  
accuracy=sum(class==realclass)/length(class);  
display(['正确率：',num2str(accuracy)])  