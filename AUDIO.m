fileID = fopen('rebeca.txt','w');
fileID0 = fopen('sound.txt.txt','w');
disp("faaa")
key_hex = dec2hex(Key_final);
key_str = num2str(reshape(key_hex', 1, []));

while i<429
  subscribe(cr2);
  sn=read(cr2); %224 
  fprintf(fileID0,'%i\n',sn);
  message_final=somdescripta(key_str,sn);
  y = typecast(uint8(message_final), 'int16');
  fprintf(fileID,'%i\n',y);
  unsubscribe(cr2);
  i=i+1
end
disp("adeus")
fclose(fileID);
fclose(fileID0);
V=importdata("rebeca.txt");
W=importdata("sound.txt.txt");
%plot(V)
%ee=KK(1.02e4:4.66e4);
%soundsc(W,16000)

soundsc(V,16000)
