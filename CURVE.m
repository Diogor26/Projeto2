
subscribe(cr3);
d='77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a';
P= '0900000000000000000000000000000000000000000000000000000000000000';
Km=Curve25519(P,d)
km_char=double(sscanf(Km, '%2x').')
write(cr3,km_char);
pause (1)
ka=read(cr3);

ka_hex=dec2hex(ka);
key_str = num2str(reshape(ka_hex', 1, []));



key=Curve25519(key_str,d);
Key_final=double(sscanf(key, '%2x').')
disp("curve feito")
%unsubscribe(cr3);
pause (5)