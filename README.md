# Critical-Values-Prototype
Contains files and programs for prototyping critical values for various genetic circuits.

So far the only program listed here is for the Toggle switch circuit. The program works for both species, TetR and LacI, since 
they both use the same reaction rate formula.

Essentially the program evaluates the reaction rate formula for each value of TetR/LacI starting at 0 through 
a user specified amount (normally around 100). It keeps the value of IPTG/aTc constant throughout (also user specified). It
also evaluates the derivative of this function, which is the instantaneous change in reaction rate. The idea is that the critical
points are those that have a change in reaction rate that is above a predetermined threshold.

To find these points, the program creates a .txt files when ran. The first file includes all the points recorded in the form:
# of LacI, reaction rate, instantaneous change in rr. The program then prints to the terminal the highest change in reaction rate 
and at what value of LacI/TetR that was recorded at in order to give the user somewhat of an idea for what value to use for the 
threshold. It then prompts for the threshold from the user. With this threshold the program then evaluates the first text file and 
creates a second one containing only values that have a difference of the threshold or greater.

MORE TO COME!!
