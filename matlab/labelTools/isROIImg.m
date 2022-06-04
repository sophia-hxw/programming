function [ imgNum,ROINum] = isROIImg( strName,Images,ROIpos,Boxes  )
%ISROIIMG �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��

imgNum=0;
ROINum=0;
for ia=1:length(Images)
    if strcmp(Images{ia},strName)
        imgNum=ia;
        break;
    end
end

if imgNum~=0
    ROIposition=Boxes{imgNum};%ROIposition is a matrix.
    for ib=1:size(ROIposition,1)
        comPare=(ROIpos==Boxes{imgNum}(ib,:));
        if(sum(comPare)==4)
            ROINum=ib;
            break;
        end
    end
end

end

