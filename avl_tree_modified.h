// Modified by: Yiheng Cen Feng
// Date of Creation: March 12th, 2019
// File's Title: avl_tree.h
// Description: an AVL Tree class that implements double rotations directly instead of calling the two single rotations.

#ifndef AVL_TREE_MODIFIED_H
#define AVL_TREE_MODIFIED_H

#include "dsexceptions.h"
#include "sequence_map.h"
#include <algorithm>
#include <iostream>
#include <math.h>
using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// void findRecoSeq( x ) --> Find x and print its enzyme acronym
// int numberOfNodes()    --> Return number of nodes
// float averageDepth()   --> Return the average depth of the tree
// float averageDepthRatio() --> Return the ratio of the average depth of the tree
// int find( x, recursive_call ) --> Return 1 if item is found, else 0 
// int remove( x, recursive_call ) --> Return 1 if item is removed, else 0
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class AvlTree
{
public:
    AvlTree( ) : root{ nullptr }
    { }
    
    AvlTree( const AvlTree & rhs ) : root{ nullptr }
    {
        root = clone( rhs.root );
    }
    
    AvlTree( AvlTree && rhs ) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }
    
    ~AvlTree( )
    {
        makeEmpty( );
    }
    
    /**
     * Deep copy.
     */
    AvlTree & operator=( const AvlTree & rhs )
    {
        AvlTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }
    
    /**
     * Move.
     */
    AvlTree & operator=( AvlTree && rhs )
    {
        std::swap( root, rhs.root );
        return *this;
    }
    
    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMin( root )->element;
    }
    
    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMax( root )->element;
    }
    
    /**
     * Returns true if x is found in the tree.
     */
    bool contains( const Comparable & x ) const
    {
        return contains( x, root );
    }
    
    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root == nullptr;
    }
    
    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ) const
    {
        if( isEmpty( ) )
            cout << "Empty tree" << endl;
        else
            printTree( root );
    }
    
    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root );
    }
    
    /**
     * Insert x into the tree; duplicates will be merged.
     */
    void insert( const Comparable & x )
    {
        insert(x, root);
    }
    
    /**
     * Insert x into the tree; duplicates will be merged.
     */
    void insert( Comparable && x )
    {
        insert(std::move(x), root);
    }
    
    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
        remove( x, root );
    }
    
    /**
     * Find the item in the tree and print the associated enzyme acronym
     */
    void findRecoSeq( const string & x ) const
    {
        return findRecoSeq( x, root );
    }
    
    /**
     * Return the number of nodes in the tree
     */
    int numberOfNodes() const{
        return numberOfNodes( root );
    }
    
    /**
     * Return the average depth of the tree
     */
    float averageDepth() const{
        float tree_depth = 0;
        return depth( root, tree_depth ) / numberOfNodes();
    }
    
    /**
     * Return the the ratio of the average depth of the tree
     */
    float averageDepthRatio() const{
        return averageDepth() / log2(numberOfNodes());
    }
    
    /**
     * Return 1 if item is found, else 0
     */
    int find( const string & x, int &find_recursive_call ) const{
        return find( x, root, find_recursive_call);
    }
    
    /**
     * Return 1 if item is removed, else 0
     */
    int remove( const string & x, int &remove_recursive_call ){
        return remove( x, root, remove_recursive_call);
    }
    
    
private:
    struct AvlNode
    {
        Comparable element;
        AvlNode   *left;
        AvlNode   *right;
        int       height;
        
        AvlNode( const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0 )
        : element{ ele }, left{ lt }, right{ rt }, height{ h } { }
        
        AvlNode( Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0 )
        : element{ std::move( ele ) }, left{ lt }, right{ rt }, height{ h } { }
    };
    
    AvlNode *root;
    
    
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     * In case of duplicates, call Merge().
     */
    void insert( const Comparable & x, AvlNode * & t )
    {
        if( t == nullptr )
            t = new AvlNode{ x, nullptr, nullptr };
        else if( x < t->element )
            insert( x, t->left );
        else if( t->element < x )
            insert( x, t->right );
        else{
            t->element.Merge(x);
            return;
        }
        
        balance( t );
    }
    
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     * In case of duplicates, call Merge().
     */
    void insert( Comparable && x, AvlNode * & t )
    {
        if( t == nullptr )
            t = new AvlNode{ std::move( x ), nullptr, nullptr };
        else if( x < t->element )
            insert( std::move( x ), t->left );
        else if( t->element < x )
            insert( std::move( x ), t->right );
        else{
            t->element.Merge(x);
            return;
        }
        
        balance( t );
    }
    
    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, AvlNode * & t )
    {
        if( t == nullptr )
            return;   // Item not found; do nothing
        
        if( x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left != nullptr && t->right != nullptr ) // Two children
        {
            t->element = findMin( t->right )->element;
            remove( t->element, t->right );
        }
        else
        {
            AvlNode *oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;
            delete oldNode;
        }
        
        balance( t );
    }
    
    static const int ALLOWED_IMBALANCE = 1;
    
    // Assume t is balanced or within one of being balanced
    void balance( AvlNode * & t )
    {
        if( t == nullptr )
            return;
        
        if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE ) {
            if( height( t->left->left ) >= height( t->left->right ) )
                rotateWithLeftChild( t );
            else
                doubleWithLeftChild( t );
        } else if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE ) {
            if( height( t->right->right ) >= height( t->right->left ) )
                rotateWithRightChild( t );
            else
                doubleWithRightChild( t );
        }
        t->height = max( height( t->left ), height( t->right ) ) + 1;
    }
    
    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    AvlNode * findMin( AvlNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t->left );
    }
    
    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    AvlNode * findMax( AvlNode *t ) const
    {
        if( t != nullptr )
            while( t->right != nullptr )
                t = t->right;
        return t;
    }
    
    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the tree.
     */
    bool contains( const Comparable & x, AvlNode *t ) const
    {
        if( t == nullptr )
            return false;
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            return true;    // Match
    }
    /****** NONRECURSIVE VERSION*************************
     bool contains( const Comparable & x, AvlNode *t ) const
     {
     while( t != nullptr )
     if( x < t->element )
     t = t->left;
     else if( t->element < x )
     t = t->right;
     else
     return true;    // Match
     
     return false;   // No match
     }
     *****************************************************/
    
    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( AvlNode * & t )
    {
        if( t != nullptr )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullptr;
    }
    
    /**
     * Internal method to print a subtree rooted at t in sorted order.
     */
    void printTree( AvlNode *t ) const
    {
        if( t != nullptr )
        {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
    }
    
    /**
     * Internal method to clone subtree.
     */
    AvlNode * clone( AvlNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new AvlNode{ t->element, clone( t->left ), clone( t->right ), t->height };
    }
        // Avl manipulations
    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height( AvlNode *t ) const
    {
        return t == nullptr ? -1 : t->height;
    }
    
    int max( int lhs, int rhs ) const
    {
        return lhs > rhs ? lhs : rhs;
    }
    
    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild( AvlNode * & k2 )
    {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;
    }
    
    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild( AvlNode * & k1 )
    {
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;
    }
    
    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild( AvlNode * & k3 )
    {
        AvlNode *k1 = k3->left;
        AvlNode *k2 = k1->right;
        k1->right = k2 -> left;
        k2->left = k1;
        k3->left = k2->right;
        k2->right = k3;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k3->height = max( height( k3->left ), height( k3->right ) ) + 1;
        k2->height = max( k1->height, k3->height ) + 1;
        k3 = k2;
    }
    
    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild( AvlNode * & k1 )
    {
        AvlNode *k3 = k1->right;
        AvlNode *k2 = k3->left;
        k3->left = k2->right;
        k2->right = k3;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k3->height = max( height( k3->left ), height( k3->right ) ) + 1;
        k2->height = max( k1->height, k3->height ) + 1;
        k1 = k2;
    }
    
    /**
     * Find the item in the tree and print the associated enzyme acronym if found
     * Else print "Not Found"
     */
    void findRecoSeq( const string & x, AvlNode *t ) const
    {
        if( t == nullptr )
            cout<<"Not Found"<<endl;
        else if( x < t->element.getRecognitionSequence() )
            return findRecoSeq( x, t->left );
        else if( t->element.getRecognitionSequence() < x )
            return findRecoSeq( x, t->right );
        else
            t -> element.printEnzymeAcronym();
    }
    
    /**
     * Return the number of nodes in the tree
     */
    int numberOfNodes( AvlNode *t ) const{
        if(t != nullptr)
            return numberOfNodes(t -> left) + numberOfNodes(t -> right) +1;
        return 0;
    }
    
    /**
     * Return the depth of the tree
     */
    float depth( AvlNode *t, float tree_depth ) const{
        if(t != nullptr)
            return depth( t-> left, tree_depth+1 ) + depth( t-> right, tree_depth+1 ) + tree_depth;
        return 0;
    }
    
    /**
     * Search for an item in the tree
     * x is item to search for.
     * t is the node that roots the tree.
     * Update the number of recursive calls made
     */
    bool find( const string & x, AvlNode *t, int &find_recursive_call ) const{
        ++find_recursive_call;
        if( t == nullptr )
            return false;
        else if( x < t->element.getRecognitionSequence() )
            return find( x, t->left, find_recursive_call );
        else if( t->element.getRecognitionSequence() < x )
            return find( x, t->right, find_recursive_call );
        else
            return true;
    }
    
    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     * Update the number of recursive calls made.
     */
    bool remove( const string & x, AvlNode * & t, int &remove_recursive_call ){
        ++remove_recursive_call;
        if( t == nullptr )
            return false;
        if( x < t->element.getRecognitionSequence() )
            return remove( x, t->left, remove_recursive_call );
        else if( t->element.getRecognitionSequence() < x )
            return remove( x, t->right, remove_recursive_call );
        // Two children
        else if( t->left != nullptr && t->right != nullptr ){
            t->element = findMin( t->right )->element;
            return remove( t->element.getRecognitionSequence(), t->right, remove_recursive_call );
        }
        else{
            AvlNode *oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;
            delete oldNode;
            return true;
        }
        balance( t );
    }
    
};

#endif

