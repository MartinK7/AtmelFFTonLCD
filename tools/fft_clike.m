
%% Clear everthing
clear all;

%% Parameters
N = 128;
Fs = 100;

%% Time vector
t = linspace(0, N/Fs, N);

%%
bitswap = [ 0 64 32 96 16 80 48 112 8 72 40 104 24 88 56 120 4 68 36 100 20 84 52 116 12 76 44 108 28 92 60 124 2 66 34 98 18 82 50 114 10 74 42 106 26 90 58 122 6 70 38 102 22 86 54 118 14 78 46 110 30 94 62 126 1 65 33 97 17 81 49 113 9 73 41 105 25 89 57 121 5 69 37 101 21 85 53 117 13 77 45 109 29 93 61 125 3 67 35 99 19 83 51 115 11 75 43 107 27 91 59 123 7 71 39 103 23 87 55 119 15 79 47 111 31 95 63 127];
TSinCos = [ 0 64 0 64 64 0 0 64 45 45 64 0 45 -45 0 64 24 59 45 45 59 24 64 0 59 -24 45 -45 24 -59 0 64 12 62 24 59 35 53 45 45 53 35 59 24 62 12 64 0 62 -12 59 -24 53 -35 45 -45 35 -53 24 -59 12 -62 0 64 6 63 12 62 18 61 24 59 30 56 35 53 40 49 45 45 49 40 53 35 56 30 59 24 61 18 62 12 63 6 64 0 63 -6 62 -12 61 -18 59 -24 56 -30 53 -35 49 -40 45 -45 40 -49 35 -53 30 -56 24 -59 18 -61 12 -62 6 -63 0 64 3 63 6 63 9 63 12 62 15 62 18 61 21 60 24 59 27 57 30 56 32 54 35 53 38 51 40 49 42 47 45 45 47 42 49 40 51 38 53 35 54 32 56 30 57 27 59 24 60 21 61 18 62 15 62 12 63 9 63 6 63 3 64 0 63 -3 63 -6 63 -9 62 -12 62 -15 61 -18 60 -21 59 -24 57 -27 56 -30 54 -32 53 -35 51 -38 49 -40 47 -42 45 -45 42 -47 40 -49 38 -51 35 -53 32 -54 30 -56 27 -57 24 -59 21 -60 18 -61 15 -62 12 -62 9 -63 6 -63 3 -63];

%% Create signal
signalRe  = 64+64*sin(2*pi*10*t+5);
signalIm  = zeros(1,length(signalRe));

signalRe_backup  = signalRe;
signalIm_backup  = signalIm;

%% DFT


%% Recalcualate values
%S = abs(S);
%S = S/N*2;

%% Calcualte frequencx vector
%F = 0:127;
F = linspace(0, Fs, N);

%% Plot
% Signal
subplot(2, 1, 1);
plot(t, signalRe_backup, 'linewidth', 1, '-x');
hold on;
plot(t, signalIm_backup, 'linewidth', 1, '-x');
hold off;
grid on;
title('Input signal');
xlabel('Time [s]');
ylabel('Amplitude [-]');
% DFT
subplot(2, 1, 2);
plot(F, signalRe, 'linewidth', 1, '-x');
hold on;
plot(F, signalIm, 'linewidth', 1, '-o');
hold off;
grid on;
title('DFT of input signal');
xlabel('Frequency [Hz]');
ylabel('Amplitude [-]');