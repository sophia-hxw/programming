clear
clc

attr_names=[{'MALE'};{'LONG_HAIR'};{'GLASSES'};{'HAT'};{'TSHIRT'};...
    {'LONG_SLEEVES'};{'SHORTS'};{'JEANS'};{'LONG_PANTS'};{'Pattern'};...
    {'Fight'}];
 
attributes={};
boxes={};
images={};

fightNO=getJpgName('fight','NO');
fightYES=getJpgName('fight','YES');
glassesNO=getJpgName('glasses','NO');
glassesYES=getJpgName('glasses','YES');
hatNO=getJpgName('hat','NO');
hatYES=getJpgName('hat','YES');
jeansNO=getJpgName('jeans','NO');
jeansYES=getJpgName('jeans','YES');
longhairNO=getJpgName('long_hair','NO');
longhairYES=getJpgName('long_hair','YES');
longpantsNO=getJpgName('long_pants','NO');
longpantsYES=getJpgName('long_pants','YES');
longsleevesNO=getJpgName('long_sleeves','NO');
longsleevesYES=getJpgName('long_sleeves','YES');
maleNO=getJpgName('male','NO');
maleYES=getJpgName('male','YES');
patternNO=getJpgName('pattern','NO');
patternYES=getJpgName('pattern','YES');
shortsNO=getJpgName('shorts','NO');
shortsYES=getJpgName('shorts','YES');
TshirtNO=getJpgName('Tshirt','NO');
TshirtYES=getJpgName('Tshirt','YES');

% A=dir(fullfile('F:\Attributes_20161219\fight\YES\','*.jpg'));
% N=length(A);
% B={A(1:N).name};
% K=strfind(B,'');

%function [origName,atrrOut,posROI]=getAttr(name)

save('testData.mat','attr_names','attributes','boxes','images');
oneAttribute( fightNO );
oneAttribute( fightYES );
oneAttribute( glassesNO );
oneAttribute( glassesYES );
oneAttribute( hatNO );
oneAttribute( hatYES );
oneAttribute( jeansNO );
oneAttribute( jeansYES );
oneAttribute( longhairNO );
oneAttribute( longhairYES );
oneAttribute( longpantsNO );
oneAttribute( longpantsYES );
oneAttribute( longsleevesNO );
oneAttribute( longsleevesYES );
oneAttribute( maleNO );
oneAttribute( maleYES );
oneAttribute( patternNO );
oneAttribute( patternYES );
oneAttribute( shortsNO );
oneAttribute( shortsYES );
oneAttribute( TshirtNO );
oneAttribute( TshirtYES );
