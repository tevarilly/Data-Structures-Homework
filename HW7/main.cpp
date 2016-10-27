// ==========================================================================
// HOMEWORK 7 WORD FREQUENCY MAPS
// ==========================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cassert>
#include <cstdlib>

// ==========================================================================
//A map with a string as the key. This first layer consists of all the words in the text
//  Each is associated with another map of with strings as the key
//  Each of these strings are associated with another map with strings as the key and associated integers of occurrences
typedef  std::map<std::string, std::map<std::string, std::map<std::string, int> > >  my_map;
typedef  std::map<std::string, std::map<std::string, int> > my_map2;
// ==========================================================================

//Some function prototypes
void print_3window_3words(my_map data, std::vector<std::string> sentence);
void print_3window_2words(my_map data, std::vector<std::string> sentence);

// ==========================================================================
// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) 
{
    char c;
    word.clear();

    while(istr) 
    {
        // just "peek" at the next character in the stream
        c = istr.peek();
        if(isspace(c)) 
        {
            // skip whitespace before a word starts
            istr.get(c);

            if(word != "") 
            {
	            // break words at whitespace
	            return true;
            }
        } 

        else if(c == '"') 
        {
            // double quotes are a delimiter and a special "word"
            if(word == "") 
            {
	            istr.get(c);
	            word.push_back(c);
            }

            return true;
        } 

        else if(isalpha(c)) 
        {
            // this a an alphabetic word character
            istr.get(c);
            word.push_back(tolower(c));
        } 

        else 
        {
            // ignore this character (probably punctuation)
            istr.get(c);
        }
    }

    return false;
}
// ==========================================================================

// ==========================================================================
// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) 
{
    // returns a vector of strings of the different words
    std::vector<std::string> answer;
    std::string word;
    bool open_quote = false;

    while(ReadNextWord(istr,word)) 
    {
        if(word == "\"") 
        {
            if(open_quote == false) 
            { 
                open_quote=true; 
            }

            else 
            { 
                break; 
            }
        } 

        else 
        {
            // add each word to the vector
            answer.push_back(word);
        }
    }
    return answer;
}
// ==========================================================================

// ==========================================================================
// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(my_map &data, my_map2 &data2, const std::string &filename, int window, const std::string &parse_method) 
{
    // open the file stream
    std::ifstream istr(filename.c_str());

    if(!istr) 
    { 
        std::cerr << "ERROR cannot open file: " << filename << std::endl; 
        exit(1);
    } 

    // verify the window parameter is appropriate
    if(window < 2) 
    {
        std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
    }

    // verify that the parse method is appropriate
    bool ignore_punctuation = false;

    if(parse_method == "ignore_punctuation") 
    {
        ignore_punctuation = true;
    } 

    else 
    {
        std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
        exit(1);
    }

    //Use different data types for window = 2 and window = 3
    if(window == 3)
    {
        std::string word;
        std::string word_prev;
        std::string word_prev_prev;

        int count = 0;

        //For every word in the file
        while(ReadNextWord(istr,word)) 
        {
            // skip the quotation marks (not used for this part)
            if (word == "\"") continue;

            //If you've already read in the first two words
            if( count > 1)
            {
                //Add 3 consecutive words to data map
                ++data[word_prev_prev][word_prev][word];
            }

            //If you haven't yet read in the first two words
            else
            {
                count++;
            }

            //Redefine your word additions
            word_prev_prev = word_prev;
            word_prev = word;
        }

        //Account for the last strings of 3 words and add those to the map
        ++data[word_prev_prev][word_prev][word];
        ++data[word_prev][word][" "];
        ++data[word][" "][" "];
    }

    //Use different data types for window = 2 and window = 3
    if(window == 2)
    {
        std::string word;
        std::string word_prev;

        int count = 0;

        //For every word in the file
        while(ReadNextWord(istr,word))
        {
            //If you've already read in the first word
            if(count > 0)
            {
                //Add 2 consecutive words to data map
                ++data2[word_prev][word];
            }

            //If you haven't yet read in the first word
            else
            {
                count++;
            }

            //Redefine your word additions
            word_prev = word;
        }

        //Account for the last strings of 3 words and add those to the map
        ++data2[word_prev][word];
        ++data2[word][" "];    
    }
}
// ==========================================================================

// ==========================================================================
void print(my_map data, my_map2 data2, int window, std::vector<std::string> sentence)
{
    //If we're using the my_map2 type
    if(window == 2)
    {
        //For iterating through top level map
        my_map2::const_iterator it;

        int occurrences = 0;

        //If only one word is inputted
        if(sentence.size() == 1)
        {
            //For every top level word in our map
            for(it = data2.begin(); it != data2.end(); ++it)
            {
                //If you find the singular word
                if(it->first == sentence[0])
                {
                    //For iterating through second level map
                    std::map<std::string, int>::const_iterator it2;

                    //For every secondary word
                    for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        //Add how many times the first word is used
                        occurrences += it2->second;
                    }
                }
            }

            //Output total occurrences of the initial word
            std::cout << sentence[0] << " (" << occurrences << ")\n";

            //For every top level word in our map
            for(it = data2.begin(); it != data2.end(); ++it)
            {
                //If you find the singular word
                if(it->first == sentence[0])
                {
                    //For iterating through second level map
                    std::map<std::string, int>::const_iterator it2;

                    //For every secondary word
                    for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        if(it2->first[0] > 0 && it2->first != " ")
                        {
                            //Print out each secondary word and the corresponding count
                            std::cout << sentence[0] << " " << it2->first << " (" << it2->second << ")\n";
                        }
                    }
                }
            }
        }

        //If a string of two words is inputted
        if(sentence.size() == 2)
        {
            //For every top level word in our map
            for(it = data2.begin(); it != data2.end(); ++it)
            {
                //If you find the first word
                if(it->first == sentence[0])
                {
                    //For iterating through the second level map
                    std::map<std::string, int>::const_iterator it2;

                    //For every secondary word
                    for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        //If you find the second word inputted
                        if(it2->first == sentence[1])
                        {
                            //Output the total string with number of occurences
                            std::cout << sentence[0] << " " << sentence[1] << " (" << it2->second << ")\n";
                        }
                    }
                }
            }
            
        }
    }

    //Otherwise, if we're using the my_map type
    else if(window == 3)
    {
        //For iterating through the top level map
        my_map::const_iterator it;

        int occurrences_first = 0;
        std::vector<int> occurrences_second;
        int occurrences_third = 0;

        //If the user enters only one word
        if(sentence.size() == 1)
        {
            //For every top level word in our map
            for(it = data.begin(); it != data.end(); ++it)
            {
                //If you find the first string
                if(it->first == sentence[0])
                {
                    //For iterating through second level map
                    my_map2::const_iterator it2;

                    //For every second level word in our map
                    for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        //For iterating through third level map
                        std::map<std::string, int>::const_iterator it3;

                        //For every third level word in our map
                        for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
                        {
                            occurrences_third += it3->second;
                            occurrences_first += it3->second;
                            
                        }

                        occurrences_second.push_back(occurrences_third);

                        occurrences_third = 0;
                    }
                }
            }

            std::cout << sentence[0] << " (" << occurrences_first << ")\n";

            //For every top level word in our map
            for(it = data.begin(); it != data.end(); ++it)
            {
                //If you find the first string
                if(it->first == sentence[0])
                {
                    //For iterating through second level map
                    my_map2::const_iterator it2;

                    int count = 0;

                    //For every second level word in our map
                    for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        std::cout << sentence[0] << " " << it2->first << " (" << occurrences_second[count] << ")\n";

                        count++;
                    }
                }
            }
        }

        //If the user enters a two word string
        if(sentence.size() == 2)
        {
            //For every top level word in our map
            for(it = data.begin(); it != data.end(); ++it)
            {
                //If you find the first string
                if(it->first == sentence[0])
                {
                    //For iterating through second level map
                    my_map2::const_iterator it2;

                    //For every second level word in our map
                    for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        //If you find the second string
                        if(it2->first == sentence[1])
                        {
                            //For iterating through third level map
                            std::map<std::string, int>::const_iterator it3;

                            //For every third level word in our map
                            for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
                            {
                                //Add up all the times both words appear together
                                occurrences_first += it3->second;
                            }
                        }
                    }
                }
            }

            //Output total occurrences of the two word string
            std::cout << sentence[0] << " " << sentence[1] << " (" << occurrences_first << ")\n";

            print_3window_2words(data, sentence);
        }

        //If the user enters a 3 word string
        if(sentence.size() == 3)
        {
            print_3window_3words(data, sentence);
        }
    }

    else
    {
        std::cout << "window error\n";
    }
}
// ==========================================================================

// ==========================================================================
//Helper function that prints out the amounts of a specified 3 word string
//  when the window chosen is 3
void print_3window_3words(my_map data, std::vector<std::string> sentence)
{
    //For iterating through the top level map
    my_map::const_iterator it;

    //For every top level word in our map
    for(it = data.begin(); it != data.end(); ++it)
    {
        //If you find the first string
        if(it->first == sentence[0])
        {
            //For iterating through second level map
            my_map2::const_iterator it2;

            //For every second level word in our map
            for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                //If you find the second string
                if(it2->first == sentence[1])
                {
                    //For iterating through third level map
                    std::map<std::string, int>::const_iterator it3;

                    //For every third level word in our map
                    for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) 
                    {
                        //If you find the third string
                        if(it3->first == sentence[2])
                        {
                            std::cout << sentence[0] << " " << sentence[1] << " " << sentence[2] << " (" << it3->second << ")\n";
                        }
                    }
                }
            }
        }
    }
}
// ==========================================================================

// ==========================================================================
//Helper function which prints out the amounts of a specified 2 word string
//  when the window chosen is 2
void print_3window_2words(my_map data, std::vector<std::string> sentence)
{
    //For iterating through the top level map
    my_map::const_iterator it;

    //For every top level word in our map
    for(it = data.begin(); it != data.end(); ++it)
    {
        //If you find the first string
        if(it->first == sentence[0])
        {
            //For iterating through second level map
            my_map2::const_iterator it2;

            //For every second level word in our map
            for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                //If you find the second string
                if(it2->first == sentence[1])
                {
                    //For iterating through third level map
                    std::map<std::string, int>::const_iterator it3;

                    //For every third level word in our map
                    for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
                    {
                        std::vector<std::string> three_words;

                        three_words.push_back(sentence[0]);
                        three_words.push_back(sentence[1]);
                        three_words.push_back(it3->first);

                        print_3window_3words(data, three_words);
                    }
                }
            }
        }
    }

}
// ==========================================================================

// ==========================================================================
//Function that generates a plausible sentence of specified length based upon
//  common occurrences
void GenerateCommon(my_map data, my_map2 data2, std::vector<std::string> sentence, int length, int count, std::string word, std::string second_word, int window)
{
    std::vector<int> occurrences;
    std::vector<std::string> secondary_words;
    std::vector<std::string> tertiary_words;
    std::string new_word;

    //If you are only starting with one word
    if(sentence.size() == 1 && window == 2)
    {
        my_map2::const_iterator it;

        if(count == 0)
        {
            //Output the first word
            std::cout << word << " ";
        }

        else
        {
            std::cout << " ";
        }

        //For every word in the first level of the map
        for(it = data2.begin(); it != data2.end(); ++it)
        {
            //If you find the current word
            if(it->first == word)
            {
                //For iterating through second level map
                std::map<std::string, int>::const_iterator it2;

                //For every word in the second level of the map
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //Add occurrences of each secondary word to a vector
                    occurrences.push_back(it2->second);
                    secondary_words.push_back(it2->first);
                }
            }
        }

        int max = 0;
        int max_index = 0;

        //For every value in the vector
        for(uint i = 0; i<occurrences.size(); ++i)
        {
            //If the current value is larger than the current max
            if(occurrences[i] > max)
            {
                //Find the index with the maximum number of occurrences in the vector
                max = occurrences[i];
                max_index = i;
            }
        }

        //Next word with most occurrences is the one at the max_index
        new_word = secondary_words[max_index];

        std::cout << new_word;

        //Keep track of how many times we've run this function
        count++;

        //RECURSE!
        if(count < length)
        {
            GenerateCommon(data, data2, sentence, length, count, new_word, second_word, window);
        } 
    }

    if(sentence.size() == 1 && window == 3)
    {
        my_map::const_iterator it;
        int occurrences_first = 0;

        if(count == 0)
        {
            //Output the first word
            std::cout << word << " ";
        }

        else
        {
            std::cout << " ";
        }

        //For every word in the first level of the map
        for(it = data.begin(); it != data.end(); ++it)
        {
            //If you find the current word
            if(it->first == word)
            {
                //For iterating through second level map
                my_map2::const_iterator it2;

                //For every word in the second level of the map
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //For iterating through third level map
                    std::map<std::string, int>::const_iterator it3;

                    //For every third level word in our map
                    for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
                    {
                        occurrences_first += it3->second;
                    }

                    //Add occurrences of each secondary word to a vector
                    occurrences.push_back(occurrences_first);
                    secondary_words.push_back(it2->first);

                    occurrences_first = 0;
                }
            }
        }

        int max = 0;
        int max_index = 0;

        //For every value in the vector
        for(uint i = 0; i<occurrences.size(); ++i)
        {
            //If the current value is larger than the current max
            if(occurrences[i] > max)
            {
                //Find the index with the maximum number of occurrences in the vector
                max = occurrences[i];
                max_index = i;
            }
        }

        //Next word with most occurrences is the one at the max_index
        new_word = secondary_words[max_index];

        std::cout << new_word;

        //Keep track of how many times we've run this function
        count++;

        //RECURSE!
        if(count < length)
        {
            GenerateCommon(data, data2, sentence, length, count, new_word, second_word, window);
        } 
    }

    if(sentence.size() == 2 && window == 3)
    {
        my_map::const_iterator it;

        if(count == 0)
        {
            //Output the first words
            std::cout << sentence[0] << " " << sentence[1] << " ";
            second_word = sentence[1];
        }

        else
        {
            std::cout << " ";
        }

        //For every word in the first level of the map
        for(it = data.begin(); it != data.end(); ++it)
        {
            //If you find the current word
            if(it->first == word)
            {
                //For iterating through second level map
                my_map2::const_iterator it2;

                //For every word in the second level of the map
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //If you find the second word
                    if(it2->first == second_word)
                    {
                        //For iterating through the third level of the map
                        std::map<std::string, int>::const_iterator it3;

                        //For every word in the third level
                        for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
                        {
                            //Add occurrences of each tertiary word to a vector
                            occurrences.push_back(it3->second);
                            tertiary_words.push_back(it3->first);
                        }
                    }
                }
            }
        }

        int max = 0;
        int max_index = 0;

        //For every value in the vector
        for(uint i = 0; i<occurrences.size(); ++i)
        {
            //If the current value is larger than the current max
            if(occurrences[i] > max)
            {
                //Find the index with the maximum number of occurrences in the vector
                max = occurrences[i];
                max_index = i;
            }
        }

        //Next word with most occurrences is the one at the max_index
        new_word = tertiary_words[max_index];

        std::cout << new_word;

        //Keep track of how many times we've run this function
        count++;

        //RECURSE!
        if(count < length)
        {
            GenerateCommon(data, data2, sentence, length, count, second_word, new_word, window);
        } 

    }
}
// ==========================================================================

// ==========================================================================
//Function that generates a plausible sentence of specified length based upon
//  a random selection from one of the following possible words
void GenerateRandom(my_map data, my_map2 data2, std::vector<std::string> sentence, int length, int count, std::string word, std::string second_word, int window)
{
    std::vector<int> occurrences;
    std::vector<std::string> secondary_words;
    std::vector<std::string> tertiary_words;
    std::string new_word;

    if(sentence.size() == 1 && window == 2)
    {
        my_map2::const_iterator it;

        if(count == 0)
        {
            //Output the first word
            std::cout << word << " ";
        }

        else
        {
            std::cout << " ";
        }

        //For every word in the first level of the map
        for(it = data2.begin(); it != data2.end(); ++it)
        {
            //If you find the current word
            if(it->first == word)
            {
                //For iterating through second level map
                std::map<std::string, int>::const_iterator it2;

                //For every word in the second level of the map
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //Add occurrences of each secondary word to a vector
                    secondary_words.push_back(it2->first);
                }
            }
        }

        int randNum = rand()%((secondary_words.size()-1)-0 + 1) + 0;

        //Get a random plausible next word
        new_word = secondary_words[randNum];

        std::cout << new_word;

        //Keep track of how many times we've run this function
        count++;

        //RECURSE!
        if(count < length)
        {
            GenerateRandom(data, data2, sentence, length, count, new_word, second_word, window);
        } 
    }

    if(sentence.size() == 1 && window == 3)
    {
        my_map::const_iterator it;

        if(count == 0)
        {
            //Output the first word
            std::cout << word << " ";
        }

        else
        {
            std::cout << " ";
        }

        //For every word in the first level of the map
        for(it = data.begin(); it != data.end(); ++it)
        {
            //If you find the current word
            if(it->first == word)
            {
                //For iterating through second level map
                my_map2::const_iterator it2;

                //For every word in the second level of the map
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //Add occurrences of each secondary word to a vector
                    secondary_words.push_back(it2->first);
                }
            }
        }

        int randNum = rand()%((secondary_words.size()-1)-0 + 1) + 0;

        //Get a random plausible next word
        new_word = secondary_words[randNum];

        std::cout << new_word;

        //Keep track of how many times we've run this function
        count++;

        //RECURSE!
        if(count < length)
        {
            GenerateRandom(data, data2, sentence, length, count, new_word, second_word, window);
        }  
    }

    if(sentence.size() == 2 && window == 3)
    {
        my_map::const_iterator it;

        if(count == 0)
        {
            //Output the first words
            std::cout << sentence[0] << " " << sentence[1] << " ";
            second_word = sentence[1];
        }

        else
        {
            std::cout << " ";
        }

        //For every word in the first level of the map
        for(it = data.begin(); it != data.end(); ++it)
        {
            //If you find the current word
            if(it->first == word)
            {
                //For iterating through second level map
                my_map2::const_iterator it2;

                //For every word in the second level of the map
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //If you find the second word
                    if(it2->first == second_word)
                    {
                        //For iterating through the third level of the map
                        std::map<std::string, int>::const_iterator it3;

                        //For every word in the third level
                        for(it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
                        {
                            tertiary_words.push_back(it3->first);
                        }
                    }
                }
            }
        }

        int randNum = rand()%((tertiary_words.size()-1)-0 + 1) + 0;

        new_word = tertiary_words[randNum];

        std::cout << new_word;

        //Keep track of how many times we've run this function
        count++;

        //RECURSE!
        if(count < length)
        {
            GenerateRandom(data, data2, sentence, length, count, second_word, new_word, window);
        }

    }
}
// ==========================================================================

// ==========================================================================
int main () 
{
    my_map data;
    my_map2 data2;

    // Parse each command
    std::string command;  

    bool window2 = false;

    while(std::cin >> command) 
    {
        // load the sample text file
        if(command == "load") 
        {
            std::string filename;
            int window;
            std::string parse_method;
            std::cin >> filename >> window >> parse_method;   

            if(window == 2)
            {
                window2 = true;
            }   

            LoadSampleText(data, data2, filename, window, parse_method);

            std::cout << "Loaded " << filename << " with window = " << window << " and parse method = " << parse_method << "\n\n";
        } 

        // print the portion of the map structure with the choices for the
        // next word given a particular sequence.
        else if(command == "print") 
        {
            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
            
            if(window2 == true)
            {
                print(data, data2, 2, sentence);
            }

            else
            {
                print(data, data2, 3, sentence);
            }
            std::cout<<"\n";
        }

        // generate the specified number of words 
        else if(command == "generate") 
        {
            std::vector<std::string> sentence = ReadQuotedWords(std::cin);

            // how many additional words to generate
            int length;
            std::cin >> length;
            std::string selection_method;
            std::cin >> selection_method;

            if (selection_method == "random") 
            {
                if(window2 == true)
                {
                    GenerateRandom(data, data2, sentence, length, 0, sentence[0], " ", 2); 
                }

                else
                {
                    GenerateRandom(data, data2, sentence, length, 0, sentence[0], " ", 3); 
                }
            } 

            else 
            {
	            assert(selection_method == "most_common");

                if(window2 == true)
                {
                    GenerateCommon(data, data2, sentence, length, 0, sentence[0], " ", 2); 
                }

                else
                {
                    GenerateCommon(data, data2, sentence, length, 0, sentence[0], " ", 3); 
                }
                
            }

           std::cout << std::endl << std::endl;
        } 

        else if(command == "quit") 
        {
            break;
        } 

        else 
        {
            std::cout << "WARNING: Unknown command: " << command << std::endl;
        }
    }

    return 0;
}
// ==========================================================================