#ifndef MY_ARRAY_POOL_H
#define MY_ARRAY_POOL_H

#include "adt_errors.h"

#include <list>
using namespace std;

#pragma pack(1)

/**
 *@brief 数组实现T类型定长的内存池
 **/
template<typename T>
class Array_Pool
{
public:
	/**
	 *@brief 构造函数
	 **/
	Array_Pool() : p_head ( NULL ) {}
	
	/**
	 *@brief 构造函数, 指定单元的数目
	 **/
	Array_Pool ( unsigned int count ) : p_head ( NULL )
	{
		init ( count );
	}
	
	/**
	 *@brief 析构函数
	 **/
	
	~Array_Pool()
	{
		//if(p_head != NULL) delete p_head;
	}
	
	
public:

	/**
	 *@brief 初始化函数, 指定每个单元的大小和单元的数目
	 **/
	bool init ( unsigned int count )
	{
	
		p_head = new T[count];
		
		if ( p_head == NULL ) return false;
		
		T *p = p_head;
		
		for ( unsigned int i = 0; i < count; ++i )
		{
			pool.push_back ( p );
			++p;
		}
		
		return true;
	}
	
	/**
	 *@brief 重新初始化函数, 指定每个单元的大小和单元的数目
	 **/
	bool reinit ( unsigned int count )
	{
		if ( p_head != NULL ) delete [] p_head;
		
		p_head = new T[count];
		
		if ( p_head == NULL ) return false;
		
		pool.clear();
		
		T *p = p_head;
		
		for ( unsigned int i = 0; i < count; ++i )
		{
			pool.push_back ( p );
			++p;
		}
		
		return true;
	}
	
	/**
	 *@brief 分配一个单元
	 **/
	T* malloc()
	{
		if ( pool.empty() ) return NULL;
		
		T *p = pool.front();
		pool.pop_front();
		
		//int reserve = pool.size();
		//cout << "pool's reserve size = " << reserve << endl;
		
		return p;
	}
	/**
	 *@brief 释放一个单元
	 **/
	void free ( T* p )
	{
		pool.push_back ( p );
		//int reserve = pool.size();
		//cout << "----------------------- pool's reserve size = " << reserve << endl;
	}
	
	int reserve()
	{
		return pool.size();
	}
private:
	list<T*> pool;///<内存池，未分配的T*的指针的链表
	T *p_head;///<向系统请求的数组内存的头
};

#pragma pack()

#endif
