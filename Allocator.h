/** @file Allocator.h
 *  @brief Header file with the function.
 *   
 *  Handles the allocation of the memory for our Allocator<T, N>
 */
#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument
#include "gtest/gtest_prod.h" //FRIEND_TEST
#include <iostream>

using namespace std;

// ---------
// Allocator
// ---------

template <typename T, std::size_t N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;            //unsigned integer: gives us the size of the object in bytes
        typedef std::ptrdiff_t    difference_type;      //std::ptrdiff_t is the signed integer type of the result of subtracting two pointers.

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}                                      //this is correct

    private:
        // ----
        // data
        // ---
        //conventional to request a memory that is continous in bytes.
        char a[N];


        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * <br> <b>My documentation</b>
         * <br> To check if the sentinels in the memory block are assigned in the right manner
         */
        FRIEND_TEST(TestValid, valid_4);
        FRIEND_TEST(TestValid1, valid);
        FRIEND_TEST(TestValid2, valid);
        FRIEND_TEST(TestValid3, valid);
        bool valid () const {
            // <your code>
            assert(N > 8); // Assured by constructor

            int first_sentinnel = 0, another_sentinnel = 0;
            for(int i = 0; i < N; i = i + abs(first_sentinnel) + 2*sizeof(int))
            {
                first_sentinnel = (*this)[i];
                another_sentinnel = (*this)[i + abs(first_sentinnel) + 4];
                if(first_sentinnel != another_sentinnel || first_sentinnel == 0 || i + abs(first_sentinnel) + 2*sizeof(int) > N)
                    return false;
                else if( i + 7 + abs(first_sentinnel) == N - 1)
                    return true;
            }
            return false;};

        FRIEND_TEST(TestAllocator2, index);
        FRIEND_TEST(TestAllocator4, index);
        FRIEND_TEST(TestAllocator5, index);
        FRIEND_TEST(TestAllocator6, index);
        FRIEND_TEST(TestAllocator7, index);
        FRIEND_TEST(TestAllocator8, index);
        FRIEND_TEST(AllocatorTests, valid);
        FRIEND_TEST(AllocatorTests1, valid);
        FRIEND_TEST(AllocatorTests2, valid);
        FRIEND_TEST(AllocatorTests3, valid);
        FRIEND_TEST(AllocatorTests4, valid);
        FRIEND_TEST(TestAllocate3, valid);
        FRIEND_TEST(TestAllocate4, valid);

        /**
         * O(1) in space
         * O(1) in time
         * <br> <b>My documentation</b>
         * <br>To access the memory block that we are using for the allcator.
         * This one is a Read and Write operator.
         * <br> https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
         //what's yo business?
        //read and write
        int& operator [] (int i) {
            return *reinterpret_cast<int*>(&a[i]);  //gives me 4 bytes of the data since it casts and reinterprets the memory address as integer instead of char which is 1 byte
        }   

        //view(a[i]);
    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time - positive
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        //#ask do we need any arguments
        Allocator () {
            //initialize the memory with centinals
            if(N < sizeof(T) + 2 * sizeof(int))
                throw std::bad_alloc();
            
            (*this)[0] = (N - 8);
            (*this)[(N-4)] = (N-8);
            assert(valid());}

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time - positive
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * throw a bad_alloc exception, if n is invalid
         */
         //#ask how many blocks of data are giving out? Or what is our data? Why are we just giving out size of data?
        pointer allocate (size_type n) {
               // <your code>
            //int b = (*this)[i]; //this would give me the sentinnel at i
            if(n<=0){
                throw std::bad_alloc();
            }
            else if (n == 0)
                return nullptr;

            int i = 0;
            while(i<N)
            {
                int& first_sentinnel = (*this)[i];
                int n_bytes = n*sizeof(T);
                int abs_sentinel = abs(first_sentinnel);
                if( first_sentinnel > 0)
                {   
                    int& another_sentinnel = (*this)[i + first_sentinnel + sizeof(int)];
                        //if the caller requests the exact amount of data i.e N-8 bytes
                        if(first_sentinnel >= n_bytes && first_sentinnel <= 2 * sizeof(int) + n_bytes + sizeof(T))
                        {
                            first_sentinnel = -first_sentinnel;
                            another_sentinnel = -another_sentinnel;
                            assert(valid());
                            return reinterpret_cast<pointer>(&first_sentinnel + 1);
                        }
                        else if(first_sentinnel > n_bytes + 2 * sizeof(int) + sizeof(T))
                        {
                            //cool I have enough space to store my data
                            //so I change my sentinnels
                           first_sentinnel = -n_bytes;
                           ((*this)[i + n_bytes + 4]) = -n_bytes;
                           ((*this)[i + n_bytes + 8]) = another_sentinnel - n_bytes - 8;
                           another_sentinnel = another_sentinnel - n_bytes - 8;
                           assert(valid());
                           return reinterpret_cast<pointer>(&first_sentinnel + 1);
                            //we want to make sure everything is valid
                        }
                }
                i = i + abs_sentinel + 8;
            }

            throw std::bad_alloc();
            // cout << "returning 0" << endl;
            return 0;}

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time - positive
         */
         //#ask do we call construct when we create a new value_type pointer?
        void construct (pointer p, const_reference v) {
            //cout<<"I am in construct"<<endl;
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time - positive
         * <br>after deallocation adjacent free blocks must be coalesced
         * throw an invalid_argument exception, if p is invalid.
         * <br> <b>My documentation</b>
         * <br>Deallocate to free the memory in the block. We change the sentinels to indicate that.
         */
        void deallocate (pointer p, size_type) {
            // <your code>
             if (p == nullptr)
                return;

            int& first_sentinnel = *(reinterpret_cast<int*>(p)- 1);
            int& last_sentinnel = *(reinterpret_cast<int*>(p) + ((abs(first_sentinnel))/sizeof(int)));

            //to check if our address is invalid
            if(first_sentinnel > 0)
                throw std::invalid_argument("invalid pointer");
            if(abs(first_sentinnel) > N - 8)
                throw std::invalid_argument("invalid pointer");
            if(first_sentinnel != last_sentinnel)
                throw std::invalid_argument("invalid pointer");

            if(&first_sentinnel - 1 > &(*this)[4] || &last_sentinnel + 1 < &(*this)[N-8]) // to check whether we have space in front of the first and the last sentinnel of the data we need to deallocate
            {
                int& previous_sentinnel =  *(&first_sentinnel - 1);
                int& after_sentinnel = *(&last_sentinnel + 1);

                if(previous_sentinnel > 0 && after_sentinnel > 0) //If there are open spaces before and after, we will have to merge them
                {
                    int& pre_sentinnel = *(&previous_sentinnel - (previous_sentinnel/sizeof(int)) - 1);
                    int& aft_sentinnel = *(&after_sentinnel + (after_sentinnel/sizeof(int)) + 1);
                    pre_sentinnel = previous_sentinnel + 4 + 4 + abs(first_sentinnel) + 4 + 4 + after_sentinnel;
                    aft_sentinnel = pre_sentinnel;
                }
                else if(previous_sentinnel > 0 ) //If there is open space only in the front, we merge it and change previous sentinnel and last sentinnel
                {
                    int& pre_sentinnel = *(&previous_sentinnel - (previous_sentinnel/sizeof(int)) - 1);
                    pre_sentinnel = previous_sentinnel + 4 + 4 + abs(first_sentinnel);
                    last_sentinnel = pre_sentinnel;
                }
                else if(after_sentinnel > 0) //If there is open space only in the back, we merge it, change after sentinnel and first sentinnel
                {
                    int& aft_sentinnel = *(&after_sentinnel + (after_sentinnel/sizeof(int)) + 1);
                    aft_sentinnel = after_sentinnel + 4 + 4 + abs(last_sentinnel);
                    first_sentinnel = aft_sentinnel;
                }
                else //If we cannot merge it, we just make that space available by making the sentinels positive
                {
                    first_sentinnel = abs(first_sentinnel);
                    last_sentinnel = first_sentinnel;
                }
            }
            else //If there is no space available before or after the sentinnels which is the whole memory block is to be deallocated
            {
                first_sentinnel = abs(first_sentinnel);
                last_sentinnel = first_sentinnel;
            }
            
            assert(valid());}

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time - positive
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * <br> <b>My documentation</b>
         * <br>To access the memory block that we are using for the allcator.
         * This one is Read only operator since it is const.
         * We do not want users to mess with the memory.
         */
         //const so read only
        const int& operator [] (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

#endif // Allocator_h