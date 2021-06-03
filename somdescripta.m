function Out_aud = somdescripta(key_str,mes_dec)

%key_hex = dec2hex(key_dec);
%key_str = num2str(reshape(key_hex', 1, []));

len_msg=size(mes_dec,2);
clearvars mes_original_hex;
mes_original_hex="";

for i=1:16:len_msg
    mes_parte_dec =mes_dec(i:i+15);
    mes_hex = dec2hex(mes_parte_dec);
    mes_str = num2str(reshape(mes_hex', 1, []));

    mes_parte_original_hex=InvCipher(key_str, mes_str)
    
    mes_original_hex = mes_original_hex + mes_parte_original_hex;
   
    
    
end
%mes_original_char=convertStringsToChars(mes_original_hex)

%n=1;

%for i=1:2:448
  % out_prep(n)=hex2dec(mes_original_char(i:i+1))
   
  % n=n+1
%end
 Out_aud=double(sscanf(mes_original_hex, '%2x').');
 
 
%typecast(int16(base2dec(mes_original_dec, 16)), 'int16')

