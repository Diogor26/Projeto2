function Out_mes = Matlab_desencripta(key_dec,mes_dec)

key_hex = dec2hex(key_dec)
key_str = num2str(reshape(key_hex', 1, []))

len_msg=size(mes_dec,2)

mes_original_hex=""

for i=1:16:len_msg
    mes_parte_dec =mes_dec(i:i+15);
    mes_hex = dec2hex(mes_parte_dec);
    mes_str = num2str(reshape(mes_hex', 1, []));

    mes_parte_original_hex=InvCipher(key_str, mes_str);
    
    mes_original_hex = mes_original_hex + mes_parte_original_hex
end

Out_mes=char(sscanf(mes_original_hex, '%2x').')
