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

MemoryManager::MemoryManager(size_t memSize, size_t memPerFrame) : backingStore("backing_store.txt")
{
    this->memPerFrame = memPerFrame;
	this->maxOverallMem = memSize;

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
			frameTable[i] = std::make_tuple(false, nullptr, Time::time_point::min());
			freeFrames.push_back(i);
		}
	}

	else
	{
		totalFrames = memSize;
		this->memPerFrame = 1;
		for (size_t i = 0; i < totalFrames; i++)
		{
			frameTable[i] = std::make_tuple(false, nullptr, Time::time_point::min());
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
		if (sizeToFrame(size) <= freeFrames.size())
			canAllocate = true;
	}

	else
	{
		for (size_t mainPointer = 0; mainPointer < totalFrames; mainPointer++)
		{
			canAllocate = true;
			for (size_t tracker = mainPointer; tracker < mainPointer + sizeToFrame(size); tracker++)
			{
				if (std::get<bool>(frameTable[tracker]) == true) 
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

bool MemoryManager::allocate(std::shared_ptr<Process> process)
{
	if (process->getMemoryRequired() > this->maxOverallMem)
		return false;

	bool succeed= false;
	if (pagingAlgo)
		succeed = allocatePaging(process);
	else
		succeed = allocateFlatMem(process);

	if (succeed)
	{
		process->setInMemory(true);
	}
		
	return succeed;
}

bool MemoryManager::allocateFlatMem(std::shared_ptr<Process> process)
{
	size_t frameIndex;

	bool possible = false;

	auto loadedProcess = backingStore.loadProcess(process->getID());
	if (loadedProcess != nullptr)
	{
		//process = loadedProcess;
		numPagedIn++; 
	}

	size_t size = process->getMemoryRequired();

	do
	{
		possible = canAllocate(size, &frameIndex);

		if (!possible)
		{
			backingStoreOperation();
		}

	} while (!possible);

	Time::time_point time_now = Time::now();

	for (size_t i = frameIndex; i < frameIndex + sizeToFrame(size); i++)
	{
		frameTable[i] = std::make_tuple(true, process, time_now);
	}

	return possible;
}

bool MemoryManager::allocatePaging(std::shared_ptr<Process> process)
{
	bool possible = false;

	// load Process from backing store if its there, if not use the process made by Scheduler.
	if (backingStore.loadProcess(process->getID()) != nullptr)
	{
		//process = backingStore.loadProcess(process->getID());
		numPagedIn++;
	}
		

	size_t size = process->getMemoryRequired();

	do
	{
		possible = canAllocate(size, nullptr);

		if (!possible)
		{
			backingStoreOperation();
		}

	} while (!possible);

	Time::time_point time_now = Time::now();

	for (size_t i = 0; i < sizeToFrame(size); i++)
	{
		size_t frameIndex = freeFrames.front();
		process->addAllocatedFrame(frameIndex);
		frameTable[frameIndex] = std::tuple(true, process, time_now);
		freeFrames.pop_front();
	}

	return possible;
}


void MemoryManager::deallocate(std::shared_ptr<Process> process)
{
	if (pagingAlgo)
	{
		std::vector<size_t> frames = process->getAllocatedFrames();

		for (size_t frameIndex : frames)
		{
			std::get<bool>(frameTable[frameIndex]) = false;
			freeFrames.push_back(frameIndex);
		}

		process->clearAllocatedFrames();
	}

	else
	{
		size_t startIndex = process->getStartingMemIndex();

		for (size_t frameIndex = startIndex; frameIndex < startIndex + process->getMemoryRequired(); frameIndex++)
		{
			std::get<bool>(frameTable[frameIndex]) = false;
		}

		process->setStartingMemIndex(0);
	}

	process->setInMemory(false);
}

void MemoryManager::backingStoreOperation()
{
	std::shared_ptr<Process> process = findOldestProcessInMemory();
	if (process)
		transferToBackingStore(process);
}

std::shared_ptr<Process> MemoryManager::findOldestProcessInMemory()
{
	Time::time_point oldestTime = std::get<Time::time_point>(frameTable[0]);
	std::shared_ptr<Process> process = std::get<std::shared_ptr<Process>>(frameTable[0]);
	for (size_t i = 1; i < totalFrames; i++)
	{
		if (std::get<Time::time_point>(frameTable[i]) < oldestTime)
		{
			oldestTime = std::get<Time::time_point>(frameTable[i]);
			process = std::get<std::shared_ptr<Process>>(frameTable[i]);
		}
	}

	return process;
}

void MemoryManager::transferToBackingStore(std::shared_ptr<Process> process)
{
	deallocate(process);
	backingStore.storeProcess(process);
	numPagedOut++;
}

std::list<size_t> MemoryManager::getFreeFrames()
{
	return freeFrames;
}


const std::unordered_map<size_t, std::pair<bool, int>>& MemoryManager::getAllocationMap() const {
	return allocationMap;
}

size_t MemoryManager::getMemorySize() {
	return maxOverallMem;
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

const std::unordered_map<size_t, std::tuple<bool, std::shared_ptr<Process>, Time::time_point>>& MemoryManager::getFrameTable() const {
	return frameTable;
}