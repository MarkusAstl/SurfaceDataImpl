% Specify folder name
folder = './';

% Get all filenames in folder that start with "MEASURED_"
filelist1 = dir([folder 'MEASURED_*.csv']);

% Get all filenames in folder that start with "LABELED_"
filelist2 = dir([folder 'LABELED_*.csv']);

% Initialize total non-matching count
total_non_matching_count = 0;

% Initialize difference counts
diff_counts = zeros(1,10);

% Loop through all file pairs
for i = 1:length(filelist1)
    
    % Get current file names
    file1 = filelist1(i).name;
    file2 = filelist2(i).name;
    
    % Read first two columns of both files
    data1 = csvread(file1, 0, 0, [0 0 inf 1]);
    data2 = csvread(file2, 0, 0, [0 0 inf 1]);

    % Check if first and last value of first column match
    if data1(1,1) == data2(1,1) && data1(end,1) == data2(end,1)

        % Count number of non-matching values in second column
        non_matching_count = 0;
        for j = 1:length(data1)
            if data1(j,2) ~= data2(j,2) && ~(data1(j,2) == 0 || data2(j,2) == 0)
                non_matching_count = non_matching_count + 1;
                diff = abs(data1(j,2) - data2(j,2));
                if diff > 1 && diff <= 10
                    diff_counts(diff) = diff_counts(diff) + 1;
                end
            end
        end
        
        % Add non-matching count to total non-matching count
        total_non_matching_count = total_non_matching_count + non_matching_count;

    else

        % Display error message if first and/or last value of first column do not match
        error('%s and/or %s: First and/or last value of first column do not match', file1, file2);

    end
    
end

% Display total number of non-matching values
fprintf('Total number of non-matching values across all files: %d\n', total_non_matching_count);

% Display number of times each difference appeared
fprintf('Difference Counts:\n');
for i = 1:length(diff_counts)
    fprintf('%d: %d\n', i, diff_counts(i));
end