#include "Semaphore.h"

Semaphore::Semaphore(int available) {
	semaphore = new std::counting_semaphore<GLOBAL_MAX_PERMITS>(available);
}

Semaphore::~Semaphore() {
	delete semaphore;
}

void Semaphore::acquire() {
	semaphore->acquire();
}

void Semaphore::acquire(int permitCount) {
	for (int i = 0; i < permitCount; i++) {
		semaphore->acquire();
	}
}

void Semaphore::release() {
	semaphore->release();
}

void Semaphore::release(int permitCount) {
	semaphore->release(permitCount);
}