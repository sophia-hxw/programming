%-----Info
%Author     sophia
%Blog       http://blog.csdn.net/sophia_xw?viewmode=contents
%Date       2017/4/6

function ptSets=k_means(matD)

cptNum=2;
[m,n]=size(matD);

matNew=reshape(matD,m*n,1);%with column, size(matNew)=[m*n,1]
[Val1,index1]=min(matNew(:));
[Val2,index2]=max(matNew(:));
cpt=[index1, index2];


% specialV=(Val2-Val1)^2;
% disCPT1=repmat(Val2,m*n,1)-matNew;
% disCPT2=matNew-repmat(Val1,m*n,1);
% disCPT=disCPT1.*disCPT1+disCPT2.*disCPT2;
% disCPT(disCPT==specialV)=0;
% [Val0,index0]=max(disCPT(:));
% cpt=[cpt, index0];


ptSets=cell(cptNum,1);
dis=abs(repmat(matNew,1,cptNum)-repmat(matNew(cpt)',m*n,1));
disC=dis';
[minDis, minIndex]=min(disC);
indNew=minIndex';
orNumber=1:m*n;
for ia=1:cptNum
    indNewC=(indNew==ia);
    oneClass=indNewC.*orNumber';
    oneClass(oneClass==0)=[];
    ptSets{ia}=oneClass;
end

