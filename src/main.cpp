/*
    Program 1 Main Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "main.h"

// ========================================================= //

int main (int argc, char** argv)
{
	cout << program_header;

	// Start by asking if the user wants to rebuild the binary file, or go to the menu
	if (!userPrompt_bulkBuild()) {
		// Ask what the input filename is
		std::string input_filename;
		cout << "Please enter the input file name: ";
		cin  >> input_filename;

		// Build the file and the indexes.
		if (!performBulkBuild(input_filename))
			return 0;

		if (userPrompt_Confirmation(false, "Go to menu?"))
			return binaryMenu();
		else {
			cout << "Exiting.";
			return 0;
		}
	} else {
		if (binaryFileExists())
			return binaryMenu();
		else
			cout << "Binary file nonexistant. Exiting.";
	}
	return 0;
}

// ========================================================= //

bool binaryFileExists()
{
	// Do a quick open of the file we're looking for (binary_file.bin, in this case) and check if it's good
	ifstream in(bin_filename.c_str());
	if (in.good()) {
		in.close();
		return true;
	} else {
		in.close();
		return false;
	}
}

// ========================================================= //

bool userPrompt_bulkBuild()
{
	int result;
	cout << "Please enter (0) to perform the bulk build, or enter (1) to enter the menu program: ";
	cin  >> result;

	if (cin.fail()) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Unknown case. Entering default input (1). Please hit enter.\n";
		return 1;
	}

	switch (result) {
	case 0:
        if (userPrompt_Confirmation(false, "Are you sure you wish to rebuild the binary file?")) {
            return 0;
        }
        else return 1;
	case 1:
		cout << "Entering menu program.\n";
		return 1;
	default:
		cout << "Unknown case. Entering default input (1).\n";
		return 1;
	}
}

// ========================================================= //

bool userPrompt_Confirmation(bool def, std::string message)
{
    char confirmation;
    cout << message << " (y/n): ";
    cin  >> confirmation;

    // Make it lower case
    if (isupper(confirmation))
        confirmation = char(tolower(confirmation));

    switch (confirmation) {
    case 'y':
        return true;
    case 'n':
        return false;
    default:
        return def;
    }
}

// ========================================================= //

bool performBulkBuild(std::string input_filename)
{
	// Create an array of data objects as large as the buffer located in the PrimaryIndex class
	//unsigned int buffer = PrimaryIndex::LISTING_BUFFER - 1;
	BinaryData obj [buffer];

	// Create the indexes
	PrimaryIndex prime_index;
	SecondaryIndex second_index;

	// Open the stream to the input file
	std::fstream in (input_filename.c_str(), ios::in);
	if (in.good()) {
		cout << "Reading input file.\n";

		// Read each line onto each object
		for (int i = 0; i < buffer; i++) {
			obj[i].read_sequential(in, i);
		}
	} else {
		cout << "Input file nonexistant. Exiting bulk build.\n";
		return false;
	}

	// Close the input stream
	in.close();

	// Create the output stream
	std::fstream out (bin_filename.c_str(), ios::out | ios::binary);
	if (out.good()) {
		cout << "Writing binary file.\n";

		// Write out the binary file
		for (int i = 0; i < buffer; i++) {
			obj[i].write (out, i);
		}
	}

	// Close output
	out.close();

	// Read in the indexes from the object array
	cout << "Reading objects for index creation.\n";
	prime_index.read(obj, buffer);
	second_index.read(obj, buffer);

	// Open output streams to write out the files
	std::fstream    prime_out (prime_index_filename.c_str(), ios::out);
	std::fstream    second_out (second_index_filename.c_str(), ios::out);

	if (prime_out.good() && second_out.good()) {
		cout << "Writing indexes.\n";

		// Write the indexes
		prime_index.write(prime_out);
		second_index.write(second_out);

		// Close the streams
		prime_out.close();
		second_out.close();

		return true;
	} else {
		cout << "Index streams could not be opened. Exiting.";
		return false;
	}
}

// ========================================================= //

int binaryMenu()
{
    // Reset cin, in case the doofus at the keyboard put in a char for the bulk build/menu input.
    // Ignore until new line is read. Just so no infinite loops happen...
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    // Read back in the binary file
    fstream in (bin_filename, ios::in | ios::binary);
    BinaryData *objects = new BinaryData[buffer];
    for (int i = 0; i < buffer; i++) {
        objects[i].read_binary(in, i);
    }

    // Resize the global buffer to be only valid entries that came from the input file
    /*int temp_buffer = buffer;
    buffer = 0;
    for (int i = 0; i < temp_buffer; i++) {
        #ifdef _DEBUG_
        cout << "Reading for buffer resize. Current object: " << objects[i].title() << endl;
        #endif

        if (objects[i].title() != "0") {
            buffer++;
        }
    }*/

    // Regenerate the indexes
    PrimaryIndex    prime_index;
    SecondaryIndex  second_index;
    prime_index.read(objects, buffer);
    second_index.read(objects, buffer);

    // Start menu
	char input;
	while (running) {
		cout << "\nEnter a command or type 'h' for a list of commands.\n> ";
		cin  >> input;

		// If input is upper case, make sure it's lower case
		if (isupper(input))
			input = char(tolower(input));

		switch (input) {
		case 'h':
			help();
			break;
		case 'q':
			quit();
			break;
		case 'w':
			write(objects, prime_index, second_index);
			break;
		case 'd':
			deleteRecord(objects);
			break;
        case 's':
            sellRecord(objects);
            break;
		case 'c':
			changeRecord(objects);
			break;
		case 'a':
			addRecord(objects, prime_index);
			break;
        case 'l':
            listData(objects);
            break;
        case 'p':
            print(objects, prime_index, second_index);
            break;
        default:
            cout << "Invalid command. Displaying help.\n\n";
            help();
		}
	}

	return 0;
}

// ========================================================= //

bool addRecord(BinaryData *obj, PrimaryIndex prime)
{
    // Check for jumps in the array (deleted or null items with title == "0")
    for (int i = 0; i < buffer; i++) {
        // Put the add there if it exists.
        if (obj[i].title() == "0") {
            obj[i] = recordPrompt();
            // Check if it exists in the primary. Check both in lower case for accuracy.
            if (prime.title_exists(obj[i].title())) {
                // If so, tell the user and set it back
                cout << "Title already exists in the prime index. No add performed.\n";
                obj[i] = *(new BinaryData);
                return false;
            }
            // If it didn't exist, all is well.
            return true;
        }
    }

    // Increment the buffer
    #ifdef _DEBUG_
    cout << "Incrementing the buffer field to " << buffer+1 << endl;
    #endif // _DEBUG_
    buffer += 1;

    // Buffer can increase, copy over data to a new array
    // Create new array of a new buffer size and copy over
    #ifdef _DEBUG_
    cout << "Creating new dynamic binary object array." << endl;
    #endif // _DEBUG_
    BinaryData *_new = new BinaryData[buffer];

    #ifdef _DEBUG_
    cout << "Copying data to new array." << endl;
    #endif // _DEBUG_
    for (int i = 0; i < buffer - 1; i++)
        _new[i] = obj[i];

    // Prompt for a new entry at the end of new array
    #ifdef _DEBUG_
    cout << "Recording new entry at " << buffer-1 << endl;
    #endif // _DEBUG_
    _new[buffer-1] = recordPrompt();

    // Point over to the new array
    #ifdef _DEBUG_
    cout << "Cleaning up..." << endl;
    #endif // _DEBUG_
    delete [] obj;
    obj = _new;

    return true;
}

// ========================================================= //

bool changeRecord(BinaryData *obj)
{
    int input;
    cout << "Choose a record to formulate changes to (or -1 to cancel).\n";

    // Iterate and display all valid entries (Title != "0")
    listData(obj);

    // Take input
    cout << "Input> ";
    cin  >> input;

    // Cancel
    if (input <= -1) {
        cout << "Cancelled. Returning.\n";
        return true;
    }

    // Object exceeds buffer
    else if (input >= buffer) {
        cout << "Input exceeds buffer. Returning.\n";
        return false;
    }

    // Valid input
    else {
        // Take object input and emplace it onto that record
        obj[input] = recordPrompt();
    }

    return true;
}

// ========================================================= //

bool deleteRecord(BinaryData *obj)
{
    int input;
    cout << "Choose a record to delete (or -1 to cancel).\n";

    // Iterate and display all valid entries (Title != "0")
    listData(obj);

    // Take input
    cout << "Input> ";
    cin  >> input;

    // Cancel
    if (input <= -1) {
        cout << "Canceled. Returning.\n";
        return true;
    }

    // Object exceeds buffer
    else if (input >= buffer) {
        cout << "Input exceeds buffer. Returning.\n";
        return false;
    }

    // Valid entry
    else {
        if (!obj[input].is_deleted())
            obj[input] = *(new BinaryData);
        else
            cout << "Object already flagged for deletion.\n";
    }
    return true;
}

// ========================================================= //

bool sellRecord(BinaryData *obj)
{
    int input;

    // Iterate and display all valid entries (Title != "0")
    listData(obj);

    cout << "Choose a record to sell (or -1 to cancel).\n";
    // Take input
    cout << "Input> ";
    cin  >> input;

    if (cin.fail()) {
        cout << "Invalid input. Returning.\n";
        return false;
    }

    // Cancel
    if (input <= -1) {
        cout << "Canceled. Returning.\n";
        return true;
    }

    // Object exceeds buffer
    else if (input >= buffer) {
        cout << "Input exceeds buffer. Returning.\n";
        return false;
    }

    // Valid entry
    else {
        if (!obj[input].is_deleted()) {
            if (obj[input].count() > 0) {
                int amount;
                cout << "How many of this object are you selling?: ";
                cin  >> amount;
                if (amount > obj[input].count()) {
                    if (userPrompt_Confirmation(false, "You don't have that many of that item. Sell all?")) {
                        cout << "Sold " << obj[input].count() << " of item " << obj[input].title() << " for a total of $" << obj[input].price() * obj[input].count() << endl;
                        obj[input].set_count(0);
                        return true;
                    } else {
                        cout << "Exiting.\n";
                        return false;
                    }
                } else if (amount <= 0) {
                    cout << "Amount invalid. Exiting.\n";
                    return false;
                } else {
                    cout << "Sold " << amount << " of item " << obj[input].title() << " for a total of $" << obj[input].price() * amount << endl;
                    obj[input].set_count(obj[input].count() - amount);
                    return true;
                }
            } else
                cout << "You have none of that item.\n";
        }
        else
            cout << "Object flagged for deletion.\n";
    }
    return true;
}

// ========================================================= //

void write(BinaryData obj[], PrimaryIndex& prime_index, SecondaryIndex& second_index)
{
    fstream bin_out (bin_filename, ios::out | ios::binary);
    fstream prime_out (prime_index_filename, ios::out);
    fstream second_out (second_index_filename, ios::out);

	if (userPrompt_Confirmation(false, "Are you sure you wish to commit and write the changes?")) {
	    // Write changes binary
        for (int i = 0; i < buffer; i++)
            obj[i].write(bin_out, i);

        // Reset the indexes and let them read the binary
        prime_index.read(obj, buffer);
        second_index.read(obj,buffer);

        // Write them out
        prime_index.write(prime_out);
        second_index.write(second_out);
        cout << "Changes committed and written to file system.\n";
	}
	else
		cout << "Changes will not be committed. Returning.\n";

	// Close and finish
	bin_out.close();
    prime_out.close();
    second_out.close();
}

// ========================================================= //

void quit()
{
	cout << "Quitting menu program...\n";
	running = false;
}

// ========================================================= //

void help()
{
	for (int i = 0; i < possible_inputs; i++)
		cout << input_list[i] << "\t" << help_display_strings[i] << endl;
}

// ========================================================= //

void listData(BinaryData obj[])
{
    for (int i = 0; i < buffer; i++) {
        if (obj[i].title() != "0")
            cout << i << ") " << obj[i].to_string() << endl;
    }
}

// ========================================================= //

void print(BinaryData *obj, PrimaryIndex prime_index, SecondaryIndex second_index)
{
    int selection;
    cout << "Enter a print method. (0) for \"By Title\" or (1) for \"By Type\": ";
    cin  >> selection;

    if (cin.fail()) {
        cout << "Invalid input. Returning.\n";
        return;
    }
    else {
        BinaryData result;
        switch (selection) {
        case 0:
            result = getByTitle(obj, prime_index);
            break;
        case 1:
            result = getByType(obj, prime_index, second_index);
            break;
        default:
            cout << "Invalid input. Returning.\n";
            return;
        }

        if (result.title() != "0")
            cout << result.to_string() << endl;
        else
            cout << "Result returned null.\n";
    }
}

// ========================================================= //

BinaryData recordPrompt()
{
    std::string title, artist, type;
    int year, price, count;

    cout << "Enter a title: ";
    cin  >> title;
    cout << "Enter an artist: ";
    cin  >> artist;
    cout << "Enter a type: ";
    cin  >> type;

    cout << "Enter a year: ";
    cin  >> year;
    while(cin.fail()){
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Failed: Integer only. Enter a year: ";
        cin  >> year;
    }

    cout << "Enter a price: ";
    cin  >> price;
    while(cin.fail()){
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Failed: Integer only. Enter a price: ";
        cin  >> price;
    }

    cout << "Enter count: ";
    cin  >> count;
    while(cin.fail()){
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Failed: Integer only. Enter a count: ";
        cin  >> count;
    }


    BinaryData result(title, artist, type, year, price, count);
    return result;
}

// ========================================================= //

BinaryData getByTitle(BinaryData *obj, PrimaryIndex pi)
{
    // Get a title
    string input;
    cout << "Enter a title to search for: ";
    cin  >> input;

    // Invalid input type exception
    if (cin.fail()) {
        cout << "Invalid input.\n";

        // Return new null binary data
        return *(new BinaryData);
    } else if (!pi.title_exists(input)) {
        cout << "Title nonexistant.\n";
        return *(new BinaryData);
    }
    else {
        // Perform linear search and get it
        /*for (int i = 0; i < buffer; i++) {
            if (toLowerCase(obj[i].title()) == input)
                return obj[i];
        }*/
        return obj[ pi.index_of(input) ];
    }
    return *(new BinaryData);
}

// ========================================================= //

BinaryData getByType(BinaryData *obj, PrimaryIndex pi, SecondaryIndex si)
{
    string input;
    cout << "Enter a type to search for: ";
    cin  >> input;

    // Invalid input type exception
    if (cin.fail()) {
        cout << "Invalid input.\n";
        return *(new BinaryData);
    }
    else {
        // How many duplicates of the type are there
        int dupes = si.duplicates_of(input);

        // Array of the found objects
        BinaryData found[buffer];
        int currentIndex = 0;

        // Convert to lower case
        input = toLowerCase(input);

        // Linear search
        for (int i = 0; i < buffer; i++) {
            if (toLowerCase(obj[i].type()) == input) {
                found[currentIndex] = obj[i];
                currentIndex++;
            }
        }

        // If there's just one of them, return it
        if (dupes == 1)
            return found[currentIndex -1];
        else {
            // Allow them to search among the duplicates by title
            if (userPrompt_Confirmation(false, "Duplicates found. Search the duplicates by title? (n) for more options."))
                return getByTitle(found, pi);
            else {
                // Or just allow them to print all the duplicates and return a null bin object
                if (userPrompt_Confirmation(false, "Print all duplicates?")) {
                    for (int i = 0; i < buffer; i++) {
                        // Print all valid entries in the duplicates buffer
                        if (found[i].title() != "0")
                            cout << found[i].to_string() << endl;
                    } // Buffer iteration
                } // User Prompt Confirmation
            } // Else
        } // Else
    }

    return *(new BinaryData);
}
