#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <format>
#include <iostream>

enum class ConversionMode
{
	TO_MORSE,
	FROM_MORSE
};

std::vector<std::string> morse_code = {
	".-",		// A
	"-...",		// B
	"-.-.",		// C
	"-..",		// D
	".",		// E
	"..-.",		// F
	"--.",		// G
	"....",		// H
	"..",		// I
	".---",		// J
	"-.-",		// K
	".-..",		// L
	"--",		// M
	"-.",		// N
	"---",		// O
	".--.",		// P
	"--.-",		// Q
	".-.",		// R
	"...",		// S
	"-",		// T
	"..-",		// U
	"...-",		// V
	".--",		// W
	"-..-",		// X
	"-.--",		// Y
	"--..",		// Z

	"-----",	// 0
	".----",	// 1
	"..---",	// 2
	"...--",	// 3
	"....-",	// 4
	".....",	// 5
	"-....",	// 6
	"--...",	// 7
	"---..",	// 8
	"----.",	// 9
};

std::vector<std::string> SplitByDelimiter(const std::string message, const std::string delimiter)
{
	std::vector<std::string> result;

	size_t last = 0;
	size_t next = 0;

	while ((next = message.find(delimiter, last)) != std::string::npos)
	{
		result.push_back(message.substr(last, next - last));
		last = next + 1;
	}

	result.push_back(message.substr(last));
	return result;
}

bool IsValidInputToMorse(const std::string message)
{
	/*
	* Check if the input string is valid to convert to Morse code. Conditions:
	* - Not empty
	* - Only alphanumeric and spaces
	* - Not fully consisting of spaces				
	*/

	bool valid_input = true;

	if (message.empty() == true)
	{
		valid_input = false;
	}
	else
	{
		int num_spaces = 0;

		for (auto c : message)
		{
			if (c == ' ')
			{
				num_spaces++;
			}

			if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' '))
			{
				valid_input = false;
			}
		}

		if (num_spaces == message.length())
		{
			valid_input = false;
		}
	}

	return valid_input;
}

bool IsValidInputFromMorse(const std::string message)
{
	/*
	* Check if the input string is valid to convert from Morse code. Conditions:
	* - Not empty
	* - Only dots, dashes, spaces and slashes
	* - Not fully consisting of spaces or slashes 				
	*/

	bool valid_input = true;

	if (message.empty() == true)
	{
		valid_input = false;
	}
	else
	{
		int num_spaces = 0;
		int num_slashes = 0;

		for (auto c : message)
		{
			if (c == ' ')
			{
				num_spaces++;
			}

			if (c == '/')
			{
				num_slashes++;
			}

			if (!(c == '.' || c == '-' || c == ' ' || c == '/'))
			{
				valid_input = false;
				break;
			}
		}

		if (num_spaces == message.length() || num_slashes == message.length())
		{
			valid_input = false;			
		}
	}

	return valid_input;
}

std::string TranslateToMorse(std::string message)
{
	/*n
	* 1. Transform the input to uppercase
	* 2. Check for valid input 
	* 3. Split the input message by spaces, into individual words
	* 4. Translate the characters of each word into their Morse representation, adding to an output string
	* 5. Add spaces after each character except the last, and slashes after each word except the last
	*/

	std::string output;

	std::transform(message.begin(), message.end(), message.begin(), ::toupper);

	if (IsValidInputToMorse(message))
	{
		std::vector<std::string> split_message = SplitByDelimiter(message, " ");

		for (int word = 0; word < split_message.size(); word++)
		{
			for (int character = 0; character < split_message[word].length(); character++)
			{
				int offset = (isalpha(split_message[word][character])) ? 65 : 22;

				output += morse_code[split_message[word][character] - (long long)offset];

				if (split_message[word].length() > 0 && character < split_message[word].length() - 1)
				{
					output += " ";
				}
			}

			if (split_message.size() > 0 && word < split_message.size() - 1)
			{
				output += " / ";
			}
		}
	}
	else
	{
		output = "Invalid input.";
	}

	return output;
}

std::string TranslateFromMorse(const std::string message)
{
	/*
	* 1. Check input is valid
	* 2. Split the input message by word separator "/"
	* 3. Split each word by spaces
	* 4. Translate each character into it's ASCII representation, add it to an output string
	* 5. Add a space at the end of each word except the last word
	*/

	std::string output;

	if (IsValidInputFromMorse(message))
	{
		std::vector<std::string> message_words = SplitByDelimiter(message, "/");

		for (int word = 0; word < message_words.size(); word++)
		{
			std::vector<std::string> split_message = SplitByDelimiter(message_words[word], " ");

			for (int character = 0; character < split_message.size(); character++)
			{
				for (int morse = 0; morse < morse_code.size(); morse++)
				{
					if (split_message[character] == morse_code[morse])
					{
						int offset = (morse <= 25) ? 65 : 25;

						output += morse + offset;
					}
				}				
			}
			if (message_words.size() > 0 && word < message_words.size() - 1)
			{
				output += " ";
			}
		}
	}
	else
	{
		output = "Invalid input.";
	}

	return output;
}

void TranslateMessage(const std::string message, const enum ConversionMode conversion_mode)
{
	if (conversion_mode == ConversionMode::TO_MORSE)
	{
		std::cout << "Output: " << TranslateToMorse(message) << "\n\n";
	}
	else if (conversion_mode == ConversionMode::FROM_MORSE)
	{
		std::cout << "Output: " << TranslateFromMorse(message) << "\n\n";
	}
}

int main()
{
	bool running = true;
	
	while (running == true)
	{
		std::cout << "TELEGRAM: Convert between text and Morse code.\n";
		std::cout << "[1] to convert TO morse.\n[2] to convert FROM morse.\n[q] to quit program.\n";

		std::cout << "\nInput: ";
		std::string input;
		std::getline(std::cin, input);
		ConversionMode conversion_mode{};
		
		if (input == "q")
		{
			running = false;
			break;
		}

		if (input == "1")
		{
			conversion_mode = ConversionMode::TO_MORSE;
		}

		if (input == "2")
		{
			conversion_mode = ConversionMode::FROM_MORSE;
		}

		if (input == "1" || input == "2")
		{
			system("cls");
			std::string message;
			std::cout << "Input: ";
			std::getline(std::cin, message);
			TranslateMessage(message, conversion_mode);
		}
	}

	return 0;
}