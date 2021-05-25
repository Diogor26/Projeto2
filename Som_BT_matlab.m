blelist
ze=1; 
i=0;

while ze==1
    A = input('Digite o nome do ble \n','s');
    try
       b=ble(A);
       ze=0;
    catch
       ze=1;
    end
       
end

b.Characteristics
bvec=b.Characteristics;
S1=bvec{4,2};
C1=bvec{4,4};
cr=characteristic(b,S1,C1);
disp("faaa")
fileID = fopen('rebeca.txt','w');

while i<400
  subscribe(cr);
  sn=read(cr);
  y = typecast(uint8(sn), 'int16');
  fprintf(fileID,'%i\n',y);
  unsubscribe(cr);
  i=i+1
end
disp("adeus")
fclose(fileID);



clear cr
clear b








