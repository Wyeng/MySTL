#include<iostream>
#include<cstddef>
#include<cstring>
#include<memory>
#include<stdexcept>

class MyVector
{
private:
	int* data;	//指向动态数组
	std::size_t size;	//当前元素个数
	std::size_t capacity;	//当前可容纳的元素个数

public:
	// 构造函数
	MyVector():data(nullptr),size(0),capacity(0)
	{

	}

	// 拷贝构造函数
	MyVector(const MyVector& other) :data(nullptr),size(other.size),capacity(other.capacity)
	{
		// 如果容量大于0，申请新数组
		if (other.capacity > 0)
		{
			this->data = new int[other.capacity];
			// 将other中的元素复制到数组中
			for (std::size_t i = 0;i < other.size;i++)
			{
				this->data[i] = other.data[i];
			}
		}
	}

	// 实现拷贝赋值运算符
	MyVector& operator=(const MyVector& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (other.capacity == 0)
		{
			delete[] this->data;
			this->data = nullptr;
			this->capacity = 0;
			this->size = 0;
			return *this;
		}

		// 创建一个临时变量来接收数据
		int* tmp = new int[other.capacity];
		for (std::size_t i = 0;i < other.size;i++)
		{
			tmp[i] = other.data[i];
		}

		// 防止之前申请了空间，造成内存泄露
		delete[] this->data;


		this->size = other.size;
		this->capacity = other.capacity;
		this->data = tmp;

		return *this;
	}

	// 析构函数,释放申请的空间，防止内存泄露
	~MyVector()
	{
		delete[] data;
		std::cout << "MyVector destroyed" << std::endl;
	}

	// 返回当前元素数量
	std::size_t getSize() const
	{
		return this->size;
	}

	// 返回当前容量
	std::size_t getCapacity() const
	{
		return this->capacity;
	}

	// 实现扩容
	void resizeCapacity()
	{
		std::size_t newCapacity = this->capacity * 2;

		int* newData = new int[newCapacity];

		// 先将旧数据复制到新分配的空间中
		for (std::size_t i = 0;i < this->size;i++)
		{
			newData[i] = this->data[i];
		}

		// 释放旧空间
		delete[] data;

		// 更新data指向的地址和capacity
		this->data = newData;
		this->capacity = newCapacity;
	}

	// 实现push_back
	void push_back(int value)
	{
		// 初始时capacity为0，无法正常存入数据
		if (this->capacity == 0)
		{
			// 申请一块能存储4个int的内存
			this->data = new int[4];
			// 更新capacity
			this->capacity += 4;
		}

		// 如果内存已满则需要重新处理
		if (this->size == this->capacity)
		{
			resizeCapacity();
		}

		// 将value存入数组末尾
		this->data[size] = value;
		// 更新元素数量
		++this->size;
	}

	// 打印容器中的元素
	void Print() const
	{
		for (std::size_t i = 0;i < this->size;i++)
		{
			std::cout << this->data[i] << std::endl;
		}
	}

	// 重载[]操作符
	int& operator[](std::size_t index)
	{
		if (index >= size)
		{
			throw std::out_of_range("MyVector index out of range");
		}

		return data[index];
	}

	// 调试函数，观察data的地址
	const int* getData() const
	{
		return data;
	}
};

class MyString
{
private:
	char* data;
	std::size_t size;

public:
	// 构造函数
	MyString() :data(new char[1]),size(0)
	{
		data[0] = '\0';
	}

	// 实现带参的构造函数
	MyString(const char* str)
	{
		// 如果传入的是空指针则抛出异常
		if (str == nullptr)
		{
			throw std::invalid_argument("Null pointer passed to MyString");
		}

		std::size_t i = std::strlen(str);
		this->size = i;

		// 将源字符串复制到新数组中
		data = new char[this->size + 1];
		std::memcpy(data, str, i);
		data[this->size] = '\0';
	}

	// 实现拷贝构造函数
	MyString(const MyString& other)
	{
		this->size = other.size;

		this->data = new char[this->size + 1];
		std::memcpy(this->data, other.data, this->size+1);
	}

	// 实现拷贝赋值运算符
	MyString& operator=(const MyString& other)
	{
		if (this == &other)
		{
			return *this;
		}

		// 创建一个临时值来接收数据,使用智能指针防止内存泄露
		std::unique_ptr<char[]> temp(
			new char[other.size + 1]
		);
		std::memcpy(temp.get(), other.data, other.size + 1);

		char* newData = temp.release();

		// 释放原始的空间
		delete[] this->data;
		// 转移所有权
		this->data = newData;
		// 更新size
		this->size = other.size;

		return *this;
	}

	// 析构函数，释放申请的内存，防止内存泄露
	~MyString()
	{
		delete[] data;
	}

	// 输出字符串长度
	std::size_t length() const
	{
		return this->size;
	}

	// 输出字符串,不包含结尾的'\0'
	void Print() const
	{
		for (std::size_t i = 0;i < this->size;i++)
		{
			std::cout << data[i];
		}
		std::cout << std::endl;
	}

	// 判断两个字符串的内容是否相同
	bool operator==(const MyString& other) const
	{
		if (this->size != other.size)
			return false;
		for (std::size_t i = 0;i < this->size;i++)
		{
			if (this->data[i] != other.data[i])
				return false;
		}
		return true;
	}

	// 实现字符串拼接
	MyString operator+(const MyString& other) const
	{
		std::size_t length = this->size + other.size;

		char* newdata = new char[length+1];

		for (std::size_t i = 0;i < this->size;i++)
		{
			newdata[i] = this->data[i];
		}

		for (std::size_t i = 0;i <= other.size;i++)
		{
			newdata[this->size + i] = other.data[i];
		}

		MyString result(newdata);

		// 释放空间，防止内存泄露
		delete[] newdata;
		return result;
	}

	char& operator[](std::size_t index)
	{
		// 判断是否访问越界
		if (index >= size)
		{
			throw std::out_of_range("MyString index out of range");
		}
		return data[index];
	}

	// 调试函数，查看申请的内存地址
	const void* getData() const
	{
		return this->data;
	}
};

int main()
{
	MyVector vec;
	vec.push_back(10);
	vec.push_back(20);

	std::cout << vec[0] << '\n';  // 10

	vec[1] = 30;
	std::cout << vec[1] << '\n';  // 30

	MyString str("hello");
	try
	{
		std::cout << str[5] << std::endl;
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}