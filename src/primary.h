/*
    Primary Index Class Header File
    John "Jack" Riales, CSC230. J00515647

    Class will keep track of unique values (the "titles" section)
    of the relative file, and print it through to a sequential file.

    Class Duties:
        Maintain a linked list of titles

    Dependencies:
        BinaryData
            Standard IO
            STL String
            File stream
        String Utilities
*/

#ifndef PRIMARY_H
#define PRIMARY_H

#include "binary.h"
#include "stringutils.h"

//#define _DEBUG_

// Defines a node of a bst (primary index version)
class P_Node
{
    friend class PrimaryIndex;
    public:
        P_Node(): _title("0"), _tag(0) { left = right = NULL; }
        P_Node(std::string title, int tag): _title(title), _tag(tag) { left = right = NULL; }
        static void add(std::string title, int key, P_Node *&r);
        static bool write(std::ostream& out, P_Node *r);
    private:
        std::string _title;
        int         _tag;
        P_Node      *left;
        P_Node      *right;
};

class PrimaryIndex
{
	public:
		PrimaryIndex ();
		/**
		    \name Constructor
		    \brief Initializes attributes. Sets defaults for array data.
		    \param Count what to instantiate _count as. Default is 0.
		*/

		~PrimaryIndex ();
		/**
		    \name Destructor.
		    \brief Kills the list
		*/

		bool set_title (unsigned int key, std::string title);
		/**
		    \name Title index setter
		    \brief Adds title at the given key index.
		    \return False if error check prompts any of the following:
		        - Key is out of range of the index buffer
		        - Title already exists
		    \return True if no errors are detected.
		    \param Key Key of index to assign to. Is unsigned int to take care of negative key value problems.
		    \param Title title to set at the index.

		    Returns false if the title is already set at the index because we don't want users
		    accidentally overwriting titles all over the place.
		*/

		bool write (std::fstream& out);
		/**
		    \name Write
		    \brief Writes the primary index to a sequential file.
		*/

		bool read (BinaryData objects[], unsigned int length = 0);
		/**
		    \name Read
		    \brief Creates a primary index based on an array of data objects
		    \param Objects Array of binary objects
		    \param Length Length of the array. If zero, will attempt to use byte sizing to find length.

		    Performed at start.
		*/

		bool title_exists (std::string title);
		/**
		    \name Title Exists
		    \brief Returns whether or not there is a title set yet in the index.
		    \return False no title was found
		    \return True title was found somewhere

		    Effectively a boolean wrapper function for "indexOf." If -1 is returned, return false.
		    Used as a slightly easier to write way of determining whether or not to write a title.
		*/

		int index_of (std::string title);
		/**
            \name Index Search
            \brief Returns index of title found via linear search of linked list
		*/

    protected:
        void kill_tree(P_Node *r);
		/**
            \name Kill tree
            \brief Kills the bst. Should only occur at destruction time.
		*/

	private:
		P_Node *root;
		int     _size;
};

#endif // PRIMARY_H
