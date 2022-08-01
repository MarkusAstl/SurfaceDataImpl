% Define parameters
start_factor = 0.5;     % Cut dataset to start at this percentage
end_factor = 0.91;      % Cut dataset to end at this percentage
smoothing_val = 8;      % Window size for smoothing avergage filter
brth_cyc_dur = 2.42;    % Estimated breath cycle duration in seconds   -> 12 Hz? nur für Erwachsene
interval_factor = 1.2;  % Adjusts estimated breath cycle duration interval 
                        %   for search interval of next extremum
cycles_for_avg = 1;     % Number of last breath cycles considered for
                        %   calculation of average breath amplitude range


%--------------------------------------------------------------------------                        
%--------------------------------------------------------------------------                        

% Read data
[data] = readtable('BruMi-20210606-083243_clean.csv');
t = data.Time_ms_/1000;                   % change unit from ms to s
a1 = data.PriAmplitude_mm_;

% Cut data to certain interval
new_start = round(length(t) * start_factor);
new_end = round(length(t) * end_factor);
t = t(new_start:new_end);
a1 = a1(new_start:new_end);

% Smooth list
a1_smth = smooth_list(a1, smoothing_val);

% Interpolation
[t_intp, a1_intp] = interpolation(t,a1_smth);
dt_intp = t_intp(2) - t_intp(1);

% Plot raw, smoothed and interpolated data
figure('Position',[10 10 1200 700], 'OuterPosition',[1 200 1400 600])
subplot(3,1,1)
plot(t,a1)
subtitle("Raw Surface Data")
xlabel("t [ms]")
ylabel("Amp. [mm]")
subplot(3,1,2)
plot(t,a1_smth)
subtitle("Smoothed Surface Data")
xlabel("t [ms]")
ylabel("Amp. [mm]")
subplot(3,1,3)
plot(t_intp,a1_intp)
subtitle("Interpolated Smoothed Surface Data")
xlabel("t [ms]")
ylabel("Amp. [mm]")

% Find extrema
[min_ind_list, max_ind_list] = find_extrema(a1_intp, brth_cyc_dur, dt_intp, interval_factor);

% Find last extremum
ends_with_min = check_last_extremum(min_ind_list, max_ind_list);

if ends_with_min
    [last_max, last_interv_max_ind] = max(a1_intp(min_ind_list(end):end));  % local max index in last interval
    last_max_ind = min_ind_list(end) + last_interv_max_ind - 1;             % index of last max in whole dataset
    max_ind_list = [max_ind_list, last_max_ind];

else
    [last_min, last_interv_min_ind] = min(a1_intp(max_ind_list(end):end));  % local min index in last interval
    last_min_ind = max_ind_list(end) + last_interv_min_ind - 1;             % index of last min in whole dataset
    min_ind_list = [min_ind_list, last_min_ind];
end

ends_with_min = check_last_extremum(min_ind_list, max_ind_list);

% Plot data to check extrema
figure('Position',[10 10 1200 700], 'OuterPosition',[1 200 1400 600])
plot(t_intp, a1_intp, t_intp(max_ind_list), a1_intp(max_ind_list), '*', t_intp(min_ind_list), a1_intp(min_ind_list), '*')

% Define range of last half breathing cycle
if ends_with_min
    half_cycle_data = a1_intp(min_ind_list(end):end);
    half_cycle_start_ind = min_ind_list(end);
else
    half_cycle_data = a1_intp(max_ind_list(end):end);
    half_cycle_start_ind = max_ind_list(end);
end

% Calculate average amplitude range for phase seperation
num_of_cycles = length(min_ind_list);
ampl_range_list = zeros(1,num_of_cycles);

for cycle_ind = 1:num_of_cycles
    ampl_range = abs(a1_intp(max_ind_list(cycle_ind)) - a1_intp(min_ind_list(cycle_ind)));
    ampl_range_list(cycle_ind) = ampl_range;
end

avg_ampl_range = mean(ampl_range_list(end-cycles_for_avg:end));
phase_ampl_range = avg_ampl_range/5;

% Determine last phase number
last_ampl_range = abs(half_cycle_data(end) - half_cycle_data(1));
last_phase = 0;

if last_ampl_range <= phase_ampl_range
    last_phase = 1;
elseif last_ampl_range <= phase_ampl_range * 2
    last_phase = 2;
elseif last_ampl_range <= phase_ampl_range * 3
    last_phase = 3;
elseif last_ampl_range <= phase_ampl_range * 4
    last_phase = 4;
elseif last_ampl_range > phase_ampl_range * 4
    last_phase = 5;
end

if ends_with_min == false
    last_phase = last_phase + 5;
    incr_sign = -1;
    border_num = last_phase - 6;
else
    incr_sign = 1;
    border_num = last_phase - 1;
end

% Check phase
phase_border_ind_list = zeros(1,border_num);

for phase_num = 0:border_num
    calc_border_val = incr_sign * phase_ampl_range * phase_num + a1_intp(half_cycle_start_ind);
    [phase_border_val, phase_border_ind] = min(abs(half_cycle_data-calc_border_val));
    phase_border_ind_list(phase_num+1) = phase_border_ind + half_cycle_start_ind - 1;
end

figure('Position',[10 10 1200 700], 'OuterPosition',[1 200 1400 600])
plot(t_intp(min_ind_list(end-1):end), a1_intp(min_ind_list(end-1):end))
xline(t_intp(phase_border_ind_list))
text(min(t_intp(min_ind_list(end-1):end)),max(a1_intp(min_ind_list(end-1):end))-0.1, ...
    ["Last phase: " + int2str(last_phase)])

%--------------------------------------------------------------------------
%--------------------------------------------------------------------------

% Functions
function smoothed_list = smooth_list(list, window_size)
    b = (1/window_size)*ones(1,window_size);
    a = 1;
    smoothed_list = filtfilt(b,a,(list));
end

function [new_t, new_vals] = interpolation(t, vals)
    new_t = linspace(t(1),t(end),length(t)*2);
    new_vals = interp1(t, vals, new_t);
end

function [min_ind_list, max_ind_list] = find_extrema(data_list, brth_cyc_dur, dt, interval_factor)
% finds all extrema in data_list except the last one
% data_list...Breathing amplitude data
% brth_cyc_dur...Estimated breath cycle duration in seconds

brth_cyc_ind_dur = brth_cyc_dur/dt;                             % Estimated breath cycle duration in indices
interval_size = round(brth_cyc_ind_dur*interval_factor);        % index interval size for extremum search
start_ind = 1;
max_ind_list = [];
min_ind_list = [];

    while start_ind <= length(data_list)
        try
            relevant_interval = data_list(start_ind:(start_ind + interval_size));
            [loc_max, interv_loc_max_ind] = max(relevant_interval);
            loc_max_ind = interv_loc_max_ind + start_ind - 1;
            max_ind_list = [max_ind_list, loc_max_ind];
            start_ind = loc_max_ind;
        catch
            break
        end
        
        try
            relevant_interval = data_list(start_ind:(start_ind + interval_size));
            [loc_min, interv_loc_min_ind] = min(relevant_interval);
            loc_min_ind = interv_loc_min_ind + start_ind - 1;
            min_ind_list = [min_ind_list, loc_min_ind];
            start_ind = loc_min_ind;
            
        catch
            break
        end
        
    end
end

function ends_with_min = check_last_extremum(min_ind_list, max_ind_list)
    % Determines if last extremum is a minimum
    if length(max_ind_list) == length(min_ind_list) % Extremum search always starts with a max
        ends_with_min = true;
    elseif length(max_ind_list) > length(min_ind_list)
        ends_with_min = false;
    else
        error("Number of minimum indices is too high. length(min_ind_list) " + ...
            "must be smaller or equal to length(max_ind_list)")
    end
end
