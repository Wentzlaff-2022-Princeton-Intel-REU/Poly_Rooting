% buildPoly
% author: Mukund Ramakrishnan
% This is a helper function for testCaseGenerator.m. When the function is
% called, one random polynomial that obeys the project constraints is
% produced and returned in an array. 


function [n,c,r] = buildPoly()

tooSmallGap = 1;

while (tooSmallGap == 1)
% We first try building one polynomial right here. 

n = randi(10); % This is the degree of our polynomial.

r = -5 + (5+5)*rand(1,n); % We generate n random solutions for our polynomial. 
r = sort(r); % We sort the array so we can check if any two numbers are too close. 

k = -5 + (5+5)*rand; % We generate random number k.

% Here is a for-loop that checks and sees if any two numbers are too close.

for i = 1:length(r)-1;
 
    if (r(i+1) - r(i) < 0.001) % This could also be accomplished with the diff() function.
       tooSmallGap = 0; 
       break;
    end
    
    tooSmallGap = 1;
end

end

% With this while loop, we have ensured that the polynomial's solutions are
% not too close. 

c = k.*poly(r);

end
