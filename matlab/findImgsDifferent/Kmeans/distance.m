function  dis  = distance( xs, ys, cpt_xs, cpt_ys)
%MAXDISTANCE 此处显示有关此函数的摘要
%-----Input:
%xs         x-axis values of all points.
%ys         y-axis values of all points.
%cpt        all center points.
%-----Output:
%dis        size(dis)=[m,n],m is the length of xs(or ys) and n is the 
%           number of cpy.
%-----Info
%Author     sophia
%Blog       http://blog.csdn.net/sophia_xw?viewmode=contents
%Date       2017/4/5

m_pt=length(xs);
n_cpt=length(cpt_xs);

[size_x, size_y]=size(cpt_xs);
if(size_x>size_y)
    cpt_xs=cpt_xs';
    cpt_ys=cpt_ys';
end

cpt_mat_x=repmat(cpt_xs, m_pt,1);
cpt_mat_y=repmat(cpt_ys, m_pt,1);


[size_xs, size_ys]=size(xs);
if(size_xs<size_ys)
    xs=xs';
    ys=ys';
end

xs=repmat(xs, 1, n_cpt);
ys=repmat(ys, 1, n_cpt);

dis=(xs-cpt_mat_x).*(xs-cpt_mat_x)+(ys-cpt_mat_y).*(ys-cpt_mat_y);

end

