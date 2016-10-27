// priority_queue.h
// The DistancePixel_PriorityQueue is a customized, non-templated
// priority queue that stores DistancePixel pointers in a heap.  The 
// elements in the heap can be looked up in a map, to quickly find out
// the current index of the element within the heap.

#ifndef priority_queue_h_
#define priority_queue_h_

#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>

#include "image.h"

// =========================================================================
// =========================================================================
class DistancePixel_PriorityQueue {

public:
  
    // --------------------------
    // CONSTRUCTORS
    // --------------------------
    // Default constructor
    DistancePixel_PriorityQueue() {}

    // Construct a heap from a vector of data
    DistancePixel_PriorityQueue(const std::vector<DistancePixel*> &values) 
    {
        //Add every pixel in the values vector to the heap
        for(uint i=0; i<values.size(); ++i)
        {
            push(values[i]);
        }
    }

    // ------------------------
    // ACCESSORS
    // -------------------------
    // Get the heap size
    int size() 
    { 
        return m_heap.size(); 
    }

    // Access whether or not the heap is empty
    bool empty() 
    { 
        return m_heap.empty(); 
    }

    // Access the index of last element in the tree which is not a leaf
    int last_non_leaf() 
    { 
        return (size()-1) / 2; 
    }

    // Get the parent index
    int get_parent(int i) 
    { 
        assert (i > 0 && i < size()); return (i-1) / 2; 
    }

    // Return whether or not the element has a left child
    int has_left_child(int i) 
    { 
        return (2*i)+1 < size(); 
    }

    // Return whether or not the element has a right child
    int has_right_child(int i) 
    { 
        return (2*i)+2 < size(); 
    }

    // Get the left child index
    int get_left_child(int i) 
    {
        assert (i >= 0 && has_left_child(i)); return 2*i + 1; 
    }

    // Get the right child index
    int get_right_child(int i) 
    { 
        assert (i >= 0 && has_right_child(i)); return 2*i + 2; 
    }

    // --------------------------
    // OTHER HELPFUL MEMBER FUNCTIONS
    // --------------------------
    // Read the top element
    const DistancePixel* top() const  
    {
        assert(!m_heap.empty());
        return m_heap[0]; 
    }

    // Is this element in the heap?
    bool in_heap(DistancePixel* element) const 
    {
        std::map<DistancePixel*,int>::const_iterator itr = backpointers.find(element);
        return (itr != backpointers.end());
    }

    // add an element to the heap
    void push(DistancePixel* element) 
    {
        std::map<DistancePixel*,int>::iterator itr = backpointers.find(element);
        //std::cout<<"Assert D="<<element->getValue()<<" C="<<element->getX()<<","<<element->getY()<<" matches end\n";

        assert (itr == backpointers.end());
        m_heap.push_back(element);
        backpointers[element] = m_heap.size()-1;
        this->percolate_up(int(m_heap.size()-1));
    }

    // The value of this element has been edited, move the element up or down
    void update_position(DistancePixel* element) 
    {
        std::map<DistancePixel*,int>::iterator itr = backpointers.find(element);
        assert (itr != backpointers.end());
        this->percolate_up(itr->second);
        this->percolate_down(itr->second);
    }
  
    // Remove the top (minimum) element
    void pop() 
    {
        assert(!m_heap.empty());
        int success = backpointers.erase(m_heap[0]);
        assert (success == 1);
        m_heap[0] = m_heap.back();
        m_heap.pop_back();
        this->percolate_down(0);
    }
  
private:

    // --------------------------
    // REPRESENTATION
    // --------------------------
    // The heap is stored in a vector representation (the binary tree
    //  structure "unrolled" one row at a time)
    std::vector<DistancePixel*> m_heap;
    // The map stores a correpondence between elements & indices in the heap
    std::map<DistancePixel*,int> backpointers;

    // --------------------------
    // HELPER FUNCTIONS
    // --------------------------
    // Function that helps add an item to the heap
    // It begins with the item at the bottom of the heap and checks the parent
    // If the item you're adding has priority over the parent, switch the two
    void percolate_up(int i) 
    {
        // If you are not on the first pixel in the vector
        //  and your value is less than that of your parent
        if((i > 0) && (m_heap[i]->getValue() < m_heap[get_parent(i)]->getValue()))
        {
            // Make a temp equal to the pixel you're moving
            DistancePixel* tmp;
            tmp = m_heap[i];

            // Swap pixel you're on with parent
            m_heap[i] = m_heap[get_parent(i)];

            // Swap parent with original pixel
            m_heap[get_parent(i)] = tmp;

            // RECURSE!
            percolate_up(get_parent(i));
        }
    }
  
    // When you remove the top element of the heap, you must replace it
    // This helper function takes the element from the bottom of the heap and adds it to the top
    // It then compares the element to its children
    // If either of the children have precedence, they are switched with the parent
    void percolate_down(int i) 
    {
        // If either the left or right child has precedence over the parent
        if(
            ( has_left_child(i)  && (m_heap[i]->getValue() >  m_heap[get_left_child(i)]->getValue()) ) 
            || 
            ( has_right_child(i) && (m_heap[i]->getValue() > m_heap[get_right_child(i)]->getValue()) )
          )
        {
            // If you need to compare both the left and right children
            if(has_left_child(i) && has_right_child(i))
            {
                // If the right child has precedence
                if(m_heap[get_left_child(i)]->getValue() > m_heap[get_right_child(i)]->getValue())
                {
                    // Make a temp equal to the pixel you're moving
                    DistancePixel* tmp;
                    tmp = m_heap[i];

                    // Swap the pixel you're on with the child
                    m_heap[i] = m_heap[get_right_child(i)];

                    // Swap the child with the parent
                    m_heap[get_right_child(i)] = tmp;

                    // RECURSE!
                    percolate_down(get_right_child(i));
                }

                // If the left child has precedence
                else
                {
                    // Make a temp equal to the pixel you're moving
                    DistancePixel* tmp;
                    tmp = m_heap[i];

                    // Swap the pixel you're on with the child
                    m_heap[i] = m_heap[get_left_child(i)];

                    // Swap the child with the parent
                    m_heap[get_left_child(i)] = tmp;

                    // RECURSE!
                    percolate_down(get_left_child(i));
                }
            }

            // Otherwise, if there is only a left child
            else if(has_left_child(i))
            {
                // Make a temp equal to the pixel you're moving
                DistancePixel* tmp;
                tmp = m_heap[i];

                // Swap the pixel you're on with the child
                m_heap[i] = m_heap[get_left_child(i)];

                // Swap the child with the parent
                m_heap[get_left_child(i)] = tmp;

                // RECURSE!
                percolate_down(get_left_child(i));
            }

            // Otherwise, if there is only a right child
            else
            {
                // Make a temp equal to the pixel you're moving
                DistancePixel* tmp;
                tmp = m_heap[i];

                // Swap the pixel you're on with the child
                m_heap[i] = m_heap[get_right_child(i)];

                // Swap the child with the parent
                m_heap[get_right_child(i)] = tmp;

                // RECURSE!
                percolate_down(get_right_child(i));
            }
        }
    }
};
// =========================================================================
// =========================================================================

#endif
