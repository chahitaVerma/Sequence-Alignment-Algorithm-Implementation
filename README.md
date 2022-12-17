# Sequence-Alignment-Algorithm-Implementation
The project is related to the implementation of the two different solutions for the Sequence Alignment problem. One is basic solution. Other is space efficient solution.

For Sequence Alignment, we first used Dynamic Programming to find optimal alignments. The memory requirement for that was quite large. However, we know that for the calculation of optimal cost, the only requirement is the immediate previous output rather than all the previous outputs. Hence efficient version of algorithm is generated using divide and conquer, will only stored the immediate previous output at every step which will eventually reduced our memory usage. The substantial difference can be seen in memory requirement as the input size increases.

Following graphs depicts the  difference between the time and memory requirements in both algorithm.

**Graph1** – Time vs Problem Size (M+N)

<img width="565" alt="image" src="https://user-images.githubusercontent.com/118519162/208269554-567df857-7468-4463-b467-a65c1526502f.png">

Basic: Graph for basic algorithm takes O(m*n) time. So it is polynomial type.
Efficient: Graph for efficient algorithm takes O(m * n) time. So it is polynomial type.
Explanation: 
Both algorithms work on m*n size workspace, so they take polynomial time. However, efficient algorithm takes more time for larger sized inputs because it runs recursively.

**Graph2** – Memory vs Problem Size (M+N)

<img width="502" alt="image" src="https://user-images.githubusercontent.com/118519162/208269572-bcdb204d-cff0-4c96-a6b7-bb381f09a7fb.png">

Basic: Graph for basic algorithm takes O(m*n) memory. So it is polynomial type.
Efficient: Graph for efficient algorithm takes O(m + n) memory. So it is linear type.
Explanation: 
As we use recursive functions in efficient algorithm, we are reusing the workspace. Hence reducing memory usage. This difference in memory of both algorithms is visible when the input size is large. 

**How to run :**
Both files (basic.cpp and efficient.cpp) can be executed from terminal using command :

./basic.sh input.txt output.txt

./efficient.sh input.txt output.txt
