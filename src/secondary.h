/*
    Secondary Index Class Header file
    John "Jack" Riales, CSC230, J00515647

    Class will keep track of values ("types" section), counting
    duplicate values, of the relative file, and print it through
    to a sequential file.

    Dependencies:
        BinaryData
            Standard IO
            STL String
            File Stream
        String Utilities
*/

#ifndef SECONDARY_H
#define SECONDARY_H

#include "binary.h"
#include "stringutils.h"

//#define _DEBUG_

class SE_Node
{
    friend class SecondaryIndex;
    public:
        SE_Node(): _type("0"), _duplicates(0) { left = right = NULL; }
        SE_Node(std::string type): _type(type), _duplicates(0) { left = right = NULL; }
        static void add(std::string type, SE_Node *&r);
        static bool write(std::ostream& out, SE_Node *r);
    private:
        std::string _type;
        int         _duplicates;
        SE_Node     *left;
        SE_Node     *right;
};

class SecondaryIndex
{
	public:
		SecondaryIndex ();
		/**
		    \name Constructor
		*/

		~SecondaryIndex ();
		/**
		    \name Destructor
		*/

		bool update_type (std::string type);
		/**
		    \name Update type
		    \brief Adds the type if it does not exist, or updates its duplication value if it does
		*/

		bool write (std::fstream& out);
		/**
		    \name Write
		    \brief Writes the secondary index to a sequential file.
		*/

		bool read (BinaryData objects[], unsigned int length = 0);
		/**
		    \name Read
		    \brief Reads the primary index back from the binary file.

		    Performed at start.
		*/

		bool type_match (std::string type);
		/**
		    \name Type Match
		    \brief Returns if the type was found at the given position
		    \return True type was found
		    \return False type was not found
		    \param Type type to search for
		*/

		int duplicates_of (std::string type);
		/**
            \name Duplicates Of
            \brief Returns the number of duplicates of the given type, or 0 if not found
		*/

    protected:
		void kill_tree(SE_Node *r);
		/**
            \name Kill List
            \brief Kills the bst. Should only occur at destruction time.
		*/

	private:
		SE_Node *root;
		int     _size;
};

#endif // SECONDARY_H
