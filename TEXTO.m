w=1;
subscribe(cr1);

disp("digite um texto no monitor de serie no arduino")
    y=1;
    while y==1
        try
            message=read(cr1)
            %Len = size(message);
            y=0;
        catch
            y=1;
        end
    end
    
   disp("texto:") 
   message_final=Matlab_desencripta(Key_final,message)
