% Random Polynomial Generator
% author: Mukund Ramakrishnan
% This program will generate 50 random polynomials; whose coefficients are
% represented by arrays. The polynomials will be between degree 1 and 10.



% Please look at the function buildPoly, which is used to create a random
% polynomial within the given constraints. 


fileID = fopen('Polynomials.txt','w');
for i = 1:50
    array = buildPoly();
    fmt=[repmat(' %5f',1,numel(array))]; % Modify this line as needed to adjust the precision of the coefficients.
    fprintf(fileID,fmt,array);
    fprintf(fileID, '\n');
end

fclose(fileID);





