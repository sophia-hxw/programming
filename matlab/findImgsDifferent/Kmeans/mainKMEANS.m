function allSETS=mainKMEANS(x,y,setsN)

center_pt_index=init(x, setsN);

dis  = distance( x, y, x(center_pt_index), y(center_pt_index));
cpset = chooseClass( dis );
cpts  = updateCenter( cpset, x, y );

boolChange = 1;

while(1)
    if(boolChange==1)
        dis4  = distance( x, y, x(cpts), y(cpts));
        cpset4 = chooseClass( dis4 );
        cpts4  = updateCenter( cpset4, x, y );
        
        boolChange = cptChange( cpts, cpts4 );
        cpts=cpts4;
        cpset=cpset4;
    else
        allSETS=cpset;
        break;
    end
end