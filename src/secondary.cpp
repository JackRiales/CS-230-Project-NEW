/*
    Secondary Index Class Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "secondary.h"
using namespace std;

// Constructor
SecondaryIndex::SecondaryIndex():
head (new SE_Node(" ")),
tail (new SE_Node("~")),
_size(2)
{
    #ifdef _DEBUG_
	printf("Instantiating a new secondary index.\n");
    #endif

    head->next = tail;
    tail->prev = head;
}

SecondaryIndex::~SecondaryIndex()
{
    #ifdef _DEBUG_
    printf ("Destroying secondary index.\n");
    #endif // _DEBUG_
	kill_list();
}

bool SecondaryIndex::update_type(std::string type)
{
    if (type == "0") {
        #ifdef _DEBUG_
        printf ("Do not try to set a null type. No add performed.\n");
        #endif // _DEBUG_
        return false;
    }

	if (type_match(type)) {
        // Type is matched with another type. Find that type and increment its duplication tag
        #ifdef _DEBUG_
        printf ("Type %s is matched with another node. Updating duplication.\n", type.c_str());
        #endif // _DEBUG_
        SE_Node *wp = head;
        while (wp->next != NULL) {
            if (toLowerCase(wp->_type) == toLowerCase(type)) {
                wp->_duplicates++;
                return true;
            } else {
                wp = wp->next;
            }
        }
	} else {
        // Type not in the secondary. Add it.
        SE_Node *wp = head;
        while (wp->next != NULL && wp->_type < type) {
            if (wp->next->_type > type || wp->next == tail) {
                #ifdef _DEBUG_
                printf ("Placing new node with type %s into secondary index.\n", type.c_str());
                #endif // _DEBUG_
                // Place it
                SE_Node *_new_node = new SE_Node(type);
                _new_node->next = wp->next;
                _new_node->prev = wp;
                wp->next->prev = _new_node;
                wp->next = _new_node;

                // Increment size
                _size++;

                return true;
            } else {
                wp = wp->next;
            }
        }
	}
	return true;
}

bool SecondaryIndex::write (std::fstream& out)
{
	// Error check stream
	if (!out) {
        #ifdef _DEBUG_
		printf ("Output stream is not open. Please check your parameters.\n");
        #endif // _DEBUG_

		return false;
	}

    #ifdef _DEBUG_
	printf ("Writing Secondary Index to output stream.\n");
    #endif // _DEBUG_

	// Iterate through listing
	SE_Node *wp = head;
	while (wp->next != NULL) {
		// Output; one line per listing
		out
		<< "\n"                     // New line
        << wp->_type << "\t\t"      // Output the title at that list
		<< wp->_duplicates << "\t"  // Output the duplicates at that list
		<< "\n";

        wp = wp->next;
	}

	// All is well
	return true;
}

bool SecondaryIndex::read (BinaryData objects[], unsigned int length)
{
    #ifdef _DEBUG_
	printf ("Reading binary object array into secondary index.\n");
    #endif // _DEBUG_

	// If length arg is 0, attempt to calculate the length using size division
	if (length == 0) {
        #ifdef _DEBUG_
		printf ("Length argument 0. Attempting to calculate length from array.");
        #endif // _DEBUG_

		length = sizeof(objects)/sizeof(objects[0]);
	}

	// Calculated length still zero exception.
	if (length == 0) {
        #ifdef _DEBUG_
		printf ("Calculated length is zero! Exiting.\n");
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
            #ifdef _DEBUG_
			printf ("Update to secondary index: Type %s\n", objects[i].type().c_str());
            #endif // _DEBUG_

			update_type(objects[i].type());
		}
	}

	return true;
}

bool SecondaryIndex::type_match(std::string type)
{
	SE_Node *wp = head;
	while (wp->next != NULL) {
        if (toLowerCase(wp->_type) == toLowerCase(type)) {
            return true;
        } else {
            wp = wp->next;
        }
	}
	#ifdef _DEBUG_
	printf ("Type %s not found.\n", type.c_str());
	#endif // _DEBUG_
	return false;
}

int SecondaryIndex::duplicates_of(std::string type) {
    if (type_match(type)) {
        SE_Node *wp = head;
        while (wp->next != NULL) {
            if (toLowerCase(wp->_type) == toLowerCase(type)) {
                return wp->_duplicates;
            }
        }
    }
    return -1;
}

void SecondaryIndex::kill_list()
{
    SE_Node *wp = head;
    while (wp->next != NULL) {
        wp = wp->next;
        delete wp->prev;
    }
    delete tail;
    head = tail = NULL;
}
