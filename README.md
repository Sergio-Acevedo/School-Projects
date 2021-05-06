## Knapsack project

The knapsack problem involves being given a list of objects, values, weights, and a weight limit, outputs the combination of objects that yields the highest value, without exceeding the weight limit.
The a format of the input is in a txt file as follows:
<br />NumberofItems <br />
Name0 Value0 Weight0<br />
Name1 Value1 Weight1<br />
...<br />
Namen-1 Valuen-1 Weightn-1<br />
<br />
Execution of the program abides by the following format:<br />
./Knapsack.exe maximumweight txtfile<br />

Sample execution: 
./Knapsack.exe 75 manifest.04.txt

The output is a list of the selected items, followed by their collective value, and the maximum weight limit.