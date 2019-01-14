#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <time.h>

using namespace std;

struct Rhyme
{
    string first_sentence;

    string second_sentence;

    double length_avg; // stores the average # words of the sentences.

    double length_dif; // stores the difference in # words of the sentences.

    double similarity; // stores what percent of words in first_sentence are also in second_sentence.

    // Could add more qualities of a rhyme here.

    double total_value; // stores the total "value" of the rhyme, by considering the above values.
};

// Helper Functions:

bool is_small_sentence(const string& sentence)
{
    // If sentence has <= 2 words (numbers count as words!) then function returns true.

    // I'll do a pass through sentence to check if it does contain a space (which means it probably has >= 2 words):

    for (char c: sentence)
    {
        if (c == ' ')
        {
            return false; // contains a space ==> most likely contains at least 2 words ==> not a small sentence.
        }
    }

    return true;
}

bool is_lower_case(char c)
{
    // Function returns true if c is lower case:

    return (c >= 'a' && c <= 'z');
}

bool is_upper_case(char c)
{
    // Function returns true if c is upper case:

    return (c >= 'A' && c <= 'Z');
}

bool is_letter(char c)
{
    // Function returns true if c is a letter:

    return (is_lower_case(c) || is_upper_case(c));
}

bool is_digit(char c)
{
    // Function returns true if c is a digit (i.e, '0' to '9'):

    return (c >= '0' && c <= '9');
}

bool contains_letters(const string& sentence)
{
    // Function determines if sentence contains at least one letter:

    for (char c: sentence)
    {
        if (is_letter(c))
        {
            // contains at least one letter:

            return true;
        }
    }

    return false;
}

bool contains_numbers(const string& sentence)
{
    // Function determines if sentences contains at least one number:

    for (char c: sentence)
    {
        if (is_digit(c))
        {
            // contains at least one number:

            return true;
        }
    }

    return false;
}

void make_lowercase(string& text)
{
    for (char& c: text)
    {
        if (c >= 'A' && c <= 'Z') // Uppercase, make lowercase:
        {
            c += ('a' - 'A');
        }
    }
}

string get_lowercase(string text)
{
    // Functions makes all uppercase chars in text lowercase, and then returns this new string.

    make_lowercase(text);

    return text;
}

string last_word(const string& sentence)
{
    // Function returns the last word of the sentence.  Only letters + numbers are returned.  If characters like ), (, ", etc
    // are in the last word, they are ommitted.  If they ARE the last word, skip and go back.

    bool flag = false; // set to true when I've determined I'm on a word with a letter/number in it, meaning this word is good.

    int index_of_space = 0; // stores the index of the space character right before the last valid word.

    for (int i = sentence.size() - 1; i >= 0; i--)
    {
        if (is_letter(sentence[i]) || is_digit(sentence[i]))
        {
            flag = true; // I'm on the last valid word right now.
        }

        if (sentence[i] == ' ' && flag == true) // I'm on the space before the last valid word right now.
        {
            index_of_space = i;

            break;
        }
    }

    // Now, it's possible I reached the beginning of the sentence without stumbling on a letter/number.  In this scenario,
    // flag would still = false:

    if (flag == false)
    {
        return ""; // since there is no VALID last word, so I'm returning nothing.
    }

    // Now to go forward from the space, until hitting another space or the end of the remaining part of the sentence.

    // Any letters/numbers are added to my new string storing the word.

    string word = "";

    for (int i = index_of_space + 1; i < sentence.size(); i++)
    {
        if (i == ' ') // reached a space, meaning it's the end of this valid word....
        {
            break;
        }

        if (is_letter(sentence[i]) || is_digit(sentence[i]))
        {
            word += sentence[i];
        }
    }

    return word;
}

string last_chars(const string& text, int num_chars)
{
    // Function returns the last  num_chars  chars of text.

    string word = "";

    int start_index = text.size() - num_chars; // index of first char I will return.

    if (start_index < 0 || start_index >= text.size())
    {
        return word;
    }

    for (int i = start_index; i < text.size(); i++)
    {
        word += text[i];
    }

    return word;
}

string lower_case(const string& text)
{
    // Function makes all upper-case characters of text lowercase, and then returns the new string:

    string replacement = text;

    for (int i = 0; i < replacement.size(); i++)
    {
        if (is_upper_case(replacement[i]))
        {
            replacement[i] += ('a' - 'A'); // makes the current char lowercase.
        }
    }

    return replacement;
}


bool are_a_rhyme(const string& sentence_1, const string& sentence_2)
{
    // Function returns true if the sentences rhyme.

    // To figure this out, I need to know the last pronouncable word of both of them (i.e., word with some letters/numbers):

    string last_word_1 = last_word(sentence_1);

    string last_word_2 = last_word(sentence_2);

    // if either last_word_1 or last_word_2 equal "", it means one of them had no words with letters/numbers.

    // In this case, a rhyme is impossible since one of them has only unprouncable characters:

    if (last_word_1 == "" || last_word_2 == "")
    {
        return false; // not a rhyme.
    }

    // Now to check that the two words don't equal each other.  To rhyme, two words MUST NOT BE EQUAL:

    if (lower_case(last_word_1) == lower_case(last_word_2)) // comparing them both as lowercase to ensure something like "Add" and "add" is not allowed to be a rhyme.
    {
        return false; // not a rhyme.
    }

    // Now to get the last three characters of last_word_1 and last_word_2, and then see if they are equal.

    // Before doing this though, I need to ensure both last_word_1 and last_word_2 are at least 3 chars long.

    if (last_word_1.size() < 3 || last_word_2.size() < 3)
    {
        return false; // Without at least 3 chars each, the words cannot rhyme...
                      // I don't want to deal with special cases of words with 2 chars or less.
    }

    string last_three_chars_1 = last_chars(last_word_1, 3); // last three chars of word 1.
    string last_three_chars_2 = last_chars(last_word_2, 3); // last three chars of word 2.

    return (lower_case(last_three_chars_1) == lower_case(last_three_chars_2));
    // comparing lower-case versions, since I only care about how something SOUNDS for rhyming...
    // capitalization does not affect pronounciation.
}

// Core Functions (called from main):

void store_in_vector (vector <string>& item, const string& filename)
{
    // Function stores all the sentences in filename into vector item.  Each sentence is its own element.

    // I'll divide up the sentences based on '\n' chars or '\t' chars (since a tab pretty much means a new sentence):

    ifstream infile;

    infile.open(filename);

    if (infile.fail())
    {
        throw runtime_error("Error - file could not be read.");
    }

    // Continue here.... transfer file contents to item vector.

    char c = infile.get();

    string current_sentence = "";

    while (!infile.eof())
    {
        if (c == '\n' || c == '\t') // new line, new sentence:
        {
            item.push_back(current_sentence);

            current_sentence = "";
        }

        else // not a new sentence, just another char of the current sentence:
        {
            current_sentence += c;
        }

        c = infile.get();
    }

    infile.close();
}

void eliminate_small_sentences (vector <string>& item)
{
    // Function eliminates all sentences (elements) with <= 2 words.  Do not include the ending char (. ! ?) in the algorithm!
    // Note: numbers count as words!  When reading a poem, you'd read a number as a word.

    // I'll run through the vector, find if a string is NOT <= 2 words, and if so add it to the new replacement vector.
    // Only the "good" strings (i.e., not small) get added to the replacement vector.

    vector <string> replacement;

    for (string current: item) // first pass:
    {
        if (is_small_sentence(current) == false) // not a small sentence, so add it to the replacement vector:
        {
            replacement.push_back(current);
        }
    }

    item = replacement;
}

void eliminate_non_letter_sentences(vector <string>& item)
{
    // Function eliminates all sentences (elements) with no letters (or numbers) in them.

    // To eliminate "non-letter/number" sentences, I'll run through vector item, and add all the "good" sentences
    // to a replacement vector.

    vector <string> replacement;

    for (string current: item)
    {
        if (contains_letters(current) || contains_numbers(current))
        {
            // It is good...

            replacement.push_back(current);
        }
    }

    item = replacement;
}

void add_new_rhyme(vector<Rhyme>& rhymes, const string& first_sentence, const string& second_sentence)
{
    rhymes.push_back({first_sentence, second_sentence});

    if (rhymes.size() % 2000 == 0)
    {
        cout << "Generated " << rhymes.size() << " rhymes.\n";
    }
}

void make_rhymes(vector <Rhyme>& rhymes, const vector <string>& sentences)
{
    // Function makes rhymes from the sentences vector, and stores them in the rhymes vector.

    // Use an embedded for loop to go through sentences, in order to compare every pair of sentences:

    for (int i = 0; i < sentences.size() - 1; i++) // .size() - 1 because I don't want to test the last sentence with itself!
    {
        string lowercase_version_of_last_chars = get_lowercase(last_chars(sentences[i], 3));

        // sentences[i] should be surrounded on both sides by sentences ending in the same syllable. When I sorted the sentences vector,
        // I made ending in the same 3 chars the first requirement to check.

        // So, run a for loop outwards from sentences[i] in both directions, until reaching a sentence ending with different chars
        // (or getting out-of-bounds in sentences):

        for (int x = i+1; x < sentences.size() && lowercase_version_of_last_chars == get_lowercase(last_chars(sentences[x], 3)); x++)
        {
            // Loop from sentences[i] to the right.

            if (are_a_rhyme(sentences[i], sentences[x]))
            {
                add_new_rhyme(rhymes, sentences[i], sentences[x]);
            }
        }

        for (int x = i-1; x >= 0 && lowercase_version_of_last_chars == get_lowercase(last_chars(sentences[x], 3)); x--)
        {
            // Loop from sentences[i] to the left.

            if (are_a_rhyme(sentences[i], sentences[x]))
            {
                add_new_rhyme(rhymes, sentences[i], sentences[x]);
            }
        }
    }
}

void remove_non_letter_last_char(string& text)
{
    while (!is_letter(text[text.size()-1]) && text.size() > 0)
    {
        text.pop_back();
    }
}

bool compare_sentences(string first, string second)
{
    // Passing by value since I want to make both strings lowercase:

    make_lowercase(first);

    make_lowercase(second);

    // Now see if the sentences end in the same manner. First I want to get rid of any periods, exclamation points, etc:

    remove_non_letter_last_char(first);

    remove_non_letter_last_char(second);

    string last_portion_of_first = last_chars(first, 3);

    string last_portion_of_second = last_chars(second, 3);

    if (last_portion_of_first < last_portion_of_second)
    {
        return true;
    }

    if (last_portion_of_first > last_portion_of_second)
    {
        return false;
    }

    // So the sentences end with the same 3 chars. Now compare them by size and alphabetically:

    if (first.size() < second.size())
    {
        return true;
    }

    if (first.size() > second.size())
    {
        return false;
    }

    // Sentences have the same number of chars, so run through them in parallel.

    for (int i = 0; i < first.size(); i++)
    {
        if (first[i] < second[i])
        {
            return true;
        }

        if (first[i] > second[i])
        {
            return false;
        }
    }

    return true; // sentences are identical.
}

void remove_duplicates(vector<string>& sentences) // Pre-condition: sentences is sorted
{
    if (sentences.empty())
    {
        return;
    }

    vector<string> replacement;

    replacement.push_back(sentences[0]);

    for (int i = 1; i < sentences.size(); i++)
    {
        if (sentences[i] != sentences[i-1])
        {
            replacement.push_back(sentences[i]);
        }
    }

    sentences = replacement;
}

void remove_rhymes_with_same_sentences(vector<Rhyme>& rhymes)
{
    // If one rhyme has first sentence A and second sentence B, while the other rhyme has first sentence B and second sentence A,
    // then I want to remove one of these rhymes.

    vector<Rhyme> replacement;

    for (int i = 0; i < rhymes.size(); i++)
    {
        // See if there is an "equal" rhyme that came before, in the past 30 sentences:

        bool is_duplicate = false;

        for (int x = i-1; x >= 0 && x >= i-30; x--)
        {
            if (rhymes[i].first_sentence == rhymes[x].second_sentence && rhymes[i].second_sentence == rhymes[x].first_sentence)
            {
                is_duplicate = true;

                break;
            }
        }

        if (!is_duplicate)
        {
            replacement.push_back(rhymes[i]);
        }
    }

    rhymes = replacement;
}

int main()
{
    srand(time(NULL));

    // Sentences:

    vector <string> sentences; // vector will store all of my sentences from sentences.txt

    store_in_vector(sentences, "sentences.txt"); // Stores all the sentences in sentences.txt into the sentences vector.

    eliminate_small_sentences(sentences); // Eliminates sentences with <= 2 words.

    eliminate_non_letter_sentences(sentences); // Eliminates sentences with no letters (or numbers).

    sort(sentences.begin(), sentences.end(), compare_sentences);

    remove_duplicates(sentences);

    // Rhymes:

    vector <Rhyme> rhymes; // vector will store all the combinations of sentences that rhyme.

    make_rhymes(rhymes, sentences); // function makes rhymes from the sentences vector, and stores them in the rhymes vector.

    remove_rhymes_with_same_sentences(rhymes);

    random_shuffle(rhymes.begin(), rhymes.end());

    // Output rhymes to a file:

    ofstream fout("Rhymes.txt");

    for (const Rhyme& current: rhymes)
    {
        fout << current.first_sentence << "\n" << current.second_sentence << "\n\n";
    }

    cout << rhymes.size() << " rhymes have been generated and written to the textfile: \"Rhymes.txt\"\n";
}
