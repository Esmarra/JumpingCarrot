clc; clear all;close all;

%%==== LDR's Calibration ====%%
syms x y;

adc_res=4095;
samples=100; % Number of Samples to Plot
calib_round = 3; % Valid Values: 1 | 2 | 3

%==== Trendline calib_round From Excell ====%%
% y -> ADC calib_round  |  x -> light calib_round

%% Round 1: Sensor Alfa With 1000 lumen Light Bulb & Random lux units %% 
if(calib_round==1)
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

%% Round 2: Sensor Alfa With 1000 lumen Light Bulb using Android Photo Sensor %% 
if(calib_round==2)
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

%% Round 3: Sensor Final With 680 lumen Light Bulb using Android Photo Sensor %% 
if(calib_round==3)
    % LDR_A
    eq = y == 253.17*log(x) - 52.857;
    sa=solve(eq,x);
    % LDR_B
    eq = y == 712.06*log(x) - 3330.7;
    sb=solve(eq,x);
    % LDR_C
    eq = y == 752.94*log(x) - 3195.7;
    sc=solve(eq,x);
    % LDR_D
    eq = y == 788.78*log(x) - 3540.9;
    sd=solve(eq,x);
end

disp('==== Light Value Solutions ====')
fprintf('>Solution for LDR A:  x = ')
disp(sa)
fprintf('>Solution for LDR B:  x = ')
disp(sb)
fprintf('>Solution for LDR C:  x = ')
disp(sc)
fprintf('>Solution for LDR D:  x = ')
disp(sd)

b=0;
for(i=1:samples:adc_res)
    b=b+1;
    ca(b)=eval(subs(sa,y,i));
    cb(b)=eval(subs(sb,y,i));
    cc(b)=eval(subs(sc,y,i));
    cd(b)=eval(subs(sd,y,i));
end
xl=linspace(1,adc_res,b);
figure('Name','Error Over Samples');
plot(xl,ca,xl,cb,xl,cc,xl,cd)
legend('LDR A','LDR B','LDR C','LDR D')

fprintf('==== Checksum over %.0f samples ====\n',adc_res)
fprintf('>Light Val for LDR A: ')
disp(eval(subs(sa,y,adc_res)))
fprintf('>Light Val for LDR B: ')
disp(eval(subs(sb,y,adc_res)))
fprintf('>Light Val for LDR C: ')
disp(eval(subs(sc,y,adc_res)))
fprintf('>Light Val for LDR D: ')
disp(eval(subs(sd,y,adc_res)))