%autolog = csvread('logPID2.txt');
close all;

load 'autolog'
sz = size(autolog);
s = sz(1)*sz(2);
values = zeros(1, s);

dt = 0.05
tfin = s * dt;
c = 1;

for i = 1:sz(1)
  for k = 1:sz(2)
    values(c) = autolog(i, k);
    c++;
  end
end

t = 0: dt:  tfin - dt;

figure(1);
plot(t, values);
