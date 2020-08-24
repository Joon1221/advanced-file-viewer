//==============================================================================
// How to use
//
// Step 1: Choose setting
//
// Comment out #define COMPRESS 1 or #define DECOMPRESS 1 depending on which one
// you are going to use.
//
// Commenting it out means you are not going to use it.
//
// The default key for commenting should be CMD + ?
//
// Step 2: Change the settings
//
// Scroll down and change the settings (if needed) for the methods you are using
// (compression or decompression)
//
// Step 3: Run the source code
//
//------------------------------------------------------------------------------
// Default settings
//------------------------------------------------------------------------------
// By default compression and decompression are both turned on
//
// Running the source code will create two files named output.lz77 and
// output.lz77.org
//
// output.lz77 should be the compressed version of input.txt in binary form and
// output.lz77.org should have the same values as input.txt
//==============================================================================

//==============================================================================
// Compression
//==============================================================================
// Step 1: Set input
//
// Change value in istrstream input("input.txt") to another string or a file
// name. For example istrstream input("ABC"); or istrstream input("hello.txt);
//
// Step 2: Set output
//
// Change value in ofstream output("output.lz77"); to another file name. For
// example ofstream output("test/lz77");
//
//------------------------------------------------------------------------------
// Default settings
//------------------------------------------------------------------------------
// By default it automatically reads from the file input.txt, compresses it
// using lz77, and sets the value in output.lz77 in binary form
//
// Reading output.lz77 in text form will have all the numbers blanked out since
// it is written in binary
//------------------------------------------------------------------------------

//==============================================================================
// Decompression
//==============================================================================
// Step 1: Set input
//
// Change value in ifstream inputForDecompress("output.lz77") to another string
// or a filename. For example istrstream inputForDecompress("ABC"); or
// istrstream inputForDecompress("input.txt");
//
// The input should be a binary file
//
// Step 2: Set output
//
// Change value in ofstream outputForDecompress("output.lz77"); to another file
// name. For example ofstream outputForDecompress("out.lz77");
//
//
//------------------------------------------------------------------------------
// Default settings
//------------------------------------------------------------------------------
// By default it automatically reads from the file output.lz77, compresses it
// using lz77, and sets the value in output.lz77.org
//
// The file output.lz77 should be a binary file
//
// Reading output.lz77.org in text form will not have all the numbers blanked
// out since it is written in plain text
//------------------------------------------------------------------------------

#define COMPRESS 1
#define DECOMPRESS 1

#include <iostream>
#include <fstream>
#include <strstream>

using namespace std;

#include "lz77.h"
#include "input_window.h"

int main() {
    
#ifdef COMPRESS
//    istrstream input("ABCDABCD");
//    istrstream input("AAAAAAA\n0000000\niiiiiii\nlllllll\n");
//    ifstream input("1502599490921.gif");
//    ifstream input("WebDriver-367.15.10.45f01.pkg");
//    ifstream input("BgBoxColliders.java");
//    ifstream input("test_0001_size_1.txt");
//    ifstream input("test_0002_size_2_same_char.txt");
//    ifstream input("test_0003_size_3_same_char.txt");
//    ifstream input("test_0004_size_4_same_char.txt");
    
//    string inputFileName = "1502599490921.gif";
//    string inputFileName = "test_0003_size_3_same_char.txt";
//    string inputFileName = "test_0004_size_4_same_char.txt";
//    string inputFileName = "test_0004_size_4_diff_char.txt";
//    string inputFileName = "test_0005_size_500_same.txt";
    string inputFileName = "BgBoxColliders.java"; // now works
//    string inputFileName = "BgBoxColliders1.java";
//    string inputFileName = "BgBoxColliders1.txt"; // now works
//    string inputFileName = "BgBoxColliders2.txt";
    
//    string inputFileName = "test_0006_error_case_with_a_and_b.txt"; // now works
//    string inputFileName = "test_0006_error_case_with_slash_and_enter.txt"; // now works
    
    int fileSize = InputWindow::getFileSize(inputFileName.c_str());
    cout << "main(): fileSize = " << fileSize << endl;
    ifstream input(inputFileName.c_str());
    
    ofstream output("output.lz77");
    int outputByteSize = lz77Compress(input, fileSize, output);
    output.close();

#endif
    
#ifdef DECOMPRESS
    ifstream inputForDecompress("output.lz77");
    ofstream outputForDecompress("output.lz77.org");
    int outputByteSizeForDecompress = lz77Decompress(inputForDecompress, outputForDecompress);
    outputForDecompress.close();
#endif



    return 0;
}

//#include <iostream>
//#include <strstream>
//
//using namespace std;
//
//#include "lz77.h"
//
//#define NUM_TESTCASES 3
////#define NUM_TESTCASES 9
//
//string testcases_input[] = {
//    "A",
//    //    "AA",
//    //    "AB",
//    //    "AAA",
//    //    "AAAB",
//    //    "AAABA",
//    //    "AABCBBABC",
//    "ABABCDABCD",
//    //    "AABCBBABC\nABABCDABCD\n"
//    "A ",
//    //    "A \n",
//};
//
//#define MAX_NUM_BYTES_RESULT 1000
//char testcases_expected[][MAX_NUM_BYTES_RESULT] = {
//    {0, 0, 'A'},
//    {0, 0, 'A', 0, 0, 'B', 2, 2, 'C', 0, 0, 'D', 4, 3, 'D'},
//    {0, 0, 'A', 0, 0, ' '},
//    //    {0, 0, 'A', 0, 0, ' ', 0, 0, '\n'},
//};
//
//char testcases_expected_byte_size[] = {
//    3,
//    15,
//    6,
//    //    9,
//};
//
//
////string testcases_expected[] = {
////    "(0,0)A",
////    "(0,0)A (1,1)_",
////    "(0,0)A (0,0)B",
////    "(0,0)A (1,1)A",
////    "(0,0)A (1,1)A (0,0)B",
////    "(0,0)A (1,1)A (0,0)B (4,1)_",
////    "(0,0)A (1,1)B (0,0)C (2,1)B (5,2)C",
////    "(0,0)A (0,0)B (2,2)C (0,0)D (4,3)D",
////    "(0,0)A (1,1)B (0,0)C (2,1)B (5,2)C (0,0)A (0,0)B (2,2)C (0,0)D (4,3)D"
////};
//
//int main() {
//    //    istrstream input("ABC\nDEF\n\nGHI\n");
//    //    string output = lz77(input);
//    unsigned char output[MAX_NUM_BYTES_RESULT];
//    
//    int countFailed = 0;
//    
//    for (int i = 0; i < NUM_TESTCASES; i++) {
//        istrstream input(testcases_input[i].c_str());
//        int outputByteSize = lz77(input, output);
//        
//        bool matched = false;
//        if (outputByteSize == testcases_expected_byte_size[i]) {
//            matched = true;
//            for (int j = 0; j < outputByteSize && matched; j++) {
//                if (output[j] != testcases_expected[i][j]) {
//                    matched = false;
//                }
//            }
//        }
//        
//        if (!matched) {
//            countFailed++;
//            cout << "---------------------------------------------------------------------" << endl;
//            cout << "failed: i = " << i << endl;
//            cout << "        input              = \"" << testcases_input[i] << "\"" << endl;
//            cout << "        testcases_expected = \"" << testcases_expected[i] << "\"" << endl;
//            cout << "        output             = \"" << output << "\"" << endl;
//        }
//    }
//    
//    // summary
//    cout << "=====================================================================" << endl;
//    cout << "Number of failed testcases: " << countFailed << endl;
//    if (countFailed == 0) {
//        cout << "Congratulation!!!!!!" << endl;
//    }
//    else {
//        cout << ":(" << endl;
//    }
//    cout << "=====================================================================" << endl;
//    
//    return 0;
//}
//
