#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <filesystem>
#include<regex>
using namespace std;

class WordInDocument
{
    string documentName;
    string word;  // Store the word itself
    vector<int> positions;
    int occurrenceCount;

public:
    WordInDocument(const string& docName, const string& w, int pos)
        : documentName(docName), word(w), occurrenceCount(1)
    {
        positions.push_back(pos);
    }

    void addPosition(int pos)
    {
        positions.push_back(pos);
        occurrenceCount++;
    }

    const string& getDocumentName() const
    {
        return documentName;
    }

    const string& getWord() const
    {  // Getter for the word
        return word;
    }

    int getOccurrenceCount() const
    {
        return occurrenceCount;
    }

    const vector<int>& getPositions() const
    {
        return positions;
    }
};


class SearchEngine
{
    unordered_map<string, vector<WordInDocument>> index;

public:
    string extractFileName(const string& absolutePath)
    {
        // Find the position of the last directory separator (either '/' or '\\')
        size_t pos = absolutePath.find_last_of("/\\");

        // If the separator is found, return the substring after it (i.e., the filename)
        if (pos != string::npos) {
            return absolutePath.substr(pos + 1);
        }

        // Return the original path if no separator is found (this might be the filename or a directory without a trailing separator)
        return absolutePath;
    }

    void addDocument(const string& content)
    {
        ifstream file(content);
        if (!file.is_open()) {
            cerr << "Unable to open file: " << content << endl;
            return;
        }

        string line;
        int position = 0;
        while (getline(file, line))
        {
            // Using regex to split the line into words (including punctuation)
            regex word_regex(R"(\w+|\S)");
            auto words_begin = sregex_iterator(line.begin(), line.end(), word_regex);
            auto words_end = sregex_iterator();

            for (sregex_iterator i = words_begin; i != words_end; ++i)
            {
                string word = i->str();
                // Convert the word to lowercase if needed
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                if (index[word].empty() || index[word].back().getDocumentName() != extractFileName(content))
                {
                    index[word].emplace_back(extractFileName(content), word, position);
                }
                else {
                    index[word].back().addPosition(position);
                }
                position++;
            }
        }
        file.close();
    }
    void displayAllWordsWithPositions(const string& documentName)
    {
        // Extract the filename from the provided document path
        string docName = extractFileName(documentName);

        // Check if the document exists in the index
        bool documentFound = false;
        for (const auto& entry : index) {
            for (const WordInDocument& wordInfo : entry.second)
            {
                if (wordInfo.getDocumentName() == docName)
                {
                    documentFound = true;
                    cout << "Word: " << wordInfo.getWord() << ", Occurrences: " << wordInfo.getOccurrenceCount() << ", Positions: ";

                    // Display each position of the word in the document
                    for (int pos : wordInfo.getPositions())
                    {
                        cout << pos << " ";
                    }
                    cout << endl;
                }
            }
        }

        if (!documentFound)
        {
            cout << "Document '" << docName << "' not found in the indexed documents." << endl;
        }
    }
    vector<string> search(const string& word) const
    {
        vector<string> result;
        if (index.find(word) != index.end()) {
            for (const auto& wd : index.at(word)) {
                result.push_back(wd.getDocumentName() + " (Occurrences: " + to_string(wd.getOccurrenceCount()) + ")");
            }
        }
        return result;
    }

    void crawl(const vector<string>& files)
    {
        for (const auto& file : files) {
            addDocument(file);
        }
    }

    void dumpSearchEngine(const string& filename) const
    {
        ofstream outFile(filename);
        for (const auto& entry : index) {
            outFile << entry.first << ": ";
            for (const auto& wd : entry.second) {
                outFile << wd.getDocumentName() << "(" << wd.getOccurrenceCount() << ")" << ",";
            }
            outFile << endl;
        }
        outFile.close();
    }
    vector<string> searchtwoword(const string& word1, const string& word2) const
    {
        vector<string> result;

        // Check if either word exists in the index
        bool existsWord1 = (index.find(word1) != index.end());
        bool existsWord2 = (index.find(word2) != index.end());

        // Map to store the occurrences of each word in documents
        unordered_map<string, unordered_map<string, int>> wordOccurrences;

        // Check if both words exist in the index
        if (existsWord1 && existsWord2)
        {
            for (const auto& wd1 : index.at(word1))
            {
                wordOccurrences[wd1.getDocumentName()][word1] += 1;
            }

            for (const auto& wd2 : index.at(word2))
            {
                wordOccurrences[wd2.getDocumentName()][word2] += 1;
            }
        }

        // Check for individual words
        if (existsWord1)
        {
            for (const auto& wd1 : index.at(word1))
            {
                wordOccurrences[wd1.getDocumentName()][word1] = wd1.getOccurrenceCount();
            }
        }

        if (existsWord2)
        {
            for (const auto& wd2 : index.at(word2))
            {
                wordOccurrences[wd2.getDocumentName()][word2] = wd2.getOccurrenceCount();
            }
        }

        // Construct the result string
        for (const auto& docPair : wordOccurrences)
        {
            string docName = docPair.first;
            string docResult = docName + " (";
            for (const auto& wordPair : docPair.second)
            {
                docResult += wordPair.first + ": " + to_string(wordPair.second) + ", ";
            }
            // Remove trailing comma and space
            docResult.pop_back();
            docResult.pop_back();
            docResult += ")";
            result.push_back(docResult);
        }

        // Sort and remove duplicates
        sort(result.begin(), result.end());
        result.erase(unique(result.begin(), result.end()), result.end());

        return result;
    }

    vector<string> subtractionSearch(const string& word1, const string& word2) const
    {
        vector<string> result;

        // Check if both words exist in the index
        bool existsWord1 = (index.find(word1) != index.end());
        bool existsWord2 = (index.find(word2) != index.end());

        // If word2 doesn't exist, return documents containing word1
        if (existsWord1 && !existsWord2)
        {
            for (const auto& wd1 : index.at(word1))
            {
                result.push_back(wd1.getDocumentName());
            }
            // Sort and remove duplicates
            sort(result.begin(), result.end());
            result.erase(unique(result.begin(), result.end()), result.end());

            return result;
        }

        // Iterate over documents containing word1 but not word2
        for (const auto& wd1 : index.at(word1)) {
            bool hasWord2 = false;

            // Check if the same document contains word2
            for (const auto& wd2 : index.at(word2)) {
                if (wd1.getDocumentName() == wd2.getDocumentName()) {
                    hasWord2 = true;
                    break;
                }
            }

            // If the document contains word1 but not word2, add it to the result
            if (!hasWord2) {
                result.push_back(wd1.getDocumentName());
            }
        }

        // Sort and remove duplicates
        sort(result.begin(), result.end());
        result.erase(unique(result.begin(), result.end()), result.end());

        return result;
    }
    vector<string> searchMultipleWords(const string& words) const {
        vector<string> result;

        // Splitting the input string into individual words
        istringstream iss(words);
        string word;
        vector<string> wordList;
        while (iss >> word) {
            if (word != ".") {  // Ignore the dot
                wordList.push_back(word);
            }
        }

        // Map to store the occurrences of each word in documents
        unordered_map<string, unordered_map<string, int>> wordOccurrences;

        // Process each word in the wordList and find occurrences in each document
        for (const auto& word : wordList) {
            if (index.find(word) != index.end()) {
                for (const auto& wd : index.at(word)) {
                    wordOccurrences[wd.getDocumentName()][word] += wd.getOccurrenceCount();  // Increase the count
                }
            }
        }


        // Construct the result string
        for (const auto& docPair : wordOccurrences) {
            string docName = docPair.first;
            string docResult = docName + " (";
            for (const auto& wordPair : docPair.second) {
                docResult += wordPair.first + ": " + to_string(wordPair.second) + ", ";
            }
            // Remove trailing comma and space
            if (!docResult.empty()) {
                docResult.pop_back();
                docResult.pop_back();
            }
            docResult += ")";
            result.push_back(docResult);
        }

        // Sort and remove duplicates
        sort(result.begin(), result.end());
        result.erase(unique(result.begin(), result.end()), result.end());

        return result;
    }

    vector<string> searchFirstWordNotFollowingWords(const string& words) const {
        vector<string> result;

        // Splitting the input string into individual words
        istringstream iss(words);
        string word;
        vector<string> wordList;
        while (iss >> word) {
            if (word != ".") {  // Ignore the dot
                wordList.push_back(word);
            }
        }

        if (wordList.empty()) {
            return result; // Return empty result if no words provided
        }

        // Map to store the occurrences of each word in documents
        unordered_map<string, unordered_map<string, bool>> wordPresence;

        // Process the first word in the wordList and find documents where it exists
        const string& firstWord = wordList[0];
        if (index.find(firstWord) != index.end()) {
            for (const auto& wd : index.at(firstWord)) {
                wordPresence[wd.getDocumentName()][firstWord] = true;
            }
        }

        // Now, iterate through the subsequent words and mark their presence in documents
        for (size_t i = 1; i < wordList.size(); ++i) {
            const string& currentWord = wordList[i];
            if (index.find(currentWord) != index.end()) {
                for (const auto& wd : index.at(currentWord)) {
                    wordPresence[wd.getDocumentName()][currentWord] = true;
                }
            }
        }

        // Iterate through documents and check for the required condition
        for (const auto& docPair : wordPresence) {
            const string& docName = docPair.first;
            if (docPair.second.find(firstWord) != docPair.second.end()) {
                bool hasFollowingWord = false;
                for (size_t i = 1; i < wordList.size(); ++i) {
                    if (docPair.second.find(wordList[i]) != docPair.second.end()) {
                        hasFollowingWord = true;
                        break;
                    }
                }
                if (!hasFollowingWord) {
                    result.push_back(docName);
                }
            }
        }

        // Sort and remove duplicates
        sort(result.begin(), result.end());
        result.erase(unique(result.begin(), result.end()), result.end());

        return result;
    }



    void searchSentence(const string& sentence) {
        istringstream iss(sentence);
        vector<string> words;
        string word;

        // Split the sentence into individual words including punctuations and symbols
        regex word_regex(R"(\w+|[^\s])");  // Match word characters or any character that's not a space

        auto words_begin = sregex_iterator(sentence.begin(), sentence.end(), word_regex);
        auto words_end = sregex_iterator();

        for (sregex_iterator i = words_begin; i != words_end; ++i) {
            word = i->str();
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            words.push_back(word);
        }

        // Check if each word in the sentence exists in the index
        vector<vector<WordInDocument>> wordDocs;
        for (const auto& word : words) {
            if (index.find(word) != index.end()) {
                wordDocs.push_back(index[word]);
            }
            else {
                cout << "Word '" << word << "' not found in any indexed documents." << endl;
                return;
            }
        }

        // Map to track occurrence count and positions for each document
        unordered_map<string, vector<pair<int, int>>> documentOccurrences;

        // For each document, find all possible occurrences of the sentence
        for (const auto& doc : wordDocs[0]) {
            vector<int> startPositions = doc.getPositions();
            for (int startPos : startPositions) {
                bool found = true;
                int currentPos = startPos;

                for (size_t i = 1; i < words.size(); ++i) {
                    bool wordFound = false;
                    for (const auto& wordInfo : wordDocs[i]) {
                        if (wordInfo.getDocumentName() == doc.getDocumentName()) {
                            for (int pos : wordInfo.getPositions()) {
                                if (pos == currentPos + i) {
                                    wordFound = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (!wordFound) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    int endPos = currentPos + words.size() - 1;
                    documentOccurrences[doc.getDocumentName()].push_back({ startPos, endPos });
                }
            }
        }

        // Print occurrence count and positions for each document
        for (const auto& entry : documentOccurrences) {
            int count = 0;
            cout << "Occurrences in document '" << entry.first << "': ";
            for (const auto& posPair : entry.second) {
                count++;
                cout << "from position " << posPair.first << " to " << posPair.second << ", ";
            }
            cout << "Occurance : " << count << endl;
        }

        // If no occurrence is found, notify the user
        if (documentOccurrences.empty()) {
            cout << "Sentence not found in the indexed documents." << endl;
        }
    }
    void processFilesInDirectory(const string& directoryPath) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            std::string filePath = entry.path().string();

            // Check if the file is a .txt file and is NOT CMakeLists.txt
            if (filePath.substr(filePath.find_last_of(".") + 1) == "txt" &&
                filePath.find("CMakeLists.txt") == std::string::npos) {
                addDocument(filePath);
            }
        }
    }
};


int main() {
    SearchEngine se;
    se.processFilesInDirectory("C:/Users/HP/OneDrive/Desktop/New folder/bscs22131_search_engine_DSA/bscs22131_search_engine_DSA");

    while (true) {
        cout << "\nEnter your search query (or 'exit' to quit): ";
        string query;
        getline(cin, query);

        if (query == "exit") {
            break;
        }

        // Trim leading and trailing whitespaces from the query
        query.erase(0, query.find_first_not_of(" \t\n\r\f\v"));
        query.erase(query.find_last_not_of(" \t\n\r\f\v") + 1);

        // Check for the query format and perform the appropriate search
        if (query.find("|") == 0 && query.rfind("|") == query.size() - 1) {
            // Sentence search: Remove the delimiter '|' and perform the search
            string sentence = query.substr(1, query.size() - 2);
            se.searchSentence(sentence);
        }
        else if (query.find("-") != string::npos && query.back() == '.') {
            size_t pos = query.find("-");
            string firstWord = query.substr(0, pos);
            string remainingQuery = query.substr(pos + 1);

            // Remove any leading spaces from remainingQuery
            remainingQuery.erase(0, remainingQuery.find_first_not_of(" \t\n\r\f\v"));

            // Adjust the remaining query by removing any leading spaces after '-'
            if (remainingQuery.front() == ' ') {
                remainingQuery = remainingQuery.substr(1);
            }

            // Remove the trailing '.' from the remainingQuery
            remainingQuery.pop_back();

            // Combine words after '-' until you encounter a '.'
            while (!remainingQuery.empty()) {
                string nextWord;
                if (remainingQuery.find(' ') != string::npos) {
                    nextWord = remainingQuery.substr(0, remainingQuery.find(' '));
                    remainingQuery = remainingQuery.substr(remainingQuery.find(' ') + 1);
                }
                else {
                    nextWord = remainingQuery;
                    remainingQuery.clear();
                }
                firstWord += " " + nextWord;
            }

            cout << "Processed query: " << firstWord << endl;

            // Process the adjusted query
            auto result = se.searchFirstWordNotFollowingWords(firstWord);
            for (const auto& doc : result) {
                cout << "Found in: " << doc << endl;
            }
        }
        else if (query.find(".") != string::npos && query.back() == '.') {
            // Multiple word search: Pass the entire string to searchMultipleWords function

            auto result = se.searchMultipleWords(query);
            for (const auto& doc : result) {
                cout << "Found in: " << doc << endl;
            }
        }
        else if (query.find("-") != string::npos) {
            // Split the query around '-' to get the two words
            size_t pos = query.find("-");
            string word1 = query.substr(0, pos);
            string word2 = query.substr(pos + 1);

            // Trim any extra spaces from the words
            word1.erase(remove_if(word1.begin(), word1.end(), ::isspace), word1.end());
            word2.erase(remove_if(word2.begin(), word2.end(), ::isspace), word2.end());

            auto result = se.subtractionSearch(word1, word2);
            for (const auto& doc : result) {
                cout << "Found in: " << doc << endl;
            }
        }
        else if (query.find("+") != string::npos) {
            // Split the query around '-' to get the two words
            size_t pos = query.find("+");
            string word1 = query.substr(0, pos);
            string word2 = query.substr(pos + 1);

            // Trim any extra spaces from the words
            word1.erase(remove_if(word1.begin(), word1.end(), ::isspace), word1.end());
            word2.erase(remove_if(word2.begin(), word2.end(), ::isspace), word2.end());

            auto result = se.searchtwoword(word1, word2);
            for (const auto& doc : result) {
                cout << "Found in: " << doc << endl;
            }
        }
        else {
            // Single word search
            auto result = se.search(query);
            for (const auto& doc : result) {
                cout << "Found in: " << doc << endl;
            }
        }
    }

    return 0;
}
//
//int main() {
//    SearchEngine se;
//    se.processFilesInDirectory("C:/Users/Mudassir/CLionProjects/untitled/");
////    se.addDocument("C:/Users/Mudassir/CLionProjects/untitled/review_4.txt");
////    se.addDocument("C:/Users/Mudassir/CLionProjects/untitled/review_5.txt");
////    se.displayAllWordsWithPositions("C:/Users/Mudassir/CLionProjects/untitled/review_5.txt");
////    auto result = se.searchtwoword("constipation", "relief");
////    for (const auto &doc : result) {
////        cout << "Found in: " << doc << endl;
////    }
//
////    auto result = se.subtractionSearch("constipation", "relief");
////    for (const auto &doc : result) {
////        cout << "Found in: " << doc << endl;
////    }
////    auto result = se.search("constipation");
////    for (const auto &doc : result) {
////        cout << "Found in: " << doc << endl;
////    }
//    auto result = se.searchFirstWordNotFollowingWords("constipation relief .");
//    for (const auto &doc : result) {
//        cout << "Found in: " << doc << endl;
//    }
////    auto result = se.searchMultipleWords("constipation relief .");
////    for (const auto &doc : result) {
////        cout << "Found in: " << doc << endl;
////    }
//
////    se.searchSentence("We have a 7 week old...");
////    auto result = se.searchSentence("constipation relief");
////    for (const auto &doc : result) {
////        cout << "Found in: " << doc.first << doc.second<< endl;
////    }
////
////    se.dumpSearchEngine("SEARCH_ENGINE.DAT");
////
////    SearchEngine newSE;
////    newSE.loadSearchEngine("SEARCH_ENGINE.DAT");
//
//    return 0;
//}

