template<typename T>
inline T& Singleton<T>::get_instance()
{
	static T instance;
	return instance;
}


template<typename T>
	requires std::is_enum_v<T>
inline Enum<T>::Enum() : m_value(type()) {}

template<typename T>
	requires std::is_enum_v<T>
inline Enum<T>::Enum(const type& value) : m_value(value) {}

template<typename T>
	requires std::is_enum_v<T>
inline Enum<T>::Enum(const T& value) : m_value(static_cast<type>(value)) {}

template<typename T>
	requires std::is_enum_v<T>
inline Enum<T>::Enum(const Enum& other) : m_value(other.m_value) {}

template<typename T>
	requires std::is_enum_v<T>
inline Enum<T>::Enum(Enum&& other) noexcept : m_value(std::move(other.m_value)) {}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T>& Enum<T>::operator|=(const Enum& value)
{
	m_value |= value.m_value;
	return *this;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T>& Enum<T>::operator&=(const Enum& value)
{
	m_value &= value.m_value;
	return *this;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T>& Enum<T>::operator^=(const Enum& value)
{
	m_value ^= value.m_value;
	return *this;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T> Enum<T>::operator|(const Enum& value) const
{
	return Enum(m_value | value.m_value);
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T> Enum<T>::operator&(const Enum& value) const
{
	return Enum(m_value & value.m_value);
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T> Enum<T>::operator^(const Enum& value) const
{
	return Enum(m_value ^ value.m_value);
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T> Enum<T>::operator~() const
{
	return Enum(~m_value);
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr bool Enum<T>::operator==(const Enum& value) const
{
	return m_value == value.m_value;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr bool Enum<T>::operator!=(const Enum& value) const
{
	return m_value != value.m_value;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr bool Enum<T>::operator>(const Enum& value) const
{
	return m_value > value.m_value;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr bool Enum<T>::operator>=(const Enum& value) const
{
	return m_value >= value.m_value;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr bool Enum<T>::operator<(const Enum& value) const
{
	return m_value < value.m_value;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr bool Enum<T>::operator<=(const Enum& value) const
{
	return m_value <= value.m_value;
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T>::operator T() const
{
	return static_cast<T>(m_value);
}

template<typename T>
	requires std::is_enum_v<T>
inline constexpr Enum<T>::operator type() const
{
	return m_value;
}