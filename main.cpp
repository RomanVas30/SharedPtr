#include"SharedPtr.hpp"

int main() {
	SharedPtr<int> ptr(5);
	std::cout << *ptr << std::endl;
	int* people = new int(30);
	ptr.reset(people);
	std::cout << ptr.use_count() << std::endl;
	std::cout << *ptr << std::endl;
	SharedPtr<int> ptr1(1);
	SharedPtr<int> ptr2 = ptr1;
	SharedPtr<int> ptr3(ptr2);
	return 0;
}