
%% Clear everthing
clear all;

%% Parameters
N = 128;
Fs = 100;

%% Time vector
t = linspace(0, N/Fs, N);

%% Create signal
s  = 300+256*sin(2*pi*40*t);

%% DFT
S = [];
for k=0:(N-1)
  S_tmp = 0;
  for n=0:(N-1)
    S_tmp = S_tmp + s(n+1)*exp((-i*2*pi*n*k)/N);
  end
  S = [S S_tmp];
end

%% Recalcualate values
S = abs(S);
S = S/N*2;

%% Calcualte frequencx vector
%F = 0:127;
F = linspace(0, Fs, N);

%% Plot
% Signal
subplot(2, 1, 1);
plot(t, s, 'linewidth', 1, '-x');
grid on;
title('Input signal');
xlabel('Time [s]');
ylabel('Amplitude [-]');
% DFT
subplot(2, 1, 2);
plot(F, real(S), 'linewidth', 1, '-x');
hold on;
plot(F, imag(S), 'linewidth', 1, '-o');
hold off;
grid on;
title('DFT of input signal');
xlabel('Frequency [Hz]');
ylabel('Amplitude [-]');