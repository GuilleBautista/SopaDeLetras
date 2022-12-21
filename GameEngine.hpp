#include<iostream>
#include<string>
#include<algorithm>

using std::cin;
using std::cout;
using std::string;


typedef struct s{
	int start[2] = { 0,0 };
	int end[2] = { 0,0 };
	string word = "";
	int word_distance = 0;
	int increment[2] = { 0, 0 };
	int distance[2] = { 0, 0 };
} intent;

template <int soup_size, int word_num>
class GameEngine {
private:
	intent current_intent;
	string wordlist[word_num];
	bool words_completed[word_num];
	char soup[soup_size][soup_size];
	int size; //word soup must be size*size
	int nwords;
public:
	GameEngine(char _soup[soup_size][soup_size], string wordlist[word_num]) {
		this->size = soup_size;
		this->nwords = word_num;
		
		for(int i=0; i<soup_size; i++) for(int j=0; j<soup_size; j++) this->soup[i][j] = _soup[i][j];
			
		for (int i = 0; i < word_num; i++) {
			this->wordlist[i] = wordlist[i];
			words_completed[i] = false;
		}

	}

	string printSoup() {
		string out = "";
		out += "  ";
		for (int i = 0; i < size; i++) { out += ' ' + std::to_string(i); }
		out += '\n';
		for (size_t i = 0; i < size; i++) {
			out += std::to_string(i) + ": ";
			for (size_t j = 0; j < size; j++)
				out += string(1, this->soup[i][j]) + ' ';
			out += '\n';
		}
		return out;
	}

	string printRemainingWords() {
		string out = "Palabras restantes: ";
		for (size_t i = 0; i < nwords; i++) {
			if(!words_completed[i]) out += this->wordlist[i] + " ";
		}
		out += "\n";
		return out;
	}

	//Function to get the line with coordinates and word
	//returns a search intent initialized to that data
	intent parseArgs(string raw_input) {
		int coords[4] = { -1, -1, -1, -1 };
		string word = raw_input;
		try {
			for (int i = 0; i < 4; i++) {
				coords[i] = std::stoi(word.substr(0, word.find(' ')));
				if (coords[i] < 0 || coords[i] >= size) {
					cout << "Exception, wrong size: " << coords[i] << " in coord " << i << "\n";
				}
				word.erase(0, word.find(' ') + 1);
			}
		}
		catch (const std::exception& ex) {
			cout << "Introduce las coordenadas y la palabra en el fomato indicado...\n";
		}

		//Vector representing direction, distance and word size
		int wordv[2] = {coords[2] - coords[0], coords[3] - coords[1]};
		//Vector modulus (word discrete length)
		int vmodulus = std::max(abs(wordv[0]), abs(wordv[1]));
		
		if (!vmodulus) {
			cout << "Las coordenadas no forman una linea recta, introduce unas coordenadas validas...\n";
			intent i = intent{ {0}, {0}, "", -1, {0}, {0} };
			return i;
		}

		//Create a search intent to return
		intent si = intent{
			{coords[0], coords[1]}, 
			{coords[2], coords[3]},
			word,
			vmodulus + 1,
			{(int)(wordv[0] / vmodulus), (int)(wordv[1] / vmodulus)},
			{wordv[0], wordv[1]},
		};

		//Check if both positions make a (valid) straight line
		if ((si.distance[0] != 0 && si.distance[1] != 0 && abs(si.increment[0]) != abs(si.increment[1]) )) {
			cout << "Las coordenadas no forman una linea recta, introduce unas coordenadas validas...\n";
		}
	
		return si;
	}

	int checkWord(intent si) {
		bool found = false;
		for (auto s : this->wordlist) {
			if (si.word == s) {
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "La palabra no se encuentra en la lista de palabras...\n";
			return 0;
		}
		if (si.word.length() != si.word_distance) {
			cout << "La longitud de la palabra es distinta de la distancia entre ambos puntos\n";
			return 0;
		}
		for (int i = 0, x = si.start[0], y = si.start[1]; i < si.word.length();
			x += si.increment[0], y += si.increment[1], i++) {
			if (si.word.at(i) != this->soup[x][y]) {
				cout << "Letra " << i << " incorrecta: " << si.word.at(i) << " != " << this->soup[x][y] << std::endl;
				return 0;
			}
		}
		return 1;
	}

	void markWord(string w) {
		for (int i = 0; i < this->nwords; i++) {
			if (wordlist[i] == w) {
				this->words_completed[i] = true;
				break;
			}
		}
	}
	
	bool checkWin() {
		for (int i = 0; i < nwords; i++) {
			if (!this->words_completed[i]) return false;
		}
		return true;
	}

	void mainLoop() {
		while (1) {
			cout << this->printSoup() << this->printRemainingWords();

			cout << "Introduce las coordenadas y la palabra en formato \"fila1 columna1 fila2 columna2 palabra\":\n";
			string raw_input;
			std::getline(std::cin, raw_input);

			intent guess = this->parseArgs(raw_input);

			if (this->checkWord(guess)) {
				cout << "Enhorabuena!\n";
				this->markWord(guess.word);
			}
			else {
				cout << "Prueba otra vez...\n";
			}
			if (this->checkWin()) {
				cout << "Encontraste todas las palabras, bien hecho!\n";
				break;
			}
		}
	}

};

/*
int main() {
	const int size = 3, nwords = 1;
	char soup[size][size] = { {'a', 'b', 'c'}, {'a', 'b', 'c'}, {'a', 'b', 'c'} };
	string wordlist[nwords] = { "ba" };

	GameEngine<size, nwords> ge = GameEngine<size, nwords>(soup, wordlist);

	ge.mainLoop();
}
*/