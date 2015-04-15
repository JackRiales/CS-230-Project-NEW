/*
    Binary Bulk Build File Header
    John "Jack" Riales, CSC230. J00515647

    Class will contain data such as char arrays
    and ints to be stored in a binary ("relative") file.

    Class Duties:
        Create and maintain a binary file of the given data
        based on the parsed input from an external sequential file.

    Dependencies:
        IO Stream
        Standard IO
        STL String
        File stream
        String Stream
        String Utilities
*/

#ifndef BINARY_H
#define BINARY_H

/*
    Debug preprocessing definition.
    I've defined this inside my IDE, but enabling this
    defines it in another box as needed. Enables
    a whole lot of "printf's" and debugging directives I want to use,
    and is generally unpleasant, I find. Helps me, though.
    Uncomment at your own risk.
*/
//#define _DEBUG_

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include "stringutils.h"

class BinaryData
{
	public:
		BinaryData
		(
		    std::string title   = "0",
		    std::string artist  = "0",
		    std::string type    = "0",
		    int year            = 2015,
		    int price           = 0,
		    int count           = 1
		);
		/**
		    \name Constructor
		    \brief Instantiates based on given arguments. Defaults are given.
		*/

		~BinaryData ();
		/**
		    \name Destructor
		    \brief Not used, yet*.
		*/

		bool write (std::fstream& out, long position) const;
		/**
		    \name Binary Write
		    \brief Writes the data through the file stream
		    \return False if file could not be opened or read.
		    \return True if successful.
		*/

		bool read_sequential (std::fstream& in, long line_number);
		/**
		    \name Sequential Read
		    \brief Reads from a formatted sequential file (format shown below) and parses to data for object
		    \return True if read is successful
		    \return False if error checking prompts any of the following:
		        - Stream is not opened (if no error check was performed on creation, this is useful)
		        - Line number exceeds the number of new line characters in the file
		    \param in File stream input
		    \param line_number Which line of the text file to put

		    File must be formatted by line in this order:
		    $TITLE $ARTIST $TYPE $YEAR $PRICE $COUNT

		    Failing to format properly will /not/ cause the function to return false.
		    Use. Caution.
		*/

		bool read_binary (std::fstream& in, long position) const;
		/**
		    \name Binary Read
		    \brief Reads the file from the given input stream
		    \return False if file could not be opened or read.
		    \return True if successful.
		*/

		std::string to_string();
		/**
            \name To String
            \brief Returns a formatted string with the object's data
		*/

		friend std::ostream& operator<<(std::ostream& out, const BinaryData& obj);
		/**
		    Friending the insertion operator for convenience.
		    Effectively used as a file-streamed toString,
		    for creating a sequential file of this object.
		*/

	private:
		// Arbitrary character array data with arbitrary buffer sizes.
		char    _title  [35],
		        _artist [25],
		        _type   [12];

		// Arbitrary integer data
		int     _year,
		        _price,
		        _count;

		// Flag (default FALSE) determines if the index is "alive"
		// a TRUE value determines that it has been flagged for deletion.
		bool    _del;

    // Mutators
	public:
		void set_title  (std::string);
		void set_artist (std::string);
		void set_type   (std::string);
		void set_year   (int);
		void set_price  (int);
		void set_count  (int);
		void flag       ();
		void unflag     ();

    // Accessors (read-only)
	public:
		inline std::string  title() const
		{
			return _title;
		}
		inline std::string  artist()const
		{
			return _artist;
		}
		inline std::string  type()  const
		{
			return _type;
		}
		inline const int&   year()  const
		{
			return _year;
		}
		inline const int&   price() const
		{
			return _price;
		}
		inline const int&   count() const
		{
			return _count;
		}
		inline const bool&  is_deleted() const
		{
			return _del;
		}
};

#endif // BINARY_H
