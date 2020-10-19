#include<iostream>
using namespace std;

class Animal 
{
public:
    Animal() 
    {
        std::cout << "Animal() is called" << std::endl;
    }

    Animal(const Animal &) 
    {
        std::cout << "Animal (const Animal &) is called" << std::endl;
    }

    Animal &operator=(const Animal &) 
    {
        std::cout << "Animal & operator=(const Animal &) is called" << std::endl;
        return *this;
    }

    ~Animal() 
    {
        std::cout << "~Animal() is called" << std::endl;
    }
};

class Dog :{
public:
    Dog(const Animal &animal) : __animal(animal) 
    {
        std::cout << "Dog(const Animal &animal) is called" << std::endl;
    }
    // // Dog(const Animal &animal) 
    // // {
    // //     __animal = animal;
    // //     std::cout << "Dog(const Animal &animal) is called" << std::endl;
    // // }
    ~Dog() 
    {
        std::cout << "~Dog() is called" << std::endl;
    }

private:
    Animal __animal;
};


//case 1：类的列表初始化和赋值初始化的区别

//case 2: 引用类成员数据必须类列表初始化

int main() {
    Animal animal;
    std::cout << std::endl;
    Dog d(animal);
    std::cout << std::endl;

    //case 3：多了一个public Animal 多个一个调用默认构造函数的机会
    // Dog g; 
    //return 0;
}
