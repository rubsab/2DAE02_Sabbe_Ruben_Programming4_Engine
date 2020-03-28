#pragma once
#include <vector>

namespace MyEngine
{
	class FixedSizeAllocator
	{
	public:
		FixedSizeAllocator(const FixedSizeAllocator& other) = delete;
		FixedSizeAllocator(FixedSizeAllocator&& other) noexcept = delete;
		FixedSizeAllocator& operator=(const FixedSizeAllocator& other) = delete;
		FixedSizeAllocator& operator=(FixedSizeAllocator&& other) noexcept = delete;

		explicit FixedSizeAllocator(int size, size_t blockSize);
		~FixedSizeAllocator();

		// Acquire a pointer to a buffer 
		void* Acquire();

		// Release a buffer
		void Release(void* release);
		
		// Clears Memory Pool
		void Reset();

		// Get the amount of free memory that is left on the stack.
		size_t GetFreeSpaceAmount() const;
		size_t GetUsedSpaceAmount() const;

		std::vector<char*> GetUsedSpots() const;

	private:
		std::vector<char*> m_pMemoryPool;
		std::vector<bool> m_IsUsed;
		const size_t m_BlockSize;
	};
}
