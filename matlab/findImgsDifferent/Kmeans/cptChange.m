function boolChange = cptChange( cpts_old, cpts_new )
%CPTCHANGE �˴���ʾ�йش˺�����ժҪ
%DRAWPIC �˴���ʾ�йش˺�����ժҪ
%-----Input:
%cpts_old       center points in last loop.
%cpts_new       center points in the current time.
%-----Output:
%boolChange         boolChange==1,there are changes among center points or
%                   no changes.
%-----Info
%Author     sophia
%Blog       http://blog.csdn.net/sophia_xw?viewmode=contents
%Date       2017/4/6

[old_x, old_y]=size(cpts_old);
if(old_x>old_y)
    cpts_old=cpts_old';
end

[new_x, new_y]=size(cpts_new);
if(new_x>new_y)
    cpts_new=cpts_new';
end

if(cpts_old==cpts_new)
    boolChange=0;
else
    boolChange=1;
end

end

