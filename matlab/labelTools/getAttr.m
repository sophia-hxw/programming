function [origName,atrrOut,posROI]=getAttr(name)

pos1=strfind(name,'_');
pos2=strfind(name,'.');

origName=name(1:pos1(2)-1);

attrName=name(pos1(2)+1:pos1(3)-1);
attrValue=name(pos1(3)+1:pos1(4)-1);
attrNameNum=str2double(attrName(5:end));
attrValuNum=str2double(attrValue(4:end));
if attrValuNum==0
    realattrValuNum=1;
elseif attrValuNum==1
	realattrValuNum=0;
elseif attrValuNum==2 
    realattrValuNum=-1;
else
    disp('Sothing wrong with the value of the attributes.')
end
atrrOut=[attrNameNum realattrValuNum];

rectX=str2double(name(pos1(4)+1:pos1(5)-1));
rectY=str2double(name(pos1(5)+1:pos1(6)-1));
rectW=str2double(name(pos1(6)+1:pos1(7)-1));
rectH=str2double(name(pos1(7)+1:pos2-1));
rectX2=rectX+rectW;
rectY2=rectY+rectH;

posROI=[rectX,rectY,rectX2,rectY2];
end