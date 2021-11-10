# Homework 5

NOTE: This homework should be completed working together with Your Project partner. If that is not possible, there is a reduced workload scenario for those doing it solo.
In the beginning of `answers.txt` note wether this project is done as a team or by one person. 

If it is done by team, also note in comments in code, which sections were done by which person. You can submit the code to both GIT repositories or one, but you MUST submit the `answers.txt` file to both teammates GIT, and note there which of the two repositories should be evaluated for code.

The final Questionnaire should be fulfilled individually in Ortus.

Supplementary reading, that might be help with this task, can be found here: http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.45.3382&rep=rep1&type=pdf

## Overall process of submitting HW5
Please follow these steps to develop and submit the HW successfully:
1) **Follow the instructions precisely!** Ask if You don't understand them!
2) Check out this Git repository and commit Your changes often - **often and granular commits** with easy to understand improvements described in comments will both make it easy for me to understand Your though process and allow Yourself to easily overlook Your progress and revert specific changes in cases of errors.
3) I will only evaluate (and comment on/provide suggestions) on Your homework **once** (except if You request specific help beforehand)
4) By default I will evaluate the last commit pushed to this repository before the deadline announced. In case *You request it*, I will evaluate the latest commited code by applying the appropriate point reduction for late commits.
5) You can request help with understanding the homework and in some cases even with code itself - do this by commiting the code to Git and then sending me Your related question to e-mail. I reserve the right to help only with understanding/debugging, but will not provide any code for You.
6) In addition to commiting Your working code, You also **must fill HW5 Questionnaire in Ortus** - this will be ralated to Your completed code, so do it after the code is mostly done.
7) As stated before - academic integrity is key in any task in this course - violators will face serious consequences - DO NOT COPY or REWRITE CODE by other people. DO NOT EVEN LOOK AT OTHER students HW code before both of You have submitted Your final code. I will not try to distinguish who copied from whom - both parties will face equal punishment. Also don't borrow code from online pages and **don't use any C libraries, that I have not explicitly allowed**.

## Overall task in short
In this Homework You will evaluate algorithms for dynamic memory allocation. Write Your answers in `answers.txt` file.

## Step by step INSTRUCTIONS for HW5

**TASK1** Create a memory allocator program.

Assume that the total memory the allocator has to work with is 1024 bytes. Note that:

* These bytes also must include any bookkeeping for the algorithms (pointers to next/previous empty chunks of memory etc. as discussed in previous lectures about dynamic memory allocation).
* Some of these bytes can already be assumed taken. You will be provided a list of free memory chunks. If these are less than the total 1024 bytes, assume that everytihn else is reserved or for bookkeeping.

The program that You will write in `hw5.c` should be able to take two parameters `./a.out -c=chunks -s=sizes`.

The parameter `chunks` is a filename containing a list of available free chunks - each row a single integer representing the chunk size in bytes e.g.

    10
    16
    8
    960
    32
    
NOTE - these chunk sizes are the actual free memory available, You can use additional bookkeeping information that fits in the 1024 bytes, but no need to use the chunk itself internally, if there is too little space. Also these chunks can be any size starting from 1 byte, not only multiples of 8 - in these cases You also might want to align to whole bytes for easier calculation.

The parameter `sizes` is a filename containing a list of incoming memory requests - each row a single integer representing the requested size in bytes e.g.:

    8
    8
    128
    10
    12

Some sample test files are provided in this repository already.

To create the allocator program You need to do the following:

* Allocate the 1024 byte block of memory
* Decide on data to be stored for each chunk and store that tada in this memory
* Create a `find_free_chunk` function that takes the pointer to the list of empty data chunks and a number of bytes to be allocated and either finds a block for storing this data and returns pointer to that block or returns NULL if there is no such block left. (You will later need to implement several different searching algorithms in this function - You can also pass a parameter to this function on which algorithm should be used.
* Create another function that takes the address of a free block and size of data to be reserved and does it - set it up so the free block is reserved and if there is enough space left a free chunk is cut off and added back to free list. No need to actually implement freeing the memory as that will not be used in this task.

**TASK2**

In the `find_free_chunk` function implement the following options for finding the chunk:

* BestFit
* WorstFit
* FirstFit
* NextFit
* Your own alternative solution for extra credit (describe it in `answers.txt` file as well)

NOTE: If You don't have a team partner for this task, You can choose only 2 of these algorithms to implement/test (more if You wish for extra credit).

Test these algorithms with amount of data in `sizes` that exceeds the total free memory available to Your porgram. Try a mix of bigger and smaller pieces.

**TASK3**
In `answers.txt` describe how each of the algorithms work on Your test data - what is the fragmentation, what amount of data form total test set does not fit depending on the algorithm chosem.

For each of the algorithms also evaluate their speed/efficiency, note specific measurable data for all compared measurements (e.g. allocation speed in pieces per second etc.)

Write all analysis in `answers.txt`. Check in the test files in Git as well, and in the `answers.txt` refer to specific test files when providing specific data.

Specifics that will be evaluated in the analysis/`answers` file (no need to write lengthy - be concise and to the point):
* Description of algorithms used
* Description of measurements/experiments
* Evaluation of fragmentation, speed/efficiency and related discussion of what this means
* Overall conclusions and analysis for selection of a specific algorithm.





That is it!


