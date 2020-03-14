#ifndef UTI_EXPECTED_HPP
#define UTI_EXPECTED_HPP

#include <variant>
#include <system_error>

template<typename T>
class Expected
{
public:
    constexpr Expected() : value_(std::errc::argument_out_of_domain){}
    constexpr Expected(std::errc rct) : value_(rct){}
    constexpr Expected(const T& t) : value_(t){}
    constexpr const T& value() const { return std::get<T>(value_); }
   // T* operator *() { return &std::get<T>(_value); }
    constexpr std::errc error() const {
        if (std::holds_alternative<std::errc>(value_)){
            return std::get<std::errc>(value_);
        }
        else {
                return static_cast<std::errc>(0);
        }
    }
    constexpr operator bool() const {
        return std::holds_alternative<T>(value_);
    }
private:
    const std::variant<T, std::errc> value_;
};

#endif //UTI_EXPECTED_HPP

