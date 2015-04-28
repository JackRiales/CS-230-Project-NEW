/*
    Secondary Index Class Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "secondary.h"
using namespace std;

void SE_Node::add(std::string type, SE_Node *&r)
{
    #ifdef _DEBUG_
    printf("Adding new node with type %s.\n", type.c_str());
    #endif
    if (r == NULL) {
        #ifdef _DEBUG_
        printf("Emplacing node.\n");
        #endif
        r = new SE_Node(type);
    } else {
        if (toLowerCase(type) < toLowerCase(r->_type)) {
            #ifdef _DEBUG_
            printf("type %s is lower than %s. Going left.\n", toLowerCase(type).c_str(), toLowerCase(r->_type).c_str());
            #endif
            add(type, r->left);
        }
        else {
            #ifdef _DEBUG_
            printf("type %s is higher than %s. Going right.\n", toLowerCase(type).c_str(), toLowerCase(r->_type).c_str());
            #endif
            add(type, r->right);
        }
    }
}

bool SE_Node::write(std::ostream& out, SE_Node *r)
{
    #ifdef _DEBUG_
    printf("Attempting to write node.\n");
    #endif
    if (r == NULL) {
        printf("Node is null.\n");
        return false;
    } else {
        #ifdef _DEBUG_
        printf("Writing node with type %s.\n", r->_type.c_str());
        #endif
        out << r->_type.c_str() << '\t' << r->_duplicates << '\n';
        write(out, r->left);
        write(out, r->right);
    }
    return true;
}

// =====

// Constructor
SecondaryIndex::SecondaryIndex():
root(NULL),
_size(0)
{
    #ifdef _DEBUG_
	printf("Instantiating a new secondary index.\n");
    #endif
}

SecondaryIndex::~SecondaryIndex()
{
    #ifdef _DEBUG_
    printf ("Destroying secondary index.\n");
    #endif // _DEBUG_

	kill_tree(root);
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
        SE_Node *c = root;
        while (c != NULL) {
            if (toLowerCase(c->_type) == toLowerCase(type)) {
                c->_duplicates++;
                return true;
            } else if (toLowerCase(c->_type) < toLowerCase(type)) {
                c = c->left;
            } else {
                c = c->right;
            }
        }
	} else {
        // Set the new node
        SE_Node::add(type, root);

        // Increment size
        _size++;
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

    // Recursively write tree
    SE_Node::write(out, root);

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
	SE_Node *c = root;
    while (c != NULL) {
        if (toLowerCase(c->_type) == toLowerCase(type)) {
            return true;
        } else if (toLowerCase(c->_type) > toLowerCase(type)) {
            c = c->left;
        } else {
            c = c->right;
        }
    }
	#ifdef _DEBUG_
	printf ("Type %s not found.\n", type.c_str());
	#endif // _DEBUG_
	return false;
}

int SecondaryIndex::duplicates_of(std::string type) {
    if (type_match(type)) {
        SE_Node *c = root;
        while (c != NULL) {
            if (toLowerCase(c->_type) == toLowerCase(type)) {
                return c->_duplicates;
            } else if (toLowerCase(c->_type) > toLowerCase(type)) {
                c = c->left;
            } else {
                c = c->right;
            }
        }
    }
    return -1;
}

void SecondaryIndex::kill_tree(SE_Node *r)
{
    if (r != NULL)
    {
        kill_tree(r->left);
        kill_tree(r->right);
        delete r;
    }
}
