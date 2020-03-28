#include "MiniginPCH.h"
#include "FixedSizeAllocator.h"
#include <malloc.h>
#include <exception>

MyEngine::FixedSizeAllocator::FixedSizeAllocator(const int size, size_t blockSize)
	:m_BlockSize{blockSize}
{
	// Initialize
	m_pMemoryPool.resize(size);
	m_IsUsed.resize(size);
	for (size_t i{}; i < m_pMemoryPool.size(); i++)
	{
		m_pMemoryPool[i] = reinterpret_cast<char*>(calloc(blockSize, blockSize));
		m_IsUsed[i] = false;
	}
}

MyEngine::FixedSizeAllocator::~FixedSizeAllocator()
{
	// Destroy
	for (size_t i{}; i < m_pMemoryPool.size(); i++)
		free(m_pMemoryPool[i]);
}

void* MyEngine::FixedSizeAllocator::Acquire()
{
	// return a pointer to a buffer of requested size
	for (size_t i{}; i < m_pMemoryPool.size(); i++)
	{
		if (!m_IsUsed[i])
		{
			m_IsUsed[i] = true;
			return m_pMemoryPool[i];
		}
	}
	throw std::exception("out of memory");
}

void MyEngine::FixedSizeAllocator::Release(void* release)
{
	// we do not release in a stacked allocator. why?

	for (size_t i{}; i < m_pMemoryPool.size(); i++)
	{
		if (m_pMemoryPool[i] == release)
		{
			m_IsUsed[i] = false;
		}
	}
}

void MyEngine::FixedSizeAllocator::Reset()
{
	std::fill(m_IsUsed.begin(), m_IsUsed.end(), false);
}

size_t MyEngine::FixedSizeAllocator::GetFreeSpaceAmount() const
{
	// return the amount of memory that is still free for use.
	size_t counter{};
	for (size_t i{}; i < m_IsUsed.size(); i++)
	{
		if (!m_IsUsed[i])
			counter++;
	}
	return counter;
}

size_t MyEngine::FixedSizeAllocator::GetUsedSpaceAmount() const
{
	return m_IsUsed.size() - GetFreeSpaceAmount();
}

std::vector<char*> MyEngine::FixedSizeAllocator::GetUsedSpots() const
{
	std::vector<char*> returnVector;
	for (size_t i{}; i < m_IsUsed.size(); i++)
	{
		if (m_IsUsed[i])
			returnVector.push_back(m_pMemoryPool[i]);
	}
	return returnVector;
}