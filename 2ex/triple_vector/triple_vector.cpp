#include "triple_vector.hpp"

namespace Triple_signal
{
    Triple_vector::Triple_vector(int cnt_unknown)
    {
        if (cnt_unknown <= 0)
        {
            throw std::invalid_argument("count less zero");
        }

        size = cnt_unknown;
        array = new Triple_signal[size]{};
    }

    Triple_vector::Triple_vector(std::string signals)
    {
        size = signals.size();
        array = new Triple_signal[size]{};

        size_t index = 0;
        std::for_each_n(this->array, this->size, [&](Triple_signal &signal)
                        { signal.set_signal(signals[index++]); });
    }

    Triple_vector::Triple_vector(const Triple_vector &vector) noexcept
    {
        size = vector.size;
        array = new Triple_signal[size]{};

        std::copy_n(vector.array, vector.size, array);
    }

    Triple_vector::Triple_vector(Triple_vector &&vector) noexcept : array(vector.array), size(vector.size)
    {
        vector.array = nullptr;
        vector.size = 0;
    }

    void Triple_vector::set_signals(std::string signals)
    {
        delete[] array;

        size = signals.size();
        array = new Triple_signal[size]{};

        size_t index = 0;
        std::for_each_n(this->array, this->size, [&](Triple_signal &signal)
                        { signal.set_signal(signals[index++]); });
    }

    std::string Triple_vector::get_signals() const
    {
        std::string result;
        std::for_each_n(this->array, this->size, [&result](Triple_signal &signal)
                        { result += signal.get_signal_char(); });
        return result;
    }

    Triple_signal *Triple_vector::get_signals(int &size) const
    {
        size = this->size;
        return this->array;
    }

    bool Triple_vector::is_known() const
    {
        bool flag = true;
        std::for_each_n(array, size, [&flag](const Triple_signal &signal)
                        {
            if(signal.get_signal() == UNKNOWN){
                flag = false;
                return;
            } });

        return flag;
    }

    void Triple_vector::input(std::istream &in)
    {
        std::string input;
        in >> input;
        this->set_signals(input);
    }

    void Triple_vector::print(std::ostream &out)
    {
        out << (this->get_signals());
    }

    Triple_vector &Triple_vector::operator=(const Triple_vector &vector)
    {
        if (this != &vector)
        {
            delete[] array;

            array = nullptr;
            if ((size = vector.size) != 0)
            {
                array = new Triple_signal[size]{};
                std::copy_n(vector.array, vector.size, array);
            }
        }
        return *this;
    }

    Triple_vector &Triple_vector::operator=(Triple_vector &&vector) noexcept
    {
        std::swap(this->array, vector.array);
        std::swap(this->size, vector.size);
        return *this;
    }

    bool Triple_vector::operator==(const Triple_vector &vector_2) const
    {
        return this->get_signals() == vector_2.get_signals();
    }

    bool Triple_vector::operator!=(const Triple_vector &vector_2) const
    {
        return this->get_signals() != vector_2.get_signals();
    }

    Triple_vector Triple_vector::operator|(const Triple_vector &vector_2) const
    {
        int size = std::min(this->size, vector_2.size);

        std::string new_signals;
        Triple_signal *signals_1 = this->array;
        Triple_signal *signals_2 = vector_2.array;

        int i = 0;
        std::for_each_n(signals_1, size, [&](const Triple_signal &signal)
                        { new_signals += (signal || (signals_2[i++])).get_signal_char(); });
        return Triple_vector(new_signals);
    }

    Triple_vector Triple_vector::operator&(const Triple_vector &vector_2) const
    {
        int size = std::min(this->size, vector_2.size);

        std::string new_signals;
        Triple_signal *signals_1 = this->array;
        Triple_signal *signals_2 = vector_2.array;

        int i = 0;
        std::for_each_n(signals_1, size, [&](const Triple_signal &signal)
                        { new_signals += (signal && (signals_2[i++])).get_signal_char(); });
        return Triple_vector(new_signals);
    }

    Triple_vector Triple_vector::operator~() const
    {
        int size = this->size;

        std::string new_signals;
        Triple_signal *signals_1 = this->array;

        std::for_each_n(signals_1, size, [&new_signals](const Triple_signal &signal)
                        { new_signals += (!signal).get_signal_char(); });
        return Triple_vector(new_signals);
    }

    Triple_vector Triple_vector::operator+(const Triple_vector &vector_2) const
    {
        return Triple_vector(this->get_signals() + vector_2.get_signals());
    }

    Triple_vector Triple_vector::operator+(const Triple_signal &signal) const
    {
        return Triple_vector(this->get_signals() + signal.get_signal_char());
    }

    std::istream &operator>>(std::istream &in, Triple_vector &vector)
    {
        std::string input;
        in >> input;
        vector.set_signals(input);

        return in;
    }

    std::ostream &operator<<(std::ostream &out, const Triple_vector &vector)
    {
        out << vector.get_signals();

        return out;
    }

    Triple_signal &Triple_vector::operator[](size_t index)
    {
        if (index >= size)
        {
            throw std::out_of_range("Index " + std::to_string(index) +
                                    " out of range for vector of size " +
                                    std::to_string(size));
        }
        return array[index];
    }

    Triple_vector Triple_vector::operator[](const std::pair<size_t, size_t> &indices) const
    {
        size_t start = indices.first;
        size_t end = indices.second;

        if (start >= size || end >= size)
        {
            throw std::out_of_range("Indices out of range: [" +
                                    std::to_string(start) + ", " +
                                    std::to_string(end) + "] for size " +
                                    std::to_string(size));
        }

        if (start > end)
        {
            throw std::invalid_argument("Start index " + std::to_string(start) +
                                        " cannot be greater than end index " +
                                        std::to_string(end));
        }

        std::string substr = this->get_signals().substr(start, start + end - 1);

        return Triple_vector(substr);
    }

    bool Triple_vector::operator!() const
    {
        bool flag = true;
        std::for_each_n(array, size, [&flag](const Triple_signal &signal)
                        {
            if(signal.get_signal() == UNKNOWN){
                flag = false;
                return;
            } });

        return flag;
    }
}
