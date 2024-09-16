#pragma once

template<typename T>
class Singleton
{
public:
	static T& get_instance();
private:
	Singleton() = delete;
	~Singleton() = delete;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
};


template<typename T>
	requires std::is_enum_v<T>
class Enum
{
	using type = std::underlying_type_t<T>;
public:
	Enum();
	Enum(const type& value);
	Enum(const T& value);
	Enum(const Enum& other);
	Enum(Enum&& other) noexcept;

	// bit op
	NODISCARD constexpr Enum& operator|=(const Enum& value);
	NODISCARD constexpr Enum& operator&=(const Enum& value);
	NODISCARD constexpr Enum& operator^=(const Enum& value);
	NODISCARD constexpr Enum operator|(const Enum& value) const;
	NODISCARD constexpr Enum operator&(const Enum& value) const;
	NODISCARD constexpr Enum operator^(const Enum& value) const;
	NODISCARD constexpr Enum operator~() const;

	// compare
	NODISCARD constexpr bool operator==(const Enum& value) const;
	NODISCARD constexpr bool operator!=(const Enum& value) const;
	NODISCARD constexpr bool operator>(const Enum& value) const;
	NODISCARD constexpr bool operator>=(const Enum& value) const;
	NODISCARD constexpr bool operator<(const Enum& value) const;
	NODISCARD constexpr bool operator<=(const Enum& value) const;

	// transform
	NODISCARD constexpr operator T() const;
	NODISCARD constexpr operator type() const;
private:
	type m_value;
};



#include "template.inl"