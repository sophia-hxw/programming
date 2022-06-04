function varargout = ImgData(varargin)
% IMGDATA MATLAB code for ImgData.fig
%      IMGDATA, by itself, creates a new IMGDATA or raises the existing
%      singleton*.
%
%      H = IMGDATA returns the handle to a new IMGDATA or the handle to
%      the existing singleton*.
%
%      IMGDATA('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in IMGDATA.M with the given input arguments.
%
%      IMGDATA('Property','Value',...) creates a new IMGDATA or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before ImgData_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to ImgData_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help ImgData

% Last Modified by GUIDE v2.5 18-Nov-2016 12:28:22

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @ImgData_OpeningFcn, ...
                   'gui_OutputFcn',  @ImgData_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before ImgData is made visible.
function ImgData_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to ImgData (see VARARGIN)

% Choose default command line output for ImgData
handles.output = hObject;

handles.whichrect=0;
handles.issave=0;
handles.haceimg=0;
% Update handles structure
guidata(hObject, handles);


function varargout = ImgData_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--------push button--------------
function choosePATH_Callback(hObject, eventdata, handles)
% hObject    handle to choosePATH (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
strPath = uigetdir('F:\Attributes_20161219\','choose a dir');
if ~ischar(strPath)
    return
end

setappdata(hObject, 'strPath', strPath);

handles.mat_file='testData.mat';
load(handles.mat_file);
%后期要添加变量名字的方便录入，变量名是否存在的容错处理
handles.attr_names=attr_names;
handles.attributes=attributes;
handles.boxes=boxes;
handles.images=images;
guidata(hObject,handles);

setappdata(hObject,'strAllPath',images);
index=1;
JPGstr=strcat(images{index},'.jpg');

set(hObject,'UserData',index);
set(handles.pic_name,'string',JPGstr);

M=imread(fullfile(strPath, JPGstr));

himage=image(M);
hold on

for ia=1:size(boxes{index},1)
    box_cur=boxes{index}(ia,:);
    box_x=box_cur(1);
    box_y=box_cur(2);
    box_w=box_cur(3)-box_cur(1);
    box_h=box_cur(4)-box_cur(2);
    rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',1,'EdgeColor','r');
end
hold off

% % set(himage,'ButtonDownFcn',{@mycallback});
set(himage,'UIContextMenu',handles.RightMenu);
% 
handles.himage=himage;
handles.haceimg=1;
guidata(hObject, handles);


function LASTpic_Callback(hObject, eventdata, handles)
% hObject    handle to LASTpic (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if handles.haceimg
    strAllPath=getappdata(handles.choosePATH, 'strAllPath');
    strPath=getappdata(handles.choosePATH, 'strPath');
    indexLast=get(handles.choosePATH,'UserData');
    boxes=handles.boxes;
    if indexLast(1)>1
        index=indexLast(1)-1;
    else
        index=length(strAllPath);
    end
    
    set(handles.choosePATH,'userData',index);%效果类似追加？所以下面的index引用变成index(1)
    
    
    %set();
    JPGstr=strcat(strAllPath{index(1),1},'.jpg');
    set(handles.pic_name,'string',JPGstr);
    M=imread(fullfile(strPath,JPGstr));
    
    himage=image(M);
    hold on
    
    for ia=1:size(boxes{index},1)
        box_cur=boxes{index}(ia,:);
        box_x=box_cur(1);
        box_y=box_cur(2);
        box_w=box_cur(3)-box_cur(1);
        box_h=box_cur(4)-box_cur(2);
        rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',1,'EdgeColor','r');
    end
    hold off
    
    % % set(himage,'ButtonDownFcn',{@mycallback});
    set(himage,'UIContextMenu',handles.RightMenu);
    
    handles.himage=himage;
    guidata(hObject, handles);
end


function NEXTpic_Callback(hObject, eventdata, handles)
% hObject    handle to NEXTpic (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if handles.haceimg
    strAllPath=getappdata(handles.choosePATH, 'strAllPath');
    strPath=getappdata(handles.choosePATH, 'strPath');
    indexLast=get(handles.choosePATH,'userData');
    boxes=handles.boxes;
    if indexLast(1) < length(strAllPath)
        index=indexLast(1)+1;
    else
        index=1;
    end
    
    set(handles.choosePATH,'userData',index);
    %更新菜单选项checked值???
    % set(findobj(gcf,'Type','uimenu','Position',indexLast(1)),'Checked','off');
    % set(findobj(gcf,'Type','uimenu','Position',index),'Checked','on');
    % cla;
    
    JPGstr=strcat(strAllPath{index(1), 1},'.jpg');
    M=imread(fullfile(strPath,JPGstr));
    
    himage=image(M);
    set(handles.pic_name,'string',JPGstr);
    hold on
    
    for ia=1:size(boxes{index},1)
        box_cur=boxes{index}(ia,:);
        box_x=box_cur(1);
        box_y=box_cur(2);
        box_w=box_cur(3)-box_cur(1);
        box_h=box_cur(4)-box_cur(2);
        rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',1,'EdgeColor','r');
    end
    hold off
    
    % % set(himage,'ButtonDownFcn',{@mycallback});
    set(himage,'UIContextMenu',handles.RightMenu);
    
    handles.himage=himage;
    guidata(hObject, handles);
end


function NEWrect_Callback(hObject, eventdata, handles)
% hObject    handle to deleterect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    images=handles.images;
    boxes=handles.boxes;
    attributes=handles.attributes;
    
    JPGstr=strcat(images{index(1)},'.jpg');
    strPath=getappdata(handles.choosePATH, 'strPath');
    M=imread(fullfile(strPath, JPGstr));
    himage=image(M);
    hold on
    
    for ia=1:size(boxes{index(1)},1)
        box_cur=boxes{index(1)}(ia,:);
        box_x=box_cur(1);
        box_y=box_cur(2);
        box_w=box_cur(3)-box_cur(1);
        box_h=box_cur(4)-box_cur(2);
        rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',1,'EdgeColor','r');
    end
    

    % Hint: get(hObject,'Value') returns toggle state of deleterect
    if get(hObject,'value')
        hRect=imrect;
        %pos=wait(hRect);%画完后需要双击框框确定
        pos=getPosition(hRect);%size(pos)=[1 4]
        delete(hRect);%画过的框框会消失
        
        point1=[round(pos(1)),round(pos(2))];
        point2=point1+[round(pos(3)),round(pos(4))];
        size(handles.boxes{index(1)},1);
        rectangle('Position',[round(pos(1)),round(pos(2)),round(pos(3)),round(pos(4))],'LineWidth',1,'EdgeColor','r');
        
        if length(boxes{ index(1)})
            handles.boxes{index(1)}=[boxes{ index(1)};[ point1, point2] ];
            handles.attributes{index(1)}=[attributes{ index(1)};zeros(1,11) ];
        else
            handles.boxes{index(1)}=[ point1, point2];
            handles.attributes{index(1)}=zeros(1,11);
        end
        guidata(hObject,handles)
        
        boxes=handles.boxes;
        attributes=handles.attributes;
        save(handles.mat_file,'boxes','-append');
        save(handles.mat_file,'attributes','-append');
        
        set(hObject, 'value', 0);
    end
    
    hold off
    set(himage,'UIContextMenu',handles.RightMenu);
    
end


function figure1_WindowKeyPressFcn(hObject, eventdata, handles)
switch eventdata.Key
    case{'pageup','leftarrow','uparrow'}
        LASTpic_Callback(hObject, eventdata, handles);
    case{'pagedown','rightarrow','downarrow'}
        NEXTpic_Callback(hObject, eventdata, handles);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--------push button--------------



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--------right menu--------------
function Inflag=IsInRect(rect,point)
xMin=rect(1);
yMin=rect(2);
xMax=rect(3);
yMax=rect(4);
if point(1)>xMin && point(1)<xMax && point(2)>yMin && point(2)<yMax
    Inflag=1;
else
    Inflag=0;
end


function ButtonGroupDefault(initValue,handles)
switch initValue(1)
    case 1
        set(handles.male,'SelectedObject',handles.male1);
    case 0
        set(handles.male,'SelectedObject',handles.male2);
    case -1
        set(handles.male,'SelectedObject',handles.male3);
end
switch initValue(2)
    case 1
        set(handles.longhair,'SelectedObject',handles.longhair1);
    case 0
        set(handles.longhair,'SelectedObject',handles.longhair2);
    case -1
        set(handles.longhair,'SelectedObject',handles.longhair3);
end
switch initValue(3)
    case 1
        set(handles.glasses,'SelectedObject',handles.glasses1);
    case 0
        set(handles.glasses,'SelectedObject',handles.glasses2);
    case -1
        set(handles.glasses,'SelectedObject',handles.glasses3);
end
switch initValue(4)
    case 1
        set(handles.hat,'SelectedObject',handles.hat1);
    case 0
        set(handles.hat,'SelectedObject',handles.hat2);
    case -1
        set(handles.hat,'SelectedObject',handles.hat3);
end
switch initValue(5)
    case 1
        set(handles.tshirt,'SelectedObject',handles.tshirt1);
    case 0
        set(handles.tshirt,'SelectedObject',handles.tshirt2);
    case -1
        set(handles.tshirt,'SelectedObject',handles.tshirt3);
end
switch initValue(6)
    case 1
        set(handles.longsleeves,'SelectedObject',handles.longsleeves1);
    case 0
        set(handles.longsleeves,'SelectedObject',handles.longsleeves2);
    case -1
        set(handles.longsleeves,'SelectedObject',handles.longsleeves3);
end
switch initValue(7)
    case 1
        set(handles.shorts,'SelectedObject',handles.shorts1);
    case 0
        set(handles.shorts,'SelectedObject',handles.shorts2);
    case -1
        set(handles.shorts,'SelectedObject',handles.shorts3);
end
switch initValue(8)
    case 1
        set(handles.jeans,'SelectedObject',handles.jeans1);
    case 0
        set(handles.jeans,'SelectedObject',handles.jeans2);
    case -1
        set(handles.jeans,'SelectedObject',handles.jeans3);
end
switch initValue(9)
    case 1
        set(handles.longpants,'SelectedObject',handles.longpants1);
    case 0
        set(handles.longpants,'SelectedObject',handles.longpants2);
    case -1
        set(handles.longpants,'SelectedObject',handles.longpants3);
end
switch initValue(10)
    case 1
        set(handles.pattern,'SelectedObject',handles.pattern1);
    case 0
        set(handles.pattern,'SelectedObject',handles.pattern2);
    case -1
        set(handles.pattern,'SelectedObject',handles.pattern3);
end
switch initValue(11)
    case 1
        set(handles.fight,'SelectedObject',handles.fight1);
    case 0
        set(handles.fight,'SelectedObject',handles.fight2);
    case -1
        set(handles.fight,'SelectedObject',handles.fight3);
end


function RightMenu_Callback(hObject, eventdata, handles)


function RectProperty_Callback(hObject, eventdata, handles)
[x,y] = ginput(1);
posi=[round(x),round(y)];

index=get(handles.choosePATH,'userData');
boxes=handles.boxes;
len=size(boxes{index(1)},1);

for lo=1:len
    if IsInRect(boxes{index(1)}(lo,:), posi)
        handles.whichrect=lo;
        handles.issave=1;
        
        attributes=handles.attributes{index(1)}(lo,:);%before change
        ButtonGroupDefault(attributes,handles);
        
        
        images=handles.images;
        JPGstr=strcat(images{index(1)},'.jpg');
        strPath=getappdata(handles.choosePATH, 'strPath');
        M=imread(fullfile(strPath, JPGstr));
        himage=image(M);
        hold on
    
        for ia=1:size(boxes{index(1)},1)
            box_cur=boxes{index(1)}(ia,:);
            box_x=box_cur(1);
            box_y=box_cur(2);
            box_w=box_cur(3)-box_cur(1);
            box_h=box_cur(4)-box_cur(2);
            if (ia~=lo)
                rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',1,'EdgeColor','r');
            else
                rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',2,'EdgeColor','b');
            end
        end
        hold off
        set(himage,'UIContextMenu',handles.RightMenu);
        
        break;
    end
end
guidata(hObject, handles);


function DeleteRect_Callback(hObject, eventdata, handles)
% hObject    handle to DeleteRect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%pos=get(handles.himage,'Parent');
[x,y] = ginput(1);
posi=[round(x),round(y)];

index=get(handles.choosePATH,'userData');
boxes=handles.boxes;
attributes=handles.attributes;
%size(attributes{index(1)},1)
len=size(boxes{index(1)},1);
for lo=1:len
    if IsInRect(boxes{index(1)}(lo,:), posi)
        if (lo==1)
            handles.boxes{index(1)}=boxes{index(1)}(lo+1:len,:);%rects
            handles.attributes{index(1)}=attributes{index(1)}(lo+1:len,:);%attributes
        else
            if(lo==len)
                handles.boxes{index(1)}=boxes{index(1)}(1:lo-1,:);%rects
                handles.attributes{index(1)}=attributes{index(1)}(1:lo-1,:);%attributes
            else
                handles.boxes{index(1)}=[boxes{index(1)}(1:lo-1,:);boxes{index(1)}(lo+1:len,:)];%rects
                handles.attributes{index(1)}=[attributes{index(1)}(1:lo-1,:);attributes{index(1)}(lo+1:len,:)];%attributes
            end
        end
        guidata(hObject,handles);
        
        boxes=handles.boxes;
        attributes=handles.attributes;

        save(handles.mat_file,'boxes','-append');
        save(handles.mat_file,'attributes','-append'); 
        
        images=handles.images;
        JPGstr=strcat(images{index(1)},'.jpg');
        strPath=getappdata(handles.choosePATH, 'strPath');
        M=imread(fullfile(strPath, JPGstr));
        himage=image(M);
        hold on
    
        for ia=1:size(boxes{index(1)},1)
            box_cur=boxes{index(1)}(ia,:);
            box_x=box_cur(1);
            box_y=box_cur(2);
            box_w=box_cur(3)-box_cur(1);
            box_h=box_cur(4)-box_cur(2);
            rectangle('Position',[box_x,box_y,box_w,box_h],'LineWidth',1,'EdgeColor','r');
        end
        hold off
        set(himage,'UIContextMenu',handles.RightMenu);
        
        break;
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--------right menu--------------



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--------button group--------------
function male_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'male1'
                handles.attributes{index(1)}(WhichRect,1)=1;
            case 'male2'
                handles.attributes{index(1)}(WhichRect,1)=0;
            case 'male3'
                handles.attributes{index(1)}(WhichRect,1)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function longhair_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'longhair1'
                handles.attributes{index(1)}(WhichRect,2)=1;
            case 'longhair2'
                handles.attributes{index(1)}(WhichRect,2)=0;
            case 'longhair3'
                handles.attributes{index(1)}(WhichRect,2)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function glasses_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'glasses1'
                handles.attributes{index(1)}(WhichRect,3)=1;
            case 'glasses2'
                handles.attributes{index(1)}(WhichRect,3)=0;
            case 'glasses3'
                handles.attributes{index(1)}(WhichRect,3)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function hat_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'hat1'
                handles.attributes{index(1)}(WhichRect,4)=1;
            case 'hat2'
                handles.attributes{index(1)}(WhichRect,4)=0;
            case 'hat3'
                handles.attributes{index(1)}(WhichRect,4)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function tshirt_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'tshirt1'
                handles.attributes{index(1)}(WhichRect,5)=1;
            case 'tshirt2'
                handles.attributes{index(1)}(WhichRect,5)=0;
            case 'tshirt3'
                handles.attributes{index(1)}(WhichRect,5)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function longsleeves_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'longsleeves1'
                handles.attributes{index(1)}(WhichRect,6)=1;
            case 'longsleeves2'
                handles.attributes{index(1)}(WhichRect,6)=0;
            case 'longsleeves3'
                handles.attributes{index(1)}(WhichRect,6)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function shorts_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'shorts1'
                handles.attributes{index(1)}(WhichRect,7)=1;
            case 'shorts2'
                handles.attributes{index(1)}(WhichRect,7)=0;
            case 'shorts3'
                handles.attributes{index(1)}(WhichRect,7)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function jeans_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'jeans1'
                handles.attributes{index(1)}(WhichRect,8)=1;
            case 'jeans2'
                handles.attributes{index(1)}(WhichRect,8)=0;
            case 'jeans3'
                handles.attributes{index(1)}(WhichRect,8)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function longpants_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'longpants1'
                handles.attributes{index(1)}(WhichRect,9)=1;
            case 'longpants2'
                handles.attributes{index(1)}(WhichRect,9)=0;
            case 'longpants3'
                handles.attributes{index(1)}(WhichRect,9)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function pattern_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'pattern1'
                handles.attributes{index(1)}(WhichRect,10)=1;
            case 'pattern2'
                handles.attributes{index(1)}(WhichRect,10)=0;
            case 'pattern3'
                handles.attributes{index(1)}(WhichRect,10)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end


function fight_SelectionChangeFcn(hObject, eventdata, handles)
if handles.haceimg
    index=get(handles.choosePATH,'userData');
    attributes=handles.attributes;
    WhichRect=handles.whichrect;
    IsSave=handles.issave;
    
    if IsSave
        switch get(hObject,'tag')
            case 'fight1'
                handles.attributes{index(1)}(WhichRect,11)=1;
            case 'fight2'
                handles.attributes{index(1)}(WhichRect,11)=0;
            case 'fight3'
                handles.attributes{index(1)}(WhichRect,11)=-1;
        end
        
        attributes=handles.attributes;
        save(handles.mat_file,'attributes','-append');
        guidata(hObject,handles);
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%--------button group--------------
