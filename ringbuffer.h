#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <algorithm>
#include <memory.h>

//作为我们后面测试填入模板的具体类型进行测试，可忽略
struct Info{
    u_int32_t timestamp;
    double p;
    u_int32_t v;
};

//环形缓存
template<typename T> 
class ringbuffer {
public:
    /**
      * create a ringbuffer with space for up to size elements.
      */
    explicit ringbuffer(size_t size)
            : size(size)
            , begin(0)
            , end(0)
            , wrap(false)
    {
        buffer = new T[size];
    }

    /**
      * copy constructor
      */
    ringbuffer(const ringbuffer<T> & rb)
    {
        this(rb.size);
        begin = rb.begin;
        end = rb.end;
        memcpy(buffer, rb.buffer, sizeof(T) * size);
    }

    /**
      * destructor
      */
    ~ringbuffer()
    {
        delete[] buffer;
    }

    size_t overWrite(const T * data, size_t n)
    {
        size_t sum = 0;
        if(wrap){
            while(n > 0){
                size_t chunk_size = std::min(size - end, n);
                memcpy(buffer + end, data, chunk_size * sizeof(T));
                begin = (begin + chunk_size) % size;
                end = (end + chunk_size) % size;
                n -= chunk_size;
                sum += chunk_size;
                if(n > 0){
                    data += chunk_size;
                }
            }
        }
        return sum;
                 

    }
    size_t write(const T * data, size_t n){
        size_t sum = 0;
        size_t chunk_size = std::min(size - end, n);
        memcpy(buffer + end, data, chunk_size * sizeof(T));
        end = (end + chunk_size) % size;

        if(n <= chunk_size){
            return n;
        }

        sum += chunk_size;
        n -= chunk_size;
        data += chunk_size;     

        chunk_size = std::min(begin, n);
        memcpy(buffer + end, data, chunk_size * sizeof(T));
        end = (end + chunk_size) % size;
        sum += chunk_size;

        if(end == begin){
            n -= chunk_size;
            wrap = true;
            if(n > 0){
                data += chunk_size;
                sum += overWrite(data, n);
            }

        }

        return sum;

    }



    size_t read(T * dest, size_t n)
    {
        n = std::min(n, getOccupied());

        if (n == 0) {
            return n;
        }

        if (wrap) {
            wrap = false;
        }

        const size_t first_chunk = std::min(n, size - begin);
        memcpy(dest, buffer + begin, first_chunk * sizeof(T));
        begin = (begin + first_chunk) % size;

        if (first_chunk < n) {
            const size_t second_chunk = n - first_chunk;
            memcpy(dest + first_chunk, buffer + begin, second_chunk * sizeof(T));
            begin = (begin + second_chunk) % size;
        }

        return n;        
    }

    size_t getOccupied() {
        return wrap ? size : (end - begin + size) % size;
    }

    size_t getFree() {
        return size - getOccupied();
    }

    size_t getBegin(){
        return begin;
    }
    
    size_t getEnd(){
        return end;
    }

    size_t getSize(){
        return size;
    }

    const T& getNum(size_t pos){
        return *(buffer+pos);
    }

private:
    T * buffer;
    size_t size;
    size_t begin;
    size_t end;
    bool wrap;
};

#endif // RINGBUFFER_H