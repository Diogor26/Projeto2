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
C1=bvec{4,4}; %texto
C2=bvec{5,4}; %audio
C3=bvec{6,4}; %curve 
cr1=characteristic(b,S1,C1);
cr2=characteristic(b,S1,C2);
cr3=characteristic(b,S1,C3);
disp("connected")
