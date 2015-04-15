/*
    Binary Bulk Build Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "binary.h"
using namespace std;

// Constructor
BinaryData::BinaryData (string title, string artist, string type, int year, int price, int count):
	_year (year),
	_price(price),
	_count(count)
{
	// Printing to log the creation of the object and what arguments were passed. Can never be too safe.
	// I've defined _DEBUG_ only in the "Debug" build in my IDE.
	// If you want to see these print on your end, uncomment line 21 in binary.h
    #ifdef _DEBUG_
	printf ("Constructed binary object. Args--\n\tTitle: %s\n\tArtist: %s\n\tType: %s\n",
	        title.c_str(),
	        artist.c_str(),
	        type.c_str());
    #endif

	// Copy the strings to the char arrays.
	strcpy(_title, title.c_str());
	strcpy(_artist, artist.c_str());
	strcpy(_type, type.c_str());
}

BinaryData::~BinaryData()
{
	; // Nothing to see here... yet!
}

bool BinaryData::write (fstream& out, long position) const
{
    #ifdef _DEBUG_
	printf ("Writing object to position %ld.\n", position);
    #endif

	// Error check
	// Error check
	if (!out) {
        #ifdef _DEBUG_
		printf ("Error: File stream is not opened!\n");
        #endif

		return false;
	}

	out.seekp(position * (sizeof(BinaryData)));
	out.write((char*) this, sizeof(BinaryData));

	return true;
}

bool BinaryData::read_sequential(fstream& in, long line_number)
{
    #ifdef _DEBUG_
	printf ("Reading sequential for object from line %ld.\n", line_number);
    #endif

	// Error check
	if (!in) {
        #ifdef _DEBUG_
		printf ("Error: File stream is not opened!\n");
        #endif

		return false;
	}

	// Begin reading
	string line_text;
	while (in.good()) {
		getline (in, line_text);

		// Stream to gather the words
		stringstream line_stream (line_text);

		// Begin setting from words
		line_stream >> _title;  // Transfer title
		line_stream >> _artist; // Transfer artist
		line_stream >> _type;   // Transfer type

		// The rest are ints, so we have to parse them
		std::string integers [3];
		line_stream >> integers[0]; // Year
		line_stream >> integers[1]; // Price
		line_stream >> integers[2]; // Count

		// Parse
		_year = stringToInt(integers[0]);
		_price= stringToInt(integers[1]);
		_count= stringToInt(integers[2]);

        #ifdef _DEBUG_
		printf ("Constructed binary object. Args--\n\tTitle: %s\n\tArtist: %s\n\tType: %s\n\tYear: %d\n\tPrice: %d\n\tCount: %d\n",
		        _title,
		        _artist,
		        _type,
		        _year,
		        _price,
		        _count);
        #endif

		// The line's been parsed successfully. Exit.
		return true;
	}

	// For some reason, the current line could not parse. Exit.
    #ifdef _DEBUG_
	printf ("Warning: File was unable to be parsed! Object data has not changed.\n");
    #endif

	return false;
}

bool BinaryData::read_binary (fstream& in, long position) const
{
    #ifdef _DEBUG_
	printf ("Reading object from position %ld.\n", position);
    #endif

	// Error check
	if (!in) {
    #ifdef _DEBUG_
		printf ("Error: File stream is not opened!\n");
    #endif

		return false;
	}

	// Read
	in.seekg(position * sizeof(BinaryData));
	in.read((char*) this, sizeof(BinaryData));

	return true;
}

string BinaryData::to_string()
{
    return
        "\tTitle:\t" + string(title()) + "    " + "\n" +
        "\tArtist:\t" + string(artist()) + "    " + "\n" +
        "\tType:\t" + string(type()) + "    " + "\n" +
        "\tYear:\t" + intToString(year()) + "    " + "\n" +
        "\tPrice:\t" + "$" + intToString(price()) + "    " + "\n" +
        "\tCount:\t" + intToString(count());
}

ostream& operator<<(ostream& out, const BinaryData& obj)
{
	out
	        << obj.title()          << "\t"
	        << obj.artist()         << "\t"
	        << obj.type()           << "\t"
	        << obj.year()           << "\t"
	        << "$" << obj.price()   << "\t"
	        << obj.count();
	return out;
}

// Mutators //////////////////////////////////////////////////
void BinaryData::set_title (std::string title)
{
    #ifdef _DEBUG_
	printf ("Changing character array from '%s' to '%s'.\n", _title, title.c_str());
    #endif // _DEBUG_
	strcpy(_title, title.c_str());
}

void BinaryData::set_artist (std::string artist)
{
    #ifdef _DEBUG_
	printf ("Changing character array from '%s' to '%s'.\n", _artist, artist.c_str());
    #endif // _DEBUG_
	strcpy(_artist, artist.c_str());
}

void BinaryData::set_type (std::string type)
{
    #ifdef _DEBUG_
	printf ("Changing character array from '%s' to '%s'.\n", _type, type.c_str());
    #endif // _DEBUG_
	strcpy(_type, type.c_str());
}

void BinaryData::set_year (int year)
{
	_year = year;
}

void BinaryData::set_price (int price)
{
	_price = price;
}

void BinaryData::set_count (int count)
{
	_count = count;
}

void BinaryData::flag()
{
	_del = true;
}

void BinaryData::unflag()
{
	_del = false;
}
