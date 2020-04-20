function hash = strcrc(str)

nchars=numel(str);
c=uint16(str);
bits=16;

hash=uint16(0);

for i=1:nchars,
    shift=mod(i-1,bits);
    
%     fprintf('Char %d:\t\t\t\t\t\t%s.\n',c(i),dec2bin(c(i),bits));
    
    upperbitsmadelower=bitshift(c(i),-bits+shift);
%     fprintf('Upperbits, shifted to lower:\t%s.\n',dec2bin(upperbitsmadelower,bits));
    
    shifted=bitshift(c(i),shift);
%     fprintf('Lowerbits, shifted to upper:\t%s.\n',dec2bin(shifted,bits));
    
    shifted=bitor(shifted,upperbitsmadelower);
%     fprintf('Rotated (recombined):\t\t\t%s.\n\n',dec2bin(shifted,bits));
    
    hash=bitxor(shifted,hash);
%     fprintf('Hash:\t\t\t%s.\n\n',dec2bin(hash,bits));
end
