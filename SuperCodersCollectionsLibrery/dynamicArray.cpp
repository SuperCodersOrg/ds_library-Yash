#include <cstdlib> 


template<typename T>
class DynamicArray{
    private:
        T* arr;
        int size;
        int capacity;
        void resize(){
            capacity *= 2;
            T* newArr = new T[capacity];
            for(int i = 0; i<size; i++){
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
        }
    public:
        DynamicArray(){
            capacity = 4;
            arr = (T*)malloc(capacity*sizeof(T));
            size = 0;

        }
        DynamicArray(const DynamicArray& other){
            capacity = other.capacity;
            size = other.size;
            arr = (T*)malloc(capacity*sizeof(T));
            for(int i =0; i<size ;i++){
                arr[i] = other.arr[i];
            }
        }
        ~DynamicArray(){
            free(arr);
            arr = nullptr;
        }
        void append(T val){
             if(size == capacity){
                resize();
             }
             arr[size] = val;
             size++;
        }
        T& get(int idx){
            if(idx<0 || idx >= size){
                throw "Index out of bounds";
            }
            return arr[idx];
        }
        void insert(int idx, T val){
            if(idx<0 || idx > size){
                throw "Index out of bounds";
            }
            if(size == capacity){
                resize();
            }
            for(int i = size-1; i>= idx; i--){
                arr[i+1] = arr[i];
            }
            arr[idx] = val;
            size++;
        }
        void remove(int idx){
            if(idx<0 || idx >= size){
                throw "Index out of bounds";
            }
            for(int i = idx; i<size-1; i++){
                arr[i] = arr[i+1];
            }
            size--;
        }
        int getSize(){
        return size;
    }
};
