% Load data from CSV file
dataName = 'labeled_data/LABELED_BruMi-20210531-083409_clean';
fileName = append(dataName, '.csv');
data = readmatrix(fileName);

% Extract the first two columns and skip the first row (headers)
x = data(2:end, 1);
y = data(2:end, 2);