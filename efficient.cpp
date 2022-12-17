/*
 * Implementation of Memory Efficient Version of Sequence Alignment Problem
 */
#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<sys/resource.h>
#include<sys/time.h>
#include<errno.h>
#include<algorithm>
#include<unordered_map>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<math.h>
using namespace std;

// Gap penalty (delta)
int gapPenalty;
// Mismatch penalty (alpha)
unordered_map<char, unordered_map<char, int> > penaltyTable;

/*
 * This method is input string generator.
 * It takes the given input string and the values of index after which copy of previous strings needs to be inserted.
 * And outputs resultant string
 */
string createCummulativeString(string baseString, vector<int> index){
	string result = baseString;
	if (!result.empty() && result[result.size() - 1] == '\r'){
		result.erase(result.size() - 1);
	}

	for(int i=0; i< index.size();i++){
		if (index[i] < 0) {
			index[i] = 0;
		} else if (index[i] > result.size() - 1) {
			index[i] = result.size() - 1;
		}
		result.insert(index[i]+1, result);
	}

	return result;
}

/*
 * This method reads from the input file
 */
vector<string> readInputFile(string& inputFileName){
	vector<string> baseStrings;
	vector<vector<int> > index(2, vector<int>());
	vector<string> result;

    ifstream inputfile;
    inputfile.open(inputFileName);
    if (!inputfile) {
        cout << "File not found";
        exit(1);
    }

    string readString;
    while (getline(inputfile, readString)) {
        if (isalpha(readString[0])) {
        	baseStrings.push_back(readString);
        } else if (isdigit(readString[0])) {
        	if(baseStrings.size() > 0){
        		index[baseStrings.size() - 1].push_back(stoi(readString));
        	}
        }
    }
    inputfile.close();
    if (!baseStrings[0].empty() && baseStrings[0][baseStrings[0].size() - 1] == '\r'){
		baseStrings[0].erase(baseStrings[0].size() - 1);
	}

	if (!baseStrings[1].empty() && baseStrings[1][baseStrings[1].size() - 1] == '\r'){
		baseStrings[1].erase(baseStrings[1].size() - 1);
	 }

	int validateString1 = int(pow(2, index[0].size())) * (baseStrings[0].size());
	int validateString2 = int(pow(2, index[1].size())) * (baseStrings[1].size());

    result.push_back(createCummulativeString(baseStrings[0],index[0]));
    result.push_back(createCummulativeString(baseStrings[1],index[1]));

    try {
		if (result[0].size() == validateString1 && result[1].size() == validateString2)
			return result;
	} catch (exception e) {
		cout<<"String size validation failed"<<endl;
		exit(1);
	}
	return vector<string>();
}

/*
 * This method writes the final alignments, optimal alignment cost, time taken and memory taken by the program
 * in final output file
 */
void writeOuputFile(string& outputFileName, vector<string> finalAlignmentAndOptimalValue, double totalTime, double totalMemory)
{
    ofstream outputFile;
    outputFile.open(outputFileName);

    if (!outputFile) {
        cout << outputFileName << " File not found!";
        exit(1);
    }

    outputFile << finalAlignmentAndOptimalValue[2] << endl;
    outputFile << finalAlignmentAndOptimalValue[0] << endl;
    outputFile << finalAlignmentAndOptimalValue[1] << endl;
    outputFile << totalTime << endl;
    outputFile << totalMemory << endl;

    outputFile.close();
}

/*
 * This method gets the total memory taken by the program
 */
long getTotalMemory() {
   struct rusage usage;
   int returnCode = getrusage(RUSAGE_SELF, &usage);
   if(returnCode == 0) {
       return usage.ru_maxrss;
   } else {
       //It should never occur. Check man getrusage for more info to debug.
       // printf("error %d", errno);
	   return -1;
   }
}

/*
 * This method creates the output file containing problem size, total time taken
 * ans total memory taken for each of the given datapoints
 *
 */
/*void writeDataPoints(int totalSize, double totaltime, double totalmemory){
	ofstream outputFile;
	outputFile.open("efficientDataPoints.txt",ios_base::app);

	outputFile<<totalSize<<" "<<totaltime<<" "<<totalmemory<<endl;
	outputFile.close();

}
*/

/*
 * This method initializes gap and mismatch penalty
 */
void initializePenalty(){
	gapPenalty = 30;
	penaltyTable['A']['A'] = 0;
	penaltyTable['A']['C'] = 110;
	penaltyTable['A']['G'] = 48;
	penaltyTable['A']['T'] = 94;
	penaltyTable['C']['A'] = 110;
	penaltyTable['C']['C'] = 0;
	penaltyTable['C']['G'] = 118;
	penaltyTable['C']['T'] = 48;
	penaltyTable['G']['A'] = 48;
	penaltyTable['G']['C'] = 118;
	penaltyTable['G']['G'] = 0;
	penaltyTable['G']['T'] = 110;
	penaltyTable['T']['A'] = 94;
	penaltyTable['T']['C'] = 48;
	penaltyTable['T']['G'] = 110;
	penaltyTable['T']['T'] = 0;
}

/*
 * This is dynamic programming implementation which is used internally by divide and conquer algorithm
 */
vector<string> inbuiltDP(string x, string y){
	vector<string> result;
	int xlen = x.length();
	int ylen = y.length();
	vector<vector<int> > M(xlen+1, vector<int>(ylen+1));

	//Initiliazing M matrix
	for(int i=0;i<= xlen ;i++){
		for(int j=0; j<=ylen;j++){
			M[i][j]=0;
		}
	}

	for(int i=1;i<=xlen;i++) {
		M[i][0] = i * gapPenalty;

	}

	for(int i=1;i<=ylen;i++) {
		M[0][i] = i * gapPenalty;
	}

	//Calculating the minimum alignment cost
	for(int i=1;i<=xlen;i++) {
		for(int j=1;j<=ylen;j++) {
			 int temp = min(penaltyTable[x[i-1]][y[j-1]] + M[i-1][j-1], gapPenalty+M[i-1][j]);
			 M[i][j] = min(temp, gapPenalty + M[i][j-1]);
		}
	}

	//Getting the optimal pairing sequence
	string firstAlignment, secondAlignment;
	int j=ylen,i=xlen;
	while(i>0 && j>0){
		if(x[i-1] == y[j-1]){
			firstAlignment = firstAlignment + x[i-1];
			secondAlignment = secondAlignment + y[j-1];
			i--;
			j--;
		}
		else if(M[i][j] == (M[i-1][j-1] + penaltyTable[x[i-1]][y[j-1]])){
			firstAlignment = firstAlignment + x[i-1];
			secondAlignment = secondAlignment + y[j-1];
			i--;
			j--;
		}
		else if(M[i][j] == (M[i-1][j] + gapPenalty)){
			firstAlignment = firstAlignment + x[i-1];
			secondAlignment = secondAlignment + '_';
			i--;
		}
		else if(M[i][j] == (M[i][j-1] + gapPenalty)){
			firstAlignment = firstAlignment + '_';
			secondAlignment = secondAlignment + y[j-1];
			j--;
		}
	}

	while(j>0){
		firstAlignment = firstAlignment + '_';
		secondAlignment = secondAlignment + y[j-1];
		j--;
	}
	while(i>0){
		firstAlignment = firstAlignment + x[i-1];
		secondAlignment = secondAlignment + '_';
		i--;
	}

	reverse(firstAlignment.begin(),firstAlignment.end());
	reverse(secondAlignment.begin(),secondAlignment.end());

	result.push_back(firstAlignment);
	result.push_back(secondAlignment);
	result.push_back(to_string(M[xlen][ylen]));
	return result;
}

/*
 * This method calculates optimal cost array for the given substrings
 */
vector<int> stringCostComputation(string s1, string s2){
	int s1Len = s1.size();
	int s2Len = s2.size();

	vector<vector<int> > M(2, vector<int>(s2Len+1));

	for(int i=0;i<=s2Len;i++){
		M[0][i] = i * gapPenalty;
	}

	for(int i=1;i<=s1Len;i++){
		M[1][0] = i * gapPenalty;
		for(int j=1;j<=s2Len;j++){
			int temp = min(penaltyTable[s1[i-1]][s2[j-1]] + M[0][j-1], gapPenalty + M[0][j]);
			M[1][j] = min(temp, gapPenalty + M[1][j-1]);
		}
		M[0] = M[1];
	}

	return M[1];
}

/*
 * This method is used to find minimum element in an array
 */
int findMinElement(vector<int> arr){
	int minElement, position;
	minElement = arr[0];
	position = 0;

	for(int i=1;i<arr.size();i++){
		if(arr[i]<minElement){
			minElement=arr[i];
			position = i;
		}
	}
	return position;
}

/*
 * This is the main implementation of memory efficient algorithm
 */
vector<string> divideAndConquer(string firstString, string secondString) {
	vector<string> answer(3);
	vector<string> answerLeft;
	vector<string> answerRight;

	//Base Cases
	if(firstString.length()==0){
		for(int i=0;i<secondString.length();i++){
			answer[0] = answer[0] + '_';
			answer[1] = answer[1] + secondString[i];
		}
		return answer;
	} else if(secondString.length()==0){
		for(int i=0;i<firstString.length();i++){
			answer[0] = answer[0] + firstString[i];
			answer[1] = answer[1] + '_';
		}
		return answer;
	} else if(firstString.length()<=2 || secondString.length()<=2){
		answer= inbuiltDP(firstString, secondString);
		return answer;
	}


	//Divide step
	int firstStringMid = firstString.length()/2;
	string firstStringLeft = firstString.substr(0, firstStringMid);
	string firstStringRight = firstString.substr(firstStringMid, firstString.size()-firstStringMid);

	vector<int> firstStringLeftCost = stringCostComputation(firstStringLeft, secondString);

	string reversed_firstStringRight = firstStringRight;
	string reversed_secondString = secondString;
	reverse(reversed_firstStringRight.begin(),reversed_firstStringRight.end());
	reverse(reversed_secondString.begin(),reversed_secondString.end());
	vector<int> firstStringRightCost = stringCostComputation(reversed_firstStringRight, reversed_secondString);

	vector<int> cost(firstStringLeftCost.size());
	for(int i=0;i<int(firstStringLeftCost.size());i++){
		cost[i] = firstStringLeftCost[i]+firstStringRightCost[firstStringRightCost.size()-1-i];

	}

	int secondStringDividePoint = findMinElement(cost);

	//Conquer step
	answerLeft = divideAndConquer(firstStringLeft, secondString.substr(0,secondStringDividePoint));
	answerRight = divideAndConquer(firstStringRight, secondString.substr(secondStringDividePoint, secondString.size()-secondStringDividePoint));

	//Combine step
	answer[0] = answer[0] + answerLeft[0] + answerRight[0];
	answer[1] = answer[1] + answerLeft[1]+ answerRight[1];
	answer[2] = answer[2] + to_string(cost[secondStringDividePoint]);
	return answer;
}

int main(int argc, char **argv){

	struct timeval begin, end;
	gettimeofday(&begin, 0);

	//solution starts here
	string program = argv[0];
	string inputFileName = argv[1];
	string outputFileName = argv[2];
	vector<string> cummulativeInputString = readInputFile(inputFileName);

	initializePenalty();
	vector<string> finalAlignmentAndOptimalValue = divideAndConquer(cummulativeInputString[0], cummulativeInputString[1]);

	double totalmemory = getTotalMemory();
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
	long microseconds = end.tv_usec - begin.tv_usec;
	double totaltime = seconds*1000 + microseconds*1e-3;

	writeOuputFile(outputFileName, finalAlignmentAndOptimalValue, totaltime, totalmemory);

//	int totalSize = cummulativeInputString[0].size() + cummulativeInputString[1].size();
//	writeDataPoints(totalSize, totaltime, totalmemory);

}
