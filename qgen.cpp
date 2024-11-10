#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

vector<string> quotes = {"you cannot kill me in a way that matters.", "just think, every step taken is another soul left behind", "everything burns every single day until it's reduced to dust", "this doesn't end well", "you think you're safe?", "one cannot create beauty without destruction", "every single moment has consequence", "you wouldn't want anyone to know what you're hiding.", "where are you right now? what do you fear?", "it doesn't make sense to save now.", "it's too late.", "where is it.", "there is no threat", "it's always been there", "never make another wish ever again.", "where are you right now?", "why? it will never end now.", "do not.", "they are not your enemy", "this is your fault.", "we are not dead yet.", "it's finally happening", "please verify your humanity", "no one will matter", "this is not a matter of caring.", "are you okay with what you just did?", "stop reading this.", "watch your head.", "if you see this", "do not look at it", "observation is prohibited.", "your mind is nonexistent"}; 

// returns random number in range [begin, end)
int rand_range(int begin, int end){
    return rand() % (end - begin) + begin;
}

void print_help_message() {
    cout
        << "Usage: ./qgen <amount> <length> [-h] [-c] [-s] [-f <file>] [-o <file>]\n"
        << "Arguments:\n"
        <<     "\t-h\n"
        <<         "\t\tPrint this help notice.\n"
        <<     "\t-c\n"
        <<         "\t\tMake -o add to the default quotes as opposed to overwriting them.\n"
        <<     "\t-s\n"
        <<         "\t\tForce output to console even if output file is provided.\n"
        <<     "\t-f <file>\n"
        <<         "\t\tOutput to file instead of console.\n"
        <<     "\t-o <file>\n"
        <<         "\t\tOverwrite the source quotes with quotes from file, separated by newlines.\n";
}

int main(int argc, char* argv[]) {
    bool file_output = false;
    bool force_stdout_output = false;
    string out_filename = "quotes.txt"; // file to output to - will not output to file if empty, define with -f

    bool override_clear_quotes = true;
    string override_filename = ""; // file to read override quotes from, define with -o to use

    bool print_help = false; // whether to print a help message for the arguments

    // <base arguments>
    int base_argc = 2;
    int amt_quotes = 0, quote_len = 0; // how many quotes of what length to print
    // <\base arguments>

    bool parsing_value = false; // whether we're parsing a value for a preceding value argument
    char current_arg = -1;
    int current_base_arg = 0;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string arg(argv[i]);
            bool is_arg_def = arg.length() >= 2 && arg[0] == '-';
            if (is_arg_def) {
                if (parsing_value) {
                    cerr << "No value provided for argument '-" << current_arg << "'." << endl;
                    parsing_value = false;
                    print_help = true;
                }
                current_arg = arg[1];
            }
            bool is_flag = is_arg_def && arg.length() == 2;
            bool next_is_value = false;
            if (is_flag) { // if it could possibly be a flag try parse it as a flag
                switch (current_arg) {
                    case 'c': { // if flag set, add override quotes to current as opposed to overwriting current
                        override_clear_quotes = !override_clear_quotes;
                        break;
                    }
                    case 's': { // output to stdout instead of file; will still also output to file if -f provided
                        force_stdout_output = true;
                        break;
                    }
                    case 'h': {
                        print_help = true;
                        break;
                    }
                    default: { // not a flag, must be a value argument with a space after it
                        is_flag = false;
                        next_is_value = true;
                    }
                }
            }
            if (!is_flag) { // we're not parsing a flag, so we're parsing a value argument or value
                if (!next_is_value) {
                    if (!parsing_value && !is_arg_def) { // this is an orphan value so we're probably reading a base argument
                        switch (current_base_arg) {
                            case 0: {
                                amt_quotes = stoi(arg);
                                break;
                            }
                            case 1: {
                                quote_len = stoi(arg);
                                break;
                            }
                            default: {
                                cerr << "Orphan argument '" << arg << "'." << endl;
                                print_help = true;
                            }
                        }
                        ++current_base_arg;
                    } else {
                        string arg_value = is_arg_def ? arg.substr(2, arg.length() - 2) : arg;
                        switch (current_arg) {
                            case 'f': { // choose which file to output to
                                out_filename = arg_value;
                                file_output = true;
                                break;
                            }
                            case 'o': { // override the quotes
                                override_filename = arg_value;
                                break;
                            }
                            default: {
                                cerr << "Unrecognized argument '-" << current_arg << "'." << endl;
                                print_help = true;
                            }
                        }
                    }
                } else if (i == argc - 1) {
                    cerr << "No value provided for last argument." << endl;
                    print_help = true;
                }
            }
            // let the next iteration know if it's parsing a value
            parsing_value = next_is_value;
        }
    }
    if (current_base_arg < base_argc) {
        if (!print_help && argc > 1) {
            print_help_message();
            cerr << "Provided " << current_base_arg << " arguments, but " << base_argc << " are required." << endl;
            return 1;
        }
        print_help = true;
    }

    bool stdout_output = force_stdout_output || !file_output;

    if (print_help) {
        print_help_message();
        return 0;
    }

    if (override_filename.length() > 0) {
        ifstream quotes_override_s(override_filename);
        if (!quotes_override_s) {
            cerr << "Failed to open quote override file." << endl;
            return 1;
        }
        if (override_clear_quotes) {
            quotes.clear();
        }
        string next;
        while (getline(quotes_override_s, next)) {
            quotes.push_back(next);
        }
        cout << "Loaded custom quotes from " << override_filename << "." << endl;
    }

    vector<vector<string>> split_quotes;
    for (string quote : quotes) {
        vector<string> split_quote;
        size_t pos = 0, last_pos = 0;
        while ((pos = quote.find(" ", last_pos)) != string::npos) {
            split_quote.push_back(quote.substr(last_pos, pos - last_pos));
            last_pos = pos + 1;
        }
        split_quote.push_back(quote.substr(last_pos, pos - last_pos));
        split_quotes.push_back(split_quote);
    }
#ifdef DEBUG
    for(vector<string>& splitquote : splitQuotes){
        cout << "----" << endl;
        for(string& s : splitquote){
            cout << s << endl;
        }
    }
#endif
    srand(time(0));
    ofstream outs_file;
    if (file_output) {
        outs_file.open(out_filename);
        if (!outs_file) {
            cerr << "Failed to open output file." << endl;
            return 1;
        }
    }
    for (int n = 0; n < amt_quotes; n++) {
        string result = "";
        for (int i = 0; i < quote_len; i++) {
            vector<string> quote = split_quotes[rand() % split_quotes.size()];
            int from = rand() % max<int>(quote.size() - 2, 1);
            int to = rand_range(from, quote.size());
            for (int j = from; j <= to; j++) {
                result += quote[j] + " ";
            }
        }
        if (file_output) {
            outs_file << result << '\n';
        }
        if (stdout_output) {
            cout << result << '\n';
        }
    }
    return 0;
}
