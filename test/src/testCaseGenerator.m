% Random Polynomial Generator
% author: Mukund Ramakrishnan
% This program will generate 50 random polynomials; whose coefficients are
% represented by arrays. The polynomials will be between degree 1 and 10.

% Please look at the function buildPoly, which is used to create a random
% polynomial within the given constraints. 

fileID = fopen('../data/polynomials.txt','w');
for i = 1:50
    [n,c,r] = buildPoly();
    fprintf(fileID, '%d', n);
    fmt=[repmat(' %.18f', 1, n + 1)]; % Modify this line as needed to adjust the precision of the coefficients.
    fprintf(fileID, fmt, c);
    fmt=[repmat(' %.18f', 1, n)]; % Modify this line as needed to adjust the precision of the coefficients.
    fprintf(fileID, fmt, r);
    fprintf(fileID, '\n');
end

fclose(fileID);
