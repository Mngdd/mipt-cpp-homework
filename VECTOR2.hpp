#ifndef VECTOR2
#define VECTOR2

#include<iostream>
#include<string>

template<typename T>
class Vector2 {
private:
    int vec_size;
    int reserved_size;
    T *data;
    std::string destructor_bad = "cant use template's destructor\n";
    std::string copy_bad = "Cant use copy constructor\n";
    std::string out_of_range = "wrong index: out of range\n";
    std::string empty_vec = "No elements in vector\n";

public:
    Vector2();

    Vector2(int n); // размер n
    
    Vector2(T *arr); // копир
    
    Vector2(const Vector2 &); // к копир
    
    Vector2 &operator=(const Vector2 &); // копир

    Vector2(Vector2 &&); // перемещ

    Vector2 &operator=(Vector2 &&); // перемещ

    ~Vector2();

    T &operator[](int index);

    T &back();

    T &front();

    int size() const;

    int get_reserved_size() const;

    bool empty() const;

    void reserve(int new_alloc);

    void resize(int new_size);

    void resize(int new_size, T fill_value);

    void push_back(T new_elem);

    void insert(int ind, T new_elem);

    void pop_back(); // а эта не питоновские pop'ы, тут воид

    void pop(int index);
};


template<typename T>
Vector2<T>::Vector2() : vec_size{0}, reserved_size{0}, data{nullptr} {}


template<typename T>
Vector2<T>::Vector2(int n) : vec_size{n}, reserved_size{n}, data{new T[n]} {
    try {
        for (size_t i = 0; i < n; ++i)
            data[i] = T();
    }
    catch (...) {
        delete[] data;
        throw std::runtime_error(copy_bad);
    }
}

template<typename T>
Vector2<T>::Vector2(T *array) : vec_size{int(sizeof(*array) / sizeof(T))}, reserved_size{(sizeof(*array) / sizeof(T))},
                                data{new T[int(sizeof(*array) / sizeof(T))]} {
    try {
        for (size_t i = 0; i < int(sizeof(*array) / sizeof(T)); ++i)
            data[i] = array[i];
    }
    catch (...) {
        delete[] data;
        throw std::runtime_error(copy_bad);
    }
}

template<typename T>
Vector2<T>::Vector2(const Vector2<T> &v) : vec_size{int(v.size())}, reserved_size{int(v.reserved_size())},
                                           data{new T[v.reserved_size]} {
    try {
        for (size_t i = 0; i < v.size(); ++i)
            data[i] = v[i];
    }
    catch (...) {
        delete[] data;
        throw std::runtime_error(copy_bad);
    }
}

template<typename T>
Vector2<T> &Vector2<T>::operator=(const Vector2<T> &v) {
    if (this == &v)
        return *this;

    bool flag = false;
    if (v.size() <= reserved_size) {
        try {
            for (size_t i = 0; i < v.size(); ++i) {
                try {
                    data[i].~T();
                }
                catch (...) {
                    flag = true;
                    throw std::runtime_error(destructor_bad);
                }
                data[i] = v[i];
            }
            vec_size = v.size();
            return *this;
        }
        catch (...) {
            if (flag)
                throw std::runtime_error(destructor_bad);
            throw std::runtime_error(copy_bad);
        }
    }
    T *temp_elem;
    try {
        T *temp_elem = new T[v.size()];
        for (size_t i = 0; i < v.size(); ++i)
            temp_elem[i] = v[i];
    }
    catch (...) {
        delete[] data;
        throw std::runtime_error(destructor_bad);
    }
    try {
        for (size_t i = 0; i < vec_size; ++i)
            data[i].~T();
        delete[] data;
    }
    catch (...) {
        delete[] data;
        throw std::runtime_error(destructor_bad);
    }
    delete[] data;
    data = temp_elem;
    reserved_size = v.size();
    vec_size = v.size();
    return *this;
}

template<typename T>
Vector2<T>::Vector2(Vector2<T> &&v) : data{v.data} {
    reserved_size = v.reserved_size();
    v.vec_size = 0;
    v.reserved_size = 0;
    v.data = nullptr;
}

template<typename T>
Vector2<T> &Vector2<T>::operator=(Vector2<T> &&v) {
    try {
        for (size_t i = 0; i < vec_size; ++i)
            data[i].~T();
    }
    catch (...) {
        delete[] data;
        throw std::runtime_error(destructor_bad);
    }

    delete[] data;
    data = v.data;
    data = v.size();
    reserved_size = v.reserved_size();

    v.data = nullptr;
    v.reserved_size = 0;
    v.vec_size = 0;
    return *this;
}

template<typename T>
Vector2<T>::~Vector2() {
    if (data != nullptr && reserved_size != 0 && vec_size != 0) {
        try {
            for (size_t i = 0; i < vec_size; ++i)
                data[i].~T();
        }
        catch (...) {
            delete[] data;
            throw std::runtime_error(destructor_bad);
        }

        delete[] data;
    }
}

template<typename T>
T &Vector2<T>::operator[](int index) {
    if (index < 0 || index >= vec_size)
        throw std::runtime_error(out_of_range);
    return data[index];
}

template<typename T>
T &Vector2<T>::back() {
    if (empty())
        throw std::runtime_error(empty_vec);
    return data[vec_size - 1];
}


template<typename T>
T &Vector2<T>::front() {
    if (empty())
        throw std::runtime_error(empty_vec);
    return data[0];
}

template<typename T>
int Vector2<T>::size() const { return vec_size; }

template<typename T>
int Vector2<T>::get_reserved_size() const { return reserved_size; }

template<typename T>
bool Vector2<T>::empty() const { return size() == 0; }


template<typename T>
void Vector2<T>::reserve(int new_alloc) {
    if (new_alloc <= reserved_size)
        return;
    T *p = new T[new_alloc];
    bool flag = false;

    try {
        for (size_t i = 0; i < vec_size; ++i) {
            try {
                p[i] = data[i];
            }
            catch (...) {
                flag = true;
                throw std::runtime_error(copy_bad);
            }
            data[i].~T();
        }
    }
    catch (...) {
        if (flag) {
            delete[] p;
        }
        delete[] p;
        throw std::runtime_error(copy_bad);
    }

    delete[] data;
    data = p;
    reserved_size = new_alloc;
}

template<typename T>
void Vector2<T>::resize(int new_size) {
    reserve(new_size);
    for (size_t i = vec_size; i < new_size; ++i)
        data[i] = 0;
    vec_size = new_size;
}

template<typename T>
void Vector2<T>::resize(int new_size, T fill_value) {
    reserve(new_size);
    for (size_t i = 0; i < new_size; ++i)
        data[i] = fill_value;
    vec_size = new_size;
}


template<typename T>
void Vector2<T>::push_back(T new_elem) {
    if (vec_size > reserved_size)
        throw std::runtime_error("Vector2 size more than allocated size\n");
    else if (reserved_size == 0) {
        T *temp_elem = new T[8];;//new T[8];
        try {
            temp_elem[0] = new_elem;
        }
        catch (...) {
            delete[] temp_elem;
            throw std::runtime_error(copy_bad);
        }

        //temp_elem[0] = new_elem;
        if (data != nullptr) {
            try {
                for (size_t i = 0; i < vec_size; ++i)
                    data[i].~T();
            }
            catch (...) {
                throw std::runtime_error(destructor_bad);
            }

            delete[] data;
        }
        data = temp_elem;
        reserved_size = 8;
        vec_size = 1;
    } else if (vec_size == reserved_size) {
        T *temp_elem = new T[reserved_size * 2];
        try {
            for (size_t i = 0; i < vec_size; ++i)
                temp_elem[i] = data[i];
            temp_elem[vec_size] = new_elem;
        }
        catch (...) {
            delete[] temp_elem;
            throw std::runtime_error(copy_bad);
        }
        try {
            for (size_t i = 0; i < vec_size; ++i)
                data[i].~T();
        }
        catch (...) {
            delete[] temp_elem;
            throw std::runtime_error(destructor_bad);
        }
        delete[] data;
        data = temp_elem;

        reserved_size = reserved_size * 2;
        vec_size++;
    } else {
        try {
            data[vec_size] = new_elem;
        }
        catch (...) {
            throw std::runtime_error(copy_bad);
        }
        vec_size++;
    }
}

template<typename T>
void Vector2<T>::insert(int ind, T new_elem) {
    if (ind < 0 || ind >= vec_size) {
        throw std::runtime_error(out_of_range);
    }
    if (ind == vec_size - 1) {
        push_back(new_elem);
        return;
    }

    push_back(0);

    try {
        for (size_t i = vec_size - 1; i > ind; i--) {
            data[i] = data[i - 1];
        }
        data[ind] = new_elem;
    }
    catch (...) {
        throw std::runtime_error(copy_bad);
    }
}

template<typename T>
void Vector2<T>::pop_back() {
    if (vec_size > reserved_size)
        throw std::runtime_error("Vector2 size more than allocated size\n");
    else if (reserved_size == 0)
        throw std::runtime_error(empty_vec);
    else if (reserved_size > 0) {
        try {
            back().~T();
        }
        catch (...) {
            throw std::runtime_error(destructor_bad);
        }

        vec_size--;
        if (vec_size == 0) {
            if (data != nullptr)
                delete[] data;
            data = nullptr;

            reserved_size = 0;
            vec_size = 0;
        } else if (2 * vec_size <= reserved_size) {
            T *temp_elem = new T[vec_size];
            bool flag = false;
            try {
                for (size_t i = 0; i < vec_size; ++i) {
                    try {
                        temp_elem[i] = data[i];
                    }
                    catch (...) {
                        flag = true;
                        throw std::runtime_error(
                                copy_bad);
                    }
                    data[i].~T();
                }
            }
            catch (...) {
                delete[] data;
                if (flag)
                    throw std::runtime_error(copy_bad);
                throw std::runtime_error(destructor_bad);
            }
            delete[] data;
            data = temp_elem;

            reserved_size = vec_size;
        }
    }
}

template<typename T>
void Vector2<T>::pop(int ind) {
    if (ind < 0 || ind >= vec_size)
        throw std::runtime_error(empty_vec);
    if (ind == vec_size - 1) {
        pop_back();
        return;
    }
    try {
        for (size_t i = ind; i < vec_size - 1; ++i)
            data[i] = data[i + 1];
    }
    catch (...) {
        throw std::runtime_error(copy_bad);
    }
    try {
        back().~T();
    }
    catch (...) {
        throw std::runtime_error(destructor_bad);
    }


    vec_size--;
    if (vec_size == 0) {
        if (data != nullptr)
            delete[] data;
        data = nullptr;

        reserved_size = 0;
        vec_size = 0;
    } else if (2 * vec_size <= reserved_size) {
        T *temp_elem = new T[vec_size];
        bool flag = false;
        try {
            for (size_t i = 0; i < vec_size; ++i) {
                try {
                    temp_elem[i] = data[i];
                }
                catch (...) {
                    flag = true;
                    throw std::runtime_error(copy_bad);
                }
                data[i].~T();
            }
        }
        catch (...) {
            delete[] data;
            if (flag)
                throw std::runtime_error(copy_bad);
            throw std::runtime_error(destructor_bad);
        }
        delete[] data;
        data = temp_elem;
        reserved_size = vec_size;
    }
}


template<typename T>
std::ostream &operator<<(std::ostream &os, const Vector2<T> &rhs) {
    if (rhs.empty())
        os << "[]";
    else {
        T *ptr = rhs.begin();
        os << '[' << *ptr;
        for (T *itr = rhs.begin() + 1; itr < rhs.end(); itr++)
            os << ", " << *itr;
        os << ']';
    }
    return os;
}


#endif