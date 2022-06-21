% buildPoly
% author: Mukund Ramakrishnan
% This is a helper function for testCaseGenerator.m. When the function is
% called, one random polynomial that obeys the project constraints is
% produced and returned in an array. 


function p = buildPoly()

tooSmallGap = 1;

while (tooSmallGap == 1)
% We first try building one polynomial right here. 



n = randi(10); % This is the degree of our polynomial.

r_i = -5 + (5+5)*rand(1,n); % We generate n random solutions for our polynomial. 

k = -5 + (5+5)*rand; % We generate random number k.

% Here is a for-loop that checks and sees if any two numbers are too close.

for i = 1:length(r_i)-1;
 
    if (r_i(i+1) - r_i(i) < 0.001)
       tooSmallGap = 0;
       break;
    end
    
    tooSmallGap = 1;
end

end

% With this while loop, we have ensured that the polynomial's solutions are
% not too close. 

p = k*poly(r_i);


end
