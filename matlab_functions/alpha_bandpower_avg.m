function [ alpha_features ] = alpha_bandpower_avg(x)

alpha_features = zeros(2, 1);
index = 1;
for j=7:8  %only get O1 and O2
    p = bandpower(x(:, j), 500, [8 13]);
    alpha_features(index) = p;
    index = index + 1;
end

end