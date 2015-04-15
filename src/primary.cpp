/*
    Primary Index Class Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "primary.h"
using namespace std;

// Constructor
PrimaryIndex::PrimaryIndex():
head (new P_Node(" ", -1)),
tail (new P_Node("~", -1)),
_size (2)
{
    #ifdef _DEBUG_
	printf("Instantiating a new primary index.\n");
    #endif

    head->next = tail;
    tail->prev = head;
}

// Destructor
PrimaryIndex::~PrimaryIndex()
{
    #ifdef _DEBUG_
	printf("Destroying primary index.\n");
    #endif
	kill_list();
}

bool PrimaryIndex::set_title (unsigned int key, std::string title)
{
    if (title == "0") {
        #ifdef _DEBUG_
        printf ("Do not try to set a null title! No add performed.\n");
        #endif // _DEBUG_

        return false;
    }
    if (!title_exists(title)) {
        #ifdef _DEBUG_
        printf("Setting title at given key %d to %s\n", key, title.c_str());
        #endif

        P_Node *wp = head;
        while (wp->next != NULL && wp->_title < title) {
            // If the next title is greater than our title, or if it's the tail
            if (wp->next->_title > title || wp->next == tail) {
                // Place it
                P_Node *_new_node = new P_Node(title, key);
                _new_node->next = wp->next;
                _new_node->prev = wp;
                wp->next->prev = _new_node;
                wp->next = _new_node;

                // Increment the size
                _size++;

                return true;
            } else {
                wp = wp->next;
            }
        }

    } else {
        #ifdef _DEBUG_
        printf ("Title %s already exists in the primary index.\n", title.c_str());
        #endif

        return false;
    }
    return true;
}

bool PrimaryIndex::write (fstream& out)
{
	// Error check stream
	if (!out) {
        #ifdef _DEBUG_
		printf ("Output stream is not open. Please check your parameters.\n");
        #endif // _DEBUG_

		return false;
	}

    #ifdef _DEBUG_
	printf ("Writing Primary Index to output stream.\n");
    #endif // _DEBUG_

    P_Node *wp = head;

	// Iterate through listing
	while (wp->next != NULL) {
        #ifdef _DEBUG_
        printf("Writing: %s at tag %d\n", wp->_title.c_str(), wp->_tag);
        #endif // _DEBUG_

		// Output; one line per listing
		out << "\n"                         // New line
		    << wp->_title.c_str() << "\t\t" // Output the title at that list
		    << wp->_tag << "\t"             // Output the tag at that list
		    << "\n";                        // New line

        // Advance
        wp = wp->next;
	}

	// All is well
	return true;
}

bool PrimaryIndex::read (BinaryData objects[], unsigned int length)
{
    #ifdef _DEBUG_
	printf ("Reading binary object array into primary index.\n");
    #endif // _DEBUG_

	// If the length is not entered, try to calculate it
	if (length == 0)
		length = sizeof(objects)/sizeof(objects[0]);

    // If it still fails, we can't do anything
    if (length == 0) {
        #ifdef _DEBUG_
		printf ("Length is zero! Exiting.\n");
        #endif // _DEBUG_

		return false;
	}

    #ifdef _DEBUG_
	printf ("Calculated length of array: %d\n", length);
	printf ("Iterating...\n");
    #endif // _DEBUG_

	// Iterate
	for (unsigned int i = 0; i < length; i++) {
		// Never add things to the index that are flagged as deleted.
		if (!objects[i].is_deleted()) {
            if (objects[i].title() != "0") {
                set_title(i, objects[i].title());
            }
		}
	}

	return true;
}

bool PrimaryIndex::title_exists(std::string title)
{
    P_Node *wp = head;
    while (wp->next != NULL) {
        if (wp->_title == title)
            return true;
        else wp = wp->next;
    }
    return false;
}

int PrimaryIndex::index_of(std::string title)
{
    P_Node *wp = head;
    while (wp->next != NULL) {
        if (toLowerCase(wp->_title) == toLowerCase(title)) {
            return wp->_tag;
        } else wp = wp->next;
    }
    return -1;
}

void PrimaryIndex::kill_list()
{
    P_Node *wp = head;
    while (wp->next != NULL) {
        wp = wp->next;
        delete wp->prev;
    }
    delete tail;
    head = tail = NULL;
}
