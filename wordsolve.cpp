#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>

using namespace std;

#define MIN_LENGTH 3

unordered_set<string> read_dict(string filename) {
    unordered_set<string> dict;
    string line;
    ifstream myfile(filename);
    if (!myfile.is_open()) {
        return dict;
    }
    while (getline(myfile, line)) {
        dict.emplace(line);
    }
    return dict;
}


unordered_set<string> power_set_aux(const string letters, unordered_set<string>& state) {
    // base case - empty string was already inserted in power_set
    if (letters.length() == 0) return state;

    // recursive case - either have this letter, or not
    // so prepend the letter to each choice we have made already
    // as well as having this letter by itself
    for (string subchoice : power_set_aux(letters.substr(1), state)) {
        state.emplace(letters[0] + subchoice);
    }

    return state;
}

unordered_set<string> power_set(const string letters) {
    unordered_set<string> results;
    results.emplace("");
    results = power_set_aux(letters, results);
    return results;
}

unordered_set<string> permute_string(const string string_) {
    unordered_set<string> results;

    // base case - empty string
    if (string_.length() == 0) {
        results.emplace(string_);
        return results;
    }

    // recursive case - front letter can put anywhere else in string
    for (string subpermutation : permute_string(string_.substr(1))) {
        for (size_t i = 0; i <= subpermutation.size(); i++) {
            results.emplace(subpermutation.substr(0, i) +
                             string_[0] +
                             subpermutation.substr(i));
        }
    }

    return results;
}

unordered_set<string> permute_subelements(const unordered_set<string> elements) {
    unordered_set<string> results;
    for (string element : elements) {
        for (string permutation : permute_string(element)) {
            results.emplace(permutation);
        }
    }

    return results;
}

unordered_set<string> filter_lengths(const unordered_set<string> strings, size_t length) {
    unordered_set<string> results;
    for (string string_ : strings) {
        if (string_.length() >= length) {
            results.emplace(string_);
        }
    }

    return results;
}

unordered_set<string> enumerate_combinations(const string letters) {
    unordered_set<string> results = power_set(letters);
    results = filter_lengths(results, MIN_LENGTH);
    results = permute_subelements(results);
    return results;
}

unordered_set<string> get_valid_results(const unordered_set<string> dict, const unordered_set<string> possibilities) {
    unordered_set<string> results;
    for (string possibility : possibilities) {
        if (dict.count(possibility) > 0) {
            results.emplace(possibility);
        }
    }
    return results;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " DICT LETTERS" << endl;
        return 1;
    }

    unordered_set<string> dict = read_dict(argv[1]);
    if (dict.size() == 0) {
        cout << "Dictionary file cannot be read or is empty" << endl;
        return 1;
    }

    unordered_set<string> results = get_valid_results(dict, enumerate_combinations(argv[2]));
    for (string result : results) {
        cout << result << endl;
    }

    return 0;
}
