## Truth Table project

Given a circuit representation, the program outputs a truth table based off of all possible inputs. Listing all possible gates once in this case, the input format is in a txt file as follows:<br />
INPUT NumberofInputs IN0 IN1 ... INn-1<br />
OUTPUT NumberofOutputs  OUT0 OUT1 ... OUTn-1 <br />
<br />
NOT Input Output<br />
AND Input0 Input1 Output<br />
NAND Input0 Input1 Output<br />
OR Input0 Input1 Output<br />
NOR Input0 Input1 Output<br />
XOR Input0 Input1 Output<br />
MULTIPLEXER NumerofInputs (2^NumberofInputs)NumberofPossibleOutputs Intput0 Input1 ... Inputn-1 Output<br />
DECODER NumberofInputs Input0 Input1 ... Inputn-1 (2^NumberofInputs)NumberofOutputs<br /> <br />
Execution of the program abides by the following format:<br />
./Truthtable.exe txtfile<br />
Sample execution:<br />
./Truthtable.exe tet.1.09.txt<br />