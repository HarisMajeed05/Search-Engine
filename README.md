# Search Engine

A document search engine in C++. It builds an inverted index of the words in text files and records where each word appears.

## Features

- Inverted index of words, with the document and position of every occurrence
- Word search that shows the documents, the number of occurrences and the positions
- Sentence search that shows where the sentence appears in each document
- Interactive prompt. Type `exit` to quit.

Sample documents are included: `doc1.txt` and `doc2.txt`.

## Build

The code uses `<filesystem>`, so it needs C++17 or newer.

```bash
g++ -std=c++17 -o search Source.cpp
./search
```

You can also open `SearchEngine.sln` in Visual Studio.
