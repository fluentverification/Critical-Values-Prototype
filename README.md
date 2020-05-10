# Critical-Values-Prototype
Contains files and programs for prototyping critical values for various genetic circuits.
#Main Idea
Being that there are a great number of possible configurations for the genetic toggle switch reqarding the amount of LacI and TetR, it makes sense that we need to abstract the amount of values (for LacI and TetR) that we are examining this circuit diagram at. This means that data will be obsorbed into other states or even lost. The question is how can we find these points of data where they provide the most meaningful info and where the obsorption of other states make the most sense. As of now, we have created state diagrams where we have used values for LacI and TetR as 0, 30, an 60. From some of the preliminary work, we have discovered that the values may be more askew, such as 0, 3, 24, etc.

The compelling idea is that we can use the change in reaction rate for each species in some way to find critical values with more meaning.

So far the only program listed here is for the Toggle switch circuit. The program works for both species, TetR and LacI, since  they both use the same reaction rate formula.
#How it Works
Essentially the program evaluates the reaction rate formula for each value of TetR/LacI starting at 0 through  a user specified amount (normally around 100). It keeps the value of IPTG/aTc constant throughout (also user specified). It also evaluates the derivative of this function, which is the instantaneous change in reaction rate. The idea is that the critical points are those that have a change in reaction rate that is above a predetermined threshold.

To find these points, the program creates a .txt files when ran. The first file includes all the points recorded in the form: number of LacI, reaction rate, instantaneous change in rr. The program then prints to the terminal the highest change in reaction rate and at what value of LacI/TetR that was recorded at in order to give the user somewhat of an idea for what value to use for the threshold. It then prompts for the threshold from the user. With this threshold, the program then evaluates the first text file and creates a second one containing only values that have a difference of the threshold or greater. It does not simply look at the change in rr for each data point to determine the values. Instead it starts at the first point. From the initial point, it then finds the next point to have a change greater than that of the threshold. This next point could be the next item or it could be 10 points away. This way the list stays dynamic depending on how many points you are trying to get.
#How to Use
To use the program you can download the already compiled file called CriticalPointFinder.out and run it on the linux command terminal using ./CriticalPointFinder.out. The program will then prompt for all the info needed and the results will be printed out to the resulting .txt files whith names specified by the user during run time.
The other option is to download the three source files and compile them yourself.


For more info contact PorterGiles123

MORE TO COME!!
