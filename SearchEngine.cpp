//#include <iostream>
//#include <vector>
//#include <string>
//#include<set>
//#include <unordered_map>
//#include <map>
//#include <algorithm>
//#include <sstream>
//#include <fstream>
//#include <limits>
//#include <conio.h>
//#include<iomanip>
//#include<Windows.h>
//#include <chrono>
//
//using namespace std;
//
//void getRowColbyLeftClick(int& rpos, int& cpos)
//{
//    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
//    DWORD Events;
//    INPUT_RECORD InputRecord;
//    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
//    do
//    {
//        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
//        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
//        {
//            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
//            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
//            break;
//        }
//    } while (true);
//}
//void gotoRowCol(int rpos, int cpos)
//{
//    COORD scrn;
//    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
//    scrn.X = cpos;
//    scrn.Y = rpos;
//    SetConsoleCursorPosition(hOuput, scrn);
//}
//void color(int clr){
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
//}
//
//class WordInDocument {
//public:
//    string documentName;
//    vector<int> position;
//
//    WordInDocument(const string& docName, int pos) : documentName(docName) {
//        position.push_back(pos);
//    }
//
//    void addPosition(int pos) {
//        position.push_back(pos);
//    }
//    bool operator==(const WordInDocument& other) const {
//        return documentName == other.documentName && position == other.position;
//    }
//};
//
//class MapSearchEngine {
//    map<string, vector<WordInDocument>> mapSearchEngine;
//public:
//    void crawlDocument(const string& fileName) {
//        ifstream file(fileName);
//        if (!file.is_open()) {
//            cout << "Error opening file: " << fileName << endl;
//            return;
//        }
//        string line;
//        string docName;
//        int wordPosition = 1;
//
//        while (getline(file, line)) {
//            if (line == "---END.OF.DOCUMENT---") {
//                docName.clear();
//                wordPosition = 1;
//            }
//            else {
//                if (docName.empty())
//                    docName = fileName;
//                if (line.back() == '.') {
//                    processLine(docName, line.substr(0, line.size() - 1), wordPosition);
//                    wordPosition += 1;
//                }
//                else {
//                    processLine(docName, line, wordPosition);
//                    wordPosition += 1;
//                }
//            }
//        }
//        file.close();
//    }
//    void processLine(const string& docName, const string& line, int wordPosition) {
//        istringstream iss(line);
//        string word;
//        while (iss >> word) {
//            if (iss.eof() && line.back() == '.')
//                processWord(docName, word + '.', wordPosition);
//            else
//                processWord(docName, word, wordPosition);
//        }
//    }
//    void processWord(const string& docName, const string& word, int wordPosition) {
//        if (mapSearchEngine.find(word) == mapSearchEngine.end()) {
//            mapSearchEngine[word] = { WordInDocument(docName, wordPosition) };
//        }
//        else {
//            auto& instances = mapSearchEngine[word];
//            auto it = find_if(instances.begin(), instances.end(),
//                [docName](const WordInDocument& w) { return w.documentName == docName; });
//            if (it != instances.end())
//                it->addPosition(wordPosition);
//            else
//                instances.push_back(WordInDocument(docName, wordPosition));
//        }
//    }
//    void processExclusionSearch(const string& query, vector<string>& result) {
//        istringstream iss(query);
//        string word;
//        vector<string> wordsToInclude;
//        vector<string> phrasesToExclude;
//        while (iss >> word) {
//            if (word == "-")
//                continue;
//            if (word.front() == '\"' && word.back() != '\"') {
//                string phrase = word.substr(1);
//                while (iss >> word && word.back() != '\"')
//                    phrase += " " + word;
//                if (word.back() == '\"')
//                    phrase += " " + word.substr(0, word.size() - 1);
//                phrasesToExclude.push_back(phrase);
//            }
//            else if (word.front() == '\"' && word.back() == '\"')
//                phrasesToExclude.push_back(word.substr(1, word.size() - 2));
//            else
//                wordsToInclude.push_back(word);
//        }
//        for (const auto& word : wordsToInclude) {
//            auto it = mapSearchEngine.find(word);
//            if (it != mapSearchEngine.end()) {
//                for (const auto& instance : it->second)
//                    result.push_back(instance.documentName);
//            }
//        }
//        for (const auto& phrase : phrasesToExclude) {
//            auto it = mapSearchEngine.find(phrase);
//            if (it != mapSearchEngine.end()) {
//                result.erase(remove_if(result.begin(), result.end(),
//                    [&it](const string& document) {
//                        return any_of(it->second.begin(), it->second.end(),
//                        [&document](const WordInDocument& instance) {
//                                return instance.documentName == document;
//                            });
//                    }), result.end());
//            }
//        }
//    }
//    void processPhraseSearch(const string& phrase, vector<string>& result) {
//        istringstream iss(phrase);
//        vector<string> words;
//        string word;
//        while (iss >> word)
//            words.push_back(word);
//        cout << "Words in the phrase: ";
//        for (const auto& w : words)
//            cout << w << " ";
//        cout << endl;
//        auto firstWordIt = mapSearchEngine.find(words.front());
//        if (firstWordIt != mapSearchEngine.end()) {
//            const auto& firstWordInstances = firstWordIt->second;
//            for (const auto& firstWordInstance : firstWordInstances) {
//                const string& documentName = firstWordInstance.documentName;
//                int firstWordPosition = firstWordInstance.position.front();
//                cout << documentName << " at position " << firstWordPosition << endl;
//                bool allWordsPresent = true;
//                for (size_t i = 1; i < words.size(); ++i) {
//                    const string& nextWord = words[i];
//                    auto nextWordIt = mapSearchEngine.find(nextWord);
//
//                    if (nextWordIt != mapSearchEngine.end()) {
//                        const auto& nextWordInstances = nextWordIt->second;
//                        if (any_of(nextWordInstances.begin(), nextWordInstances.end(),
//                            [&](const WordInDocument& nextWordInstance) {
//                                return nextWordInstance.documentName == documentName &&
//                                    nextWordInstance.position.front() == (firstWordPosition + static_cast<int>(i));
//                            })) {
//                            continue;
//                        }
//                    }
//                    allWordsPresent = false;
//                    break;
//                }
//                if (allWordsPresent) {
//                    cout << "All words present in order. Adding document to result." << std::endl;
//                    result.push_back(documentName);
//                }
//            }
//        }
//    }
//    /*vector<string> search(const string& query) {
//        vector<string> result;
//        if (query.front() == '\"' && query.back() == '\"') {
//            string phrase = query.substr(1, query.size() - 2);
//            processPhraseSearch(phrase, result);
//        }
//        else if (query.find('-') != string::npos) {
//            processExclusionSearch(query, result);
//        }
//        else if (mapSearchEngine.find(query) != mapSearchEngine.end()) {
//            auto& instances = mapSearchEngine[query];
//            sort(instances.begin(), instances.end(),
//                [](const WordInDocument& w1, const WordInDocument& w2) {
//                    return w1.position.size() > w2.position.size();
//                });
//
//            for (const auto& instance : instances) {
//                result.push_back(instance.documentName);
//            }
//        }
//        else if (query.find('+') != string::npos) {
//            istringstream iss(query);
//            string word;
//            vector<vector<WordInDocument>> wordInstances;
//            while (iss >> word) {
//                if (word.back() == '+') {
//                    word.pop_back();
//                }
//                if (mapSearchEngine.find(word) != mapSearchEngine.end()) {
//                    wordInstances.push_back(mapSearchEngine[word]);
//                }
//            }
//            if (wordInstances.size() > 1) {
//                sort(wordInstances.begin(), wordInstances.end(),
//                    [](const vector<WordInDocument>& w1, const vector<WordInDocument>& w2) {
//                        return w1.size() > w2.size();
//                    });
//                for (const auto& instance : wordInstances[0]) {
//                    if (all_of(wordInstances.begin() + 1, wordInstances.end(),
//                        [&instance](const vector<WordInDocument>& w) {
//                            return find(w.begin(), w.end(), instance) != w.end();
//                        })) {
//                        result.push_back(instance.documentName);
//                    }
//                }
//            }
//            else if (wordInstances.size() == 1) {
//                auto& instances = wordInstances[0];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        else if (query.front() == '\"' && query.back() == '\"') {
//            string sentence = query.substr(1, query.size() - 2);
//            if (mapSearchEngine.find(sentence) != mapSearchEngine.end()) {
//                auto& instances = mapSearchEngine[sentence];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        else if (query.find('-') != string::npos) {
//            istringstream iss(query);
//            string word1, word2;
//            iss >> word1 >> word2;
//            if (mapSearchEngine.find(word1) != mapSearchEngine.end() &&
//                mapSearchEngine.find(word2) != mapSearchEngine.end()) {
//                auto& instances1 = mapSearchEngine[word1];
//                auto& instances2 = mapSearchEngine[word2];
//                for (const auto& instance : instances1) {
//                    if (find(instances2.begin(), instances2.end(), instance) == instances2.end()) {
//                        result.push_back(instance.documentName);
//                    }
//                }
//            }
//        }
//        return result;
//    }*/
//
//    vector<string> search(const string& query) {
//        vector<string> result;
//        if (query.front() == '\"' && query.back() == '\"') {
//            // Case for phrase search
//            string phrase = query.substr(1, query.size() - 2);
//            processPhraseSearch(phrase, result);
//        }
//        else if (query.find('-') != string::npos) {
//            processExclusionSearch(query, result);
//        }
//        else if (mapSearchEngine.find(query) != mapSearchEngine.end()) {
//            auto& instances = mapSearchEngine[query];
//            sort(instances.begin(), instances.end(),
//                [](const WordInDocument& w1, const WordInDocument& w2) {
//                    return w1.position.size() > w2.position.size();
//                });
//
//            for (const auto& instance : instances) {
//                result.push_back(instance.documentName);
//            }
//        }
//        else if (query.find('+') != string::npos) {
//            istringstream iss(query);
//            string word;
//            vector<vector<WordInDocument>> wordInstances;
//            while (iss >> word) {
//                if (word.back() == '+') {
//                    word.pop_back();
//                }
//                if (mapSearchEngine.find(word) != mapSearchEngine.end()) {
//                    wordInstances.push_back(mapSearchEngine[word]);
//                }
//            }
//            if (wordInstances.size() > 1) {
//                sort(wordInstances.begin(), wordInstances.end(),
//                    [](const vector<WordInDocument>& w1, const vector<WordInDocument>& w2) {
//                        return w1.size() > w2.size();
//                    });
//                for (const auto& instance : wordInstances[0]) {
//                    if (all_of(wordInstances.begin() + 1, wordInstances.end(),
//                        [&instance](const vector<WordInDocument>& w) {
//                            return find(w.begin(), w.end(), instance) != w.end();
//                        })) {
//                        result.push_back(instance.documentName);
//                    }
//                }
//            }
//            else if (wordInstances.size() == 1) {
//                auto& instances = wordInstances[0];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        else if (query.front() == '\"' && query.back() == '\"') {
//            string sentence = query.substr(1, query.size() - 2);
//            if (mapSearchEngine.find(sentence) != mapSearchEngine.end()) {
//                auto& instances = mapSearchEngine[sentence];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        else if (query.find('-') != string::npos) {
//            istringstream iss(query);
//            string word1, word2;
//            iss >> word1 >> word2;
//            if (mapSearchEngine.find(word1) != mapSearchEngine.end() &&
//                mapSearchEngine.find(word2) != mapSearchEngine.end()) {
//                auto& instances1 = mapSearchEngine[word1];
//                auto& instances2 = mapSearchEngine[word2];
//                for (const auto& instance : instances1) {
//                    if (find(instances2.begin(), instances2.end(), instance) == instances2.end()) {
//                        result.push_back(instance.documentName);
//                    }
//                }
//            }
//        }
//        return result;
//    }
//
//};
//
//class HashMapSearchEngine {
//    unordered_map<string, vector<WordInDocument>> hashMapSearchEngine;
//public:
//    void crawlDocument(const string& fileName) {
//        ifstream file(fileName);
//        if (!file.is_open()) {
//            cout << "Error opening file: " << fileName << endl;
//            return;
//        }
//        string line;
//        string docName;
//        int wordPosition = 1;
//        while (getline(file, line)) {
//            if (line == "---END.OF.DOCUMENT---") {
//                docName.clear();
//                wordPosition = 1;
//            }
//            else {
//                if (docName.empty()) {
//                    docName = fileName;
//                }
//                if (line.back() == '.') {
//                    processLine(docName, line.substr(0, line.size() - 1), wordPosition);
//                    wordPosition += 1;
//                }
//                else {
//                    processLine(docName, line, wordPosition);
//                    wordPosition += 1;
//                }
//            }
//        }
//        file.close();
//    }
//    void processLine(const string& docName, const string& line, int wordPosition) {
//        istringstream iss(line);
//        string word;
//        while (iss >> word) {
//            if (iss.eof() && line.back() == '.') {
//                processWord(docName, word + '.', wordPosition);
//            }
//            else {
//                processWord(docName, word, wordPosition);
//            }
//        }
//    }
//    void processWord(const string& docName, const string& word, int wordPosition) {
//        if (hashMapSearchEngine.find(word) == hashMapSearchEngine.end()) {
//            hashMapSearchEngine[word] = { WordInDocument(docName, wordPosition) };
//        }
//        else {
//            auto& instances = hashMapSearchEngine[word];
//            auto it = find_if(instances.begin(), instances.end(),
//                [docName](const WordInDocument& w) { return w.documentName == docName; });
//            if (it != instances.end()) {
//                it->addPosition(wordPosition);
//            }
//            else {
//                instances.push_back(WordInDocument(docName, wordPosition));
//            }
//        }
//    }
//    void processExclusionSearch(const string& query, vector<string>& result) {
//        istringstream iss(query);
//        string word;
//        vector<string> wordsToInclude;
//        vector<string> phrasesToExclude;
//        while (iss >> word) {
//            if (word == "-") {
//                continue;
//            }
//            if (word.front() == '\"' && word.back() != '\"') {
//                string phrase = word.substr(1);
//                while (iss >> word && word.back() != '\"') {
//                    phrase += " " + word;
//                }
//                if (word.back() == '\"') {
//                    phrase += " " + word.substr(0, word.size() - 1);
//                }
//                phrasesToExclude.push_back(phrase);
//            }
//            else if (word.front() == '\"' && word.back() == '\"') {
//                phrasesToExclude.push_back(word.substr(1, word.size() - 2));
//            }
//            else {
//                wordsToInclude.push_back(word);
//            }
//        }
//        for (const auto& word : wordsToInclude) {
//            auto it = hashMapSearchEngine.find(word);
//            if (it != hashMapSearchEngine.end()) {
//                for (const auto& instance : it->second) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        for (const auto& phrase : phrasesToExclude) {
//            auto it = hashMapSearchEngine.find(phrase);
//            if (it != hashMapSearchEngine.end()) {
//                for (const auto& instance : it->second) {
//                    result.erase(remove(result.begin(), result.end(), instance.documentName), result.end());
//                }
//            }
//        }
//    }
//    void processPhraseSearch(const string& phrase, vector<string>& result) {
//        istringstream iss(phrase);
//        vector<string> words;
//        string word;
//        while (iss >> word) {
//            words.push_back(word);
//        }
//        cout << "Words in the phrase: ";
//        for (const auto& w : words) {
//            cout << w << " ";
//        }
//        cout << endl;
//        auto firstWordIt = hashMapSearchEngine.find(words.front());
//        if (firstWordIt != hashMapSearchEngine.end()) {
//            const auto& firstWordInstances = firstWordIt->second;
//            for (const auto& firstWordInstance : firstWordInstances) {
//                const string& documentName = firstWordInstance.documentName;
//                int firstWordPosition = firstWordInstance.position.front();
//                cout << documentName << " at position " << firstWordPosition << endl;
//                bool allWordsPresent = true;
//                for (size_t i = 1; i < words.size(); i++) {
//                    const string& nextWord = words[i];
//                    auto nextWordIt = hashMapSearchEngine.find(nextWord);
//
//                    if (nextWordIt != hashMapSearchEngine.end()) {
//                        const auto& nextWordInstances = nextWordIt->second;
//                        if (any_of(nextWordInstances.begin(), nextWordInstances.end(),
//                            [&](const WordInDocument& nextWordInstance) {
//                                return nextWordInstance.documentName == documentName &&
//                                    nextWordInstance.position.front() == (firstWordPosition + static_cast<int>(i));
//                            })) {
//                            continue;
//                        }
//                    }
//                    allWordsPresent = false;
//                    break;
//                }
//                if (allWordsPresent) {
//                    cout << "All words present in order. Adding document to result." << std::endl;
//                    result.push_back(documentName);
//                }
//            }
//        }
//    }
//    vector<string> search(const string& query) {
//        vector<string> result;
//        if (query.front() == '\"' && query.back() == '\"') {
//            // Case for phrase search
//            string phrase = query.substr(1, query.size() - 2);
//            processPhraseSearch(phrase, result);
//        }
//        else if (query.find('-') != string::npos) {
//            istringstream iss(query);
//            string word;
//            vector<vector<WordInDocument>> wordInstances;
//            vector<string> words;
//            while (iss >> word) {
//                if (word.back() == '-')
//                    word.pop_back();
//                if (!word.empty())
//                    words.push_back(word);
//            }
//            for (const auto& word : words) {
//                if (hashMapSearchEngine.find(word) != hashMapSearchEngine.end())
//                    wordInstances.push_back(hashMapSearchEngine[word]);
//            }
//            if (wordInstances.size() > 1) {
//                sort(wordInstances.begin(), wordInstances.end(),
//                    [](const vector<WordInDocument>& w1, const vector<WordInDocument>& w2) {
//                        return w1.size() > w2.size();
//                    });
//                for (const auto& instance : wordInstances[0]) {
//                    if (all_of(wordInstances.begin() + 1, wordInstances.end(),
//                        [&instance](const vector<WordInDocument>& w) {
//                            return find(w.begin(), w.end(), instance) != w.end();
//                        })) {
//                        result.push_back(instance.documentName);
//                    }
//                }
//            }
//            else if (wordInstances.size() == 1) {
//                auto& instances = wordInstances[0];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//            else {
//                processExclusionSearch(query, result);
//            }
//        }
//        else if (hashMapSearchEngine.find(query) != hashMapSearchEngine.end()) {
//            auto& instances = hashMapSearchEngine[query];
//            sort(instances.begin(), instances.end(),
//                [](const WordInDocument& w1, const WordInDocument& w2) {
//                    return w1.position.size() > w2.position.size();
//                });
//            for (const auto& instance : instances) {
//                result.push_back(instance.documentName);
//            }
//        }
//        else if (query.find('+') != string::npos) {
//            istringstream iss(query);
//            string word;
//            vector<vector<WordInDocument>> wordInstances;
//            while (iss >> word) {
//                if (word.back() == '+') {
//                    word.pop_back();
//                }
//                if (hashMapSearchEngine.find(word) != hashMapSearchEngine.end()) {
//                    wordInstances.push_back(hashMapSearchEngine[word]);
//                }
//            }
//            if (wordInstances.size() > 1) {
//                sort(wordInstances.begin(), wordInstances.end(),
//                    [](const vector<WordInDocument>& w1, const vector<WordInDocument>& w2) {
//                        return w1.size() > w2.size();
//                    });
//                for (const auto& instance : wordInstances[0]) {
//                    if (all_of(wordInstances.begin() + 1, wordInstances.end(),
//                        [&instance](const vector<WordInDocument>& w) {
//                            return find(w.begin(), w.end(), instance) != w.end();
//                        })) {
//                        result.push_back(instance.documentName);
//                    }
//                }
//            }
//            else if (wordInstances.size() == 1) {
//                auto& instances = wordInstances[0];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        else if (query.front() == '\"' && query.back() == '\"') {
//            string sentence = query.substr(1, query.size() - 2);
//            if (hashMapSearchEngine.find(sentence) != hashMapSearchEngine.end()) {
//                auto& instances = hashMapSearchEngine[sentence];
//                sort(instances.begin(), instances.end(),
//                    [](const WordInDocument& w1, const WordInDocument& w2) {
//                        return w1.position.size() > w2.position.size();
//                    });
//                for (const auto& instance : instances) {
//                    result.push_back(instance.documentName);
//                }
//            }
//        }
//        return result;
//    }
//};
//
//void performSearch(MapSearchEngine& mapSearchEngine, HashMapSearchEngine& hashMapSearchEngine, const string& query) {
//    cout << "Query: " << query << endl;
//
//    auto startMap = chrono::high_resolution_clock::now();
//    vector<string> resultMap = mapSearchEngine.search(query);
//    auto endMap = chrono::high_resolution_clock::now();
//    chrono::duration<double> mapDuration = endMap - startMap;
//    if (!resultMap.empty()) {
//        cout << "\nMap Search Results: ";
//        for (const auto& result : resultMap) {
//            cout << result << endl;
//        }
//        cout << endl;
//        cout << "Map Search Duration: " << mapDuration.count() << " seconds\n";
//    }
//    else {
//        cout << "Map => No results.....\n";
//    }
//    auto startHashMap = chrono::high_resolution_clock::now();
//    vector<string> resultHashMap = hashMapSearchEngine.search(query);
//    auto endHashMap = chrono::high_resolution_clock::now();
//    chrono::duration<double> hashMapDuration = endHashMap - startHashMap;
//    if (!resultHashMap.empty()) {
//        cout << "\nHash_Map Search Results: ";
//        for (const auto& result : resultHashMap) {
//            cout << result << endl;
//        }
//        cout << endl;
//        cout << "Hash_Map Search Duration: " << hashMapDuration.count() << " seconds\n";
//    }
//    else {
//        cout << "Hash_Map => No results.....\n";
//    }
//    cout << "-----------------------------------------" << endl;
//}
//
//void performSearchForPlus(MapSearchEngine& mapSearchEngine, HashMapSearchEngine& hashMapSearchEngine, const string& query1,const string & query2) {
//    cout << "Query: " << query1;
//    cout << " + ";
//    cout << query2 << endl;
//
//    auto startMap = chrono::high_resolution_clock::now();
//    vector<string> resultMap1 = mapSearchEngine.search(query1);
//    vector<string> resultMap2 = mapSearchEngine.search(query2);
//    auto endMap = chrono::high_resolution_clock::now();
//    chrono::duration<double> mapDuration = endMap - startMap;
//
//    
//    for (const auto& r1 : resultMap1) {
//        for (const auto& r2 : resultMap2) {
//            if (r1 == r2) {
//                cout << "Map Search Results: ";
//                cout << r1 << endl;
//                cout << "Map Search Duration: " << mapDuration.count() << " seconds\n";
//            }
//        }
//    }
//    cout << endl;
//    auto startHashMap = chrono::high_resolution_clock::now();
//    vector<string> resultHashMap1 = hashMapSearchEngine.search(query1);
//    vector<string> resultHashMap2 = hashMapSearchEngine.search(query2);
//    auto endHashMap = chrono::high_resolution_clock::now();
//    chrono::duration<double> hashMapDuration = endHashMap - startHashMap;
//    for (const auto& r1 : resultHashMap1) {
//        for (const auto& r2 : resultHashMap2) {
//            if (r1 == r2) {
//                cout << "HashMap Search Results: ";
//                cout << r1 << endl;
//                cout << "Hash_Map Search Duration: " << hashMapDuration.count() << " seconds\n";
//            }
//        }
//    }
//    cout << endl;
//    cout << "-----------------------------------------" << endl;
//}
//// Helper function to find the intersection of two vectors
//vector<string> intersection(const vector<string>& v1, const vector<string>& v2) {
//    vector<string> result;
//    set<string> s1(v1.begin(), v1.end());
//    for (const auto& elem : v2) {
//        if (s1.count(elem) > 0) {
//            result.push_back(elem);
//        }
//    }
//    return result;
//}
//void performSearchForMultiPlus(MapSearchEngine& mapSearchEngine, HashMapSearchEngine& hashMapSearchEngine, const string& query) {
//    cout << "Query: " << query << endl;
//    string cleanedQuery = query;
//    cleanedQuery.erase(remove(cleanedQuery.begin(), cleanedQuery.end(), '+'), cleanedQuery.end());
//    istringstream iss(cleanedQuery);
//    vector<string> words;
//    string word;
//    while (iss >> word)
//        words.push_back(word);
//    if (words.empty()) {
//        cout << "No valid words in the query." << endl;
//        return;
//    }
//    auto startMap = chrono::high_resolution_clock::now();
//    vector<vector<string>> resultMap(words.size());
//    for (size_t i = 0; i < words.size(); ++i) {
//        resultMap[i] = mapSearchEngine.search(words[i]);
//    }
//    auto endMap = chrono::high_resolution_clock::now();
//    chrono::duration<double> mapDuration = endMap - startMap;
//    auto startHashMap = chrono::high_resolution_clock::now();
//    for (size_t i = 0; i < words.size(); ++i) {
//        vector<string> resultHashMap = hashMapSearchEngine.search(words[i]);
//    }
//    auto endHashMap = chrono::high_resolution_clock::now();
//    chrono::duration<double> hashMapDuration = endHashMap - startHashMap;
//
//    // Find common documents
//    vector<string> commonDocuments = resultMap[0];
//
//    for (size_t i = 1; i < resultMap.size(); i++) {
//        commonDocuments = intersection(commonDocuments, resultMap[i]);
//    }
//
//    if (!commonDocuments.empty()) {
//    cout << "Found in: ";
//        for (const auto& doc : commonDocuments) {
//            cout << doc << " ";
//        }
//        cout << "\nMap Search Duration for " << query << ": " << mapDuration.count() << " seconds\n";
//        cout << "Hash_Map Search Duration for " << query << ": " << hashMapDuration.count() << " seconds\n";
//    }
//    else {
//        cout << "No such file exists which contains " << query << endl;
//    }
//    cout << endl;
//
//    cout << "-----------------------------------------" << endl;
//}
//
//
//
//
//
//
//
//int main() {
//    MapSearchEngine mapSearchEngine;
//    HashMapSearchEngine hashMapSearchEngine;
//    long long filesToSearch;
//    
//    color(9);
//    gotoRowCol(10, 55);
//    cout << "-----------------------------------------------------\n";
//    gotoRowCol(12, 55);
//    cout << "|   Welcome to Search Engine by Haris Majeed Raja   |\n";
//    gotoRowCol(14, 55);
//    cout << "-----------------------------------------------------\n";
//A:
//    gotoRowCol(16, 55);
//    color(10);
//    cout << "Enter the Number of Files you want to Search: ";
//    cin >> filesToSearch;
//    for (int i = 1; i <= filesToSearch; i++) {
//        string fileName = "C:\\Users\\HP\\OneDrive\\Desktop\\Semester 3\\DSA-Sarfraz Raza\\review_text\\review_" + to_string(i) + ".txt";
//        mapSearchEngine.crawlDocument(fileName);
//        hashMapSearchEngine.crawlDocument(fileName);
//    }
//B:
//    system("cls");
//    gotoRowCol(4, 50);
//    cout << "---------------------------------------------------------\n";
//    gotoRowCol(6, 50);
//    cout << "|               Following are the Options               |\n";
//    gotoRowCol(8, 50);
//    cout << "---------------------------------------------------------\n";
//
//    color(14);
//    gotoRowCol(12, 50);
//    cout << "1-> Single Word Search\n";
//    gotoRowCol(14, 50);
//    cout << "2-> Two Words Search\n";
//    gotoRowCol(16, 50);
//    cout << "3-> Multiple words Search\n";
//    gotoRowCol(18, 50);
//    cout << "4-> Sentence Search\n";
//    gotoRowCol(20, 50);
//    cout << "5-> Subtraction Search\n";
//    gotoRowCol(22, 50);
//    cout << "6-> Subtraction with sentence as well as multiple words\n";
//
//    int option;
//    color(10);
//    gotoRowCol(26, 50);
//    cout << "Enter the Option: ";
//    cin >> option;
//    if (option < 1 || option > 6) {
//        gotoRowCol(28, 50);
//        cout << "Enter the right option.....";
//        Sleep(2000);
//        goto B;
//    }
//    else {
//        cin.ignore(INT_MAX, '\n');
//        while (true) {
//        C:
//            system("cls");
//            color(9);
//            gotoRowCol(4, 8);
//            cout << "------------------------\n";
//            gotoRowCol(5, 8);
//            cout << "|         Exit         |\n";
//            gotoRowCol(6, 8);
//            cout << "------------------------\n";
//            gotoRowCol(4, 55);
//            cout << "-------------------------------\n";
//            gotoRowCol(5, 55);
//            cout << "|         Enter Words         |\n";
//            gotoRowCol(6, 55);
//            cout << "-------------------------------\n";
//            gotoRowCol(4, 110);
//            cout << "-----------------------------------------------------\n";
//            gotoRowCol(5, 110);
//            cout << "| Change Number of Files.. Current Files = " << filesToSearch << " |";
//            gotoRowCol(6, 110);
//            cout << "-----------------------------------------------------\n";
//            string query;
//            gotoRowCol(8, 65);
//            getline(cin, query);
//            switch (option) {
//            case 1://single word search
//                color(15);
//                cout << endl;
//                gotoRowCol(10, 0);
//                performSearch(mapSearchEngine, hashMapSearchEngine, query);
//                break;
//            case 2://two words OR
//                color(15);
//                gotoRowCol(10, 0);
//                performSearchForMultiPlus(mapSearchEngine, hashMapSearchEngine, query);
//                cout << endl;
//                break;
//            case 3:
//                color(15);
//                gotoRowCol(10, 0);
//                performSearchForMultiPlus(mapSearchEngine, hashMapSearchEngine, query);
//                cout << endl;
//                break;
//            case 4:
//                color(15);
//                query = "\"" + query + "\"";
//                gotoRowCol(10, 0);
//                performSearch(mapSearchEngine, hashMapSearchEngine, query);
//                cout << endl;
//                break;
//            case 5:
//                color(15);
//                gotoRowCol(10, 0);
//                performSearch(mapSearchEngine, hashMapSearchEngine, query);
//                break;
//            case 6:
//                color(15);
//                gotoRowCol(10, 0);
//                performSearch(mapSearchEngine, hashMapSearchEngine, query);
//                break;
//            default:
//                cout << "Plz Enter a right option......";
//                break;
//            }
//            int row, col;
//            getRowColbyLeftClick(row, col);
//            if ((row >= 4 && row <= 6) && (col >= 8 && col <= 30)) {
//                system("cls");
//                goto B;
//            }
//            else if ((row >= 4 && row <= 6) && (col >= 110 && col <= 163)) {
//                system("cls");
//                goto A;
//            }
//            else {
//                system("cls");
//                goto C;
//            }
//        }  
//    }
//    _getch();
//    return 0;
//}