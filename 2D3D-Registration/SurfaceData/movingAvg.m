M = readmatrix('Sarah_4D_CT_dummy.csv');
M(:,2) = movmean(M(:,2),9);
M(:,2) = round(M(:,2),2);
plot(M(:,1), M(:,2));
writematrix(M,'Sarah_4D_CT_dummy_smoothed.csv')