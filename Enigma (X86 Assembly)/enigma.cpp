#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <sstream>


using namespace std;

const int DRUMSIZE = 64;
const char* ROTOR_FILE     = "data/rotors.txt";
const char* INIT_FILE      = "data/init.txt";
const char* PLAINTEXT_FILE = "data/plaintext.txt";
const char* OUTPUT_FILE    = "data/ciphertext.txt";
const char* LOG_FILE       = "data/log.txt";

// Drum arrays (signal goes from the left)
int D1L[DRUMSIZE];
int D2L[DRUMSIZE];
int D3L[DRUMSIZE];

// Drum arrays (signal goes from the right)
int D1R[DRUMSIZE];
int D2R[DRUMSIZE];
int D3R[DRUMSIZE];

// Reverese drum
int DR[DRUMSIZE];

// Drum rotations: number of times we would need to 
// shift the drum right to return the drum to initial setting
int d1, d2, d3; 


// Shifts drum left
void shl(int *drum) {
  int tmp = drum[0];
  int i = 0;
  while ( i < DRUMSIZE-1 ) { 
    drum[i] = drum[i+1];
    i++;
  }
  drum[DRUMSIZE-1] = tmp;
}

// Rotates drums after signal pass
void rotate_drums() {
  // Rotate first drum
  shl(D1L); shl(D1R);
  if ( ++d1 > DRUMSIZE ) {
    d1 = 0;
    // Rotate second drum
    shl(D2L); shl(D2R);
    if ( ++d2 > DRUMSIZE ) {
      d2 = 0;
      // Rotate third
      shl(D3L); shl(D3R);
      if ( ++d3 > DRUMSIZE ) d3 = 0;
    }
  }
}

// Returns transposed drum index, rotates drums
int encrypt(int c) {
  // Go through drums right to left
  c = D3R[D2R[D1R[c]]];
  // Reversing drum
  c = DR[c];
  // Go through drums left to right
  c = D1L[D2L[D3L[c]]];
  // Rotate drums
  rotate_drums();
  // Return drum index changed to character
  return c;
  // return c + 32;
}

// Transforms a character from range 32-95 into
// a valid drum index 
int index(char character) {
  return (int)character-32;
}

// Transforms an index to a character representation
// (64 drum indexes assigned to ascii character range
// 32-95)
char character(int index) {
  return (char)index+32;
}

// Encrypts a character from ascii range 32-95
char encrypt_char(char c) {
  return character(encrypt(index(c)));
}

// Init rotor connections
void init_drums() {
  ifstream file(ROTOR_FILE);
  string line;
  int all[512];
  int i = 0;
  int k;
  while ( getline(file, line) ) {
    char a[2];
    a[0] = line.at(0); a[1] = line.at(1); string s1(a);
    a[0] = line.at(3); a[1] = line.at(4); string s2(a);
    int b = atoi(s1.c_str()); int c = atoi(s2.c_str());
    // cout << b << "--" << c << endl;
    all[i++] = b; all[i++] = c;
  }

  for ( i = 0, k = 0; i < DRUMSIZE; i++, k+=2) {
    D1L[all[k]] = all[k+1];
    D1R[all[k+1]] = all[k];
  }
  for ( i = 0; i < DRUMSIZE; i++, k+=2) {
    D2L[all[k]] = all[k+1];
    D2R[all[k+1]] = all[k];
  }
  for ( i = 0; i < DRUMSIZE; i++, k+=2) {
    D3L[all[k]] = all[k+1];
    D3R[all[k+1]] = all[k];
  }
  for ( i = 0; i < DRUMSIZE/2; i++, k+=2) {
    DR[all[k]] = all[k+1];
    DR[all[k+1]] = all[k];
  }
}

void init_drum_positions() {
  d1 = d2 = d3 = 0;
  // Read initial drum positions
  ifstream file(INIT_FILE);
  int a, b, c;

  file >> a; file >> b; file >> c; 
  
  for ( int i = 0; i < a; i++ ) {
    d1++;
    shl(D1L);
    shl(D1R);
  }
  for ( int i = 0; i < b; i++ ) {
    d2++;
    shl(D2L);
    shl(D2R);
  }
  for ( int i = 0; i < c; i++ ) {
    d3++;
    shl(D3L);
    shl(D3R);
  }
  
}

// Used for debugging purposes
void print_drums() {
  cout << "d3: " <<  d3 << ", " << "d2: " << d2 << ", " << "d1: " << d1 << endl;
  for ( int i = 0; i < DRUMSIZE; i++ ) {
    cout << "|";
    cout << setw(2) << i;
    cout << "|";

    cout << setw(4) ;
    cout << DR[i];

    cout << setw(4) ;
    cout << D3L[i] << ";" << D3R[i];
    cout << setw(4) ;
    cout << D2L[i] << ";" << D2R[i];
    cout << setw(4) ;
    cout << D1L[i] << ";" << D1R[i];

    cout << endl;
  }
  cout << endl << endl;
}

void init() {
  init_drums();
  init_drum_positions();
}

string encrypt_string(string s) {
  string ret; 
  for ( int i = 0; i < s.length(); ++i )  {
    ret += encrypt_char(s[i]);
    print_drums();
  }
  
  return ret;
}

string encrypt_file() {
  ifstream file(PLAINTEXT_FILE);
  ofstream ofile;
  ofile.open(OUTPUT_FILE);

  string line;
  while ( getline(file, line) ) {
    ofile << encrypt_string(line) << endl;
  }

  ofile.close();
}





int main( ) {
  init();
  encrypt_file();
  // print_drums();
  // string output = encrypt_string("ALA MA KOTA");
  // cout << output << endl;
  // init();
  // cout << encrypt_string(output) << endl;
  // init_drum_positions();
  // cout << encrypt(0) << endl; 
  //cout << encrypt(1) << endl; 
  // encrypt_file();
  // cout << encrypt(encrypt(32));
  // init_drums();
  // init_drum_positions();
  // print_drum(D1L);
  // // print_drum(D1R);
  // print_drum(D2L);
  // // print_drum(D2R);
  // print_drum(D3L);
  // // print_drum(D3R);
  // print_drum(DR);
  // init_drums();
  // cout << endl;
  // shl(D1L);
  // print_drum(D1L);
}

// UNUSED CODE

// Shifts drum right
// void shr(int *drum) {
//   int tmp = drum[DRUMSIZE-1];
//   int i = DRUMSIZE-1;
//   while ( i > 0 ) { 
//     drum[i] = drum[i-1];
//     i--;
//   }
//   drum[0] = tmp;
// }

