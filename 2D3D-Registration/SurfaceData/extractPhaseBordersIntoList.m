% Load data from CSV file
dataName = '2021_11_30_Andi_RespSignal_20211130_171611_clean';
fileName = append(dataName, '.csv');
data = readmatrix(fileName);

% Extract the first two columns and skip the first row (headers)
x = data(2:end, 1);
y = data(2:end, 2);

% Smooth data
y = smoothdata(y);

% Plot the data
figure('units','normalized','outerposition',[0 0 1 1]);
plot(x, y);
title('Original Data');

% Allow user to select two points on the plot with the mouse
[x_selected, ~] = ginput();
selected_x_values = [];

for plotInd = 1:length(x_selected)-1

    % Extract data between the selected x values
    x_min = x_selected(plotInd);
    x_max = x_selected(plotInd+1);
    idx = (x >= x_min) & (x <= x_max);
    interval = [x(idx), y(idx)];
    
    % Plot the extracted data
    figure('units','normalized','outerposition',[0 0 1 1]);
    plot(interval(:, 1), interval(:, 2));
    title('Extracted Data');
    
    % Allow user to select points on the plot with the mouse
    [max_selected, ~] = ginput();
    
    % Save the selected x values to an array and convert to seconds
    selected_x_values = [selected_x_values, max_selected'];
end

% Data must start with first selected max
idxCut = (x >= selected_x_values(1) & x <= selected_x_values(end));
selected_data = [x(idxCut), y(idxCut)];

% Find time value of each phase border
phaseBorderList = zeros(2,(length(selected_x_values)-1)*10);
for maxInd = 1:(length(selected_x_values) - 1)
    cycleDur = selected_x_values(maxInd+1) - selected_x_values(maxInd);
    phaseDur = cycleDur / 10.0;
    phaseBorderTime = selected_x_values(maxInd);
    for phaseInd = 1:10
        phaseBorderTime = phaseBorderTime + phaseDur;
        phaseBorderList(1,(maxInd-1)*10+phaseInd) = phaseBorderTime;
        phaseBorderList(2,(maxInd-1)*10+phaseInd) = phaseInd;
    end
end

% Find phase for each datapoint
labeledData = zeros(length(selected_data(:,1)), 2);    
phaseBorderListInd = 1;
for z = 1:length(selected_data(:,1))
    if selected_data(z,1) > phaseBorderList(1, phaseBorderListInd)
        phaseBorderListInd = phaseBorderListInd + 1;
    end
    labeledData(z,1) = selected_data(z,1)/1000;
    labeledData(z,2) = phaseBorderList(2,phaseBorderListInd);
end

%%
% Plot the maxima
figure('units','normalized','outerposition',[0 0 1 1]);
plot(selected_data(:, 1), selected_data(:, 2));
xline(selected_x_values);
title('Extracted Data with Selected Maxima');


%%
% SAVED CODE

% Create subfolder to save extracted data
if ~exist('labeled_data', 'dir')
    mkdir('labeled_data');
end

% Save extracted data to CSV file in subfolder
csvwrite(fullfile('labeled_data', append('LABELED_' , fileName)), labeledData);
