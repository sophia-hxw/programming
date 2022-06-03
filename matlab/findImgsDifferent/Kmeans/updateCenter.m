function  cpts  = updateCenter( cpsets, xs, ys )
%UPDATECENTER 此处显示有关此函数的摘要
%-----Input:
%cpsets         cpsets=cell(n,1) and n is the number of center points.
%-----Output:
%cpts           new center points.
%-----Info
%Author     sophia
%Blog       http://blog.csdn.net/sophia_xw?viewmode=contents
%Date       2017/4/5

n_cpts=length(cpsets);
cpts=zeros(n_cpts,1);

for loop=1:n_cpts
    if(isempty(cpsets{loop})==1)
        continue;
    else
        xs_c=sum(xs(cpsets{loop}))/length(cpsets{loop});
        ys_c=sum(ys(cpsets{loop}))/length(cpsets{loop});
        dis=distance( xs(cpsets{loop}), ys(cpsets{loop}), xs_c,  ys_c);
        [value, index]=min(dis);
        cpts(loop)=cpsets{loop}(index);       
    end
end

end

