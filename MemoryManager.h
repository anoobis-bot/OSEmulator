#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Process.h"
#include "BackingStore.h"

class MemoryManager
{
public:
	static void initialize(size_t memSize, size_t memPerFrame);
	static MemoryManager* getInstance();

	BackingStore* getBackingStore();

	bool allocate(std::shared_ptr<Process> process);
	void deallocate(std::shared_ptr<Process> process);
	bool canAllocate(size_t size, size_t* frameIndex);

	size_t getnNumFrames();
	size_t getMemorySize();
	size_t getMemPerFrame();
	size_t getMemPerProc();
	const std::unordered_map<size_t, std::pair<bool, int>>& getAllocationMap() const;

private: 
	// Singleton Members
	MemoryManager(size_t memSize, size_t memPerFrame);
	static MemoryManager* sharedInstance;

	BackingStore backingStore;

	// Memory
	std::list<std::shared_ptr<Process>> memory;
	std::list<std::shared_ptr<Process>> bStore;
	std::unordered_map<size_t, std::pair<bool, int>> allocationMap;
	std::unordered_map<size_t, std::tuple<bool, std::shared_ptr<Process>, Time::time_point>> frameTable;
	std::list<size_t> freeFrames;	
	size_t maxOverallMem = 0;
	size_t memPerFrame = 0;
	size_t memPerProc = 0;
	size_t totalFrames = 0;

	bool pagingAlgo;

	bool allocatePaging(std::shared_ptr<Process> process);
	bool allocateFlatMem(std::shared_ptr<Process> process);

	void backingStoreOperation();
	std::shared_ptr<Process> findOldestProcessInMemory();
	void transferToBackingStore(std::shared_ptr<Process> process);

	size_t sizeToFrame(size_t size);
};

