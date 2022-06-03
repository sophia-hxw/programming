function  drawPic( x, y, center_pt_index, index, cpset, namePic )
%DRAWPIC 此处显示有关此函数的摘要
%-----Input:
%x                  x-axis value of the points.
%y                  y-axis value of the points.
%center_pt_index    index of the center points.
%index              index==0, points are not separated or were divided into
%                   different class.
%cpset              cpset=cell(n,1) and n is the number of center points.
%namePic            the name of the picture that U want to save.
%-----Output:
%NONE
%-----Info
%Author     sophia
%Blog       http://blog.csdn.net/sophia_xw?viewmode=contents
%Date       2017/4/5


%几种简单的不同中心点区分画法
plotType=['ro';'bo';'go';'yo';'mo';'co'];%different color
plotTypeC=['k*';'k.';'k+';'ks';'kd'];

class_num=length(center_pt_index);

if(index==0)
    plot(x,y,'k.')
    hold on
else
    for ib=1:class_num
        if(isempty(cpset{ib})~=1)
            plot(x(cpset{ib}),y(cpset{ib}),plotTypeC(ib,:))
            hold on
        end
    end
end

for ia=1:class_num
    plot(x(center_pt_index(ia)),y(center_pt_index(ia)),plotType(ia,:))
    hold on
end
saveas(gcf,namePic,'jpg');
hold off

end

