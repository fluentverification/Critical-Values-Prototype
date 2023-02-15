# Critical-Values-Prototype
Contains files and programs for prototyping critical values for various genetic circuits.

## Structure
1. C-Element
  - Contains Abstracted models for the three C-element Circuits.
  - Contains legacy abstraction tool for the "old style" abstraction
2. Case-Studies
  - This directory contains the original toggle switch model
  - No tool is located here
3. CriticalPointFinder
  - This contains standalone tools to find the thresholds for specifically the toggle script 
  - Divided into additive and derivative sub-directories which contain the cumulative and purely derivative based methods respectively
4. SingleSpecies
  - For a period of time we wanted to study the affects of thresholds on a much more simple model
  - This directory contains modles that mirror only one side of the toggle switch model
  - These are single species models that self degrade and self repress
  - No useful tools are found here
5. ToggleSwitch
  - This directory contains tools and models for analyzing the ToggleSwitch.
  - Here are Scripting tools for writing Toggle switch models
  - There are also tools for finding thresholds for Toggle switch models
  - Note that there are only tools here for working with the Toggle switch models
  - These tools will not transfer use for other models
6. janiParser
  - This is the main tool that should be utilized
  - This tool accepts a jani model and a tolerance and produces a prism model
  - There are two versions parseJani.py and parseJaniV2.py, use v2
  - The models that were converted to jani can be either traditional or sbml format
  - The script is ran like this: `parseJaniV2.py sbml.jani .5`
  - Output will be printed to the screen
  - It is multithreaded
  - parameters to change number of threads and enable debug are found toward the top of the script
7. tickets
  - Nothing useful is found here



## Legacy Readme

Being that there are a great number of possible configurations for the genetic toggle switch reqarding the amount of LacI and TetR, it makes sense that we need to abstract the amount of values (for LacI and TetR) that we are examining this circuit diagram at. This means that data will be obsorbed into other states or even lost. The question is how can we find these points of data where they provide the most meaningful info and where the obsorption of other states make the most sense. As of now, we have created state diagrams where we have used values for LacI and TetR as 0, 30, an 60. From some of the preliminary work, we have discovered that the values may be more askew, such as 0, 3, 24, etc.

The compelling idea is that we can use the change in reaction rate for each species in some way to find critical values with more meaning.

So far the only program listed here is for the Toggle switch circuit. The program works for both species, TetR and LacI, since  they both use the same reaction rate formula.

Essentially the program evaluates the reaction rate formula for each value of TetR/LacI starting at 0 through  a user specified amount (normally around 100). It keeps the value of IPTG/aTc constant throughout (also user specified). It also evaluates the derivative of this function, which is the instantaneous change in reaction rate. The idea is that the critical points are those that have a change in reaction rate that is above a predetermined threshold.

To find these points, the program creates a .txt files when ran. The first file includes all the points recorded in the form: number of LacI, reaction rate, instantaneous change in rr. The program then prints to the terminal the highest change in reaction rate and at what value of LacI/TetR that was recorded at in order to give the user somewhat of an idea for what value to use for the threshold. It then prompts for the threshold from the user. With this threshold, the program then evaluates the first text file and creates a second one containing only values that have a difference of the threshold or greater. It does not simply look at the change in rr for each data point to determine the values. Instead it starts at the first point. From the initial point, it then finds the next point to have a change greater than that of the threshold. This next point could be the next item or it could be 10 points away. This way the list stays dynamic depending on how many points you are trying to get.

INSTRUCTIONS

To use the program you can download the already compiled file called CriticalPointFinder.out and run it on the linux command terminal using ./CriticalPointFinder.out. The program will then prompt for all the info needed and the results will be printed out to the resulting .txt files whith names specified by the user during run time.
The other option is to download the three source files and compile them yourself.

For more info contact PorterGiles123

