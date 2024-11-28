#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Process.h"

class MemoryManager
{
public:
	static void initialize(size_t memSize, size_t memPerFrame, size_t memPerProc);
	static MemoryManager* getInstance();

	bool allocate(int pid, size_t size);
	void deallocate(int pid, size_t size);
	bool canAllocate(size_t size, size_t* frameIndex);

	size_t getnNumFrames();
	size_t getMemorySize();
	size_t getMemPerFrame();
	size_t getMemPerProc();
	const std::unordered_map<size_t, std::pair<bool, int>>& getAllocationMap() const;

private: 
	// Singleton Members
	MemoryManager(size_t memSize, size_t memPerFrame, size_t memPerProc);
	static MemoryManager* sharedInstance;

	// Memory
	std::list<std::shared_ptr<Process>> memory;
	std::list<std::shared_ptr<Process>> backingStore;
	std::unordered_map<size_t, std::pair<bool, int>> allocationMap;
	std::unordered_map<size_t, std::tuple<bool, int, Time::time_point>> pageTable;
	std::list<size_t> freeFrames;
	size_t maxOverallMem = 0;
	size_t memPerFrame = 0;
	size_t memPerProc = 0;
	size_t totalFrames = 0;

	bool pagingAlgo;

	bool allocatePaging(int pid, size_t size);
	bool allocateFlatMem(int pid, size_t size);

	int backingStoreOperation();
	int findOldestProcessInMemory();

	size_t sizeToFrame(size_t size);
};

