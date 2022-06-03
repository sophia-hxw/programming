function center_pt_index=init(x, setsN)

%rand(n)生成二维n*n大小矩阵；rand(n,m)生成二维n*m大小矩阵
len=length(x);

center_pt_index=unidrnd(len,1,setsN);

end