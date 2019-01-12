clc; clear all;close all;

%%==== LDR's Calibration ====%%
syms x y;

values = 2;
%==== Trendline Values From Excell ====%%
% y -> ADC values  |  x -> light values

if(values==1)
    % LDR_A
    eq = y == 312.48*log(x) - 695.71;
    sa=solve(eq,x);
    % LDR_B
    eq = y == 504.93*log(x) - 736.59;
    sb=solve(eq,x);
    % LDR_C
    eq = y == 525.4*log(x) - 1659.7;
    sc=solve(eq,x);
    % LDR_D
    eq = y == 596.34*log(x) - 1902.3;
    sd=solve(eq,x);
end

if(values==2)
    % LDR_A
    eq = y == 455.43*log(x) - 610.98;
    sa=solve(eq,x);
    % LDR_B
    eq = y == 715.13*log(x) - 974.96;
    sb=solve(eq,x);
    % LDR_C
    eq = y ==695.32*log(x) - 1116.1;
    sc=solve(eq,x);
    % LDR_D
    eq = y == 726.27*log(x) - 1153.9;
    sd=solve(eq,x);
end

disp('>Solution for LDR A: ')
disp(sa)
disp('>Solution for LDR B: ')
disp(sb)
disp('>Solution for LDR C: ')
disp(sc)
disp('>Solution for LDR D: ')
disp(sd)

max=4095;
inc=100;
b=0;
for(i=1:inc:max)
    b=b+1;
    ca(b)=eval(subs(sa,y,i));
    cb(b)=eval(subs(sb,y,i));
    cc(b)=eval(subs(sc,y,i));
    cd(b)=eval(subs(sd,y,i));
    
end
xl=linspace(1,max,b);
figure
plot(xl,ca,xl,cb,xl,cc,xl,cd)
legend('LDR A','LDR B','LDR C','LDR D')
val=max;
disp('>Checksum for LDR A: ')
disp(eval(subs(sa,y,val)))
disp('>Checksum for LDR B: ')
disp(eval(subs(sb,y,val)))
disp('>Checksum for LDR C: ')
disp(eval(subs(sc,y,val)))
disp('>Checksum for LDR D: ')
disp(eval(subs(sd,y,val)))
