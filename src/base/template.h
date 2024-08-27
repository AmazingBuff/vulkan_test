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
inline T& Singleton<T>::get_instance()
{
	static T instance;
	return instance;
}
