function output=getJpgName(folderOuter,folderInner)
folderName=['DIR .\' folderOuter '\' folderInner '\*.jpg  / B >fileName.txt'];
system(folderName);
file_t = fopen('fileName.txt');
A={};
while ~feof(file_t)
    tline=fgetl(file_t);
    A=[A;tline];
end
output=A;
fclose(file_t);
end