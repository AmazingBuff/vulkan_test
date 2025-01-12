#pragma once

#include "macro.h"

AMAZING_NAMESPACE_BEGIN

template<typename T>
class Singleton
{
public:
	static T& get_instance();
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
	explicit Enum(const type& value);
	explicit Enum(const T& value);
	Enum(const Enum& other);
	Enum(Enum&& other) noexcept;

	// compare
	NODISCARD constexpr bool operator==(const Enum& value) const;
	NODISCARD constexpr bool operator!=(const Enum& value) const;
	NODISCARD constexpr bool operator>(const Enum& value) const;
	NODISCARD constexpr bool operator>=(const Enum& value) const;
	NODISCARD constexpr bool operator<(const Enum& value) const;
	NODISCARD constexpr bool operator<=(const Enum& value) const;

    NODISCARD Enum& operator=(const T& value);
    NODISCARD Enum& operator=(const Enum& value);

	NODISCARD constexpr bool operator==(const T& value) const;
	NODISCARD constexpr bool operator!=(const T& value) const;

	// transform
	NODISCARD constexpr explicit operator T() const;
	NODISCARD constexpr explicit operator type() const;
protected:
	type m_value;
};

template<typename T>
    requires std::is_enum_v<T>
class BitFlag : public Enum<T>
{
	using type = std::underlying_type_t<T>;
public:
    BitFlag() = default;
	explicit BitFlag(const type& value);
    explicit BitFlag(const T& value);
	BitFlag(const BitFlag& other);
    BitFlag(BitFlag&& other) noexcept;

    // bit op
    NODISCARD constexpr BitFlag& operator|=(const BitFlag& value);
    NODISCARD constexpr BitFlag& operator&=(const BitFlag& value);
    NODISCARD constexpr BitFlag& operator^=(const BitFlag& value);
    NODISCARD constexpr BitFlag operator|(const BitFlag& value) const;
    NODISCARD constexpr BitFlag operator&(const BitFlag& value) const;
    NODISCARD constexpr BitFlag operator^(const BitFlag& value) const;
    NODISCARD constexpr BitFlag operator~() const;

	BitFlag& operator=(const BitFlag& value);
};

#include "template.inl"

AMAZING_NAMESPACE_END