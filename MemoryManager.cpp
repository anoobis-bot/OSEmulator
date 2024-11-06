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

	memory = std::vector<char>(memSize, '.');
    numFrames = sizeToFrame(memSize);
    for (size_t i = 0; i < numFrames; i++)
    {
        allocationMap[i] = std::make_pair(false, 0);
    }
}

size_t MemoryManager::sizeToFrame(size_t size)
{
	return (size + this->memPerFrame - 1) / this->memPerFrame;
}


bool MemoryManager::canAllocate(size_t size, size_t *frameIndex)
{
    bool allocated = false;

	for (size_t mainPointer = 0; mainPointer < numFrames; mainPointer++)
	{
		allocated = true;
		for(size_t tracker = mainPointer; tracker < mainPointer + sizeToFrame(size); tracker++)
		{
			if (allocationMap[tracker].first == true)
			{
				mainPointer = tracker + 1;
				allocated = false;
				break;
			}
		}
		if (allocated == true)
		{
			*frameIndex = mainPointer;
			break;
		}
	}

	return allocated;
}

bool MemoryManager::allocate(int pid, size_t size)
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

void MemoryManager::deallocate(int pid, size_t size)
{
	for (int i  = 0; i < numFrames; i++)
	{
		if (allocationMap[i].second == pid)
		{
			allocationMap[i].first = false;
		}
	}
}




