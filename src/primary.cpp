/*
    Primary Index Class Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "primary.h"
using namespace std;

void P_Node::add(std::string title, int key, P_Node *&r)
{
    #ifdef _DEBUG_
    printf("Adding new node with title %s.\n", title.c_str());
    #endif
    if (r == NULL) {
        #ifdef _DEBUG_
        printf("Emplacing node.\n");
        #endif
        r = new P_Node(title, key);
    } else {
        if (toLowerCase(title) < toLowerCase(r->_title)) {
            #ifdef _DEBUG_
            printf("Title %s is lower than %s. Going left.\n", toLowerCase(title).c_str(), toLowerCase(r->_title).c_str());
            #endif
            add(title, key, r->left);
        }
        else {
            #ifdef _DEBUG_
            printf("Title %s is higher than %s. Going right.\n", toLowerCase(title).c_str(), toLowerCase(r->_title).c_str());
            #endif
            add(title, key, r->right);
        }
    }
}

bool P_Node::write(std::ostream& out, P_Node *r)
{
    #ifdef _DEBUG_
    printf("Attempting to write node.\n");
    #endif
    if (r == NULL) {
        #ifdef _DEBUG_
        printf("Node is null.\n");
        #endif
        return false;
    } else {
        #ifdef _DEBUG_
        printf("Writing node with title %s.\n", r->_title.c_str());
        #endif
        out << r->_title.c_str() << '\t' << r->_tag << '\n';
        write(out, r->left);
        write(out, r->right);
    }
    return true;
}

// =====

// Constructor
PrimaryIndex::PrimaryIndex():
root(NULL),
_size (0)
{
    #ifdef _DEBUG_
	printf("Instantiating a new primary index.\n");
    #endif
}

// Destructor
PrimaryIndex::~PrimaryIndex()
{
    #ifdef _DEBUG_
	printf("Destroying primary index.\n");
    #endif
	kill_tree(root);
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

        // Set new node
        P_Node::add(title, key, root);

        // Increment size
        _size++;

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

    // Recursively write out the tree
    P_Node::write(out, root);

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
    return index_of(title) != -1;
}

int PrimaryIndex::index_of(std::string title)
{
    P_Node *current = root;
    while (current != NULL) {
        if (toLowerCase(current->_title) == toLowerCase(title))
            return current->_tag;
        else {
            if (toLowerCase(title) < toLowerCase(current->_title))
                current = current->left;
            else
                current = current->right;
        }
    }
    return -1;
}

void PrimaryIndex::kill_tree(P_Node *r)
{
    if (r != NULL)
    {
        kill_tree(r->left);
        kill_tree(r->right);
        delete r;
        r=NULL;
    }
}
