function cpset = chooseClass( dis )
%CHOOSECENTER �˴���ʾ�йش˺�����ժҪ
%-----Input:
%dis                distance matrix, dis=(i,j), i stands for the point number and 
%                   j is the number of center point.
%-----Output:
%cpset      cpset=cell(n,1) and n is the number of center points.
%-----Info
%Author     sophia
%Blog       http://blog.csdn.net/sophia_xw?viewmode=contents
%Date       2017/4/5

nset=size(dis,2);

cpset=cell(nset,1);

dis=dis';
[value, index]= min (dis);

for loop=1:length(index)
    ind_cpt=index(loop);
    cpset{ind_cpt}=[cpset{ind_cpt} loop];
end

end

