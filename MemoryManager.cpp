#include "MemoryManager.h"

MemoryManager* MemoryManager::sharedInstance = nullptr;

void MemoryManager::initialize(size_t memSize, size_t memPerFrame)
{
    if (!sharedInstance)
    {
        sharedInstance = new MemoryManager(memSize, memPerFrame);
    }
}

MemoryManager* MemoryManager::getInstance()
{
    return sharedInstance;
}

MemoryManager::MemoryManager(size_t memSize, size_t memPerFrame)
{
    this->memPerFrame = memPerFrame;

	// Paging allocator
	if (memSize != memPerFrame)
		this->pagingAlgo = true;
	else
		this->pagingAlgo = false;

	if (pagingAlgo)
	{
		this->totalFrames = sizeToFrame(memSize);
		for (size_t i = 0; i < totalFrames; i++)
		{
			pageTable[i] = std::make_tuple(false, 0, Time::time_point::min());
			freeFrames.push_back(i);
		}
	}

	else
	{
		totalFrames = sizeToFrame(memSize);
		for (size_t i = 0; i < totalFrames; i++)
		{
			allocationMap[i] = std::make_pair(false, 0);
		}
	}
}

size_t MemoryManager::sizeToFrame(size_t size)
{
	return (size + this->memPerFrame - 1) / this->memPerFrame;
}


bool MemoryManager::canAllocate(size_t size, size_t *frameIndex)
{
    bool canAllocate = false;

	if (pagingAlgo)
	{
		if (sizeToFrame(size) >= freeFrames.size())
			canAllocate = true;
	}

	else
	{
		for (size_t mainPointer = 0; mainPointer < totalFrames; mainPointer++)
		{
			canAllocate = true;
			for (size_t tracker = mainPointer; tracker < mainPointer + sizeToFrame(size); tracker++)
			{
				if (allocationMap[tracker].first == true)
				{
					mainPointer = tracker + 1;
					canAllocate = false;
					break;
				}
			}
			if (canAllocate == true)
			{
				*frameIndex = mainPointer;
				break;
			}
		}
	}

	return canAllocate;
}

bool MemoryManager::allocate(int pid, size_t size)
{
	if (pagingAlgo)
		return false;
	else
		return allocateFlatMem(pid, size);
}

bool MemoryManager::allocateFlatMem(int pid, size_t size)
{
	size_t frameIndex;

	if (canAllocate(size, &frameIndex))
	{
		for (size_t i = frameIndex; i < frameIndex + sizeToFrame(size); i++)
		{
			allocationMap[i] = std::make_pair(true, pid);
		}

		return true;
	}

	return false;
}

bool MemoryManager::allocatePaging(int pid, size_t size)
{
	bool allocated = false;

	do
	{
		allocated = canAllocate(size, nullptr);

		if (!allocated)
		{
			
		}

	} while (!allocated);
}


void MemoryManager::deallocate(int pid, size_t size)
{
	for (int i  = 0; i < totalFrames; i++)
	{
		if (allocationMap[i].second == pid)
		{
			allocationMap[i].first = false;
		}
	}
}

int MemoryManager::backingStoreOperation()
{
	
}

int MemoryManager::findOldestProcessInMemory()
{
	Time::time_point oldestTime = std::get<Time::time_point>(pageTable[0]);
	int pid = std::get<int>(pageTable[0]);
	for (size_t i = 1; i < totalFrames; i++)
	{
		if (std::get<Time::time_point>(pageTable[i]) < oldestTime)
		{
			oldestTime = std::get<Time::time_point>(pageTable[i]);
			pid = std::get<int>(pageTable[i]);
		}
	}

	return pid;
}



const std::unordered_map<size_t, std::pair<bool, int>>& MemoryManager::getAllocationMap() const {
	return allocationMap;
}

size_t MemoryManager::getMemorySize() {
	return memory.size();
}

size_t MemoryManager::getMemPerFrame() {
	return memPerFrame;
}

size_t MemoryManager::getMemPerProc() {
	return memPerProc;
}

size_t MemoryManager::getnNumFrames() {
	return totalFrames;
}

BackingStore* MemoryManager::getBackingStore() {
	return &backingStore;
}