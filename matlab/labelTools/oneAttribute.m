function oneAttribute( attrFile )
%ONEATTRIBUTE �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
load('testData.mat');

for ia=1:length(attrFile)
    [origName,atrrOut,posROI]=getAttr(attrFile{ia});
    [ imgNum,ROINum] = isROIImg( origName,images,posROI,boxes  );
    if imgNum~=0
        if ROINum~=0
            attributes{imgNum}(ROINum,atrrOut(1)+1)=atrrOut(2);
        else
            boxes{imgNum}=[boxes{imgNum};posROI];
            
            curAttr=ones(1,length(attr_names));
            curAttr(atrrOut(1)+1)=atrrOut(2);
            attributes{imgNum}=[attributes{imgNum};curAttr];
        end
    else
        images=[images;origName];
        boxes=[boxes;posROI];
        
        curAttr=ones(1,length(attr_names));
        curAttr(atrrOut(1)+1)=atrrOut(2);
        attributes=[attributes;curAttr];
    end

end

save('testData.mat','attr_names','attributes','boxes','images');

end

