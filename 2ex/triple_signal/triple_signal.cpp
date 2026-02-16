#include <iostream>

#include "triple_signal.hpp"

namespace Triple_signal
{

    Triple_signal::Triple_signal(Signal value) : value(value) {}

    Triple_signal::Triple_signal(char value)
    {
        switch (value)
        {
        case '0':
            this->value = ZERO;
            break;
        case '1':
            this->value = ONE;
            break;
        case 'X':
        case 'x':
            this->value = UNKNOWN;
            break;
        default:
            throw std::invalid_argument("invalid signal");
            break;
        }
    }

    Triple_signal::Triple_signal(const Triple_signal &signal) : value(signal.value) {}

    Signal Triple_signal::get_signal() const
    {
        return value;
    }

    char Triple_signal::get_signal_char() const
    {
        switch (value)
        {
        case ZERO:
            return '0';
            break;
        case ONE:
            return '1';
            break;
        default:
            return 'X';
            break;
        }
    }

    void Triple_signal::set_signal(Signal new_value)
    {
        value = new_value;
    }

    void Triple_signal::set_signal(char new_value)
    {
        switch (new_value)
        {
        case '0':
            this->value = ZERO;
            break;
        case '1':
            this->value = ONE;
            break;
        case 'X':
        case 'x':
            this->value = UNKNOWN;
            break;
        default:
            throw std::invalid_argument("invalid signal");
            break;
        }
    }

    void Triple_signal::print(std::ostream &out)
    {
        out << this->get_signal_char();
    }

    void Triple_signal::input(std::istream &in)
    {
        std::string input;
        in >> input;
        if (input == "0")
            this->set_signal(ZERO);
        else if (input == "1")
        {
            this->set_signal(ONE);
        }
        else if (input == "UNKNOWN" || input == "x" || input == "X")
        {
            this->set_signal(UNKNOWN);
        }
        else
        {
            throw std::invalid_argument("invalid signal");
        }
    }

    Triple_signal Triple_signal::operator||(const Triple_signal &signal2) const
    {
        if (this->value == ONE || signal2.get_signal() == ONE)
            return Triple_signal(ONE);
        if (this->value == UNKNOWN || signal2.get_signal() == UNKNOWN)
            return Triple_signal(UNKNOWN);
        return Triple_signal(ZERO);
    }

    Triple_signal Triple_signal::operator&&(const Triple_signal &signal2) const
    {
        if (this->value == ONE && signal2.get_signal() == ONE)
            return Triple_signal(ONE);
        if (this->value == ZERO || signal2.get_signal() == ZERO)
            return Triple_signal(ZERO);
        return Triple_signal(UNKNOWN);
    }

    bool Triple_signal::operator==(const Triple_signal &signal2) const
    {
        return this->value == signal2.get_signal();
    }

    Triple_signal Triple_signal::operator!() const
    {
        switch (value)
        {
        case ZERO:
            return Triple_signal(ONE);
        case ONE:
            return Triple_signal(ZERO);
        default:
            return Triple_signal(UNKNOWN);
        }
    }

    std::ostream &operator<<(std::ostream &out, const Triple_signal &signal)
    {
        out << signal.get_signal_char();
        return out;
    }

    std::istream &operator>>(std::istream &in, Triple_signal &signal)
    {
        std::string input;
        in >> input;
        if (input == "0")
            signal.set_signal(ZERO);
        else if (input == "1")
        {
            signal.set_signal(ONE);
        }
        else if (input == "UNKNOWN" || input == "x" || input == "X")
        {
            signal.set_signal(UNKNOWN);
        }
        else
        {
            throw std::invalid_argument("invalid signal");
        }

        return in;
    }

}
