#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <iterator> 
#include <unordered_map>
#include <map>

using namespace std;

int main() {


	map<char, int> freqMap;
	map<char, int> freqMapCopy;

	//Reading in Ciphertext line by line and getting character frequency:
	ifstream file("ciphertext.txt");
	string line;

	while (getline(file, line)) {
		int lineLen = line.size();
		
		for (int i = 0; i < lineLen; i++) {
			map<char, int>:: iterator it = freqMap.find(line[i]);

			if(it != freqMap.end()){
				it->second++;
			}

			else {
				freqMap.insert(pair <char, int> (line[i],1));
			}
		}
	}
	file.close();
		
	// Check to see if all alphabets are present in the map and populates the map with the keys not present and assigns a value = 0
	for (char alphabet = 'A'; alphabet <= 'Z'; alphabet++ ) {
		map<char, int>:: iterator freqIt =  freqMap.find(alphabet);

		if (freqIt == freqMap.end()) {
			freqMap.insert(pair<char, int> (alphabet,0));
		}
	}

	//Displays frequency list of ciphertext to terminal
	cout << "******************************************************************\n";
	cout << "This is the frequency occurence of the alphabets in the ciphertext.\n";
	
	for (auto &e: freqMap) {
		cout << '{' << e.first << ',' << e.second << '}' << '\n';
		freqMapCopy.insert(pair<char, int> (e.first, e.second)); //Populates the copy of the freqMap used for later in the code. MAY NOT NEED; COME BACK TO THIS!!!!!!!
	}
	
	//Mapping Known English frequency with frequency from ciphertext
	string alphabet = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
        int alphaLen = alphabet.size();
	char realMax;
	map<char, char> mapping; // created map which maps frequency of ciphertext char to standard English alphabets

	for (int i = 0; i < alphaLen; i++) {
		int currentMax = 0;

		for(auto it = freqMap.begin(); it != freqMap.end(); ++it ) {
			if (it->second > currentMax) {
				realMax = it->first;
				currentMax = it->second;
			}
			else if (it->second == currentMax) { //Appends Y into the map since it has a freq count of 0
				realMax = it->first;
			}
		}
		mapping.insert(pair<char, char>(realMax,alphabet[i]));
		freqMap.erase(realMax);
	}
	
	//Printing the Mapping map to the terminal
	cout << "\n******************************************************************\n";
	cout << "This is the mapping of the alphabets in the ciphertext to standard english alphabet frequency.\n";
	
	for (auto &e: mapping) {
		cout << '{' << e.first << ',' << e.second << '}' << '\n';
	}	
	
	//Time to decypher the ciphertext:
	int score = 0;

	ofstream myfile; // Writing to this myfile variable which is plaintext.txt.
	myfile.open ("plaintext.txt"); // Open plaintext.txt to write the decyphered plaintext into it

	cout << "\n******************************************************************\n";
	cout << "Time to decypher the cipher text: \n";
	cout << "\nHere is the plaintext:: \n\n";

	//Appending plaintext charachters into plaintext.txt
	ifstream IPfile("ciphertext.txt");
	while(getline(IPfile, line)) { 
		int lineLen = line.size();	
		for (int i = 0; i <= lineLen; i++) {
			if (i == lineLen){ //Checks for end of line so the next line can be started
				myfile << '\n'; // appends new line into plaintext
			}

			for (auto &e: mapping) {
				if (e.first == line[i]) {
					myfile << e.second;
				}
			}
		}
	}
	IPfile.close();
	myfile.close();

	//prints the plaintext to the terminal.
	ifstream OPfile("plaintext.txt");

        while (getline(OPfile, line)) {
		cout << line << '\n';
	}
	OPfile.close();


	// time to compute score of key by checking to see if dictionary words are present in plaintext.
	string dictLine;
	string plainLine;

	int plainLineLen;
	int dictLineLen;
	cout << "\nStarting score computing:::\n";

	//Looping thru lines of plaintext and dictionary file 
	ifstream Ofile("plaintext.txt");

	while (getline(Ofile, plainLine)) {
		ifstream dictFile("Dictionary.txt");
		while(getline(dictFile,dictLine)) {

			plainLineLen = plainLine.length();
			dictLineLen = dictLine.length();

			for (int i = 0; i < plainLineLen; i++) {
				for (int j = 1; j <= dictLineLen; j++) {
					if (dictLine == plainLine.substr(i,j)) { //Conditional statement checks if substring of the plaintext == the dictionary word
						score++;
					}
				}
			}
		}
		dictFile.close();
	}
	cout << "\nThis is the score for the key: " << score << '\n';
	Ofile.close();

	//Code to Generate multiple keys for system to try out which is stored in a text file.
	int j = 0;
	ofstream keyfile; // Writing to this keyfile variable which is key.txt.
        keyfile.open ("key.txt"); //Open key.txt to write generated keys into it
	
	for (int i = 0; i < alphabet.length()-1; i++) {
		alphabet += alphabet[j];
		alphabet.erase(0, 1);
		keyfile << alphabet << '\n';
	}
	keyfile.close();
	
	//Clearing plaintext.txt file to store other plaintext for other keys
	ofstream ofs;
	ofs.open ("plaintext.txt", ofstream:: out | ofstream:: trunc);
	ofs.close();

	//Implementing new generated keys against ciphertext
	ifstream kfile("key.txt");
	string keyLine;
	alphabet = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
	string bestKey = alphabet;
	int bestScore = score;

	while(getline(kfile,keyLine)) {
		map<char, char> cmapping;
		cout << "\n******************************************************************\n";
		cout << "The new key is:: " << keyLine << '\n';

		//Appending into cmapping map
		for (int i = 0; i < alphabet.length(); i++) {
			for (auto &e: mapping) {
				if(alphabet[i] == e.second) {
					cmapping.insert(pair<char, char>(e.first,keyLine[i]));
				}
			}
		}

		// Prints current key mapping to terminal
		for (auto &e: cmapping) {
			cout << '{' << e.first << ',' << e.second << '}' << '\n';
		}

		// WRITING NEW PLAINTEXT INTO PLAINTEXT.TXT

		ofstream myfile; // Writing to this myfile variable which is plaintext.txt.
        	myfile.open ("plaintext.txt");
		cout << "\n******************************************************************\n";
	        cout << "Time to decypher the cipher text: \n";
        	cout << "\nHere is the plaintext:: \n\n";

        	//Appending plaintext characters into plaintext.txt

		ifstream IPfile("ciphertext.txt");
	        while(getline(IPfile, line)) {
			int lineLen = line.size();
			for (int i = 0; i <= lineLen; i++) {
				if (i == lineLen){ //Checks for end of line so the next line can be started
                                	myfile << '\n'; // appends new line into plaintext
				}

				for (auto &e: cmapping) {
                                	if (e.first == line[i]) {
                                        	myfile << e.second;
                                	}
                        	}
			}
		}
		IPfile.close();
        	myfile.close();
		
		//prints the plaintext to the terminal.
		ifstream OPfile("plaintext.txt");
        	while (getline(OPfile, line)) {
                	cout << line << '\n';
        	}
        	OPfile.close();
		
		//Time to compute score of key by checking to see if dictionary words are present in plaintext.
		//Looping thru lines of plaintext and dictionary file 
        	score = 0;
		cout << "\nStarting score computing:::\n";
		ifstream Ofile("plaintext.txt");

		while (getline(Ofile, plainLine)) {
			ifstream dictFile("Dictionary.txt");
                	while(getline(dictFile,dictLine)) {
				
				plainLineLen = plainLine.length();
                        	dictLineLen = dictLine.length();
				
				for (int i = 0; i < plainLineLen; i++) {
					for (int j = 1; j <= dictLineLen; j++) {
						if (dictLine == plainLine.substr(i,j)) { //Conditional statement checks if substring of the plaintext == the dictionary word
							score++;
						}
					}
				}
			}
			dictFile.close();
		}
		cout << "\nThis is the score for the key: " << score << '\n';
        	Ofile.close();


		if (score > bestScore) {
			bestScore = score;
			bestKey = keyLine; 
		}


		//Clearing plaintext.txt file to store other plaintext for other keys
        	ofstream ofs;
        	ofs.open ("plaintext.txt", ofstream:: out | ofstream:: trunc);
        	ofs.close();


		
		cmapping.clear(); // Clears cmapping map for other key mapping
	}

	cout << "\nThe best Key generated is:: " << bestKey << '\n';
	cout << "\nThe best score generated is:: " << bestScore << '\n';


	//Interactive portion of code for UI
	bool run = true;
	string answer;
	string currentKey;
	char initialChar;
	char finalChar;
	int initialCharPos;
	int finalCharPos;
	int count = 0;
	map<char, char> nextMapping;

	while (run) {
		cout << "Will you like to attempt to get a better key(y/n)?? \n";
		cin >> answer;

		if ((answer == "y" || answer == "Y") && count < 1) {
			cout << "\n\n\nThis is the best key map:: \n";
			
			map<char, char> cmapping;
			//Repopluates cmapping map with bestKey mapping on my first user interaction.
			for (int i = 0; i < alphabet.length(); i++) {
				for (auto &e: mapping) {
					if(alphabet[i] == e.second) {
						cmapping.insert(pair<char, char>(e.first,bestKey[i]));
					}
				}
			}
			
			// Prints current key mapping to terminal
			cout << "\n this is your best mapping so far::: \n";
			for (auto &e: cmapping) {
				cout << '{' << e.first << ',' << e.second << '}';
			}

			cout << "\n\nWhat characters in the map do you want to switch::\n"; 
			cout << "initial Character: ";
			cin >> initialChar;
			cout << "final Character: ";
			cin >> finalChar;
			

			for (auto &e: cmapping) {
				if (e.second == initialChar) {
					e.second = finalChar;
				}

				else if (e.second == finalChar) {
					e.second = initialChar;
				}
			}
			
			//This generates the new current key gotten from switching out characters
			for (auto &e: cmapping) {
				currentKey += e.second;
			}

			nextMapping = cmapping; // Assignns nextMapping a copy of the switch up mapping
			
			//Deciphering message with new key
			//
			ofstream myfile; // Writing to this myfile variable which is plaintext.txt.
        		myfile.open ("plaintext.txt");

			cout << "\n******************************************************************\n";
	        	cout << "Time to decypher the cipher text: \n";
        		cout << "\nHere is the plaintext:: \n\n";

			//Appending plaintext characters into plaintext.txt
		        ifstream IPfile("ciphertext.txt");
        		while(getline(IPfile, line)) {
				int lineLen = line.size();
                		for (int i = 0; i <= lineLen; i++) {
					if (i == lineLen){ //Checks for end of line so the next line can be started
                                		myfile << '\n'; // appends new line into plaintext
					}

					for (auto &e: cmapping) {
                                		if (e.first == line[i]) {
                                        		myfile << e.second;
                                		}
                        		}
				}
			}
			IPfile.close();
        		myfile.close();

			//prints the plaintext to the terminal.
	                ifstream OPfile("plaintext.txt");
        	        while (getline(OPfile, line)) {
                	        cout << line << '\n';
                	}
                	OPfile.close();

			//Computing Score again, LET'S GOOOOOOOO
			//
			score = 0;
                	cout << "\nStarting score computing:::\n";
                	ifstream Ofile("plaintext.txt");

                	while (getline(Ofile, plainLine)) {
				ifstream dictFile("Dictionary.txt");
	                        while(getline(dictFile,dictLine)) {
					plainLineLen = plainLine.length();
                                	dictLineLen = dictLine.length();

                                	for (int i = 0; i < plainLineLen; i++) {
						for (int j = 1; j <= dictLineLen; j++) {
							if (dictLine == plainLine.substr(i,j)) {
								score++;
							}
						}
					}
				}
				dictFile.close();
			}
			cout << "\nThis is the score for the current switched key: " << score << '\n';
			cout << "This is the current key we have:: " << currentKey << '\n';
			Ofile.close();


			if (score > bestScore) {
                	        bestScore = score;
				bestKey = currentKey;
				cout << "\nYour new key has a better score!!! It is::: " << bestScore << '\n';
				cout << "The best Key generated is:: " << bestKey << '\n';
	
				//Reading into finalPlainText.txt file
				ofstream finalFile;
				finalFile.open("finalPlainText.txt");
				ifstream OPfile("plaintext.txt");

				while (getline(OPfile,line)) {
					finalFile << line;
					finalFile << '\n';
				}
				finalFile.close();
				OPfile.close();
	                }

			// Empties cmapping map
			cmapping.clear();

			currentKey = ""; // empties currentKey string

			//Clearing plaintext.txt file to store other plaintext for other keys
                	ofstream ofs;
                	ofs.open ("plaintext.txt", ofstream:: out | ofstream:: trunc);
                	ofs.close();

			count++;
		}


		else if ((answer == "y" || answer == "Y") && count >= 1) {
			cout << "\n\n\nThis is the current key map:: \n";
			for (auto &e: nextMapping) {
				cout << '{' << e.first << ',' << e.second << '}';
			}

			cout << "\n\nWhat characters in the map do you want to switch::\n";
                        cout << "initial Character: ";
                        cin >> initialChar;
                        cout << "final Character: ";
                        cin >> finalChar;

			for (auto &e: nextMapping) {
                                if (e.second == initialChar) {
                                        e.second = finalChar;
                                }

                                else if (e.second == finalChar) {
                                        e.second = initialChar;
                                }
			}

                        //This generates the new current key gotten from switching out characters
			for (auto &e: nextMapping) {
                                currentKey += e.second;
			}

			//Deciphering message with new key
                        //
                        ofstream myfile; // Writing to this myfile variable which is plaintext.txt.
                        myfile.open ("plaintext.txt");

                        cout << "\n******************************************************************\n";
                        cout << "Time to decypher the cipher text: \n";
                        cout << "\nHere is the plaintext:: \n\n";


			//Appending plaintext charachters into plaintext.txt
                        ifstream IPfile("ciphertext.txt");
                        while(getline(IPfile, line)) {
                                int lineLen = line.size();
                                for (int i = 0; i <= lineLen; i++) {
                                        if (i == lineLen){ //Checks for end of line so the next line can be started
                                                myfile << '\n'; // appends new line into plaintext
                                        }

                                        for (auto &e: nextMapping) {
                                                if (e.first == line[i]) {
                                                        myfile << e.second;
                                                }
                                        }
				}
			}
			IPfile.close();
                        myfile.close();

			//prints the plaintext to the terminal.
                        ifstream OPfile("plaintext.txt");
                        while (getline(OPfile, line)) {
                                cout << line << '\n';
                        }
                        OPfile.close();


			//Computing Score again, LET'S GOOOOOOOO
                        //
                        score = 0;
                        cout << "\nStarting score computing:::\n";
                        ifstream Ofile("plaintext.txt");


			while (getline(Ofile, plainLine)) {
                                ifstream dictFile("Dictionary.txt");
                                while(getline(dictFile,dictLine)) {
                                        plainLineLen = plainLine.length();
                                        dictLineLen = dictLine.length();

                                        for (int i = 0; i < plainLineLen; i++) {
                                                for (int j = 1; j <= dictLineLen; j++) {
                                                        if (dictLine == plainLine.substr(i,j)) {
                                                                score++;
                                                        }
                                                }
                                        }
                                }
                                dictFile.close();
                        }
                        cout << "\nThis is the score for the current switched key: " << score << '\n';
                        cout << "This is the current key we have:: " << currentKey << '\n';
                        Ofile.close();
			
			if (score > bestScore) {
				bestScore = score;
                                bestKey = currentKey;

				cout << "\nYour new key has a better score!!! It is::: " << bestScore << '\n';				
                                cout << "The best Key generated is:: " << bestKey << '\n';

				//Reading into finalPlainText.txt file
				ofstream finalFile;
                                finalFile.open("finalPlainText.txt");
                                ifstream OPfile("plaintext.txt");

                                while (getline(OPfile,line)) {
                                        finalFile << line;
                                        finalFile << '\n';
                                }
                                finalFile.close();
                                OPfile.close();
			}

			currentKey = "";
			//Clearing plaintext.txt file to store other plaintext for other keys
                        ofstream ofs;
                        ofs.open ("plaintext.txt", ofstream:: out | ofstream:: trunc);
                        ofs.close();


		}

		else {
			run = false;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////
}
